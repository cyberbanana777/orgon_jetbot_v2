// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from aruco_msgs:msg/ArucoMarkers.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "aruco_msgs/msg/aruco_markers.hpp"


#ifndef ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__TRAITS_HPP_
#define ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "aruco_msgs/msg/detail/aruco_markers__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'markers'
#include "aruco_msgs/msg/detail/aruco_marker__traits.hpp"

namespace aruco_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const ArucoMarkers & msg,
  std::ostream & out)
{
  out << "{";
  // member: markers
  {
    if (msg.markers.size() == 0) {
      out << "markers: []";
    } else {
      out << "markers: [";
      size_t pending_items = msg.markers.size();
      for (auto item : msg.markers) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ArucoMarkers & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: markers
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.markers.size() == 0) {
      out << "markers: []\n";
    } else {
      out << "markers:\n";
      for (auto item : msg.markers) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ArucoMarkers & msg, bool use_flow_style = false)
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
  const aruco_msgs::msg::ArucoMarkers & msg,
  std::ostream & out, size_t indentation = 0)
{
  aruco_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aruco_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const aruco_msgs::msg::ArucoMarkers & msg)
{
  return aruco_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<aruco_msgs::msg::ArucoMarkers>()
{
  return "aruco_msgs::msg::ArucoMarkers";
}

template<>
inline const char * name<aruco_msgs::msg::ArucoMarkers>()
{
  return "aruco_msgs/msg/ArucoMarkers";
}

template<>
struct has_fixed_size<aruco_msgs::msg::ArucoMarkers>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aruco_msgs::msg::ArucoMarkers>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<aruco_msgs::msg::ArucoMarkers>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__TRAITS_HPP_
