// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from aruco_msgs:msg/ArucoMarkers.idl
// generated code does not contain a copyright notice
#ifndef ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "aruco_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "aruco_msgs/msg/detail/aruco_markers__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_aruco_msgs
bool cdr_serialize_aruco_msgs__msg__ArucoMarkers(
  const aruco_msgs__msg__ArucoMarkers * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_aruco_msgs
bool cdr_deserialize_aruco_msgs__msg__ArucoMarkers(
  eprosima::fastcdr::Cdr &,
  aruco_msgs__msg__ArucoMarkers * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_aruco_msgs
size_t get_serialized_size_aruco_msgs__msg__ArucoMarkers(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_aruco_msgs
size_t max_serialized_size_aruco_msgs__msg__ArucoMarkers(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_aruco_msgs
bool cdr_serialize_key_aruco_msgs__msg__ArucoMarkers(
  const aruco_msgs__msg__ArucoMarkers * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_aruco_msgs
size_t get_serialized_size_key_aruco_msgs__msg__ArucoMarkers(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_aruco_msgs
size_t max_serialized_size_key_aruco_msgs__msg__ArucoMarkers(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_aruco_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, aruco_msgs, msg, ArucoMarkers)();

#ifdef __cplusplus
}
#endif

#endif  // ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
