#include "input.h"
#include "terminal.h"

static int next_byte(int *out_byte)
{
    if (out_byte == NULL) return -2;

    unsigned char b;
    int ret = terminal_read_byte(&b);
    if (ret == 1)
    {
        *out_byte = (int)b;
        return 1;
    }
    else
    {
        return ret;
    }
}

static int avoid_unknown_buffer_trash(void)
{
    int byte = 0, ret;
    do
    {
        ret = next_byte(&byte);
    } while (ret > 0 && (byte < '@' || byte > '~'));

    return ret;
}

static int check_esc(void)
{
    int byte = KEY_UNKNOWN;
    int ret = next_byte(&byte);
    if (ret == 0)
    {
        return KEY_ESC;
    }
    else if (ret < 0)
    {
        return ret;
    }
    
    if (byte == '[')
    {
        ret = next_byte(&byte);
        if (ret < 0) return ret;
        else if (ret == 0) return KEY_UNKNOWN;
        switch (byte)
        {
            case 'A':
                byte = KEY_ARROW_UP;
                break;
            case 'B':
                byte = KEY_ARROW_DOWN;
                break;
            case 'D':
                byte = KEY_ARROW_LEFT;
                break;
            case 'C':
                byte = KEY_ARROW_RIGHT;
                break;
            case 'H':
                byte = KEY_HOME;
                break;
            case 'F':
                byte = KEY_END;
                break;
            case '2':
            case '3':
            case '5':
            case '6':
            {
                if (byte == '2') byte = KEY_INSERT;
                if (byte == '3') byte = KEY_DELETE;
                if (byte == '5') byte = KEY_PAGE_UP;
                if (byte == '6') byte = KEY_PAGE_DOWN;

                int next = KEY_UNKNOWN;
                ret = next_byte(&next);
                if (ret < 0) return ret;
                else if (ret == 0) return KEY_UNKNOWN;
                if (next != '~')
                {
                    byte = KEY_UNKNOWN;
                    if (next >= 0 && (next < '@' || next > '~'))
                    {
                        int trash = avoid_unknown_buffer_trash();
                        if (trash < 0) return trash;
                    }
                }
                break;
            }

            default:
                byte = KEY_UNKNOWN;
                int trash = avoid_unknown_buffer_trash();
                if (trash < 0) return trash;
                break;
        }
    }
    else if (byte == 'O')
    {
        ret = next_byte(&byte);
        if (ret < 0) return ret;
        else if (ret == 0) return KEY_UNKNOWN;
        switch (byte)
        {
            case 'H':
                byte = KEY_HOME;
                break;
            case 'F':
                byte = KEY_END;
                break;

            default:
                byte = KEY_UNKNOWN;
                break;
        }
    }
    else
    {
        byte = KEY_UNKNOWN;
    }

    return byte;
}

int input_read_key(int *key)
{
    if (key == NULL) return -2;

    int byte = KEY_UNKNOWN;
    int ret = next_byte(&byte);
    if (ret < 0) return ret;
    else if (ret == 0) // Timeout or interrupt
    {
        *key = KEY_NONE;
        return 0;
    }

    if (byte == '\x1b') // ESC
    {
        int esc = check_esc();
        if (esc < 0) return esc;
        *key = esc;
    }
    else
    {
        *key = (int)byte;
    }

    switch (*key)
    {
        case '\r':
        case '\n':
            *key = KEY_ENTER;
            break;
        case '\t':
            *key = KEY_TAB;
            break;
        case '\b':
        case  127:  // In some terminals backspace is returned as 127
            *key = KEY_BACKSPACE;
            break;
    }

    return 1;
}
