### 课题8 ***通过串口控制开发板载蜂鸣器的开关***
> 设计任务：
> 1．程序在开发板上运行，程序启动之后，首先按照固定的时间间隔控制板载蜂鸣器周期性的响和停（比如，响半秒钟，然后静默一秒钟，如此循环）。
> 2. 开发板程序作为串口的Server端，接收Client的请求。
> 3.	Client运行在PC上（Linux或者Windows均可），Client随时可以接受用户的输入时间周期，对用户输入的合法性进行检查后，通过串口发送给Server。
> 4.	Server接收到Client的请求，分析Client的命令并执行。
> 5.	命令的内容包括蜂鸣器的常响、关掉、周期响和静默（周期由用户指定），命令的格式自行定义。要求Server要对命令的合法性进行检查。
> 6.	命令的传输可以使用例如Base64等编码或者AES等加密，加分项。