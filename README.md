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
├── qt
│   └── ssl-gui
│       ├── build-ssl-gui-Desktop_Qt_5_14_2_GCC_64bit-Debug
│       ├── build-ssl-gui-Desktop_Qt_5_14_2_GCC_64bit-Release
│       └── ssl-gui
├── src
└── test
    ├── fileRecv
    └── fileTrans
```
### 依赖环境
`build-essential libssl-dev qt`
### 运行方法
1 安装依赖环境

openssl:

`$ sudo apt-get install libssl-dev`

qt:

`http://download.qt.io/official_releases/qt/`

2 下载源代码

`$ git clone https://github.com/ca1fee/opensslTransmit.git`

3 切换到ssl-gui分支
`$ git checkout ssl-gui`

4 切换到目录

`$ cd opensslTransmit/qt/ssl-gui/build-ssl-gui-Desktop_Qt_5_14_2_GCC_64bit-Release/
`

![avatar](./imgs/ssl_gui_cd.png)

5 运行客户端

`$ ./ssl-gui`

![avatar](./imgs/ssl_gui_run.png)

6 选择接收

![avatar](./imgs/ssl_gui_run.png)

6 运行客户端程序

`$ ./bin/main client <服务器ip> <服务器端口> <传输文件名>`

![avatar](./imgs/ssl_console_run_as_client.png)

**即可在运行目录(或者指定的目录)找到传输的文件，格式为：clt.年.月.日.时.分.秒(或指定的目录和文件名)**

![avatar](./imgs/ssl_console_result.png)
