# Section 2

# Control Structures.

## Curly Braces{}
- Group multiple statements that execute in order.

## If Statement
- Check for conditions
```c
if (<expression>)
{
    <statement>;
    <statement>:
}
else
{
    <statement>;
}
```

## Conditional Expression -or- The Ternary Operator.
- Shorthand for some `if-else` statements.
- Syntax:
    <expression1> ? <expression2> : <expression3>
- Working:
    - Evaluate expression1, if true(non-zero), evaluate and returns expression2. Otherwise, evaluate and returns expression3.
- E.g
```
    if (x < y)
    {
        min = x;
    }
    else
    {
        min = y;
    }

// You just say...
min = (x < y) ? x : y;
```

## Switch Statement.
- Separates different blocks of code based on value of integer.
- Working:
    - Evaluate switch expression, flow of control jumps to matching const-expression case.
- E.g:
```
    switch(<expression>)
    {
        case <const_expression-1>:
            <statement>;
            break;
        case <const-expression-2>:
            <statement>;
            break;
        case <const-expression-3>:  // here we combine case 3 and 4
        case <const-expression-4>:
            <statement>;
            break;

        default:        // optional
            <statement>;
    }
```
* Each constant needs its own `case` keyword and a trailing colon(:).The program will keep running through all the cases from that point down -- "fall through".

## While Loop
Evaluates the test expression before every loop
```
    while (<expression>)
    {
        <statement>
    }
```

## Do-While loop
- The test condition is at bottom of the loop.
- The body will always execute at least once.
```
do 
{
    <statement>
}
while (<expression>)
```

## For Loop.
- Parts: initialization, a continuation condition, and an action
```
    for (<intialization>; <continuation>; <action>)
    {
        <statement>;
    }
```
- E.g
```
    for (int i = 0; i < 10; i++)
    {
        <statement>;
    }
```
- The loop starts at 0 and use < in test so that it runs **up to but not equal** to the upper bound.
- Can be made of multiple expressions separated by commas. Evaluation, left to right, and represent the value of the last expression.

## Break
Do:
    - Move control outside a loop or switch statement.
```
    while(<expression>)
    {
        <statement>;
        <statement>;

        if (<condition which can only be evaluated here>)
            break;

        <statement>;
        <statement>;
    }
    
    // control jumps down here on the break
```
- Break does not work with `if`.It only works in loops and switches.

## Continue
Do:
    - Causes control to jump to the bottom of the loop, effectively skipping over any code below the continue.
```
    while (<expression>)
    {
        ... 
        if (<condition>)
            continue;
        ...;
        ...;
        // control jumps here on the continue
    }
```



