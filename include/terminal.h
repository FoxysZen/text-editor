#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <sys/types.h>

/**
 * @brief Enters in the alternative screen, enables raw mode in the terminal, 
 *        hides cursor, clears the terminal, sets the cursor on 1,1 and 
 *        modifies the handler for SIGWINCH.
 * 
 * @return int 0 if success, otherwise -1.
 */
int terminal_setup(void);
/**
 * @brief Sets the normal style in the terminal, shows the cursor and exits the
 *        alternative screen.
 * 
 * @return int 0 if success, otherwise -1.
 */
int terminal_close(void);

/**
 * @brief Gets the size of the terminal in rows and columns.
 * 
 * @param rows Pointer to store terminal rows.
 * @param cols Pointer to store terminal columns.
 * @return int 0 if success, -1 if the getter fails or the pointers are NULL.
 */
int terminal_get_size(int *rows, int *cols);
/**
 * @brief Indicates if the terminal was resized.
 * 
 * @return int 1 if was resized, otherwise 0.
 */
int terminal_was_resized(void);

/**
 * @brief Reads the next byte from STDIN_FILENO.
 * 
 * @param byte Pointer to store the byte read.
 * @return int 1 if a byte was read, 0 if timeout or interrupt except SIGWINCH,
 *         -1 if SIGWINCH interrupts the read, -2 if an error occurs or pointer
 *         is NULL.
 */
int terminal_read_byte(unsigned char *byte);
/**
 * @brief Writes in STDOUT_FILENO len bytes from buf.
 * 
 * @param buf Non-null pointer to the buffer containing the bytes to write.
 * @param len Amount of bytes to write from buf.
 * @return int 0 if all bytes were written, or -1 on error or NULL pointer.
 */
int terminal_write(const char *buf, size_t len);

#endif /* TERMINAL_H */