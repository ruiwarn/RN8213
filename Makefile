CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O0
INCLUDES = -I.
LIBS = -lm

# 源文件
SOURCES = metering.c metering_test.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = metering_test

# 默认目标
all: $(TARGET)

# 编译目标
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET).exe $(LIBS)

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 运行测试
test: $(TARGET)
	$(TARGET).exe

# 清理
clean:
	del /Q $(OBJECTS) $(TARGET).exe 2>nul || echo "Clean completed"

# 声明伪目标
.PHONY: all test clean

# 依赖关系
metering.o: metering.c metering.h
metering_test.o: metering_test.c metering.h 