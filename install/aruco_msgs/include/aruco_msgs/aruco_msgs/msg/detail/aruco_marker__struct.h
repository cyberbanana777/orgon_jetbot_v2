// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aruco_msgs:msg/ArucoMarker.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "aruco_msgs/msg/aruco_marker.h"


#ifndef ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__STRUCT_H_
#define ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'pose'
#include "geometry_msgs/msg/detail/pose__struct.h"
// Member 'dictionary'
#include "rosidl_runtime_c/string.h"
// Member 'center'
#include "geometry_msgs/msg/detail/point__struct.h"

/// Struct defined in msg/ArucoMarker in the package aruco_msgs.
typedef struct aruco_msgs__msg__ArucoMarker
{
  std_msgs__msg__Header header;
  int32_t id;
  geometry_msgs__msg__Pose pose;
  double marker_length;
  /// тип словаря, например "DICT_6X6_250", "DICT_5X5_100" и т.п.
  rosidl_runtime_c__String dictionary;
  geometry_msgs__msg__Point center;
} aruco_msgs__msg__ArucoMarker;

// Struct for a sequence of aruco_msgs__msg__ArucoMarker.
typedef struct aruco_msgs__msg__ArucoMarker__Sequence
{
  aruco_msgs__msg__ArucoMarker * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aruco_msgs__msg__ArucoMarker__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__STRUCT_H_
