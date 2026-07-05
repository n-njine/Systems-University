# Prints a column of n bricsk with a loop

# Get height from the user
while True:
    n = int(input("Height: "))
    if n > 0:
        break

# Print column of n bricks
for i in range(n):
    print("#")
