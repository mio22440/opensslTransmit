# opensslTransmit
基于openssl的加密文件传输软件

## 目录结构
```
.
├── bin
│   ├── main                release版本可执行程序
│   └── test.txt            用来测试传输的文件
├── debug                   
│   ├── bin
│   │   ├── main
│   │   └── test.txt
│   └── obj
│       ├── main.o
│       ├── socketClient.o
│       ├── socketServer.o
│       ├── socketUtil.o
│       └── timeCounter.o
├── include
│   ├── socketUtil.h
│   └── timeCounter.h
├── Makefile
├── obj
│   ├── main.o
│   ├── socketClient.o
│   ├── socketServer.o
│   ├── socketUtil.o
│   └── timeCounter.o
├── README.md
└── src
    ├── main.cpp            主函数
    ├── socketClient.cpp    客户端类的实现
    ├── socketServer.cpp    服务端类的实现
    ├── socketUtil.cpp      socket文件传输的工具类实现
    └── timeCounter.cpp     计时
```
## 运行方法
1 下载源代码

`$ git clone https://github.com/ca1fee/opensslTransmit.git`

2 切换到console分支
`$ git checkout console`

3 make

`$make`

4 运行服务端程序（正在写）

5 运行客户端程序
`$ ./bin/main client <服务器ip> <服务器端口> <传输文件名>`

**即可在服务端文件夹找到传输的文件，格式为：clt.日期.月份.年份.小时.分钟.秒**