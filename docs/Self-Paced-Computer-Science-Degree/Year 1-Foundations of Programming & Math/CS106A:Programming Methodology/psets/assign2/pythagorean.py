"""
File: pythagorean.py
--------------------
This program asks the user for two values for a and b as floats and then calculates the sulution of c and prints it.
"""

import math


def main():
    """
    Prompt user for a and b and print the third pythagorean tripple.
    Input : a and b
    Ouput: c
    """
    # Pythagorean tripples
    a = 0
    b = 0
    c = 0

    # Prompt for a and b
    a = float(input("a: "))
    b = float(input("b: "))

    # Compute c
    c = math.sqrt((a*a) + (b*b))

    # Print c
    print(f"c = {c}")

# This provided line is required at the end of a Python file
# to call the main() function.
if __name__ == '__main__':
    main()
