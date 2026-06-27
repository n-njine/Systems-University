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

    # Turn south and move two steps down
    turn_position(3)
    steps_to_move(1)

    # Turn right and towards beeper
    turn_position(1)
    steps_to_move(3)

    # Collect beeper
    pick_beeper()



def steps_to_move(n):
    for i in range(n):
        move()

def turn_position(n):
    for i in range(n):
        turn_left()


# There is no need to edit code beyond this point

if __name__ == "__main__":
    run_karel_program()
