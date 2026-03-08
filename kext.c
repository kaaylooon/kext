#include <stdlib.h> //atexit
#include <unistd.h> //read and STDIN_FILENO
#include <termios.h>
#include <ctype.h>
#include <stdio.h>

struct termios orig_termios;

void die(const char *s) {
    perror(s);
    exit(1);        

}

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    /*
        Terminal attributes can be read into a termios struct by tcgetattr(). After modifying them, you can then apply them to the terminal using tcsetattr(). The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.


    */
}

int main()
{
    enableRawMode();

    while (1)
    {
        char c = '\0';
        read(STDIN_FILENO, &c, 1);

        if (iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q') break;
    }
    return 0;
}

