# opensslTransmit
## 基于openssl的加密文件传输软件

## ssl-gui分支
在ssl-console版本基础上增加gui功能

### 目录结构
```
.
├── bin
├── certs
├── debug
│   ├── bin
│   └── obj
├── imgs
├── include
├── obj
├── src
└── test
    ├── fileRecv
    └── fileTrans
```
### 依赖环境
`build-essential libssl-dev`
### 运行方法
1 安装依赖环境

`$ sudo apt-get install libssl-dev`
2 下载源代码

`$ git clone https://github.com/ca1fee/opensslTransmit.git`

3 切换到ssl-console分支
`$ git checkout ssl-console`

4 make

`$make`

![avatar](./imgs/ssl_console_make.png)

5 运行服务端程序

`$ ./bin/main server <监听端口> [ <接收文件名> ]`

![avatar](./imgs/ssl_console_run_as_server.png)

6 运行客户端程序

`$ ./bin/main client <服务器ip> <服务器端口> <传输文件名>`

![avatar](./imgs/ssl_console_run_as_client.png)

**即可在运行目录(或者指定的目录)找到传输的文件，格式为：clt.年.月.日.时.分.秒(或指定的目录和文件名)**

![avatar](./imgs/ssl_console_result.png)
