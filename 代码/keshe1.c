#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "serial.h"
int com = 6;
// 函数声明
int sendCommandToLinux(int command, PORT com_port);
int main() {
    while (1) {
        char command;
        printf("请输入命令: ");
        // 使用 %c 前要加空格以忽略前一个输入的换行符
        scanf_s("%c", &command);
        printf("输入的命令为: %c\n", command);

        // 检查命令是否为有效字符
        if (command != '1' && command != '0' && command != '3') {
            printf("无效命令，请输入 1, 0 或 3\n");
            continue; // 跳过此循环迭代
        }
        printf("启动\n");
        // 初始化串口
        PORT com_port = serial_init(com, 115200, 8, ONESTOPBIT, NOPARITY);
        if (com_port == NULL) {
            printf("Failed to open serial port.\n");
            return 1;
        }
        printf("准备发送\n");
        // 发送命令
        int ret = sendCommandToLinux(command, com_port);
        if (ret != 0) {
            printf("Failed to send command to Linux board.\n");
        }
        else {
            printf("Command sent successfully.\n");
        }

        // 关闭串口
        ClosePort(com_port);
    }
    return 0;
}
// 函数：向 Linux 板发送命令
int sendCommandToLinux(char command, PORT com_port) {
    char buffer[1];
    buffer[0] = command;
    int bytes_sent = SendData(com_port, buffer, 1);
    if (bytes_sent <= 0) {
        printf("Failed to send command.\n");
        return 1;
    }
    return 0;
}