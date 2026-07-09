from karel.stanfordkarel import *

"""
File: StoneMasonKarel.py
------------------------
When you finish writing code in this file, StoneMasonKarel should 
solve the "repair the quad" problem from Assignment 1. You
should make sure that your program works for all of the 
sample worlds supplied in the starter folder.
"""


def main():
    """
    Goal: Repair damage done to the Main Quad.
    Basic Movements:
        move()
        turn_right()
        pick_beeper()
        put_beeper()
    Restrictions:
        Use language features described in 'Karel the Robot Learns Python' no other features of Python (including variables, parameters, break and return).  
    """
    repair_main_quad()

def repair_main_quad():
    """
    Repairs All the columns that are four columns apart
    """
    while front_is_clear():
        fix_one_column()
        move_next_column()
    if front_is_blocked():
        fix_one_column()
        
def move_next_column():
    """
    Moves Karel to the next column
    """
    for _ in range(4):
        move()

def fix_one_column():
    """
    Fixes one column by placing a beeper in a spot where none is present, moving towards the wall
    """
    turn_left()
    collect_beepers()
    turn_around()
    move_to_wall()
    turn_left()

def collect_beepers():
    """
    Collects beepers along a column that Karel is on
    """
    while front_is_clear():
        if no_beepers_present():
            put_beeper()
        move()
    if front_is_blocked():
        if no_beepers_present():
            put_beeper()

def move_to_wall():
    """
    Allows Karel to move to a wall
    """
    while front_is_clear():
        move()

def turn_around():
    """
    Makes Karel turn 180 degrees
    """
    for _ in range(2):
        turn_left()
# There is no need to edit code beyond this point

if __name__ == "__main__":
    run_karel_program()
