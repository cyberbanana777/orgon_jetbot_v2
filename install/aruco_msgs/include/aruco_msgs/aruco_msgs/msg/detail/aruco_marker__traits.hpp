// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from aruco_msgs:msg/ArucoMarker.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "aruco_msgs/msg/aruco_marker.hpp"


#ifndef ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__TRAITS_HPP_
#define ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "aruco_msgs/msg/detail/aruco_marker__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'pose'
#include "geometry_msgs/msg/detail/pose__traits.hpp"
// Member 'center'
#include "geometry_msgs/msg/detail/point__traits.hpp"

namespace aruco_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const ArucoMarker & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: id
  {
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << ", ";
  }

  // member: pose
  {
    out << "pose: ";
    to_flow_style_yaml(msg.pose, out);
    out << ", ";
  }

  // member: marker_length
  {
    out << "marker_length: ";
    rosidl_generator_traits::value_to_yaml(msg.marker_length, out);
    out << ", ";
  }

  // member: dictionary
  {
    out << "dictionary: ";
    rosidl_generator_traits::value_to_yaml(msg.dictionary, out);
    out << ", ";
  }

  // member: center
  {
    out << "center: ";
    to_flow_style_yaml(msg.center, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ArucoMarker & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << "\n";
  }

  // member: pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pose:\n";
    to_block_style_yaml(msg.pose, out, indentation + 2);
  }

  // member: marker_length
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "marker_length: ";
    rosidl_generator_traits::value_to_yaml(msg.marker_length, out);
    out << "\n";
  }

  // member: dictionary
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "dictionary: ";
    rosidl_generator_traits::value_to_yaml(msg.dictionary, out);
    out << "\n";
  }

  // member: center
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "center:\n";
    to_block_style_yaml(msg.center, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ArucoMarker & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace aruco_msgs

namespace rosidl_generator_traits
{

[[deprecated("use aruco_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const aruco_msgs::msg::ArucoMarker & msg,
  std::ostream & out, size_t indentation = 0)
{
  aruco_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aruco_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const aruco_msgs::msg::ArucoMarker & msg)
{
  return aruco_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<aruco_msgs::msg::ArucoMarker>()
{
  return "aruco_msgs::msg::ArucoMarker";
}

template<>
inline const char * name<aruco_msgs::msg::ArucoMarker>()
{
  return "aruco_msgs/msg/ArucoMarker";
}

template<>
struct has_fixed_size<aruco_msgs::msg::ArucoMarker>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aruco_msgs::msg::ArucoMarker>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<aruco_msgs::msg::ArucoMarker>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__TRAITS_HPP_
