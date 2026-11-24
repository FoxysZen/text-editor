# text-editor

Basic text editor written in c.

## Platform Support

Aimed at POSIX systems due to the use of termios and ioctl.

## Dependencies

Make sure you have the following packages installed:

* `gcc` (C17 support)
* `make`

## How to compile

1. Compile the project:
```bash
make
```
2. Run the executable:
```bash
./text-editor [file-name]
```

## Controls

* `Ctrl + S`: Save
* `Ctrl + Q`: Quit

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
