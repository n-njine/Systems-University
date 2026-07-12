"""
File: subtract_numbers.py
-------------------------
Add your comments here.
"""


def main():
    """
    A program that asks for two values from the user and prints the difference in their values.
    input: 2 values
    output: the difference of the numbers.
    """
    # Where to place the two numbers
    first_num = 0
    second_num = 0
    result = 0

    # Ask for values from the user and get their result
    print("This program subracts one number from another.")
    first_num = float(input("Enter first number: "))
    second_num = float(input("Enter second number: "))

    # Calculate the result
    result = first_num - second_num

    # Print result
    print(f"The result is {result}")

# This provided line is required at the end of a Python file
# to call the main() function.
if __name__ == '__main__':
    main()
