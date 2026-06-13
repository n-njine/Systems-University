#!/usr/bin/env python3

# Implementation of linear search:Iterate through the array comparing arr[i] with target
def linear_search(arr, target):
    # Search list one element at a time
    for i in range(len(arr)):
        if arr[i] == target:
            return i
        return -1

# Implementation of binary search:Each iteration of sorted list halves the search space
def binary_search(arr, target):
    # Set low and high end points of array
    low, high = 0, len(arr)-1

    # Set mid-point of array in loop so that it updates through each iteration
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1
    return -1

