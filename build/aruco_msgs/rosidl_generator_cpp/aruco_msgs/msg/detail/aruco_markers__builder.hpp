// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aruco_msgs:msg/ArucoMarkers.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "aruco_msgs/msg/aruco_markers.hpp"


#ifndef ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__BUILDER_HPP_
#define ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aruco_msgs/msg/detail/aruco_markers__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aruco_msgs
{

namespace msg
{

namespace builder
{

class Init_ArucoMarkers_markers
{
public:
  Init_ArucoMarkers_markers()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::aruco_msgs::msg::ArucoMarkers markers(::aruco_msgs::msg::ArucoMarkers::_markers_type arg)
  {
    msg_.markers = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aruco_msgs::msg::ArucoMarkers msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::aruco_msgs::msg::ArucoMarkers>()
{
  return aruco_msgs::msg::builder::Init_ArucoMarkers_markers();
}

}  // namespace aruco_msgs

#endif  // ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKERS__BUILDER_HPP_
