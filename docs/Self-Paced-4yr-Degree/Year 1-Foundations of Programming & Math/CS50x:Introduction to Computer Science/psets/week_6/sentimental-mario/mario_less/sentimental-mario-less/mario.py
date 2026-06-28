# Recreating a half-pyramid using hashes(#) for blocks

def main():
    height = get_height()
    print_pyramid(height)

    return 0

# Get valid input from user
def get_height():
    # Keep asking until user gives valid input
    while True:
        try:
            h = int(input("Height: "))
            # Only accept heights from 1 to 8
            if 1 <=h <= 8:
                return h
            # Ignore invalid input and keep looping
        except ValueError:
            pass

# Prints the pyramid
def print_pyramid(height):
    # For each row in height
    for i in range(1, height + 1):
        # Count spaces and hashes
        spaces = height - i
        hashes = i
        # Print right aligned hash
        print(" " * spaces + "#" * hashes)


main()
