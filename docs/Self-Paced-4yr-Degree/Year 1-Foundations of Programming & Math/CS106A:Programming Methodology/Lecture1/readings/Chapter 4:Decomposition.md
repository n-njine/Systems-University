# Chapter 4: Decomposition
Why:
    Managing a larger problems by breaking it into smaller subproblems

**decomposition**: The process of breaking a program down into smaller pieces.
**subproblems**: The components parts of a problem.

This is achieved by use of functions.

## Filling pothole problem
It can be decomposed into:
    1. Move up to the hole
    2. Fill the hole by dropping a beeper into it
    3. Move on to the next corner

## Implementation of the Karel Program
```
from karel.stanfordkarel import *

def main():
    move()
    fill_pothole()
    move()

"""
Fills the pothole beneath Karel's current position by placing a beeper on that corner. For this function to work correctly, Karel must be facing east immediately above the pothole. When execution is complete, Karel will have returned to the same square and will again be facing east.
"""
def fill_pothole():
    turn_right()
    move()
    put_beeper()
    turn_around()
    move()
    turn_right()

# Turns Karel 90 degrees to the right
def turn_right():
    turn_left()
    turn_left()
    turn_left()

# Turns Karel around 180 degrees.
def turn_around():
    turn_left()
    turn_left()
```

