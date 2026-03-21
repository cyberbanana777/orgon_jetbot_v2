// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from aruco_msgs:msg/ArucoMarker.idl
// generated code does not contain a copyright notice
#include "aruco_msgs/msg/detail/aruco_marker__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `pose`
#include "geometry_msgs/msg/detail/pose__functions.h"
// Member `dictionary`
#include "rosidl_runtime_c/string_functions.h"
// Member `center`
#include "geometry_msgs/msg/detail/point__functions.h"

bool
aruco_msgs__msg__ArucoMarker__init(aruco_msgs__msg__ArucoMarker * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    aruco_msgs__msg__ArucoMarker__fini(msg);
    return false;
  }
  // id
  // pose
  if (!geometry_msgs__msg__Pose__init(&msg->pose)) {
    aruco_msgs__msg__ArucoMarker__fini(msg);
    return false;
  }
  // marker_length
  // dictionary
  if (!rosidl_runtime_c__String__init(&msg->dictionary)) {
    aruco_msgs__msg__ArucoMarker__fini(msg);
    return false;
  }
  // center
  if (!geometry_msgs__msg__Point__init(&msg->center)) {
    aruco_msgs__msg__ArucoMarker__fini(msg);
    return false;
  }
  return true;
}

void
aruco_msgs__msg__ArucoMarker__fini(aruco_msgs__msg__ArucoMarker * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // id
  // pose
  geometry_msgs__msg__Pose__fini(&msg->pose);
  // marker_length
  // dictionary
  rosidl_runtime_c__String__fini(&msg->dictionary);
  // center
  geometry_msgs__msg__Point__fini(&msg->center);
}

bool
aruco_msgs__msg__ArucoMarker__are_equal(const aruco_msgs__msg__ArucoMarker * lhs, const aruco_msgs__msg__ArucoMarker * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // id
  if (lhs->id != rhs->id) {
    return false;
  }
  // pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->pose), &(rhs->pose)))
  {
    return false;
  }
  // marker_length
  if (lhs->marker_length != rhs->marker_length) {
    return false;
  }
  // dictionary
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->dictionary), &(rhs->dictionary)))
  {
    return false;
  }
  // center
  if (!geometry_msgs__msg__Point__are_equal(
      &(lhs->center), &(rhs->center)))
  {
    return false;
  }
  return true;
}

bool
aruco_msgs__msg__ArucoMarker__copy(
  const aruco_msgs__msg__ArucoMarker * input,
  aruco_msgs__msg__ArucoMarker * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // id
  output->id = input->id;
  // pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->pose), &(output->pose)))
  {
    return false;
  }
  // marker_length
  output->marker_length = input->marker_length;
  // dictionary
  if (!rosidl_runtime_c__String__copy(
      &(input->dictionary), &(output->dictionary)))
  {
    return false;
  }
  // center
  if (!geometry_msgs__msg__Point__copy(
      &(input->center), &(output->center)))
  {
    return false;
  }
  return true;
}

aruco_msgs__msg__ArucoMarker *
aruco_msgs__msg__ArucoMarker__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aruco_msgs__msg__ArucoMarker * msg = (aruco_msgs__msg__ArucoMarker *)allocator.allocate(sizeof(aruco_msgs__msg__ArucoMarker), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(aruco_msgs__msg__ArucoMarker));
  bool success = aruco_msgs__msg__ArucoMarker__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
aruco_msgs__msg__ArucoMarker__destroy(aruco_msgs__msg__ArucoMarker * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    aruco_msgs__msg__ArucoMarker__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
aruco_msgs__msg__ArucoMarker__Sequence__init(aruco_msgs__msg__ArucoMarker__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aruco_msgs__msg__ArucoMarker * data = NULL;

  if (size) {
    data = (aruco_msgs__msg__ArucoMarker *)allocator.zero_allocate(size, sizeof(aruco_msgs__msg__ArucoMarker), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = aruco_msgs__msg__ArucoMarker__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        aruco_msgs__msg__ArucoMarker__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
aruco_msgs__msg__ArucoMarker__Sequence__fini(aruco_msgs__msg__ArucoMarker__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      aruco_msgs__msg__ArucoMarker__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

aruco_msgs__msg__ArucoMarker__Sequence *
aruco_msgs__msg__ArucoMarker__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aruco_msgs__msg__ArucoMarker__Sequence * array = (aruco_msgs__msg__ArucoMarker__Sequence *)allocator.allocate(sizeof(aruco_msgs__msg__ArucoMarker__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = aruco_msgs__msg__ArucoMarker__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
aruco_msgs__msg__ArucoMarker__Sequence__destroy(aruco_msgs__msg__ArucoMarker__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    aruco_msgs__msg__ArucoMarker__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
aruco_msgs__msg__ArucoMarker__Sequence__are_equal(const aruco_msgs__msg__ArucoMarker__Sequence * lhs, const aruco_msgs__msg__ArucoMarker__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!aruco_msgs__msg__ArucoMarker__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
aruco_msgs__msg__ArucoMarker__Sequence__copy(
  const aruco_msgs__msg__ArucoMarker__Sequence * input,
  aruco_msgs__msg__ArucoMarker__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(aruco_msgs__msg__ArucoMarker);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    aruco_msgs__msg__ArucoMarker * data =
      (aruco_msgs__msg__ArucoMarker *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!aruco_msgs__msg__ArucoMarker__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          aruco_msgs__msg__ArucoMarker__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!aruco_msgs__msg__ArucoMarker__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
