from karel.stanfordkarel import *

"""
File: CheckerboardKarel.py
----------------------------
When you finish writing it, CheckerboardKarel should draw
a checkerboard using beepers, as described in Assignment 1. 
You should make sure that your program works for all of the 
sample worlds supplied in the starter folder.
"""


def main():
    """
    Creating a Checkerboard in any given m by n grid
    """
    # Fill Beeper in one row
    beeper_row_one()
    move()
    turn_left()
    move()
    turn_right()
    beeper_row_one()
    move()

def turn_right():
    """
    Makes Karel face right
    """
    for _ in range(3):
        turn_left()

def beeper_row_one():
    """
    Places a beeper on the first row of grid.
    """
    turn_left()
    place_beeper()
    turn_around()
    move_to_wall()
    turn_left()

def place_beeper():
    """
    Intermittently places beepers on Checkerboard
    """
    put_beeper()
    while front_is_clear():
        place_beeper_stepwise()
        

def place_beeper_stepwise():
    """
    Places beeper after a step
    """
    move()
    if front_is_clear():
        move()
        put_beeper()

def turn_around():
    """
    Turns Karel 180 degrees
    """
    for _ in range(2):
        turn_left()

def move_to_wall():
    """
    Moves Karel back to where he came form
    """
    while front_is_clear():
        move()

# There is no need to edit code beyond this point

if __name__ == "__main__":
    run_karel_program()
