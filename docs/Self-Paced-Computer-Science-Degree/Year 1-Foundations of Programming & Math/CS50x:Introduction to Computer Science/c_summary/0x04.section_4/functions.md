# Section 4

# Functions
What:
    - A construct to separate and package a block of code.
Have:
    - Name, list of arguments, block of code it executes when called.
    - The special function called "main" is where program execution begins.
Example:
```
Function name: Twice
Function argument: int num
Function body:  Computes the value which is twoce the **num** argument and returns that value back to caller.
```
```c
/*
   Computes double of a number.
   Works by trippling the numer, and then subtracting to get back to double.
*/
static int Twice (int num)
{
    int result = num * 3;
    result = result - num;
    return (result);
}
```
## Syntax
* static: Show that function is only available to callers in the file where it is declared.
* int: Type of return value
* Parameters are listed just like a variable name.
- Inside the function, parameter **num** and the local variable **result** are "local" to function. I.e They get their own memory and exist only so long as the function is executing.
The caller code looks like:
    ```
    int num = 13;
    int a = 1;
    int b = 2;
    a = Twice(a);           // call Twice() passing the value of a
    b = Twice(b + num);     // call Twice() passing the value b+num
    // a == 2
    // b == 30
    // num == 13 (this num is totally independent of the "num" local to Twice()
    ```
Things to notice...
    * (vocabulary): Expression passed to a function by its caller is called the "actual parameter"
                    Parameter storage local to the function is called the "formal parameter" in a functio definition.

    * Parameters are passed "by value": There is a single copying assignment operation (=) from each actual parameter to set each formal parameter.
    * The variables local to Twice(), num and result, only exist temporarily while Twice() is executing.
    * The return at the end of Twice() computes the return value and exits the function. Execution resumes with the caller.

## C-ing and Nothingness -- void
Void:
    - Nothing. Indicates that a function does not return anything.

## Call by Value vs. Call by Reference
Parameters in C passed "by value", actual parameters values are copied into local storage. The caller and callee functions do not share any memory -- they each have their own copy.

- Shortcommings:
    1. As caller has its own copy, modifications to that memory are not communicated back to the caller.
    2. Copying a value from the caller to the callee as the value is large and copying is expensive.

Alternative:
    - Pass arguements "by reference". I.e Pass a pointer to the value. There's only one copy of the value at any time, and the callee both access that one value through pointers.

## Swap Example
As C uses call by value, This swap will not work...
    ```
    void swap(int x, int y)
    {
        int temp;

        temp = x;   // These operations just change the local x,y-temp
        y = temp;   // -- nothing connects them back to the caller's a, b
    }

    // Some caller code which calls swap()...
    int a = 1;
    int b = 2;
    swap(a, b)
    ```
    The function above only operates on the copies of a and b local to swap() itself. When the owning function exits, its local memory disappears.

## Reference Parameter Technique.
To pass an object X as a reference parameter, the programmer must pass a pointer to X instead of X itself.

The **caller** will need to use & or other operators to compute the correct pointer actual parameter.

The **callee** will need to dereference the pointer with * where appropritate to access the value of interest.

Correct swap function.
    ```c
    static void swap(int *x, int *y)        // params are int* instead of int
    {
        int temp;

        temp = *x;      // use * to follow the pointer back to the caller's memory
        *x = *y;
        *y = temp;
    }
    ...
    // Some caller code which calls swap()
    int a = 1;
    int b = 2;
    swap(&a, &b);
    ```
Things to notice...
    - The formal parameters are int* instead of int.
    - The caller uses & to compute pointers to its local memory (a,b).
    - The callee uses * to dereference the formal parameter pointers back to get the caller's memory.
    - The formal parameters are declared to be pointers, and the values of interest (a,b) are accessed through them.
    - There is not special relationship between the **names** used for the actual and formal parameters.

## const
Do:
    - Declares that the code using the variable will not change the variable.
    - Use: clarify the role of a parameter in a function prototype...
    ```
    void foo(const struct fraction* fract);
    ```

### Bigger Picture Example
Code to demonstrate affecting memory of value in variable in function call.

Trick:
    If call swap() inside swap -- no additional use of & is necessary in this case since the parameters x, y inside IncrementAndSwap() are already pointers to the values of interest.
    The names of the variables through the program(a, b, x, y, alice, bob) do not need to match up in any particulary way for the paramters to work.

    The parameters mechanism only depends on the types of the parameters and their order in the parameter list -- not their names.
```
static void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

static void IncrementAndSwap(int *x, int *y)
{
    (*x)++;
    (*y)++;
    swap(x, y);         // don't need & here since a and b are already int *'s
}

int main(void)
{
    int alice = 10;
    int bob = 20;

    swap(&alice, &bob);
    // at this point alice == 20 and bob == 10

    IncrementAndSwap(&alice, &bob);
    // At this point alice == 11 and bob == 21

    return 0;
}
```
