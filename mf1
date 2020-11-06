TOPDIR = $(shell pwd)
CPP = g++
VPATH = client-console-version:socketUtil:timeCounter:client
INCLUDE_DIR = -I$(TOPDIR)/client-console-version \
			  -I$(TOPDIR)/socketUtil \
			  -I$(TOPDIR)/timeCounter \
			  -I$(TOPDIR)/client \
			  -I$(TOPDIR)/server

CPPFLAGS = $(INCLUDE_DIR) 

main: main.o socketUtil.o socketClient.o timeCounter.o
	$(CPP) $(INCLUDE_DIR) -o main main.o socketClient.o socketUtil.o  timeCounter.o
main.o:main.cpp socketUtil.h
	$(CPP) $(INCLUDE_DIR) -c main.cpp
socketClient.o: socketClient.cpp
	$(CPP) $(INCLUDE_DIR) -c socketClient.cpp
socketUtil.o: socketUtil.cpp
	$(CPP) $(INCLUDE_DIR) -c socketUtil.cpp
timeCounter.o: timeCounter.cpp
	$(CPP) $(INCLUDE_DIR) -c timeCounter.cpp