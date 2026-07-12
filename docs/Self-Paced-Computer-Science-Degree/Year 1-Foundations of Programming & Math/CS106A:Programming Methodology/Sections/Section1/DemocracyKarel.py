from karel.stanfordkarel import *

def turn_right():
   for i in range(3):
      turn_left()

def turn_around():
    turn_left()
    turn_left()

def main():
   pass


if __name__ == "__main__":
    run_karel_program()
