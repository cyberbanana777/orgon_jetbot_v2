// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aruco_msgs:msg/ArucoMarker.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "aruco_msgs/msg/aruco_marker.hpp"


#ifndef ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__BUILDER_HPP_
#define ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aruco_msgs/msg/detail/aruco_marker__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aruco_msgs
{

namespace msg
{

namespace builder
{

class Init_ArucoMarker_center
{
public:
  explicit Init_ArucoMarker_center(::aruco_msgs::msg::ArucoMarker & msg)
  : msg_(msg)
  {}
  ::aruco_msgs::msg::ArucoMarker center(::aruco_msgs::msg::ArucoMarker::_center_type arg)
  {
    msg_.center = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aruco_msgs::msg::ArucoMarker msg_;
};

class Init_ArucoMarker_dictionary
{
public:
  explicit Init_ArucoMarker_dictionary(::aruco_msgs::msg::ArucoMarker & msg)
  : msg_(msg)
  {}
  Init_ArucoMarker_center dictionary(::aruco_msgs::msg::ArucoMarker::_dictionary_type arg)
  {
    msg_.dictionary = std::move(arg);
    return Init_ArucoMarker_center(msg_);
  }

private:
  ::aruco_msgs::msg::ArucoMarker msg_;
};

class Init_ArucoMarker_marker_length
{
public:
  explicit Init_ArucoMarker_marker_length(::aruco_msgs::msg::ArucoMarker & msg)
  : msg_(msg)
  {}
  Init_ArucoMarker_dictionary marker_length(::aruco_msgs::msg::ArucoMarker::_marker_length_type arg)
  {
    msg_.marker_length = std::move(arg);
    return Init_ArucoMarker_dictionary(msg_);
  }

private:
  ::aruco_msgs::msg::ArucoMarker msg_;
};

class Init_ArucoMarker_pose
{
public:
  explicit Init_ArucoMarker_pose(::aruco_msgs::msg::ArucoMarker & msg)
  : msg_(msg)
  {}
  Init_ArucoMarker_marker_length pose(::aruco_msgs::msg::ArucoMarker::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return Init_ArucoMarker_marker_length(msg_);
  }

private:
  ::aruco_msgs::msg::ArucoMarker msg_;
};

class Init_ArucoMarker_id
{
public:
  explicit Init_ArucoMarker_id(::aruco_msgs::msg::ArucoMarker & msg)
  : msg_(msg)
  {}
  Init_ArucoMarker_pose id(::aruco_msgs::msg::ArucoMarker::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_ArucoMarker_pose(msg_);
  }

private:
  ::aruco_msgs::msg::ArucoMarker msg_;
};

class Init_ArucoMarker_header
{
public:
  Init_ArucoMarker_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ArucoMarker_id header(::aruco_msgs::msg::ArucoMarker::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ArucoMarker_id(msg_);
  }

private:
  ::aruco_msgs::msg::ArucoMarker msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::aruco_msgs::msg::ArucoMarker>()
{
  return aruco_msgs::msg::builder::Init_ArucoMarker_header();
}

}  // namespace aruco_msgs

#endif  // ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__BUILDER_HPP_
