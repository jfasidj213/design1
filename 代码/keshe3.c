#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#define BUZZER_CTL "/dev/buzzer_ctl"
#define BUZZER_ON 1
#define BUZZER_OFF 0
// 控制蜂鸣器的函数
void controlBuzzer(int cmd) {
    int fd, ret;
    // 打开蜂鸣器设备
    if ((fd = open(BUZZER_CTL, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        printf("打开蜂鸣器设备失败！\n");
        return;
    }
    // 发送控制命令
    ret = ioctl(fd, cmd, 1);
    close(fd); // 关闭设备
}
int main() {
    int fd;
    struct termios config;
    // 打开串口设备
    fd = open("/dev/ttySAC2", O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("打开串口设备出错");
        return 1;
    }
    // 配置串口
    memset(&config, 0, sizeof(config));
    if (tcgetattr(fd, &config) < 0) {
        perror("获取串口属性出错");
        close(fd);
        return 1;
    }
    // 设置波特率为115200
    cfsetispeed(&config, B115200);
    cfsetospeed(&config, B115200);
    // 8数据位，无校验
    config.c_cflag &= ~PARENB;
    config.c_cflag &= ~CSTOPB;
    config.c_cflag &= ~CSIZE;
    config.c_cflag |= CS8;
    // 禁用流控制
    config.c_iflag &= ~(IXON | IXOFF | IXANY);
    config.c_iflag &= ~(INLCR | ICRNL);
    config.c_iflag &= ~(IGNCR | ICRNL);
    // 设置为原始模式
    config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // 禁用输出处理
    config.c_oflag &= ~OPOST;
    // 应用新的串口设置
    if (tcsetattr(fd, TCSANOW, &config) < 0) {
        perror("设置串口属性出错");
        close(fd);
        return 1;
    }
    char cmd;
    while (1) {
        // 从串口读取数据
        if (read(fd, &cmd, sizeof(cmd)) > 0) {
            switch (cmd) {
                case '0': // 关闭蜂鸣器
                    controlBuzzer(BUZZER_OFF);
                    printf("关闭蜂鸣器。\n");
                    break;
                case '1': // 打开蜂鸣器
                    controlBuzzer(BUZZER_ON);
                    printf("打开蜂鸣器。\n");
                    break;
                case '3': // 周期性响
                    controlBuzzer(BUZZER_ON);
                    usleep(500000); // 响半秒钟
                    controlBuzzer(BUZZER_OFF);
                    usleep(1000000); // 静默一秒钟
                    break;
                default:
                    printf("未知命令：%c\n", cmd);
            }
        }
    }
    // 关闭串口
    close(fd);
    return 0;
}
