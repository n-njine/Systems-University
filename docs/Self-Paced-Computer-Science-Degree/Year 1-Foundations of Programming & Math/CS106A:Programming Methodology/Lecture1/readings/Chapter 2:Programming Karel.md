# Chapter 2:Programming Karel
First Karel program:
```
# File: FirstKarel.py
#--------------------------
# The FirstKarel program defines a "main" function with three commands that cause Karel to move forward one block, pick up a beeper and then move ahead to the next corner.

from karel.standfordkarel import *

def main():
    move()
    pick_beeper()
    move()
```
The program consists of:
    - Comment (#):Explain operations of program to human readers. Ignored by Python
    - Import line:Include all definitions from the karel.stanfordkarel library
    - Function definition:Sequence of steps necessary to respond to a command
    ```
       def main():
           body of the function definition
    ```
        - **def**:Declares new function
        - **main():**:Name of the function
    - Function body:Sequence of commands that Karel will execute in order.

