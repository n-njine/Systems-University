"""
File: random_numbers.py
-----------------------
Add your comments here.
"""

import random


def main():
    """
    A program that prints 10 random integers with value between 0 and 100, inclusive.
    Inputs: None
    Outputs: 10 random integers with values between 0 and 100, inclusive
    """
    # Definition of constants to use
    NUM_RANDOM = 10
    MIN_RANDOM = 0
    MAX_RANDOM = 100

    # Print the random values from 0 to 100 inclusive
    random_val = 0
    for i in range(NUM_RANDOM):
        # For debugging
        # print(f"i is {i}")
        random_val = random.randint(MIN_RANDOM, MAX_RANDOM)
        print(random_val)


# This provided line is required at the end of a Python file
# to call the main() function.
if __name__ == '__main__':
    main()
