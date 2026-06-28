# Chapter 5: For Loops

## Basic For Loop
Used when one needs to repeat something a couple of times.

Structure:
```
    for i in range(count):
        statements to be repeated
```
Scenario: You need to place 42 beepers
Normaly:
    - Write 42 lines of `put_beeper()`, very cumbersome.
With for loop:
```
"""
File: PlaceManyBeepers.py
----------------------------
Places 42 beepers using a for loop
"""
from karel.stanfordkarel.py import *

def main():
    move()

    # repeat put_beeper many times
    for i in range(42):
        put_beeper()
    move()
```

## Matching Postconditions with Preconditions
**Precondition**:State of world at the beginning of for loop.

**postcondition**: State of world at end of for loop.

The precondition and postcondition should match to form a perfect cycle that can use a for loop.

## Nested Loops
A for loop with another control structure in it.

