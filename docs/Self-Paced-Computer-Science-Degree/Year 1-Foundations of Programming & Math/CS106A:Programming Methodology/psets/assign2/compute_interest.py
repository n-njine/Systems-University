"""
File: compute_interest.py
-------------------------
Compute Amount gained by interest given an initial balance, start year, start month, end year, end month.
"""


def main():
    """
    Calculate amount gained by interest over a period of time
    Inputs:
    Intial balance: 
    Start year: 
    Start month:
    End year:
    End month:

    Outputs:
    Year <Start year>, month <Start month> balance: <Amount>
    """
    # Variables used in Program
    initial_balance = 0
    start_year = 0
    end_year = 0
    end_month = 0
    interest_rate = 0
    amount = 0

    # Get input from the user
    initial_balance = float(input("Initial balance: "))
    start_year = int(input("Start year: "))
    start_month = int(input("Start month: "))
    end_year = int(input("End year: "))
    end_month = int(input("End month: "))

    # Check if Starting date is before the ending date.
    if end_year - start_year < 0:
        print("Starting date needs to be before the ending date.")
        return
    elif end_year == start_year and end_month < start_month:
        print("Starting date needs to be before the ending date.")
        return
    
    # Continuously print balance if interest rate is not equal to false
    while True:
        interest_rate = get_interest_rate()
        if interest_rate == False:
            return
        else:
            get_interest(initial_balance, start_year, start_month, end_year, end_month, interest_rate)

# Get Interest.
def get_interest(initial_balance, start_year, start_month, end_year, end_month, interest_rate):
    """
    Calculate monthly balance on interest gained on initial_balance
    """
    # Do nothing for the first month
    print(f"Year {start_year}, month {start_month} balance: {initial_balance}")

    # Initialize monthly_balance
    monthly_balance = 0

    # Compute monthly amount starting with the next month
    while True:
        # Increment start_month
        start_month += 1

        # Calculate monthly balance
        monthly_balance = initial_balance + calculate_interest(initial_balance, interest_rate)
        # Print amount for the next month
        print(f"Year {start_year}, month {start_month} balance: {monthly_balance}")

        # Update intial_balance
        initial_balance = monthly_balance

        # Shift over to the next year
        if start_month == 12:
            start_year += 1
            start_month = 0

        # End calculation
        if (start_year == end_year) and (start_month == end_month):
            return

# Get interest rate from user and exit if 0
def get_interest_rate():
    interest_rate = float(input("Interest rate (0 to quit): "))
    if interest_rate == 0:
        return False
    return interest_rate

# Calculate interest
def calculate_interest(amount, interest_rate):
    """
    Calculates amount earned by interest.
    Input: amount, interest_rate
    Output: total_amount
    """
    interest = amount * interest_rate
    return interest

# This provided line is required at the end of a Python file
# to call the main() function.
if __name__ == '__main__':
    main()
