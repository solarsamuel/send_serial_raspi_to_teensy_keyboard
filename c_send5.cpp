#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

int uart_fd;

void setup_uart() {
    // Open UART device
    uart_fd = open("/dev/serial0", O_WRONLY | O_NOCTTY | O_NDELAY);
    if (uart_fd < 0) {
        perror("Failed to open UART device");
        exit(EXIT_FAILURE);
    }

    // Configure UART
    struct termios options;
    tcgetattr(uart_fd, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd, TCSANOW, &options);
}

void send_uart(char c) {
    if (write(uart_fd, &c, 1) < 0) {
        perror("Failed to write to UART device");
        exit(EXIT_FAILURE);
    }
}

int main() {
    setup_uart();

    // Set terminal to raw mode
    struct termios old_terminal, new_terminal;
    tcgetattr(STDIN_FILENO, &old_terminal);
    memcpy(&new_terminal, &old_terminal, sizeof(struct termios));
    cfmakeraw(&new_terminal);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);

    // Read keyboard input and send to UART
    char c;
    while (true) {
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 24) {  // Control-x
                break;
            }
            send_uart(c);
            cout << c << flush;  // Print the character to the terminal
        }
    }

    // Restore terminal to old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);

    // Close UART device
    close(uart_fd);

    return 0;
}
