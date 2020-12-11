# ssl_file_server
an openssl based crypto file server，the name is ytp-server.

.
├── 1.png  
├── cmd.cpp  
├── keys  
│   ├── cacert.pem  
│   └── privkey.pem  
├── makefile  
├── obj  
│   ├── cmd.o  
│   ├── login.o  
│   ├── PamClass.o  
│   ├── server.o  
│   ├── ssl_util.o  
│   ├── user.o  
│   └── ytp.o  
├── pam_util  
│   ├── login.cpp  
│   ├── login.h  
│   ├── login_test.cpp  
│   ├── makefile  
│   ├── PamClass.cpp  
│   └── PamClass.h  
├── README.md  
├── release  
│   ├── makefile  
│   └── server  
├── server.cpp  
├── ssl_util  
│   ├── makefile  
│   ├── ssl_util.cpp  
│   └── ssl_util.h  
├── test  
│   ├── debug.txt  
│   ├── pamtest  
│   ├── pamtest.c  
│   ├── test  
│   └── test.cpp          
├── user.cpp   
├── user.h  
└── ytp_util  
    ├── makefile  
    ├── ytp.cpp  
    └── ytp.h  

系统使用多进程，支持并发连接，同时需要按linux系统中的用户登录，登陆时会将进程权限设置为相应用户的权限；

运行方式：make-->cd release-->./server

即可与client交互

ytp与ftp类似，支持解析所有常见linux命令，包括ls,cd,pwd；在命令前 再加l表示在本地执行,如lls查看本地工作目录的文件，以实现定位。

向服务器发送文件：sendfile filename

从服务器接受文件:getfile filename

