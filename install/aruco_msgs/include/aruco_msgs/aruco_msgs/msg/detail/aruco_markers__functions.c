// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from aruco_msgs:msg/ArucoMarkers.idl
// generated code does not contain a copyright notice
#include "aruco_msgs/msg/detail/aruco_markers__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `markers`
#include "aruco_msgs/msg/detail/aruco_marker__functions.h"

bool
aruco_msgs__msg__ArucoMarkers__init(aruco_msgs__msg__ArucoMarkers * msg)
{
  if (!msg) {
    return false;
  }
  // markers
  if (!aruco_msgs__msg__ArucoMarker__Sequence__init(&msg->markers, 0)) {
    aruco_msgs__msg__ArucoMarkers__fini(msg);
    return false;
  }
  return true;
}

void
aruco_msgs__msg__ArucoMarkers__fini(aruco_msgs__msg__ArucoMarkers * msg)
{
  if (!msg) {
    return;
  }
  // markers
  aruco_msgs__msg__ArucoMarker__Sequence__fini(&msg->markers);
}

bool
aruco_msgs__msg__ArucoMarkers__are_equal(const aruco_msgs__msg__ArucoMarkers * lhs, const aruco_msgs__msg__ArucoMarkers * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // markers
  if (!aruco_msgs__msg__ArucoMarker__Sequence__are_equal(
      &(lhs->markers), &(rhs->markers)))
  {
    return false;
  }
  return true;
}

bool
aruco_msgs__msg__ArucoMarkers__copy(
  const aruco_msgs__msg__ArucoMarkers * input,
  aruco_msgs__msg__ArucoMarkers * output)
{
  if (!input || !output) {
    return false;
  }
  // markers
  if (!aruco_msgs__msg__ArucoMarker__Sequence__copy(
      &(input->markers), &(output->markers)))
  {
    return false;
  }
  return true;
}

aruco_msgs__msg__ArucoMarkers *
aruco_msgs__msg__ArucoMarkers__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aruco_msgs__msg__ArucoMarkers * msg = (aruco_msgs__msg__ArucoMarkers *)allocator.allocate(sizeof(aruco_msgs__msg__ArucoMarkers), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(aruco_msgs__msg__ArucoMarkers));
  bool success = aruco_msgs__msg__ArucoMarkers__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
aruco_msgs__msg__ArucoMarkers__destroy(aruco_msgs__msg__ArucoMarkers * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    aruco_msgs__msg__ArucoMarkers__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
aruco_msgs__msg__ArucoMarkers__Sequence__init(aruco_msgs__msg__ArucoMarkers__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aruco_msgs__msg__ArucoMarkers * data = NULL;

  if (size) {
    data = (aruco_msgs__msg__ArucoMarkers *)allocator.zero_allocate(size, sizeof(aruco_msgs__msg__ArucoMarkers), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = aruco_msgs__msg__ArucoMarkers__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        aruco_msgs__msg__ArucoMarkers__fini(&data[i - 1]);
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
aruco_msgs__msg__ArucoMarkers__Sequence__fini(aruco_msgs__msg__ArucoMarkers__Sequence * array)
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
      aruco_msgs__msg__ArucoMarkers__fini(&array->data[i]);
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

aruco_msgs__msg__ArucoMarkers__Sequence *
aruco_msgs__msg__ArucoMarkers__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aruco_msgs__msg__ArucoMarkers__Sequence * array = (aruco_msgs__msg__ArucoMarkers__Sequence *)allocator.allocate(sizeof(aruco_msgs__msg__ArucoMarkers__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = aruco_msgs__msg__ArucoMarkers__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
aruco_msgs__msg__ArucoMarkers__Sequence__destroy(aruco_msgs__msg__ArucoMarkers__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    aruco_msgs__msg__ArucoMarkers__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
aruco_msgs__msg__ArucoMarkers__Sequence__are_equal(const aruco_msgs__msg__ArucoMarkers__Sequence * lhs, const aruco_msgs__msg__ArucoMarkers__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!aruco_msgs__msg__ArucoMarkers__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
aruco_msgs__msg__ArucoMarkers__Sequence__copy(
  const aruco_msgs__msg__ArucoMarkers__Sequence * input,
  aruco_msgs__msg__ArucoMarkers__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(aruco_msgs__msg__ArucoMarkers);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    aruco_msgs__msg__ArucoMarkers * data =
      (aruco_msgs__msg__ArucoMarkers *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!aruco_msgs__msg__ArucoMarkers__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          aruco_msgs__msg__ArucoMarkers__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!aruco_msgs__msg__ArucoMarkers__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
