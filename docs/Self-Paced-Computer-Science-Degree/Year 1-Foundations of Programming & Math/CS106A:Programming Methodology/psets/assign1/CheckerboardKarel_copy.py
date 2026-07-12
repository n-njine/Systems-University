from karel.stanfordkarel import *

"""
File: CheckerboardKarel.py
----------------------------
Draws a checkerboard using beepers on any sized grid.
"""

def main():
    put_beeper() # Start with a beeper at (1,1)
    fill_row()
    while left_is_clear():
        transition_to_next_row_left()
        fill_row()
        if right_is_clear():
            transition_to_next_row_right()
            fill_row()
        else:
            # If we can't turn right, we've finished the grid on an eastward pass
            break

def fill_row():
    """
    Moves along a row, placing beepers every two spaces.
    Handles hitting the wall safely.
    """
    while front_is_clear():
        move()
        if front_is_clear():
            move()
            put_beeper()

def transition_to_next_row_left():
    """
    Transitions from facing East to facing West on the row above.
    Maintains the checkerboard pattern depending on if a beeper was placed at the end.
    """
    if beepers_present():
        # If the last spot had a beeper, the next row's start shouldn't have one
        turn_left()
        move()
        turn_left()
    else:
        # If the last spot was empty, the next row's start needs a beeper
        turn_left()
        move()
        turn_left()
        put_beeper()

def transition_to_next_row_right():
    """
    Transitions from facing West to facing East on the row above.
    """
    if beepers_present():
        turn_right()
        move()
        turn_right()
    else:
        turn_right()
        move()
        turn_right()
        put_beeper()

# --- Helper Functions ---

def turn_right():
    for _ in range(3):
        turn_left()

if __name__ == "__main__":
    run_karel_program()
