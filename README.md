A communication callback library


## 1.接口定义
Zxcom接口主要有：
 * 初始化和去初始化。
 * 添加命令和应答处理函数。
 * 包的接收和发送包的组建。

### 1.1 初始化和去初始化
```
int ZxcomInit()
int ZxcomDeInit()
```
### 1.2 添加命令和响应处理函数
```
int ZxcomAddCommand(COMMAND_ID_TYPE id,command_handler_t handler)
本接口由命令响应方调用，用于处理指定id的命令请求。
```
|    参数   |定义                          |
|----------------|-------------------------------|
| id|`所响应命令的id`            |
| handler          |`处理函数`            |


```
int ZxcomAddResponse(COMMAND_ID_TYPE id,command_handler_t handler)

本接口由命令请求方调用，用于处理命令响应方的应答。
```
|    参数   |定义                          |
|----------------|-------------------------------|
| id|`所响应命令的id`            |
| handler          |`处理函数`            |


### 1.3包的接收和发送包的组建
```
int ZxcomOnPacket(const char *pack,const int len)

用户收到包后，只需调用本接口即可，无需做其它事情。
```
|    参数   |定义                          |
|----------------|-------------------------------|
| pack |`收到的包`            |
| len          |`包长度`            |


```
int ZxcomOnSendMsg(COMMAND_ID_TYPE cmdId,const char *param,const unsigned int paramLen,char *packet)

这个接口输入命令Id，命令参数，参数长度，返回命令包，用户只需将得到的packet发送到命令接收方即可。
```
|    参数   |定义                          |
|----------------|-------------------------------|
|id|`所响应命令的id`            |
|param          |`命令参数`            |
|paramLen          |`参数长度`            |
|packet          |`命令包`            |

```
int ZxcomOnSendResponse(const void *recv,const char *param,const unsigned int paramLen,char *packet)
这个接口输入收到的命令参数，响应参数，参数长度，返回应答包，用户只需将得到的packet发送到命令发送方即可。
```
|    参数   |定义                          |
|----------------|-------------------------------|
| recv|`收到的命令参数`            |
|param          |`应答参数`            |
|paramLen          |`应答参数长度`            |
|packet          |`应答包`            |


### 1.4 超时处理功能
超时处理功能需要用户实现超时处理函数，然后调用接口：
```
int CommSetEventHandler(EVENT_HANDLER ev)
```
进行添加。


### 1.5 需要用户实现的回调函数
#### 1.  事件（超时）处理函数
      函数类型为：typedef int (*EVENT_HANDLER)(const int event,const int msgId)
      
#### 2.  互斥锁
```
int CRITICAL_INIT();
void ENTER_CRITICAL();
void EXIT_CRITICAL();
int CRITICAL_DEINIT();
```
在MCU中可以用关中断和开中断实现，linux下用线程锁pthread_mutex_t。

#### 3. 命令（应答）处理函数:
```
     函数类型：typedef int (*command_handler_t)(void *para)。
```

## 2．应用实例：
这里使用unix套接字模拟命令的发送和接收端，简单示范了本框架的使用。
代码：

[main.c](https://github.com/KevinZu/zxcom/blob/master/main.c)

运行服务端：
```
./main
```
运行客户端：
```
./main c
```

## 3. 构建方法：

```
git clone https://github.com/KevinZu/zxcom.git

cd zxcom

mkdir build

cd build

cmake ..

make
```


本项目依赖库：[ucomlib](https://github.com/smtp-http/ucomlib.git)

