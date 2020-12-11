SUBDIRS:=ssl_util pam_util ytp_util
.PHONY:clean test ECHO all
all:$(SUBDIRS) server
CFILES:=$(wildcard *.cpp)
FLAGS:= -std=c++11 -w 
LIBS:= -lssl -lpam -lcrypto
OBJDIR:=$(shell pwd)/obj
export FLAGS OBJDIR LIBS
#CFILES+=$(foreach n,$(SUBDIRS),$(wildcard $n/*.cpp))
#OBJFILES:=$(patsubst %.cpp,%.o,$(CFILES))
CFILES:=$(wildcard *.cpp)
OBJFILES:=$(patsubst %.cpp,$(OBJDIR)/%.o,$(CFILES))
#C=3
test:
	@echo $(CFILES)
	@echo $(OBJFILES)
	@echo $(OBJS)
$(SUBDIRS):ECHO
	make -C $@
server:$(OBJFILES)
	make -C release
#C+=1
#echo $C
#除非使用了export，否则makefile中的变量在make编译时就会被展开
$(OBJDIR)/%.o:%.cpp %.h
	g++ $< -c -o $@ $(FLAGS) 
$(OBJDIR)/%.o: %.cpp
	g++ $< -c -o $@ $(FLAGS)
ECHO:
	@echo $(SUBDIRS)
clean:
	-rm release/server  $(OBJDIR)/*.o