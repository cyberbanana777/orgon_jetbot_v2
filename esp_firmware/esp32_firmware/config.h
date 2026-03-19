#ifndef CONFIG_H
#define CONFIG_H
//Константы физических параметров
const float Pi = 3.14159;
const float WHEEL_BASE = 0.1775;  // длина базы
const float WHEEL_RADIUS = 0.065/2.0; // радиус ведущего колеса
const float max_voltage = 4.2 * 3;
const float min_voltage = 3.3 * 3;

const int ERROR_LED_PIN = 2; // Светодиод для индикации ошибок

// Подключение энкодеров
const uint8_t PIN_R_A = 34;  // Правое колесо, канал А
const uint8_t PIN_R_B = 35;  // Правое колесо, канал В
const uint8_t PIN_L_A = 32;  // Левое колесо, канал А
const uint8_t PIN_L_B = 33;  // Левое колесо, канал В

// Коэффициенты по умолчанию для регуляторов
float KP_R = 1.0;
float KI_R = 0.0;
float KD_R = 0.005;

float KP_L = 1.0; 
float KI_L = 0.0;
float KD_L = 0.005;

// Частота дискретизации для ПИДов
float DT = 3;

//  Кнопка остановки
const uint8_t PIN_EMERGENCY_STOP = 12;

// Пины сервоприводов
const int PIN_SERVO_BIG = 14;
const int PIN_SERVO_SMALL = 25;
const int PIN_SERVO_GRIPPER = 27;

// Калибровочные данные для сервоприводов
const int minPulseBig = 500;
const int maxPulseBig = 2360;
const int minPulseSmall = 500;
const int maxPulseSmall = 2500;
const int minPulseGripper = 650;
const int maxPulseGripper = 1675;


const double MAX_CONSTRUCTIVE_VELOCITY = 0.5; // м/сек
const float MAX_WHEEL_FREQUENCY = 2.5;

// === Таймеры ===
const unsigned long timer_timeout = DT * 1000; //мкс
// Для остановки после 5 секунд простоя
const unsigned int im_timer_timeout = 5000;
const unsigned long displayInterval = 200; 


#endif