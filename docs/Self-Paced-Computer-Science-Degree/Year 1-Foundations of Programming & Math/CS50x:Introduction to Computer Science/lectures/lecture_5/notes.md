# Lecture 5

- What to do:
        - Data Structures
        - Queues
        - Stacks
        - Resizing arrays
        - Arrays
        - Linked Lists
        - Trees
        - Dictionaries
        - Hashing and Hash Tables
        - Tries

## Data Structures.
+ Def:
        - Form of organisation in memory

+ Abstract data type:
        - One you conceptually imagine.

## Queues.
+ Properties:
        - FIFO ie "first in first out"
                e.g Ride to amusement park or voting line.

```C
const int CAPACITY = 50;

typedef struct
{
        person people[CAPACITY];
        int size;
}
queue;
```
- What it is:
        - An array called *people* is of type *person*
        - Capacity *How high* the *stack* could be.
        - Integer *size*: How full the queue actually is, regardless of how much it can *hold*

## Stacks
+ Properties:
        - LIFO ie "last in first out"
                - Stack of pancakes.
+ Actions:
        - push : place on top of stack
        - pop  : remove from top of stack

```C
const int CAPACITY = 50;

typedef struct
{
        person people[CAPACITY];
        int size;
}
stack;
```
+ What it means:
        - Array named people is of type person
        - *CAPACITY* How high the stack could be
        - Integer *size* How full the stack actually is, regardless of how much it could hold.
+ Limitations:
        - Misuse of memory. Either oversized or underutilized.

## Resizing Arrays.
+ Def:
        Array: Block of contiguous memory.

+ Have 3 values in memory and wanted to add a fourth one, create new area of memory and include the fourth one.

+ Drawbacks:
        - Bad design. Why: To create new area of memory, we have to copy items by items.

## Arrays.
- Want to put the 4 somewhere else in memory.

+ Connect different locations in memory?

```text
# In list_dynamic.c
        - Create list of size 3 of integers
        - Assign 3 memory addresses values 1, 2 and 3.
        - Create list of size 4
        - Copy list from the first to the second.
        - Add value of 4 to tmp list.
        - Free memory block that list points to 
        - Point list to memory block that *tmp* points to.
        - Print contenst of list
        - Free list

+ Also have:
        - #include <stdlib.h>

+ In end, both list and tmp pointed to same block.

+ You can implement without using for loop to copy block of code using *realloc*.

```

+ Note:
        - Don't allocate more memory than needed as it's taxing on systems.

## Linked Lists.
+ Usefull primitives:
        - struct - Allows defining of own data type
        - *.* - Dot notation, Allows accessing data in a structure
        - `*` - Operator, Allows declaration of pointes or dereference a variable.
        - `->` - arrow, Goes to an address and looks inside a structure.

+ Linked list:
        - Allows you to include values located in varying areas of memory and to  dynamically grow and shrink the list.

+ Have 3 values in different areas in memory:
        - How to stitch them together
                - Utilize more memory to keep track of next item.
                - 1st item will hold memory of 2nd item
                - Last item will have nothing ie NULL.
                - Keep one element in memory to keep track of first item in list. Item's called *head of list*
        - What keeps track of items in linked list is *node*
                - Contains *item* and pointer called *next*

```C
// Node for a linked list
typedef struct node
{
        int number;
        struct node *next;
}
node;
```
        - What's in this node:
                - integer called number
                - pointer to node called *next*. It will point to another node somewhere in memory.

```text
# In linked_list/list.c
        - Define node as struct
        - For each element in list, allocate memory to size of node
        - [n->number] or (n's number field) is assigned an integer.
        - [n->next] or (n's next field) is assigned null.
        - Node's placed at start of list at memory location *list*.
```
+ In `list_print.c`
        + node *ptr = list*, creates a temporary variable that points at the same spot that *list* points to.
        + The `while` prints what at the node *ptr* points to, and then updates *ptr* to point to the *next* node in the list.

+ Time complexity of searching list.

+ Cons of linked list:
        - Require more resources to keep track of.
        - Can't be indexed into like an array, require linear search to get elements in list.


## Trees
- Binary search trees:
        - Store data more efficiently so it can be searched and retrieved.

- Structure: Sorted array of numbers
        - Center becomes top of tree
        - Value less than the centre go to left and more, go to right
        - Pointers are used to point to the correct location of memory for each. To connect the nodes.

- In tree.c:
        - Search function begins by going to location of tree
        - Use `recursion` to search for `number`
        - `free_tree` function recursively frees the tree. 
        - `print_tree` recursively prints the tree.

+ Note:
        - Offers dynamism
        - Search time of O(logn) when balanced.

## Dictionaries
- Have a key and value.

- *Holy grail* of algorithmic time complexity is O(1) or `constant time`    
- Can offer through hashing

## Hashing and Hash Tables.
- Def:
        Hashing: Take value and output a value that's a shortcut later. 
- How it works:
        - Hash `apple` to value of 1: Finding apple is asking for hash algorithm where `apple` is stored.

+ Hash function:
        - Algorithm that reduces a larger value to something small and predictable.
        - It takes an item to be added to hash table and returns an integer representing the array index in which the item should be placed.

- Hash table:
        - Combines arrays and linked lists
        - It's an array of pointers to nodes.When implemented.

- Colisions:
        - When adding value to hash table and something else exists in hashed location
        + Collisions are appended to end of list.

```C
// Implementation of hash function

#include <ctype.h>

unsigned int hash(const char *word)
{
        return toupper(word[0]) - 'A';
}
```
- Search time O(n).

## Tries
- Trees of arrays.
- Searchable in constant time
- Downside:
        - Take large amount of memory.

## Summing Up
- Using pointers to build new data structures, what was learnt:
        - Data structures
        - Stacks and Queues
        - Resizing arrays
        - Linked lists
        - Dictionaries
        - Tries
