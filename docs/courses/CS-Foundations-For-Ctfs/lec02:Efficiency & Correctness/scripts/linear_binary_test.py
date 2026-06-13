#!/usr/bin/env python3

# Measure of worst case by linear and binary search
from linear_binary_algo import linear_search,binary_search
import time, random

# List of 1,000,000 ordered integers
n = 10**6
data = list(range(n))
# Data that not present
target = -1

# Count time taken to run linear search
start = time.time()
linear_search(data, target)
print("Linear:", time.time() - start)

# Count time for binary search
start = time.time()
binary_search(data, target)
print("Binary:", time.time() - start)

