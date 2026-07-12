"""
File: moon_weight.py
--------------------
A program that asks the user for their weight and prints out their weight on the moon
A persons weight is 16.5% of their weight on earth.
"""

def main():
    """
    Ask a user for their weight and prints it if they were in the moon.
    Input: User's weight
    Ouput: Weight on moon
    """
    # Used values
    ACTUAL_WEIGHT = 0
    MOON_VAL = 0
    PERCENT_WEIGHT = 0.165

    # Prompt user for weight.
    ACTUAL_WEIGHT = float(input("Enter you weight: "))
    if ACTUAL_WEIGHT < 0:
        print("Sorry, you can't have a negative weight")
        return


    # Compute weight on moon
    MOON_VAL = ACTUAL_WEIGHT * PERCENT_WEIGHT

    # Print weight on moon
    print(f"Your weight on the moon is {MOON_VAL}")


# This provided line is required at the end of a Python file
# to call the main() function.
if __name__ == '__main__':
    main()
