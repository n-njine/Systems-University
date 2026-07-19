"""
File: greater_than.py
---------------------
This program gives you practice working with lists in Python.
"""


def greater_than(threshold, num_list):
    """
    This function is passed an integer (threshold) and list of integers (num_list)
    and should return a list containing only those numbers from num_list that
    have a value strictly greater than threshold.
    >>> greater_than(6, [20, 6, 12, -3, 14])
    [20, 12, 14]
    >>> greater_than(15, [16])
    [16]
    >>> greater_than(10, [1, 2, 3, 4])
    []
    >>> greater_than(0, [])
    []
    """
    pass


def main():
    list1 = [20, 6, 12, -3, 14]
    result_list = greater_than(6, list1)
    print(result_list)      # should print [20, 12, 14]

    list2 = [16]
    result_list = greater_than(15, list2)
    print(result_list)      # should print [16]

    list3 = [1, 2, 3, 4]
    result_list = greater_than(10, list3)
    print(result_list)      # should print []

    list4 = []
    result_list = greater_than(0, list4)
    print(result_list)      # should print []


# This provided line is required at the end of a Python file
# to call the main() function.
if __name__ == '__main__':
    main()
