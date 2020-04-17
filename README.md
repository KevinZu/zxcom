A communication callback library


## 1.接口定义
Zxcom接口主要有：
 * 初始化和去初始化。
 * 添加命令和应答处理函数。
 * 包的接收和发送包的组建。

### 1.1 初始化和去初始化
int ZxcomInit()
int ZxcomDeInit()

### 1.2 添加命令和响应处理函数
int ZxcomAddCommand(COMMAND_ID_TYPE id,command_handler_t handler)
参数：
COMMAND_ID_TYPE id	command_handler_t handler
所响应命令的id	处理函数
本接口由命令响应方调用，用于处理指定id的命令请求。

int ZxcomAddResponse(COMMAND_ID_TYPE id,command_handler_t handler)
参数：
COMMAND_ID_TYPE id	command_handler_t handler
所响应命令的id	处理函数
本接口由命令请求方调用，用于处理命令响应方的应答。

### 1.3包的接收和发送包的组建
int ZxcomOnPacket(const char *pack,const int len)
参数：
const char *pack	const int len
收到的包	包长度
用户收到包后，只需调用本接口即可，无需做其它事情。


int ZxcomOnSendMsg(COMMAND_ID_TYPE cmdId,const char *param,const unsigned int paramLen,char *packet)
参数：
cmdId	param	paramLen	packet
命令ID	命令参数	参数长度	命令包
这个接口输入命令Id，命令参数，参数长度，返回命令包，用户只需将得到的packet发送到命令接收方即可。


int ZxcomOnSendResponse(COMMAND_ID_TYPE cmdId,const char *param,const unsigned int paramLen,char *packet)
参数：
cmdId	param	paramLen	packet
命令ID	命令参数	参数长度	命令包
这个接口输入命令Id，响应参数，参数长度，返回应答包，用户只需将得到的packet发送到命令发送方即可。


本项目依赖库：[ucomlib](https://github.com/smtp-http/ucomlib.git)

