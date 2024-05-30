# 默认的工具链设置，可以通过环境变量覆盖
CC ?= gcc
AR ?= ar

# 编译标志
CFLAGS = -Wall -Wextra -std=c99 -Icircular_buffer/port -Icircular_buffer/src -Itools/unity

# 可执行文件和目录名称
BIN_DIR = bin
TARGET = $(BIN_DIR)/circular_buffer_example
TEST_TARGET = $(BIN_DIR)/run_tests

# 静态库名称
LIBRARY_DIR = lib
LIBRARY = $(LIBRARY_DIR)/libcircular_buffer.a

# 源文件
SRCS = circular_buffer_example/example.c circular_buffer/src/circular_buffer.c circular_buffer/port/port.c

# 测试源文件
TEST_SRCS = test_case/test_circular_buffer.c tools/unity/unity.c

# 对应的对象文件
OBJS = $(SRCS:.c=.o)

# 测试对象文件
TEST_OBJS = $(TEST_SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 创建必要的目录
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(LIBRARY_DIR):
	mkdir -p $(LIBRARY_DIR)

# 可执行文件编译规则
$(TARGET): circular_buffer_example/example.o circular_buffer/port/port.o $(LIBRARY) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

# 测试可执行文件编译规则
$(TEST_TARGET): $(TEST_OBJS) circular_buffer/src/circular_buffer.o circular_buffer/port/port.o tools/unity/unity.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

# 静态库编译规则
lib: $(LIBRARY)

#$(LIBRARY): circular_buffer/src/circular_buffer.o circular_buffer/port/port.o | $(LIBRARY_DIR)
#	$(AR) rcs $@ $^

$(LIBRARY): circular_buffer/src/circular_buffer.o | $(LIBRARY_DIR)
	$(AR) rcs $@ $^

# 生成依赖关系
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理规则
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_TARGET)
	rm -f $(LIBRARY_DIR)/*
	rm -rf $(BIN_DIR)

.PHONY: all clean lib run_tests

# 添加run_tests目标
run_tests: $(TEST_TARGET)
