#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>     // for getpid
#include "math_utils.h" // 你的 add 函数声明
#include <stdarg.h>   // 提供 va_list、va_start、va_copy、va_end

// ================== 日志级别定义 ==================
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO  = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_FATAL = 4
} log_level_t;

// 编译时日志级别（可调：-DLOG_LEVEL=LOG_LEVEL_INFO）
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG   // 默认全部打印
#endif

// ================== 全局日志文件句柄 ==================
static FILE *g_log_file = NULL;

// ================== 内部日志写入函数 ==================
static void log_write(log_level_t level, const char *file, int line, const char *func,
                      const char *fmt, ...) {
    if (level < LOG_LEVEL) return;   // 低于编译级别则不输出

    // 级别名称映射
    const char *level_names[] = {
        "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    };

    // 防止非法级别导致 level_names 数组越界
    if (level < LOG_LEVEL_DEBUG || level > LOG_LEVEL_FATAL) {
        level = LOG_LEVEL_ERROR;
    }

    // 时间戳
    time_t now = time(NULL);
    struct tm tm_local;
    localtime_r(&now, &tm_local);
    char time_buf[32];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &tm_local);

    // 构造消息前缀：时间 + 级别 + PID + 文件:行号 + 函数
    fprintf(stderr, "%s [%s] pid=%d %s:%d %s() -> ",
            time_buf, level_names[level], getpid(), file, line, func);
    if (g_log_file) {
        fprintf(g_log_file, "%s [%s] pid=%d %s:%d %s() -> ",
                time_buf, level_names[level], getpid(), file, line, func);
    }

    // 可变参数输出：必须在第一次使用 args 前复制，不能在 vfprintf 后再 va_copy
    va_list args;
    va_list args_copy;
    va_start(args, fmt);
    if (g_log_file) {
        va_copy(args_copy, args);
    }

    vfprintf(stderr, fmt, args);
    if (g_log_file) {
        vfprintf(g_log_file, fmt, args_copy);
        va_end(args_copy);
    }
    va_end(args);

    // 换行并刷新缓冲区（保证异常退出时数据不丢失）
    fputc('\n', stderr);
    fflush(stderr);
    if (g_log_file) {
        fputc('\n', g_log_file);
        fflush(g_log_file);
    }
}

// ================== 对外日志宏 ==================
#define LOG_DEBUG(fmt, ...) \
    log_write(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    log_write(LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) \
    log_write(LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    log_write(LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_FATAL(fmt, ...) \
    log_write(LOG_LEVEL_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

// ================== 日志初始化 ==================
int log_init(const char *log_path) {
    g_log_file = fopen(log_path, "a");
    if (!g_log_file) {
        fprintf(stderr, "Failed to open log file %s: %s\n",
                log_path, strerror(errno));
        return -1;
    }
    setvbuf(g_log_file, NULL, _IOLBF, 0);   // 行缓冲
    LOG_INFO("Log system initialized (file: %s)", log_path);
    return 0;
}

void log_close(void) {
    if (g_log_file) {
        LOG_INFO("Log system shutting down");
        fclose(g_log_file);
        g_log_file = NULL;
    }
}

// ================== main 入口 ==================
int main(int argc, char *argv[]) {
    // 初始化日志（写入 ./log/app.log，追加模式）
    if (log_init("./log/app.log") != 0) {
        fprintf(stderr, "Log init failed, exiting.\n");
        return 1;
    }

    LOG_INFO("Program started, argc=%d", argc);
    // 记录命令行参数（关键上下文）
    for (int i = 0; i < argc; i++) {
        LOG_DEBUG("argv[%d] = %s", i, argv[i]);
    }

    int x = 5, y = 3;
    LOG_INFO("准备调用 add(%d, %d)", x, y);
    int result = add(x, y);
    LOG_INFO("add 返回结果: %d", result);

    printf("%d + %d = %d\n", x, y, result);

    // 模拟一个可能失败的操作（打开不存在的文件）
    FILE *fp = fopen("/nonexist/file", "r");
    if (!fp) {
        LOG_ERROR("open /nonexist/file failed: %s (errno=%d)",
                  strerror(errno), errno);
    } else {
        LOG_INFO("成功打开文件");
        fclose(fp);
    }

    // 数组越界测试（未定义行为，可能崩溃，也可能输出随机值）
    // int a[3] = {1,2,3};
    // LOG_WARN("即将访问越界索引 a[10]，当前数组大小=3");
    // printf("%d\n", a[10]);

    // 空指针测试（同样可取消注释）
    // int *p = NULL;
    // LOG_WARN("即将对空指针赋值");
    // *p = 123;

    LOG_INFO("程序正常结束");
    log_close();
    return 0;
}
