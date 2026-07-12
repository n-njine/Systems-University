from karel.stanfordkarel import *

def turn_right():
   for i in range(3):
      turn_left()

def turn_around():
    turn_left()
    turn_left()

def main():
    """
    Karel's job is to walk along 1st Street and build a new hospital in the places marked by each beeper. The new hospital should be centered at the point at which the bit of debris was left, which means that the first hospital in the diagram above will be constructed with its left edge along 2nd Avenue, since the beeper was orginally at 3rd Avenue.
    """
    # Move along 1st street towards the wall
    while front_is_clear():
        if beepers_present():
            pick_beeper()
            detect_supplies()
            build_hospital()
        if front_is_clear():
            move()

def detect_supplies():
    """
    Detects supplies along an avenue and moves back one step to start construction
    """
    # Turn 180 degrees, moves a step back and turns facing forward
    turn_around()
    move()
    for i in range(2):
        turn_left()

def build_hospital():
    """
    Build a hospital after getting supplies.
    """
    # Put beeper on first and third avenue
    put_beeper()
    for i in range(2):
        move()
    put_beeper()

    # Move to upper street and place beepers on the three avenues
    left_transition()
    turn_left()
    for i in range(2):
        place_beeper_and_move()
    put_beeper()

    # Move to third street and place beepers on the three avenues
    turn_right()
    move()
    turn_right()
    for i in range(2):
        place_beeper_and_move()
    put_beeper()

    # Move to fourth stree and place beeper on the middle avenue
    for i in range(2):
        left_transition()
    put_beeper()

    # Move down turn left and land on the third beeper of hospital
    turn_left()
    while front_is_clear():
        move()
    turn_left()
    move()


def place_beeper_and_move():
    put_beeper()
    move()
def left_transition():
    turn_left()
    move()

    # Move towards the wall


if __name__ == "__main__":
    run_karel_program()
