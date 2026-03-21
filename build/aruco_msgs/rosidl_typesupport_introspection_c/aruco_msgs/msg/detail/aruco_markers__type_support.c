// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from aruco_msgs:msg/ArucoMarkers.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "aruco_msgs/msg/detail/aruco_markers__rosidl_typesupport_introspection_c.h"
#include "aruco_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "aruco_msgs/msg/detail/aruco_markers__functions.h"
#include "aruco_msgs/msg/detail/aruco_markers__struct.h"


// Include directives for member types
// Member `markers`
#include "aruco_msgs/msg/aruco_marker.h"
// Member `markers`
#include "aruco_msgs/msg/detail/aruco_marker__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  aruco_msgs__msg__ArucoMarkers__init(message_memory);
}

void aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_fini_function(void * message_memory)
{
  aruco_msgs__msg__ArucoMarkers__fini(message_memory);
}

size_t aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__size_function__ArucoMarkers__markers(
  const void * untyped_member)
{
  const aruco_msgs__msg__ArucoMarker__Sequence * member =
    (const aruco_msgs__msg__ArucoMarker__Sequence *)(untyped_member);
  return member->size;
}

const void * aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__get_const_function__ArucoMarkers__markers(
  const void * untyped_member, size_t index)
{
  const aruco_msgs__msg__ArucoMarker__Sequence * member =
    (const aruco_msgs__msg__ArucoMarker__Sequence *)(untyped_member);
  return &member->data[index];
}

void * aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__get_function__ArucoMarkers__markers(
  void * untyped_member, size_t index)
{
  aruco_msgs__msg__ArucoMarker__Sequence * member =
    (aruco_msgs__msg__ArucoMarker__Sequence *)(untyped_member);
  return &member->data[index];
}

void aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__fetch_function__ArucoMarkers__markers(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const aruco_msgs__msg__ArucoMarker * item =
    ((const aruco_msgs__msg__ArucoMarker *)
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__get_const_function__ArucoMarkers__markers(untyped_member, index));
  aruco_msgs__msg__ArucoMarker * value =
    (aruco_msgs__msg__ArucoMarker *)(untyped_value);
  *value = *item;
}

void aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__assign_function__ArucoMarkers__markers(
  void * untyped_member, size_t index, const void * untyped_value)
{
  aruco_msgs__msg__ArucoMarker * item =
    ((aruco_msgs__msg__ArucoMarker *)
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__get_function__ArucoMarkers__markers(untyped_member, index));
  const aruco_msgs__msg__ArucoMarker * value =
    (const aruco_msgs__msg__ArucoMarker *)(untyped_value);
  *item = *value;
}

bool aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__resize_function__ArucoMarkers__markers(
  void * untyped_member, size_t size)
{
  aruco_msgs__msg__ArucoMarker__Sequence * member =
    (aruco_msgs__msg__ArucoMarker__Sequence *)(untyped_member);
  aruco_msgs__msg__ArucoMarker__Sequence__fini(member);
  return aruco_msgs__msg__ArucoMarker__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_member_array[1] = {
  {
    "markers",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aruco_msgs__msg__ArucoMarkers, markers),  // bytes offset in struct
    NULL,  // default value
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__size_function__ArucoMarkers__markers,  // size() function pointer
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__get_const_function__ArucoMarkers__markers,  // get_const(index) function pointer
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__get_function__ArucoMarkers__markers,  // get(index) function pointer
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__fetch_function__ArucoMarkers__markers,  // fetch(index, &value) function pointer
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__assign_function__ArucoMarkers__markers,  // assign(index, value) function pointer
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__resize_function__ArucoMarkers__markers  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_members = {
  "aruco_msgs__msg",  // message namespace
  "ArucoMarkers",  // message name
  1,  // number of fields
  sizeof(aruco_msgs__msg__ArucoMarkers),
  false,  // has_any_key_member_
  aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_member_array,  // message members
  aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_init_function,  // function to initialize message memory (memory has to be allocated)
  aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_type_support_handle = {
  0,
  &aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_members,
  get_message_typesupport_handle_function,
  &aruco_msgs__msg__ArucoMarkers__get_type_hash,
  &aruco_msgs__msg__ArucoMarkers__get_type_description,
  &aruco_msgs__msg__ArucoMarkers__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_aruco_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aruco_msgs, msg, ArucoMarkers)() {
  aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aruco_msgs, msg, ArucoMarker)();
  if (!aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_type_support_handle.typesupport_identifier) {
    aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &aruco_msgs__msg__ArucoMarkers__rosidl_typesupport_introspection_c__ArucoMarkers_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
