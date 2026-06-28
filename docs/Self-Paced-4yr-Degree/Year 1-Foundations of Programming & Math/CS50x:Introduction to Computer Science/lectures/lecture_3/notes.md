# Lecture 3
        
## Linear Search
        + Problem: Finding a number behind an array:
        + Algorithm:
        + Pseudocode:
                For each door from left to right
                        If 50 is behind door
                                Return true
                Return false

        + Computer Scientist version:
                For i from 0 to n-1
                        If 50 is behind doors[i]
                                Return true
                Return false
                
## Binary Search
        + Prerequisite: Array is arranged from smallest to largest
        + Pseudocode:
                If no doors left
                        Return false
                if 50 is behind middle door
                        Return true
                Else if 50 < middle door
                        Search left half
                Else if 50 > middle door
                        Search right half

        + code nomenclature:
                If no doors left
                        Return false
                If 50 is behind doors[middle]
                        Return true
                Else if 50 < doors[middle]
                        Search doors[0] through doors[middle - 1]
                Else if 50 > doors[middle]
                        Search doors[middle + 1] through doors[n - 1]

## Structs
- You can create your 'own' data type via a *struct*.
- Struct syntax:
        ```C
        typedef struct
        {
                string name;
                string number;
        }person;
        ```
- This represents our own datatype called a *person* that has a string called *name* and another string called *number*.
- A *dot notation* such as 'people[0].name', allows us to access the person at the 0th location and assign that individual a name.

## Sorting.
- Def:
        - Taking an *unsorted* list of values and changing it into a *sorted one*.
- Searching a sorted list is less taxing than an unsorted one.
- Sorting algorithms:

### Selection Sort
        + Algorithm for selection sort in pseudocode is;
                For i from 0 to n-1
                        Find smallest number between numbers[i] and numbers[n-1]
                        Swap smallest number with numbers[i]
        + How it works:
        - The first time iterating through the list took n - 1 steps. The second time, it took n - 2 steps. Carrying this logic forward, the steps required could be represented as:
        (n - 1) + (n - 2) + (n - 3) + ... + 1
        - Its simplified to n(n-1)/2, or O(n2).
        + In the worst-case or upper-bound, selection sort is in the order of O(n2).In the best-case, or lower-bound, selection sort is in the order of Omege(n2)

### Bubble Sort
        + How it works:
                - Repeatedly swap elements to "bubble" larger elements to the end.
        + Pseudocode for bubble sort is:
                ```C
                Repeat n-1 times
                        For i from 0 to n-2
                                If numbers[i] and numbers[i+1] out of order
                                        swap them
                                If no swaps
                                        Quit
                ```
        + Analysis:
                (n - 1) x (n - 1)
                n2 - 1n - 1n + 1
                n2 - 2n + 1
        - Running time? O(n2)

### Recursion
        - Def:
                - A function calls itself. Example
        ```Pseudocode
        If no doors left
                Return false
        If number behind middle door
                Return true
        Else if number < middle door
                Search left half
        Else if number > middle door
                Search right half
        ```
        - Pseudocode for week 0,
                1 Pick up phone book
                2 Open to middle of phone book
                3 Look at page
                4 If person is on page
                5   Call person
                6 Else if person is earlier in book
                7   Open to middle of left half of book
                8   Go back to line 3
                9 Else if person is later in book
                10  Open to middle of right half of book
                11  Go back to line 3
                12 Else 
                13  Quit

        - Highlight its recursive properties.
                1. Pick up the phonebook
                2. Open to middle of phone book
                3. Look at page
                4. If person is on page
                5.      Call person
                6. Else if person is earlier in book
                7.      Search left half of book
                8. Else if person is later in book
                9.      Search right half of book
                10.Else
                11.     Quit


### Recursion
        + Def:
                A function calls itself
        + Properties:
                - *base case*: Terminate and ensure code does not run forever
        - *recursive step*: Step to be done repeatedly.
        - In iteration.c: The base case ensures the code does not run forever. The line if (n <= 0) terminates the recursion because the problem has been solved. Every time, draw calls itself, it calls by n-1. At some point, n-1 will equal 0, resulting in the *draw* function returning, and the program will end.

### Merge Sort
        + Incorporates recursion in implementing its algorithm
        ```Pseudocode
        If only one number
                Quit
        Else
                Sort left half of number
                Sort right half of number
                Merge sorted halves
        ```
        + Running time:
                - worst-case O(nlog n)
                - best-case Omega(n log n) : The alogorithm must visit each place in the list.
                + It's also Theta(n log n) since the best-case and the worst-case are the same.
