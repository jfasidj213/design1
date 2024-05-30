#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#define SERIAL_PORT "/dev/ttyUSB0" // 串口设备，根据实际情况设置
#define BAUD_RATE B9600

int set_interface_attribs(int fd, int speed) {
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        printf("Error %d from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0; // no signaling chars, no echo, no canonical processing
    tty.c_oflag = 0; // no remapping, no delays
    tty.c_cc[VMIN]  = 0; // read doesn't block
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls, enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag |= 0;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error %d from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    return 0;
}

void send_command(const char* command) {
    int fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error %d opening %s: %s\n", errno, SERIAL_PORT, strerror(errno));
        return;
    }

    set_interface_attribs(fd, BAUD_RATE);

    write(fd, command, strlen(command));
    close(fd);
}

int main() {
    char command[256];
    while (1) {
        printf("Enter command (BUZZER_ON, BUZZER_OFF, BUZZER_CYCLE <on_time> <off_time>, EXIT to quit): ");
        fgets(command, sizeof(command), stdin); // 读取用户输入
        command[strcspn(command, "\n")] = 0; // 去除换行符

        if (strcmp(command, "EXIT") == 0) {
            printf("Exiting program.\n");
            break; // 退出循环
        }

        send_command(command);
    }
    return 0;
}
