def main():
    change = get_change()
    coins = calculate_coins(change)
    print(coins)

    return 0

# Get valid input from the user
def get_change():
    # Ask until user gives valid input
    while True:
        try:
            c float(input("Change owned: "))
            # Get decimal values
            if c >= 0:
                # Convert dollars to cents
                return round(c * 100)
        # Reprompt if user types text
        except ValueError:
            pass

# Greedy coin counting
def calculate_coins(cents):
    coins = 0

    # For coin in denominations
    for value in [25, 10, 5, 1]:
        # Perform integer division
        coins += cents // value
        # Get the remainder
        cents %= value

    return coins

main()



