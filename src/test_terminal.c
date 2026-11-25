#include "terminal.h"
#include <stdio.h>

int main(void)
{
    int rows = 0;
    int cols = 0;

    if (terminal_setup() < 0)
    {
        perror("terminal_setup");
        return 1;
    }

    if (terminal_get_size(&rows, &cols) == 0)
    {
        printf("Size: %d x %d (press 'q' to quit)\r\n", rows, cols);
    }

    unsigned char b = 0;
    while (1)
    {
        if (terminal_was_resized())
        {
            if (terminal_get_size(&rows, &cols) == 0)
            {
                printf("Resized: %d x %d\r\n", rows, cols);
            }
        }

        int r = terminal_read_byte(&b);
        if (r == 1)
        {
            printf("Byte: %u ('%c')\r\n", (unsigned)b,
                   (b >= 32 && b < 127) ? (char)b : '.');
            if (b == 'q') break;
        }
        else if (r < -1)
        {
            printf("Read error: %d\r\n", r);
            break;
        }
    }

    if (terminal_close() < 0)
    {
        perror("terminal_close");
        return 1;
    }

    return 0;
}