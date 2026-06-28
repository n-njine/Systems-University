# Chapter 6: While Loops
Program that moves Karel to a wall of any world
```
"""
File: MoveToWall.py
----------------------------
Uses a 'while loop' to move Karel until it hits a wall. Works on any sized world
"""

# The program starts with main
def main():
    move_to_wall()

# Move to wall function
    # Repeat the body while the condition holds
    while front_is_clear():
        move()
```
## Basic While Loop
Use: Repeat a body of code as long as a given condition holds

General form:
```
while test:
    statements to be repeated
```
Mechanism:
    - test condition of loop
    - if true, execute body of loop then check test condition
    - if true, execute body of loop until test fails.

**test** statements of Karel:
    - `front_is_clear()`: True if no wall is directly in front of Karel

```
"""
File: Beeperline.py
------------------------
Uses a while loop to place a line of beepers.
This program works for a world of any size
"""
# program starts at main
def main():
    # Repeats until karel faces a wall
    while front_is_clear():
        # Place a beeper on current square
        put_beeper()
        # Move to the next square
        move()
    # Solves the fence but
    put_beeper()
```

