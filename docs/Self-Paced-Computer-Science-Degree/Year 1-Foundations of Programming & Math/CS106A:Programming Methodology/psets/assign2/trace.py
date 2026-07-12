"""
This program is meant to test your understanding of variables, control flow, and logical operators
"""
def main():
    x = 0
    y = 0
    z = 0

    while x < 10:
        if (x % 2 == 0) or (x == 7):
            if x != 4:
                y += x
            else:
                z += 1
        elif x == 5:
            y += 5
        else:
            z + 1 # Pay close attention to this line!

        if x == 8:
            break
        
        x += 1

    # Print the results
    print(f"x = {x}")
    print(f"y = {y}")
    print(f"z = {z}")

if __name__ == "__main__":
    main()

