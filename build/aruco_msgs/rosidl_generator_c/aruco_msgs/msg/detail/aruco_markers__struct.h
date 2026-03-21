// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aruco_msgs:msg/ArucoMarkers.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "aruco_msgs/msg/aruco_markers.h"


#ifndef ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__STRUCT_H_
#define ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'markers'
#include "aruco_msgs/msg/detail/aruco_marker__struct.h"

/// Struct defined in msg/ArucoMarkers in the package aruco_msgs.
typedef struct aruco_msgs__msg__ArucoMarkers
{
  aruco_msgs__msg__ArucoMarker__Sequence markers;
} aruco_msgs__msg__ArucoMarkers;

// Struct for a sequence of aruco_msgs__msg__ArucoMarkers.
typedef struct aruco_msgs__msg__ArucoMarkers__Sequence
{
  aruco_msgs__msg__ArucoMarkers * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aruco_msgs__msg__ArucoMarkers__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__STRUCT_H_
