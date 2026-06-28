// Prints substrings via pointer arithmetic

#include <stdio.h>

int main(void)
{
        char *s = "HI!";
        printf("%s\n", s);
        printf("%s\n", (s + 1));
        printf("%s\n", (s + 2));

        // Exit successfully
        return 0;
}
/* This code prints the values stored at various memory locations starting with s. */
