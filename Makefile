TOPDIR = $(shell pwd)
CPP = g++

DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

DIR_DBG = ./debug
DIR_SRC_DBG = $(DIR_SRC)
DIR_OBJ_DBG = $(DIR_DBG)/obj
DIR_BIN_DBG = $(DIR_DBG)/bin


OBJ_FLAGS = -I./include -c -o 
MAIN_FLAGS = -I./include -o

DBG_OBJ_FLAGS = -I./include -g -c -o 
DBG_MAIN_FLAGS = -I./include -g -o

SRC = $(wildcard $(DIR_SRC)/*.cpp)
DIR = $(notdir $(SRC))
OBJ = $(patsubst %.cpp, $(DIR_OBJ)/%.o, $(notdir $(SRC)))

DBG_OBJ = $(patsubst %.cpp, $(DIR_DBG)/obj/%.o, $(notdir $(SRC)))

release: ./bin/main

./bin/main: $(OBJ)
	$(CPP) $(MAIN_FLAGS) $@ $^
$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	$(CPP) $(OBJ_FLAGS) $@ $<

debug: ./debug/bin/main

 ./debug/bin/main: $(DBG_OBJ)
	$(CPP) $(DBG_MAIN_FLAGS) $@ $^
$(DIR_OBJ_DBG)/%.o: $(DIR_SRC_DBG)/%.cpp
	$(CPP) $(DBG_OBJ_FLAGS) $@ $<

clean:
	rm -rf ./obj/*

dbgprint:
	@echo $(SRC)
	@echo $(DIR)
	@echo $(OBJ)