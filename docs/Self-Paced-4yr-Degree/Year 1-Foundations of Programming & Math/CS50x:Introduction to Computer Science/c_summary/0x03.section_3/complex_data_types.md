# Section 3
# Complex Data Types
Include:
    * Grouping things together to form composite types -- **arrays** and records **structures**.
Eg:
    Fraction struct.
```
    struct fraction 
    {
        int numerator;
        int denominator;
    };      // Don't forget the semicolon!
```
* The `struct fraction` creates a new type.
* Use period(.) to access the fields in a record.
* You can copy two records of the same type using a single assignment statement. However `==` does not work on structs.
```
fraction f1, f2;        // declare two fractions
f1.numerator = 22;
f1.demonimator = 7;

f2 = f1;                // this copies over the whole struct.
```

## Arrays
What:
    - A contigous collection of data of the same type.
    - Always indexed from 0
    ```
    int scores[100];

    scores[0] = 13          // set first element
    scores[99] = 42;        // set last element
    ```
- The name of the array refers to the whole array.
- Working:
    - Represents a pointer to the start of the array.
- Indexing:
    - 0..(number of things -1) pervades the language.

## Multidimensional Arrays.(matrices)
E.g:
    - Declare two-dimensional 10 by 10 array of integers and set the first and last elements to be 13.
    ```
    int board [10][10];

    board [0][0] = 13;
    board [10][10] = 13;
    ```
    - Implementation stores all the elements in a single contiguous block of memory.
    - The array is arranged with the elements of the rightmost index next to each other.ie board [1][8] comes before board[1][9] in memory.
    - (Optional efficiency point). It's efficient to access memory which is near other recently accessed memory.
    - Reading chunk of array:
        - Vary the rightmost index the most frequently since that will access elements that are near each other in memory.

## Array of Structs
E.g
    - Declare an array named "numbers" which holds 1000 `struct fraction's`
    ```
    fraction numbers[1000];

    numbers[0].numerator = 22;          /* set the 0th struct fraction */
    numbers[0].denominator = 7;         
    ```

## Pointers
Def:
    - A value which represents a referece to another value sometimes know as the pointer's "pointee".

### Syntax:
- Use the asterisk or "star"`(*)` to indicate a pointer.
- C pointer types based on the type pointee:
    - `char*`- Refers to a single `char`.
    - `struct fraction*`- Refers to a `struct fraction`.
    ```
    int *int_ptr;  // declare an integer pointer variable int_ptr
    char *char_ptr; // declares a character pointer --
                    // a very common type of pointer.

    // Declare two struct fraction pointers
    // (when declaring multiple variables on one line, the *
    // should go on the right with the variable)
    struct fraction *f1, *f2;
    ```

### The Floating `"*"`
- The star is allowed to be anywhere between the base type and the variable name.
- Similar to:
    ```
    int  *int_ptr;           // these are all the same
    int * int_ptr;  
    int*  int_ptr;
    ```

## Pointer Dereferencing
- Pointer points to memory of the appropriate type.
    ```
    struct fraction* f1;

    Expression          Type
    ----------          ----
    f1                  struct fraction*
    *f1                 struct fractoin
    (*f1).numerator     int
    ```
- The "->" at the right of the pointer can access any of the fields in the struct.
    ```
    f1->numeratro
    ```
- More complex declarations..
    ```
    fraction** fp;                  // a pointer to a pointer to a struct fraction
    fraction fract_array;           // an array of 20 struct fractions
    fraction* fract_ptr_array[20]   // An array of 20 pointers to struct fractions.
    ```
- Definition of node in a linked list
    ```
    struct node 
    {
        int data;
        struct node* next;
    };
    ```

## The & Operator
- One of ways that a pointer's set to point to things.
- The & operator computes a pointer to the argument to its right.
- The argument can be any variable which takes up space in the stack or heap("LValue").
    ```
    &i and &(f1->numerator) are ok
    &6 is not
    ```
- Use & when you have some memory, and you want to a pointer to that memory.
    ```
    void foo()
    {
        int* p;         // p is a pointer to an integer
        int i;          // i is an integer

        p = &i;         // Set p to point to i
        *p = 13;        // Change what p points to -- in this case i -- to 13
        
        // At this point i is 13. So is *p. In fact *p **is** i.
    }
    ```
- Use pointer to an object created with &, only if the object exists.
- Varible exists only as long as the function where it is declared is still executing.
- Any pointers which are intialized with &i are valid only as long as foo() is executing.
- This "lifetime" constraint of local memory is standard and needs to take into account when using the & operator.

## NULL
- A pointer can be assigned the value 0 to explicitly represent that it does not currently have a pointee.
- NULL will behave like a boolean false when used in a boolean context.

### Pitfall -- Uninitialized Pointers
What to keep track:
    - The pointer and the memory it is pointing to, the "pointee".
What to do for relationship to work:
    1. The pointer must be declared and allocated
    2. The pointee must be declared and allocated
    3. The pointer(1) must be intialized so that it points to the pointee(2).
Pointer error of all time:
    - Declare and allocate the pointer (step10. Forget step 2 and/or 3.
    ```
    int *p;

    *p = 13;            // NO NO NO p does not point to an int yet
                        // this just overwrites a random area in memory
    ```
## Using pointers
Declaring a pointer allocates space for the pointer itself, **but it does not allocate space for the pointee**.
    - The pointer must be set to point to something before you can dereference it.

E.g:
    - Code that demonstrates how rules (1) (2) (3) for pointer use correctly
    ```
    int *p;          // (1) allocate the pointer
    int i;           // (2) allocate the pointee
    struct fraction f1;     // (2) allocate pointee

    p = &i          // (3) setup p to point to i
    *p = 42;        // ok to use p since it's setup

    p = &(f1.numerator);        // (3) setup p to point to a different int
    *p = 7;         // (3)
    ```

## C Strings
What:
    - Ordinary array of characters.
    - The programmer is responsible for the managing the string memory and calling the right functions.

String:
    - Array of `char` with a "null" character ('\0') that marks the end of the string. String library functions operate on strings stored in t his way.
Usefull functions
    - strcpy(char dest[], const char source[]); - Copies the bytes of one string over to another. Order of arguments similar to '='.
    - strlen(const char string[]); - Returns the number of characters in C excluding the NULL.'\0'.
    - Regular assignment operator(=) does **not** do string copying which is why strcpy() is needed.
E.g
    - Code that allocates a 10 char array and uses strcpy() to copy the bytes of the string constant "binky" into that local array.
```
{
    char local_string[10];

    strcpy (local_string, "binky");
}
```
## String Code Example.
- Reversing a string stored in local array.
```
{
    char string[1000];      // string is a local 1000 char array
    int len;

    strcpy(string, "binky");
    len = strlen(string);

    /*
       Reverse the chars in the string:
       i starts at the beginning and goes up
       j starts at the end and goes down
       i / j exchange their chars as they go until they meet
    */
    int i, j;
    char temp;
    for (i = 0, j = len - 1; i < j; i++, j--)
    {
        temp = string[i];
        string[i] = string[j];
        string[j] = temp;
    }

    // At thig point the local string should be "yknib"
}
```
## "Large Enough" Strings
The owner of the string is responsible for allocating array space which is "large enough" to store whatever the string will need to store.

Avoiding buffer overflow:
    - Check the size of the data first to make sure it fits in the destination string.

## Char*
The type of variable `local_string`is essentially `char*`.
    - Manipultating actual chars in a string requires code which manipulates the underlying array.

## TypeDef
Do:
    - Introduces a shorthand name for a type. The syntax is...
    ```
    typedef <type> <name>
    ```
How:
    - It's convenient to use typedef to create types with upper case names and use the lower-case version of the same word as a variable.
    ```
    typedef struct fraction Fraction;
    
    Fraction fraction           // Declare the variable *fraction* of type "Fraction"
                                // which is really just a synonym for "struct fraction"
    ```
Example:
    - Define `Tree` as a standard pointer to a binary tree node where each node contains some data and "smaller" and "larger" subtree pointers.
    ```c
    typedef struct treenode* Tree;
    struct treenode
    {
        int data;
        Tree smaller, larger;   // equivalently, this line could say
                                // "struct treenode *smaller, *larger"
    };

