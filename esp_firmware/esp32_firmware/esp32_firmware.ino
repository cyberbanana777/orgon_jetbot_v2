// Copyright (c) 2025 Alice Zenina and Alexander Grachev RTU MIREA (Russia)
// SPDX-License-Identifier: MIT
// Details in the LICENSE file in the root of the package.

#include <Arduino.h>
#include <math.h>
#include <Preferences.h>
#include <Wire.h>                     
#include <stdio.h>
#include <GyverOLED.h>
#include "INA219.h"
#include <ESP32Servo.h>

#include "config.h"
#include "encoders.h"
#include "errors.h"
#include "protocol_wrapper.h"
#include "motor_controller.h"

Servo servoBig;
Servo servoSmall;
Servo servoGripper;

Encoders encoders;
ErrorManager error_manager;
MotorController controller(WHEEL_BASE, MAX_CONSTRUCTIVE_VELOCITY, DT,
                       KP_L, KI_L, KD_L,
                       KP_R, KI_R, KD_R);

INA219 INA(0x41);


// ========================================================================================================

GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;  // SSD1306 128x32 с буферизацией

bool init_oled(){
  oled.init();            // инициализация дисплея
  oled.clear();           // очистка буфера
  oled.home();            // курсор в (0,0)
  oled.print("ESP32 ready");
  oled.update();          // вывод на экран
  delay(1000);
  return true;
}

void init_servos(){
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);

  servoBig.attach(PIN_SERVO_BIG, minPulseBig, maxPulseBig);
  servoSmall.attach(PIN_SERVO_SMALL, minPulseSmall, maxPulseSmall);
  servoGripper.attach(PIN_SERVO_GRIPPER, minPulseGripper, maxPulseGripper);
  write_servos(90, 90, 10);
}


void write_servos(int16_t degBig, int16_t degSmall, int16_t degGripper){
  const uint8_t servoLimits[2] = {0, 180};
  const int16_t limitsBig[2] = {-45, 135};
  const int16_t limitsSmall[2] = {-45, 135};
  const int16_t limitsGripper[2] = {0, 90};

  degBig = map(degBig, limitsBig[0], limitsBig[1], servoLimits[0], servoLimits[1]);
  degSmall = map(degSmall, limitsSmall[0], limitsSmall[1], servoLimits[0], servoLimits[1]);
  degGripper = map(degGripper, limitsGripper[0], limitsGripper[1], servoLimits[0], servoLimits[1]);

  servoBig.write(degBig);
  servoSmall.write(degSmall);
  servoGripper.write(degGripper);
}


String pwd_short;
int pros;
float bus_voltage, shunt_v, current;


// Настройки для записи в постоянную память
#define RW_MODE false
#define RO_MODE true
Preferences fleshMemory;

// Для таймеров
unsigned long tmr = 0;
unsigned long delta = 0;
unsigned long lastDisplayUpdate = 0;
unsigned int im_timer = 0;

double x_pos_ = 0.0;
double y_pos_ = 0.0;
double heading_ = 0.0;

// Переменные для хранения принятых данных
String wifi_ssid = "";
String wifi_password = "";
String wifi_ip = "";

String input_m[8] = {"", "", "", "", "", "", "", ""};
String input_j[3] = {"", "", ""};



// ===== Переменные для приёма с Jetson через Serial (USB) - основной канал связи =====
String mainInputString = "";         // a String to hold incoming data
bool mainStringComplete = false;  // whether the string is complete
bool mainInputOpen = false;
// ====================================================================================

// ===== Переменные для приёма с Jetson через Serial2 - дополнительный канал связи=====
String addictionInputString = "";
bool addictionStringComplete = false;
bool addictionInputOpen = false;
// ====================================================================================


// Флаги и мьютекс
volatile bool emergencyStop = false;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// Обработчик прерывания
void IRAM_ATTR emergencyISR() {
  // Отключаем прерывания на время обработки
  portENTER_CRITICAL_ISR(&mux);
  emergencyStop = digitalRead(PIN_EMERGENCY_STOP);
  portEXIT_CRITICAL_ISR(&mux);
}

//Ответ на входное сообщение по Serial0 (USB)
void serialEvent() {
  im_timer = millis();
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if(inChar == '$' or mainInputOpen == true){
      // add it to the mainInputString:
      mainInputString += inChar;
      mainInputOpen = true;
    }
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '#') {
      mainStringComplete = true;
      mainInputOpen = false;
    }
  }
}


void parseJetsonMessage(String input){
  String trimmed = addictionInputString.substring(1, addictionInputString.length() - 2);
  String parts[4]; // массив для 4 элементов
  int count = 0;   // сколько уже сохранили

  int start = 0;
  int semicolon = trimmed.indexOf(';');

  while (semicolon != -1 && count < 4) {
    parts[count++] = trimmed.substring(start, semicolon);
    start = semicolon + 1;
    semicolon = trimmed.indexOf(';', start);
  }

  // Последняя часть
  if (count < 4) {
    parts[count++] = trimmed.substring(start);
  }

  // Теперь можно обратиться к элементам массива
  // или, если очень хочется, присвоить отдельным переменным:
  // String msg_type = parts[0];
  wifi_ssid = parts[1];
  wifi_password = parts[2];
  wifi_ip = parts[3];
}

float convert_ticks_to_freq(long ticks, long timeout_micros, char dir){
  /*
  ticks - счётчик тактов из прерываний
  timeout_micros - время замера количества меток
  dir - направеление. Может быть 'r', 'l'. При 'l'  происходит инвертирование значения, 
  необходиомое для нормальной работы логики. Чтобы при движении вперёд или назад
  знаки угловых скоростей колёс были одинаковые
  */
  const float steps_per_tick = 1.0 / 4.0; // 4 импульса на 1 шаг
  const float turn_per_step = 1.0 / 270.0; // 270 шагов в 1 полном обороте
  const double micros_to_sec= 1.0 / 1000000.0; 
  float timeout_sec = timeout_micros * micros_to_sec;
  float result = turn_per_step * steps_per_tick * ticks / timeout_sec;
  
  // для левого колеса инвертируем частоту
  if(dir == 'l'){
    result = result * (-1);
  }
  else if(dir == 'r'){
    result = result * 1;
  }

  return result;
}

void read_serial1(){
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    if (inChar == '$' || addictionInputOpen) {
      addictionInputString += inChar;
      addictionInputOpen = true;
    }
    if (inChar == '#') {
      addictionStringComplete = true;
      addictionInputOpen = false;
    }
  }
}

void print_telemetry(){
  oled.clear();
  oled.home();  // курсор в (0,0)

  oled.print("SSID: ");
  oled.println(wifi_ssid.substring(0, 15));  // обрезаем до 16 символов

  oled.print("IP: ");
  oled.println(wifi_ip);

  oled.print("PWD: ");
  pwd_short = wifi_password.substring(0, 14);
  if (wifi_password.length() > 14) pwd_short += "...";
  oled.println(pwd_short);

  oled.print("Bat: ");
  bus_voltage = INA.getBusVoltage();
  shunt_v = INA.getShuntVoltage_mV();

  pros = (bus_voltage - min_voltage) / (max_voltage - min_voltage) * 100;
  current = shunt_v / 0.1 / 1000;
  oled.print(bus_voltage);
  oled.print("V ");
  oled.print(pros);
  oled.print("% ");
  oled.print(current);
  oled.println("A");
  oled.update();  // отправляем буфер на дисплей
}

void setup() {

  pinMode(12, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_EMERGENCY_STOP), emergencyISR, CHANGE);
  

  Serial.begin(115200);
  Serial1.begin(115200);

  error_manager.begin(ERROR_LED_PIN);

  encoders.begin(PIN_R_A, PIN_R_B, PIN_L_A, PIN_L_B);
  error_manager.check_exec(controller.begin(), "Error! Failed to init motor controller");
  error_manager.check_exec(Wire.begin(), "Error! Failed to init I2C bus.");
  error_manager.check_exec(INA.begin(), "Error! Failed to init INA219 module.");  
  
  
  init_oled();
  init_servos();


  mainInputString.reserve(250);
  addictionInputString.reserve(250);
  
  delay(100);

  // =================== РАБОТА С ПОСТОЯННОЙ ПАМЯТЬЮ ==========================
  fleshMemory.begin("Memory1", RO_MODE);
  bool tpInit = fleshMemory.isKey("nvsInit");

  // Запись в постоянную память коэффициентов ПИД-регуляторов
  if (tpInit == false) {
 
    fleshMemory.end();                             
    fleshMemory.begin("Memory1", RW_MODE);

    fleshMemory.putFloat("Kp_R", KP_R); 
    fleshMemory.putFloat("Ki_R", KI_R); 
    fleshMemory.putFloat("Kd_R", KD_R); 

    fleshMemory.putFloat("Kp_L", KP_L);
    fleshMemory.putFloat("Ki_L", KI_L); 
    fleshMemory.putFloat("Kd_L", KD_L); 

    fleshMemory.putBool("nvsInit", true);
    
    fleshMemory.end();
  } 


  // Получение коэффициентов ПИД-регуляторов из постоянной памяти
  else {

    KP_R = fleshMemory.getFloat("Kp_R"); 
    KI_R = fleshMemory.getFloat("Ki_R"); 
    KD_R = fleshMemory.getFloat("Kd_R"); 

    KP_L = fleshMemory.getFloat("Kp_L"); 
    KI_L = fleshMemory.getFloat("Ki_L"); 
    KD_L = fleshMemory.getFloat("Kd_L"); 

    fleshMemory.end();

    controller.setPID(KP_L, KI_L, KD_L, KP_R, KI_R, KD_R);
  }
  // ==========================================================================
  controller.setWheelSpeeds(0.0, 0.0);


}



void loop() {
  if ((millis() - im_timer > im_timer_timeout) || (emergencyStop)){
    // Аварийная остановка
    controller.setWheelSpeeds(0.0, 0.0);

  }

  delta = micros() - tmr;
  
  if (delta >= timer_timeout){  
    tmr = micros();
    
    int32_t rightTicks = encoders.getRightAndReset();
    int32_t leftTicks  = encoders.getLeftAndReset();

    // 2. Преобразуем тики в частоту вращения (об/с)
    float rightFreq = convert_ticks_to_freq(rightTicks, timer_timeout, 'r');
    float leftFreq  = convert_ticks_to_freq(leftTicks,  timer_timeout, 'l');

    // 3. Вычисляем линейные скорости колёс (м/с)
    float rightLinVel = rightFreq * 2 * Pi * WHEEL_RADIUS;
    float leftLinVel  = leftFreq  * 2 * Pi * WHEEL_RADIUS;

    // 4. Обновляем ПИД-регуляторы и выдаём сигнал на моторы
    controller.update(leftLinVel, rightLinVel);

    // 5. Одометрия
    double linearVel   = (rightLinVel + leftLinVel) / 2.0;
    double angularVel  = (rightLinVel - leftLinVel) / WHEEL_BASE;   // для дифференциального привода
    double dtSec       = timer_timeout / 1e6;               // период в секундах
    double deltaHeading = angularVel * dtSec;
    double deltaX       = linearVel * cos(heading_) * dtSec;
    double deltaY       = linearVel * sin(heading_) * dtSec;

    x_pos_  += deltaX;
    y_pos_  += deltaY;
    heading_ += deltaHeading;

    // 6. Публикация данных одометрии
    odomPublish(x_pos_, y_pos_, heading_, linearVel, angularVel,
                leftLinVel, rightLinVel);


  }

    //Обработка входного значения
  if (mainStringComplete) {

    unsigned int i = 0;
    unsigned int j = 0;

    // msg_type
    for (i = 1; i < mainInputString.length()-1; i++){
      if (mainInputString[i] == ';') break;
      input_m[0] += mainInputString[i];
    }
    char str1[input_m[0].length() + 1];
    for(j=0; j < input_m[0].length(); j++) str1[j] = input_m[0][j];
    str1[input_m[0].length()] = '\0';
    input_m[0] = "";

    int msg_type = (int)atof(str1);

    if (msg_type == 1){
      // управление линейной и угловой скоростью платформы

      unsigned short num_flied = 2;

      for (j = 1; j <= num_flied; j++){
        for (i = i+1; i < mainInputString.length()-1; i++){
          if (mainInputString[i] == ';') break;
          input_m[j] += mainInputString[i];
        }
      }

      char str2[input_m[1].length() + 1]; 
      char str3[input_m[2].length() + 1];
      
      for(j=0; j < input_m[1].length(); j++) str2[j] = input_m[1][j];
      for(j=0; j < input_m[2].length(); j++) str3[j] = input_m[2][j];      

      str2[input_m[1].length()] = '\0';
      str3[input_m[2].length()] = '\0';

      for (j = 1; j <= num_flied; j++){
        input_m[j] = "";
      }

      controller.setTargetVelocity(atof(str2), atof(str3));
    }

    else if (msg_type == 2){
      // управление скоростями каждого из колёс

      unsigned short num_flied = 2;

      for (j = 1; j <= num_flied; j++){
        for (i = i+1; i < mainInputString.length()-1; i++){
          if (mainInputString[i] == ';') break;
          input_m[j] += mainInputString[i];
        }
      }
      char str2[input_m[1].length() + 1]; 
      char str3[input_m[2].length() + 1];
      
      for(j=0; j < input_m[1].length(); j++) str2[j] = input_m[1][j];
      for(j=0; j < input_m[2].length(); j++) str3[j] = input_m[2][j]; 

      str2[input_m[1].length()] = '\0';
      str3[input_m[2].length()] = '\0';

      for (j = 1; j <= num_flied; j++){
        input_m[j] = "";
      }

      controller.setWheelSpeeds(atof(str2), atof(str3));
    }

    else if (msg_type == 3){
      // Установка новых PID-коэффициентов во временную память

      unsigned short num_flied = 6;

      for (j = 1; j <= num_flied; j++){
        for (i = i+1; i < mainInputString.length()-1; i++){
          if (mainInputString[i] == ';') break;
          input_m[j] += mainInputString[i];
        }
      }

      char str2[input_m[1].length() + 1]; 
      char str3[input_m[2].length() + 1];
      char str4[input_m[3].length() + 1]; 
      char str5[input_m[4].length() + 1]; 
      char str6[input_m[5].length() + 1]; 
      char str7[input_m[6].length() + 1]; 

      for(j=0; j < input_m[1].length(); j++) str2[j] = input_m[1][j];
      for(j=0; j < input_m[2].length(); j++) str3[j] = input_m[2][j]; 
      for(j=0; j < input_m[3].length(); j++) str4[j] = input_m[3][j]; 
      for(j=0; j < input_m[4].length(); j++) str5[j] = input_m[4][j]; 
      for(j=0; j < input_m[5].length(); j++) str6[j] = input_m[5][j]; 
      for(j=0; j < input_m[6].length(); j++) str7[j] = input_m[6][j]; 

      str2[input_m[1].length()] = '\0';
      str3[input_m[2].length()] = '\0';
      str4[input_m[3].length()] = '\0';
      str5[input_m[4].length()] = '\0';
      str6[input_m[5].length()] = '\0';
      str7[input_m[6].length()] = '\0';

      for (j = 1; j <= num_flied; j++){
        input_m[j] = "";
      }

      KP_L = atof(str2);
      KI_L = atof(str3);
      KD_L = atof(str4);

      KP_R = atof(str5);
      KI_R = atof(str6);
      KD_R = atof(str7);
      controller.setPID(KP_L, KI_L, KD_L, KP_R, KI_R, KD_R);
    }

    else if (msg_type == 4){
      // Установка новых PID-коэффициентов и запись их во Flash-память (постоянная память)

      unsigned short num_flied = 6;

      for (j = 1; j <= num_flied; j++){
        for (i = i+1; i < mainInputString.length()-1; i++){
          if (mainInputString[i] == ';') break;
          input_m[j] += mainInputString[i];
        }
      }

      char str2[input_m[1].length() + 1]; 
      char str3[input_m[2].length() + 1];
      char str4[input_m[3].length() + 1]; 
      char str5[input_m[4].length() + 1]; 
      char str6[input_m[5].length() + 1]; 
      char str7[input_m[6].length() + 1]; 

      for(j=0; j < input_m[1].length(); j++) str2[j] = input_m[1][j];
      for(j=0; j < input_m[2].length(); j++) str3[j] = input_m[2][j]; 
      for(j=0; j < input_m[3].length(); j++) str4[j] = input_m[3][j]; 
      for(j=0; j < input_m[4].length(); j++) str5[j] = input_m[4][j]; 
      for(j=0; j < input_m[5].length(); j++) str6[j] = input_m[5][j]; 
      for(j=0; j < input_m[6].length(); j++) str7[j] = input_m[6][j]; 

      str2[input_m[1].length()] = '\0';
      str3[input_m[2].length()] = '\0';
      str4[input_m[3].length()] = '\0';
      str5[input_m[4].length()] = '\0';
      str6[input_m[5].length()] = '\0';
      str7[input_m[6].length()] = '\0';


      for (j = 1; j <= num_flied; j++){
        input_m[j] = "";
      }

      KP_L = atof(str2);
      KI_L = atof(str3);
      KD_L = atof(str4);

      KP_R = atof(str5);
      KI_R = atof(str6);
      KD_R = atof(str7);
      controller.setPID(KP_L, KI_L, KD_L, KP_R, KI_R, KD_R);


      fleshMemory.begin("Memory1", RW_MODE);
          fleshMemory.putFloat("Kp_R", KP_R); 
          fleshMemory.putFloat("Ki_R", KI_R); 
          fleshMemory.putFloat("Kd_R", KD_R); 

          fleshMemory.putFloat("Kp_L", KP_L);
          fleshMemory.putFloat("Ki_L", KI_L); 
          fleshMemory.putFloat("Kd_L", KD_L); 

      fleshMemory.end();
    }
        
    else if (msg_type == 5){
      regulatorsCoefficientsPublish(KP_L, KI_L, KD_L, KP_R, KI_R, KD_R);
    }

    else if (msg_type == 6){
      // задание положения манипулятору
      unsigned short num_flied = 3;

      for (j = 1; j <= num_flied; j++){
        for (i = i+1; i < mainInputString.length()-1; i++){
          if (mainInputString[i] == ';') break;
          input_m[j] += mainInputString[i];
        }
      }

      char str2[input_m[1].length() + 1]; 
      char str3[input_m[2].length() + 1];
      char str4[input_m[3].length() + 1]; 

      for(j=0; j < input_m[1].length(); j++) str2[j] = input_m[1][j];
      for(j=0; j < input_m[2].length(); j++) str3[j] = input_m[2][j]; 
      for(j=0; j < input_m[3].length(); j++) str4[j] = input_m[3][j]; 


      str2[input_m[1].length()] = '\0';
      str3[input_m[2].length()] = '\0';
      str4[input_m[3].length()] = '\0';



      for (j = 1; j <= num_flied; j++){
        input_m[j] = "";
      };

      write_servos((int)atof(str2), (int)atof(str3), (int)atof(str4));
    }
    


    // else{
    //   raise_error();
    // }

    mainInputString = "";
    mainStringComplete = false;

  }
  
  // ===================== ЧТЕНИЕ ДАННЫХ С JETSON NANO (Serial1) =====================
  read_serial1();

  if (addictionStringComplete) {
    parseJetsonMessage(addictionInputString);
    addictionInputString = "";
    addictionStringComplete = false;
  }
  // =========================================================================


  // ===================== ОБНОВЛЕНИЕ ДИСПЛЕЯ =====================
  if (millis() - lastDisplayUpdate > displayInterval) {
    lastDisplayUpdate = millis();
    print_telemetry();
  }
  // =========================================================================
}

