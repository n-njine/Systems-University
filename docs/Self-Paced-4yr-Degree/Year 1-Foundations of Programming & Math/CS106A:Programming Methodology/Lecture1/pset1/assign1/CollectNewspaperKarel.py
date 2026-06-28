from karel.stanfordkarel import *

"""
File: CollectNewspaperKarel.py
------------------------------
At present, the CollectNewspaperKarel file does nothing.
Your job in the assignment is to add the necessary code to
instruct Karel to walk to the door of its house, pick up the
newspaper (represented by a beeper, of course), and then return
to its initial position in the upper left corner of the house.
"""


def main():
    """
    Move Karel to collect newspapers
    Basic motions:
        - move()
        - turn_left()
        - put_beeper()
        - pick_beeper()
    Goal: Use the basic motions to move Karel to pick beeper by either the basic motions or creating one for yourself
    """
    # 1. Move to newspaper
    move_to_newspaper()

    # 2. Pick newspaper
    pick_beeper()

    # 3. Return to starting point
    return_to_start()

# Steps
# 1. Move to the newspaper
def move_to_newspaper():
    # Move 2 steps forward
    for i in range(2):
        move()

    # Turn south
    for i in range(3):
        turn_left()

    # Move to newpaper
    move()
    turn_left()
    move()

# 3. Return to starting point
def return_to_start():
    # Turn backwards
    for i in range(2):
        turn_left()

    # Move three steps and turn up
    for i in range(3):
        move()
    for i in range(3):
        turn_left()

    # Move to start position
    move()
    for i in range(3):
        turn_left()
# There is no need to edit code beyond this point

if __name__ == "__main__":
    run_karel_program()
