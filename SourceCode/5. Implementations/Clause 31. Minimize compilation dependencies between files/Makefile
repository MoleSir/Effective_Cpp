# 编译器设置
CC := g++
# 头文件目录
CPPFLAGS := -I include 

# 文件目录
SRC := ./src
# obj文件目录
OBJ_PATH := ./bin/obj
# exe文件目录
EXE_PATH := ./bin/exe
# 各个cpp文件所在的目录：在这之后添加新的文件夹即可
SRC_PATH := $(SRC) 


# 从cpp文件夹中获取所有cpp文件
SRCS := $(foreach dir, $(SRC_PATH), $(wildcard $(dir)/*.cpp))
# 去掉.cpp的前缀
SRCS_WITHOUT_DIR:=$(notdir $(SRCS))
# 将后缀.cpp改为.o
OBJS_WITHOUT_DIR:=$(patsubst %.cpp,%.o,$(SRCS_WITHOUT_DIR))
# 给.o文件加上obj文件目录
OBJS:=$(addprefix $(OBJ_PATH)/,$(OBJS_WITHOUT_DIR))

# 为编译器添加源文件搜索目录
VPATH := $(SRC_PATH) 
# 目标
TARGET := ./bin/exe/Application.exe

# 连接目标
$(TARGET) : $(OBJS) 
	$(CC) $^ -o $@
# 编译每个.cpp文件
$(OBJ_PATH)/%.o : %.cpp
	$(CC) -c $(CPPFLAGS) $< -o $@


# 伪目标
.PHYON: run

# 运行exe文件
run:
	@$(TARGET)