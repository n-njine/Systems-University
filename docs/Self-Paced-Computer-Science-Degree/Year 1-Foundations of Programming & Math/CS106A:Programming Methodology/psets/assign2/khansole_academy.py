"""
File: khansole_academy.py
-------------------------
Khansole Academy: A program that helps other people learn! It randomly generates **additional problems** for the user, reads in the answer from the user, and then checks to see if they got it right or wrong, until the user appears to have mastered the material.
"""

import random

# Global Constants
SENTINEL = 3
MIN_VAL = 10
MAX_VAL = 99

def main():
    """
    Randomly generate addition problems for the user, reads in the answer from the user and then checks to see if they got it right or wrong.
    Input: Answer from the user
    Ouput: Correct answer from the user
    """
    # Tracker of how many values are gotten correct
    counter = 0

    while SENTINEL != counter:
        num_1 = generate_random(MIN_VAL, MAX_VAL)
        num_2 = generate_random(MIN_VAL, MAX_VAL)
        result = num_1 + num_2
        print(f"What is {num_1} + {num_2}")
        user_answer = int(input("Your answer: "))
        if user_answer == result:
            counter += 1
            print(f"Correct! You've gotten {counter} correct in a row")
        else:
            print(f"Incorrect. The expected answer is {result}")


def generate_random(min_val, max_val):
    """
    Random number generator.
    Input: min and max values
    Ouput: random number
    """
    secret_val = random.randint(min_val, max_val)
    return secret_val
    


# This provided line is required at the end of a Python file
# to call the main() function.
if __name__ == '__main__':
    main()
