# Lecture 6
What to cover:
    - Hello Python!
    - Speller
    - Filter
    - Functions
    - Libraries, Modules, and Packages
    - Strings
    - Positional Parameters and Named Paremeters
    - Variables
    - Types
    - Calculator
    - Conditionals
    - Object-Oriented Programming
    - Loops
    - Abstraction
    - Truncation and Floating Poing Imprecision
    - Exceptions
    - Mario
    - Lists
    - Searching and Dictionaries
    - Command-Line Arguments
    - Exit Status
    - CSV Files
    - Third-Party Libraries

## Hello Python!
- Unlike C which is a *compiled language*, Python is an **interpreted language**, Where you run program in the *Python Interpreter*.

- A program in C:
```
#include <stdio.h>

int main(void)
{
    // Print hello, world
    printf("Hello, world\n");

    return 0;
}
```
- A program in Python:
```python
print("hello, world!")
```

- In dictionary.py
    - There are four functions:
        - `check()`: If a word is in words, it returns True
        - `load()`: The dictionary file is open. For each line in that file, we add the line to `words`. 
                    Using `rstrip`, the trailing new line is removed from the added word.
        - `size()`: Returns the `len` or length of `words`.
        - `unload()`: Needs to return `True` as `Python` handles memory management on its own.

- Why higher-level languages exist: To simplify and allow you to code more easily.
- Speed is a tradeoff. As C allows you, the programmer to make decisions about memory management, it may run faster than Python.

## Filter
- In blur.py:
    - We import modules `Image` and `ImageFilter` from a library called PIL. This takes an input file and creates an output file.

In edges.py:
    - The code is a small adjustment to `blur` but produces a dramatically different result.

## Functions
- In C:
```
printf("hello, world\n");
```
- In Python:
```
print("hello, world)
```

## Libraries, Modules, and Packages
- Have CS50 library in Python
- Functions will include:
```
    get_float
    get_int
    get_string
```
- Importing cs50 library:
```
import CS50
```
- Importing specific functions:
```
from CS50 import get_float, get_int, get_string
```

## Strings
- In C:
```
// Get string and print using %s

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    string name = get_string("What's your name? ");
    printf("hello, %s\n", name);

    return 0;
}
```
- In Python:
```
# get_string and print, with concatenation

from cs50 import get_string

answer = get_string("What's your name? ")
print("hello " + answer)
```

```
# get_string and print, without concatenation

from cs50 import get_string

answer = get_string("What's your name? ")
print("hello", answer)
```
- In `without contatenation` the print statement automatically creates a space.
- Using format strings:
```
# get_string and print, with format strings

from cs50 import get_string

answer = get_string("What's your name? ")
print(f"hello, {answer}")
```
- The curly braces allow for the `print` function to interpolate the `answer` such that answer appears within.
- The `f` is required to include the `answer` properly formatting.

## Positional Parameters and Named Parameters
- Functions in C like `fread`, `fwrite`, and `printf` use postional arguments, where you provide arguments with commas as separators. You, the programmer, must remember where what argument is in which postion. These are called **positional arguments**.

- In Python, **named parameters** allow you to provide arguments without regard to positionality.

- From the documentation print has:
```
print(*objects, sep=' ', end='\n', file=None, flush=None)
```
    - A separator of single space is provided that will display when more than one object is given to print.
    - A newline is provided at the end of the `print` statement.

## Variables
- In C:
```
int counter = 0;
```
- In Python:
```
counter = 0
```
- Incrementing:
```
counter += 1
```
- Losing the C's ability to type:
```
counter++;
```

## Types
- Data types in Python don't need to be explicitly declared.

- Common data types:
    bool
    float
    int
    str

- `Long` and `double` are missing. Python handles what data type should be used for larger and smaller numbers.

- Other data types:
```
    range   sequence of numbers
    list    sequence of mutable values
    tuple   sequence of immutable values
    dict    collection of key-value pairs
    set     collection of unique values
```

## Calculator
- Earlier had calculator.c
```calculator.c
// Addition with int

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for x
    int x = get_int("What's x: ");

    // Prompt user for y
    int y = get_int("What's y: ");

    // Perform addition
    printf("Sum is %i\n", x + y);

    // Exit successfully
    return 0;
}
```
- Calculator in Python
```calculator.py
# Addtion with int [using get_int]

from cs50 import get_int

# Prompt user for x
x = get_int("What's x: ")

# Prompt user for y
y = get_int("What's y: ")

# Perform addition
print(x + y)

# Exit successfully
return 0
```

```calculator.py
# Addition with int[using input]

# Prompt for x
x = int(input("What's x: "))

# Prompt for y
y = int(input("What's y: "))

# Perform addition
print(x + y)
```
- Input takes values as strings, you have to cast them to int() to convert the to integers.

## Conditionals
- In C:
```
// Conditionals, Boolean expressions, relational operators

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt users for x and y
    int x = get_int("What's x: ");
    int y = get_int("What's y: ");


    // Compare integers using if, else if and else
    if (x < y)
    {
        printf("x is less than y\n");
    }
    else if (x > y)
    {
        printf("x is greater than y\n");
    }
    esle
    {
        printf("x is equal to y\n");
    }

    return 0;
}
```

In Python:
```
# Conditional, Boolean expressions, relational operators

from cs50 import get_int

# Prompt users for integers
x = get_int("What's x: ")
y = get_int("What's y: ")

# Compare integers
if x < y:
    print("x is less than y")
elif x > y:
    print("x is greater than y")
else:
    print("x is equal to y")
```
- Difference from C:
    - No more curly braces, instead have indentations
    - A colon is utilized in the if statement.
    - `elif` replaces `else if`.
    - Parenthesis are no longer required in the if statements

- Looking at logical operators
```agree.c
// Logical operators

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user to agree
    char c = get_char("Do you agree: ");

    // Check whether agreed
    if (c == 'Y' || c == 'y')
    {
        printf("Agreed\n");
    }
    else if (c == 'N' || c == 'n')
    {
        printf("Not Agreed\n");
    }
}
```
In Python:
```agree.py
# Logical operators

from cs50 import get_string

# Prompt user to agree
c = get_string("Do you agree? ")

# Check whether agreed
if c == 'Y' or c == 'y':
    print("Agreed")
elif c == 'N' or c == 'n':
    print("Not agreed")
```
- Difference from C:
    - Two vertical bars in C are replaced with or
    - Char does not exist in Python, instead `str`s are utilized.

- Using lists:
```agree.py
# Logical operators, using  lists

from cs50 import get_string

# Prompt user to agree
c = get_string("Do you agree? ")

# Check whether agreed using lists
if c in ['Y', 'y']:
    print("Agreed")
elif c in ['N', 'n']:
    print("Not Agreed")
```

## Object Oriented Programming.
- Object: Certain types of values not only have properties or attributes inside of them but have functions as well.

- In C, we could create a `struct` where you could associate multiple variables inside a single self-created data type. In Python, you can do this and also include functions in a self-created data type.**When a function belongs to a specific object, it is known as a method**

- Method: Function belonging to specific object.

- `strs` in Python have built-in methods:
```
# Logical operators, using lists

# Prompt user to agree
s = input("Do you agree? ").lower()

# Check whether agreed
if s in ['y', 'yes']:
    print("Agreed")
elif s in ['n', 'no']:
    print("Not Agreed")

```
- The old value of `s` is overwritten with the result of `s.lower()`, a built-in method of `strs`.

- Coping string in C:
```
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    // Get a string
    char *s = get_string("s: ");
    if (s == NULL)
    {
        return 1;
    }

    // Allocate memory for another string
    char *t = malloc(strlen(s) + 1);
    if (t == NULL)
    {
        return 1;
    }

    // Copy string into memory
    strcpy(t, s);

    // Capitalize copy
    if (strlen(t) > 0)
    {
        t[0] = toupper(t[0]);
    }

    // Print strings
    printf("s: %i\n", s);
    printf("t: %i\n", t);

    // Free memory
    free(t);
    return 0;
}
```
- Notice the number of lines of code

- In Python:
```capitalize.py
# Capitalized a copy of a string

# Get a string
s = input("What's s: ")

# Capitalize copy of string
t = s.capitalize()

# Print strings
print(f"s: {s}")
print(f"t: {t}")
```

## Loops
- Loops in Python are similar to C.
```meow.c
// Demonstrates for loop

#include <stdio.h>

int main(void)
{
    for (int i = 0; i < 3; i++)
    {
        printf("meow\n");
    }
}
```
- `for` loops in Python are:
```
for i in range(3):
    print("meow")
```
- Here, i is never explicitly used.

- Implementation of `while` loop:
```
i = 0
while i < 3:
    print("meow")
    i += 1
```
- For further understanding:
```uppercase.py
# Uppercases string one character at a time

# Get input from user
before = input("Before: ")

# Print converted character
print("After: ", end="")

# For character in string, convert to uppercase
for c in before:
    print(c.upper(), end="")
print()
```
- Here `end=""` passes a parameter to the `print` function that continues the line without a line ending. This code passes one string at a time

- Python has methods that can be implemented upon entire strings
```uppercase.py
# Uppercases string all at once

# Get input
before = input("Before: ")

# Uppercase all characters
after = before.upper()

# Print final result
print(f"After: {after}")
```
## Abstraction
- Code can be improved using functions and abstracting away various code into functions.
```meow.py
# Abstraction

def main():
    for i in range(3):
        meow()

    return 0

# Meow once
def meow():
    print("meow")

# Call main
main()
```

- The meow function abstracts away the `print` statement. The main function appears at the top of the file. At the bottom of the file, the `main` function is called. 

- We can pass variables between our functions
```meow.py
# Abstraction with parametization

def main():
    meow(3)

# Meow some number of times
def meow(n):
    for _ in range(n):
        print(meow)

# Call main
main()
```
- In the above, meow takes in a variable n. In the `main` function, you can call `meow` and pass a value like 3 to it. Then `meow` utilizes the value of n in the for loop.


## Truncation and Floating Point Imprecision
- In C integer division results in imprecise results.
- In Python:
```calculator.py
# Division with integers, demonstration lack of trancation

# Prompt the user for x
x = int(input("x: "))

# Prompt the user for y
y = int(input("y: "))

# Divide x by y
z = x / y
print(f"{z:.50f}")
```

## Exceptions
- Explore more about exceptions that can occur when running Python code
```calculator.py
# Doesn't handle exception

# Prompt user for an integer
n = int(input("Input: "))
print("Integer")
```
- Inputting the wrong data could result in an error
- We can `try` and `catch` potential exceptions by modifying the code as follows:
```
# Handles exception

# Prompt user for an integer
try:
    n = int(input("Input: "))
    print("Integer.")
except ValueError:
    print("Not integer.")
```

## Mario.
- Implementing 3 vertical blocks of bricks.
```
# Prints a column of 3 bricks with a loop

for i in range(3):
    print("#")

```
- Python doesn't have a `do-while` loop. You have to utilize a while loop.
```mario.py
# Prints a column of n bricks with a loop

from cs50 import get_int

# Get height from user
while True:
    n = get_int("Height: ")
    if n > 0:
        break

# Print column of n bricks
for i in range(n):
    print("#")
```
- Printing a horizontal row of "?" bricks
```
# Prints a row of 4 question marks with a loop

for i in range(4):
    print("?", end="")

print()
```
- You can override the behavior of the `print` function to stay on the same line as the previous print.
- On simplification
```
# Prints a row of 4 question marks without a loop

print("?" * 4)
```
The `*` multiplies the print statement to repeat 4 times.

- Implementing a n by n brick
```
# Prints a 3-by-3 grid of bricks with loops

# Get input from the user
n = int(input("Height: "))

# Print 3-by-3 grid of bricks
for i in range(n):
    for j in range(n):
        print("#", end="")
    print()
```

## Lists
- `list`s are a data structure within Python
- `list`s have built-in methods or functions within them
```average.py
# Averages three numbers using a list

# Scores
scores = [72, 73, 33]

# Print average
average = sum(scores) / len(scores)
print(f"Average: {average}")
```
- Getting values from the user
```
# Averages three numbers using a list and a loop

from cs50 import get_int

# Get scores
scores = []
for i in range(3):
    score = int(input("Score: "))
    scores.append(score)

# Print average
average = sum(scores) / len(scores)
print(f"Average: {average}")
```

## Searching and Dictionaries
- Searching within a data structure
```phonebook.py
# Implements a linear search for names using loop

# A list of names
names = ["Yuliia", "David", "John"]

# Ask for name
name = input("Name: ")

# Search for name
for n in names:
    if name == n:
        print("Found")
        break
else:
    print("Not found")
```
- We don't need to iterate through a list. We can execute linear search as follows:
```
# Implements linear search for names using `in`

# A list of names
names = ["Yullia", "David", "John"]

# Ask for name
name = input("Name: ")

# Search for name
if name in names:
    print("Found")
else:
    print("Not found")
```
- A `dictionary` or `dict` is a collection of `key` and `value` pairs.
- Implementing a dictionary in Python as:
```
# Implements a phone book as a list of dictionaries, without a variable

from cs50 import get_string

# A dictionary of people
people = [
    {"name": "Yuliia", "number": "+1-617-495-1000"},
    {"name": "David", "number": "+1-617-495-1000"}
    {"name": "John", "number": "+1-949-468-2750"}
]

# Search for name
name = get_string("Name: ")
for person in people:
    if person["name"] == name:
        print(f"Found {person['number']})
        break
else:
    print("Not found")
```
- The dictionary is implemented having both `name` and `number` for each entry.
- We can simplify the code as:
```
# Implements a phone book using a dictionanry

from cs50 import get_string

people = {
    "Yuliia": "+1-617-495-1000",
    "David": "+1-617-495-1000",
    "John": "+1-949-468-2750",
}

# Search for name
name = get_string("Name: ")
if name in people:
    print(f"Number: {people[name]}")
else:
    print("Not found")
```
- What we have:
    - Dictionary's implemented using curly braces
    - `if name in people` searches to see if `name` is in the `people` dictionary.
    - You can index into the dictionary using `{people[name]}`.

- Python has done their best to get to `constant time` using their built-in searches.

## Command-Line Arguments
- Utilizing commandline arguments
```
# Prints a command-line argument

from sys import argv

if len(argv) == 2:
    print(f"hello, {argv[1]")
else:
    print("hello, world")
```
- `argv[1]` is printed using a `formatted string`, noted by the `f` present in the print statement.

- You can print all the arguments in `argv` as follows:
```
# Printing command-line arguments, indexing into argv

from sys import argv

for i in range(len(argv)):
    print(argv[i])
```
- The above will not present the word `python` if executed, and the first argument will be the name of the file you are running. You can think of the word `python` as being analogous to `./` when we are running programs in C.

## Exit Status
- The `sys` library also has built-in methods. 
- Use `sys.exit(i)` to exit the program with a specific exit code.
```
# Exits with explicit value, importing sys

import sys

if len(sys.argv) != 2:
    print("Missing commnad-line argument")
    sys.exit(1)

print(f"hello, {sys.argv[1]}")
sys.exit(0)
```
- The dot-notation is used to utilize the built-in funcitons of `sys`.

## CSV Files
- Python has built-in support for CSV files.
```phonebook.py
import csv

# Open file
file = open("phonebook.csv", "a")

# Get name and number from user
name = input("Name: ")
number = input("Number: ")

# Write to csv file
writer = csv.writer(file)
writer.writerow([name, number])

# Close file
file.close()
```
- `writerow` adds the commas in the CSV file for us.
- As `file.close` and `file = open` are used in Python, the code can be improved.
```
import csv

# Get name and number
name = input("Name: ")
number = input("Number: ")

# Open file, write to file and close when done
with open("phonebook.csv", "a") as file:
    writer = csv.writer(file)
    writer.writerow([name, number])

```
- The code is indented under the `with` statement. This automatically closes the file when done.
- You can write a dictionary as follows:
```
import csv

# Get name and number
name = input("Name: ")
number = input("Number: ")

# Write to csv file
with open("phonebook.csv", "a") as file:
    writer = csv.DictWriter(file, fieldnames=["name", "number])
    writer.writerow({"name": name, "number": number})

```
## Third-Party Libraries
- Advantage of Python: Massive user base and third-party libraries.
- Install cs50 library
```
$ pip install cs50
```

## Summing Up
- What was learnt:
    - Implementing building blocks of prior lessons.
    - How Python allows for more simplified code
    - Utilize Python libraries

- What was discussed:
    - Python
    - Variables
    - Conditionals
    - Loops
    - Types
    - Object-Oriented programming
    - Truncation and floating point imprecision
    - Exceptions
    - Dictionaries
    - Command-line arguments
    - Third-Party libraries




