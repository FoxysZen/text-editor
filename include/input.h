#ifndef INPUT_H
#define INPUT_H

#define CTRL_KEY(k) ((unsigned char)(k) & 0x1f)

enum KeyCode {
    // Movement
    KEY_ARROW_UP = 0x110000, // Maximum unicode value + 1
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    // Edit / Control
    KEY_BACKSPACE,
    KEY_DELETE,
    KEY_ENTER,
    KEY_TAB,
    KEY_ESC,
    KEY_INSERT,

    KEY_NONE,
    KEY_UNKNOWN
};

/**
 * @brief Reads a key from STDIN_FILENO.
 * 
 * @param key Pointer to store the key read.
 * @return int 1 if a key was read, 0 if timeout or interrupt except SIGWINCH,
 *         -1 if SIGWINCH interrupts the read, -2 if an error occurs or pointer
 *         is NULL.
 */
int input_read_key(int *key);

#endif /* INPUT_H */