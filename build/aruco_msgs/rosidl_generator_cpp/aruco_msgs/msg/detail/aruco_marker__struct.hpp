// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from aruco_msgs:msg/ArucoMarker.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "aruco_msgs/msg/aruco_marker.hpp"


#ifndef ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__STRUCT_HPP_
#define ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'pose'
#include "geometry_msgs/msg/detail/pose__struct.hpp"
// Member 'center'
#include "geometry_msgs/msg/detail/point__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__aruco_msgs__msg__ArucoMarker __attribute__((deprecated))
#else
# define DEPRECATED__aruco_msgs__msg__ArucoMarker __declspec(deprecated)
#endif

namespace aruco_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ArucoMarker_
{
  using Type = ArucoMarker_<ContainerAllocator>;

  explicit ArucoMarker_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    pose(_init),
    center(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0l;
      this->marker_length = 0.0;
      this->dictionary = "";
    }
  }

  explicit ArucoMarker_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    pose(_alloc, _init),
    dictionary(_alloc),
    center(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0l;
      this->marker_length = 0.0;
      this->dictionary = "";
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _id_type =
    int32_t;
  _id_type id;
  using _pose_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _pose_type pose;
  using _marker_length_type =
    double;
  _marker_length_type marker_length;
  using _dictionary_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _dictionary_type dictionary;
  using _center_type =
    geometry_msgs::msg::Point_<ContainerAllocator>;
  _center_type center;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__id(
    const int32_t & _arg)
  {
    this->id = _arg;
    return *this;
  }
  Type & set__pose(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->pose = _arg;
    return *this;
  }
  Type & set__marker_length(
    const double & _arg)
  {
    this->marker_length = _arg;
    return *this;
  }
  Type & set__dictionary(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->dictionary = _arg;
    return *this;
  }
  Type & set__center(
    const geometry_msgs::msg::Point_<ContainerAllocator> & _arg)
  {
    this->center = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    aruco_msgs::msg::ArucoMarker_<ContainerAllocator> *;
  using ConstRawPtr =
    const aruco_msgs::msg::ArucoMarker_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aruco_msgs::msg::ArucoMarker_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aruco_msgs::msg::ArucoMarker_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aruco_msgs::msg::ArucoMarker_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aruco_msgs::msg::ArucoMarker_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aruco_msgs::msg::ArucoMarker_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aruco_msgs::msg::ArucoMarker_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aruco_msgs::msg::ArucoMarker_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aruco_msgs::msg::ArucoMarker_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aruco_msgs__msg__ArucoMarker
    std::shared_ptr<aruco_msgs::msg::ArucoMarker_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aruco_msgs__msg__ArucoMarker
    std::shared_ptr<aruco_msgs::msg::ArucoMarker_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ArucoMarker_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->id != other.id) {
      return false;
    }
    if (this->pose != other.pose) {
      return false;
    }
    if (this->marker_length != other.marker_length) {
      return false;
    }
    if (this->dictionary != other.dictionary) {
      return false;
    }
    if (this->center != other.center) {
      return false;
    }
    return true;
  }
  bool operator!=(const ArucoMarker_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ArucoMarker_

// alias to use template instance with default allocator
using ArucoMarker =
  aruco_msgs::msg::ArucoMarker_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace aruco_msgs

#endif  // ARUCO_MSGS__MSG__DETAIL__ARUCO_MARKER__STRUCT_HPP_
