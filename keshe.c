#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <signal.h>

#define BUZZER_CTL "/dev/buzzer_ctl"
#define BUZZER_ON 1
#define BUZZER_OFF 0

int fd;
int running = 1;
int mode = 0; // 0: periodic, 1: always on, 2: always off
int interval_on = 500000; // default on interval in microseconds
int interval_off = 1000000; // default off interval in microseconds

void* control_buzzer(void* arg) {
    while (running) {
        if (mode == 0) { // periodic
            ioctl(fd, BUZZER_ON, 1);
            usleep(interval_on);
            ioctl(fd, BUZZER_OFF, 1);
            usleep(interval_off);
        } else if (mode == 1) { // always on
            ioctl(fd, BUZZER_ON, 1);
            usleep(100000);
        } else { // always off
            ioctl(fd, BUZZER_OFF, 1);
            usleep(100000);
        }
    }
    return NULL;
}

void handle_signal(int signal) {
    running = 0;
    close(fd);
    exit(0);
}

int main(int argc, char* argv[]) {
    struct termios options;
    char buffer[256];
    int n;

    if ((fd = open(BUZZER_CTL, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        perror("Open buzzer_ctl failed");
        return 1;
    }

    signal(SIGINT, handle_signal);

    pthread_t buzzer_thread;
    pthread_create(&buzzer_thread, NULL, control_buzzer, NULL);

    while (running) {
        // Read commands from the serial port
        n = read(fd, buffer, sizeof(buffer));
        if (n > 0) {
            buffer[n] = '\0';
            if (strncmp(buffer, "always on", 9) == 0) {
                mode = 1;
            } else if (strncmp(buffer, "always off", 10) == 0) {
                mode = 2;
            } else if (sscanf(buffer, "periodic %d %d", &interval_on, &interval_off) == 2) {
                mode = 0;
                interval_on *= 1000;
                interval_off *= 1000;
            } else {
                printf("Unknown command: %s\n", buffer);
            }
        }
    }

    close(fd);
=======
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
>>>>>>> 54f177594482b6eb09721d9ff8fd489959b6fda3
    return 0;
}
