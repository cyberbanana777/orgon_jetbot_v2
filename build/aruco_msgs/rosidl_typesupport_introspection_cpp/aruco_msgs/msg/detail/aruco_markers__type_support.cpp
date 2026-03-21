// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from aruco_msgs:msg/ArucoMarkers.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "aruco_msgs/msg/detail/aruco_markers__functions.h"
#include "aruco_msgs/msg/detail/aruco_markers__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace aruco_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void ArucoMarkers_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) aruco_msgs::msg::ArucoMarkers(_init);
}

void ArucoMarkers_fini_function(void * message_memory)
{
  auto typed_message = static_cast<aruco_msgs::msg::ArucoMarkers *>(message_memory);
  typed_message->~ArucoMarkers();
}

size_t size_function__ArucoMarkers__markers(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<aruco_msgs::msg::ArucoMarker> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ArucoMarkers__markers(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<aruco_msgs::msg::ArucoMarker> *>(untyped_member);
  return &member[index];
}

void * get_function__ArucoMarkers__markers(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<aruco_msgs::msg::ArucoMarker> *>(untyped_member);
  return &member[index];
}

void fetch_function__ArucoMarkers__markers(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const aruco_msgs::msg::ArucoMarker *>(
    get_const_function__ArucoMarkers__markers(untyped_member, index));
  auto & value = *reinterpret_cast<aruco_msgs::msg::ArucoMarker *>(untyped_value);
  value = item;
}

void assign_function__ArucoMarkers__markers(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<aruco_msgs::msg::ArucoMarker *>(
    get_function__ArucoMarkers__markers(untyped_member, index));
  const auto & value = *reinterpret_cast<const aruco_msgs::msg::ArucoMarker *>(untyped_value);
  item = value;
}

void resize_function__ArucoMarkers__markers(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<aruco_msgs::msg::ArucoMarker> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ArucoMarkers_message_member_array[1] = {
  {
    "markers",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<aruco_msgs::msg::ArucoMarker>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aruco_msgs::msg::ArucoMarkers, markers),  // bytes offset in struct
    nullptr,  // default value
    size_function__ArucoMarkers__markers,  // size() function pointer
    get_const_function__ArucoMarkers__markers,  // get_const(index) function pointer
    get_function__ArucoMarkers__markers,  // get(index) function pointer
    fetch_function__ArucoMarkers__markers,  // fetch(index, &value) function pointer
    assign_function__ArucoMarkers__markers,  // assign(index, value) function pointer
    resize_function__ArucoMarkers__markers  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ArucoMarkers_message_members = {
  "aruco_msgs::msg",  // message namespace
  "ArucoMarkers",  // message name
  1,  // number of fields
  sizeof(aruco_msgs::msg::ArucoMarkers),
  false,  // has_any_key_member_
  ArucoMarkers_message_member_array,  // message members
  ArucoMarkers_init_function,  // function to initialize message memory (memory has to be allocated)
  ArucoMarkers_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ArucoMarkers_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ArucoMarkers_message_members,
  get_message_typesupport_handle_function,
  &aruco_msgs__msg__ArucoMarkers__get_type_hash,
  &aruco_msgs__msg__ArucoMarkers__get_type_description,
  &aruco_msgs__msg__ArucoMarkers__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace aruco_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aruco_msgs::msg::ArucoMarkers>()
{
  return &::aruco_msgs::msg::rosidl_typesupport_introspection_cpp::ArucoMarkers_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aruco_msgs, msg, ArucoMarkers)() {
  return &::aruco_msgs::msg::rosidl_typesupport_introspection_cpp::ArucoMarkers_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
