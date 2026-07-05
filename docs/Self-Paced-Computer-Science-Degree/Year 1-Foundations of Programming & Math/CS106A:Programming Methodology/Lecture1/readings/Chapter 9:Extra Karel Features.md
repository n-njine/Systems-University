# Chapter 9: Extra Karel Features

## Painting corners
Karel also has the ability to paint corners in its world with the following colors:
    - "white", which removes any color on the square
    - "black"
    - "blue"
    - "red"
    - etc

To use Karel's paint functionality, use the following commands:
---------------------------------------------------------------
Command                         What it does
---------------------------------------------------------------
`paint_corner(COLOR_NAME)`       Karel colors the corner he's standing on `COLOR_NAME`. For example, to corner blue, you would write `paint_corner("blue")`.

