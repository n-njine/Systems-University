# Chapter 7: If Statements
Conditionals statements include, (if and if/else).

# Basic Conditionals
An if/else statement executes and "if" code-block if and only if the provided **test** is true for the state of the world at the time the program reaches the statement. Otherwise the program executes the "else" code-block.
```
    if test:
        if code-block
    else:
        else code-block
```
Scenario: Write a program that has Karel invert a line of beepers. If a square previously had a beeper, Karel should pick it up. If a square has no beeper, Karel should put one down.
```
from karel.stanfordkarel import *

# The start of the program
def main():
    while front_is_clear():
        invert_beeper()
        move()
    # To prevent a fencepost bug
    invert_beeper()

"""
Picks up a beeper if one is present puts down a beeper otherwise.
"""
def invert_beeper():
    # An if/else statement
    if beepers_present():
        pick_beeper()
    else:
        put_beeper()
```
An if statment does not need to have an else block - it operates like a while loop that only executes one time:
```
    if test:
        if code-block
```

## Conditions
Here is a clear table summarizing all of Karel's conditions:

| Condition          | Opposite               | What it checks                          |
| :----------------: | :---------------------:|:---------------------------------------:|
| `front_is_clear()` | `front_is_blocked()`   | Is there a wall in front of Karel?      |
| `beepers_present()`| `no_beepers_present()` | Are there beepers on this corner?       |
| `left_is_clear()`  | `left_is_blocked()`    | Is there a wall to Karel’s left?        |
| `right_is_clear()` | `right_is_blocked()`   | Is there a wall to Karel’s right?       |
| `beepers_in_bag()` | `no_beepers_in_bag()`  | Does Karel have any beepers in its bag? |
| `facing_north()`   | `not_facing_north()`   | Is Karel facing north?                  |
| `facing_south()`   | `not_facing_south()`   | Is Karel facing south?                  |
| `facing_east()`    | `not_facing_east()`    | Is Karel facing east?                   |
| `facing_west()`    | `not_facing_west()`    | Is Karel facing west?                   |

## Putting it all together
Methods, while loops, for loops, if/else statements work in the same way in almost all major languages, including Python

To a large extent, programming is the science of solving problems by computer. Because problems are often difficult, solutions - and the programs that implement thos solutions can be difficult as well. In order to make it easier for you to develop those solutions, you need to adopt a methodology and discipline that reduces the level of that complexity to a manageable scale.
