#include "terminal.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static volatile sig_atomic_t resized = 0;
static struct winsize ws;
static const char INIT_SEQ[]  = "\x1b[?1049h\x1b[?25l\x1b[2J\x1b[H";
static const char CLOSE_SEQ[] = "\x1b[0m\x1b[?25h\x1b[0m\x1b[?25h\x1b[?1049l";
static struct termios og_tty;

int terminal_write(const char *buf, size_t len);

static void sigwinch_handler(int sig)
{
    resized = 1;
    (void)sig;
}

static void set_terminal_raw(struct termios *tty)
{
    tty->c_iflag &= ~(BRKINT | ICRNL | INPCK | IXON);
    tty->c_oflag &= ~(OPOST);
    tty->c_lflag &= ~(ICANON | ECHO | ISIG);
    tty->c_cc[VMIN]  = 0;
    tty->c_cc[VTIME] = 1;
}

int terminal_setup(void)
{
    int status = 0;
    
    struct sigaction sa = {0};
    sa.sa_handler = sigwinch_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGWINCH, &sa, NULL) < 0) status = -1;

    // Save original terminal and start raw terminal
    if (tcgetattr(STDIN_FILENO, &og_tty) < 0) status = -1;
    struct termios raw_tty = og_tty;
    set_terminal_raw(&raw_tty);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_tty) < 0) status = -1;

    // Set alternative screen, hide cursor and clear terminal
    if (terminal_write(INIT_SEQ, sizeof(INIT_SEQ) - 1) < 0)
    {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_tty);
        status = -1;
    }

    return status;
}

int terminal_close(void)
{
    int status = 0;

    if (terminal_write(CLOSE_SEQ, sizeof(CLOSE_SEQ) - 1) < 0)
    {
        status = -1;
    }

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_tty) < 0)
    {
        status = -1;
    }

    struct sigaction sa = {0};
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGWINCH, &sa, NULL) < 0) return -1;

    return status;
}

int terminal_get_size(int *rows, int *cols)
{
    if (rows == NULL || cols == NULL) return -1;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0)
    {
        return -1;
    }
    *rows = ws.ws_row;
    *cols = ws.ws_col;

    return 0;
}

int terminal_was_resized(void)
{
    if (resized)
    {
        resized = 0;
        return 1;
    }

    return 0;
}

int terminal_read_byte(unsigned char *byte)
{
    if (byte == NULL) return -2;

    ssize_t ret = read(STDIN_FILENO, byte, 1);
    if (ret < 0)
    {
        if (errno == EINTR) // Error caused by an interrupt
        {
            return resized ? -1 : 0;
        }
        return -2;
    }
    else if (ret == 0) // Read timeout
    {
        return 0;
    }

    return 1;
}

int terminal_write(const char *buf, size_t len)
{
    if (buf == NULL) return -1;

    size_t total = 0;
    while (total < len)
    {
        ssize_t written = write(STDOUT_FILENO, buf + total, len - total);
        if (written < 0)
        {
            if (errno == EINTR) continue;
            return -1;
        }
        total += (size_t)written;
    }

    return 0;
}
