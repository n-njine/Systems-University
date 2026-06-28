# Prints a command-line argument

from sys import argv

# Check length of command-line arguments
if len(argv) == 2:
    print(f"hello, {argv[1]}")
else:
    print("hello, world")

