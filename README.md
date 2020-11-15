# opensslTransmit
## 基于openssl的加密文件传输软件

## ssl-console分支

### 目录结构
```
.
├── bin
│   └── test.txt
├── debug
│   ├── bin
│   │   └── test.txt
│   └── obj
├── include
│   ├── socketUtil.h
│   └── timeCounter.h
├── Makefile
├── obj
├── README.md
└── src
    ├── main.cpp
    ├── socketClient.cpp
    ├── socketServer.cpp
    ├── socketUtil.cpp
    └── timeCounter.cpp
```
### 运行方法
1 下载源代码

`$ git clone https://github.com/ca1fee/opensslTransmit.git`

2 切换到console分支
`$ git checkout console`

3 make

`$make`

![avatar](./imgs/make.png)

4 运行服务端程序

`$ ./bin/main server <监听端口> [ <接收文件名> ]`

![avatar](./imgs/run_as_server.png)

5 运行客户端程序

`$ ./bin/main client <服务器ip> <服务器端口> <传输文件名>`

![avatar](./imgs/run_as_client.png)

**即可在运行目录(或者指定的目录)找到传输的文件，格式为：clt.年.月.日.时.分.秒(或指定的目录和文件名)**

![avatar](./imgs/result.png)
