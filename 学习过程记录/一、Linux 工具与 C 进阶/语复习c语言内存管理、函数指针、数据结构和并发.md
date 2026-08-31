## 今日目标

  完成后应能够：

  - 正确使用 malloc/free/realloc
  - 识别内存泄漏、越界、Use-after-free、double free
  - 使用函数指针实现回调和分发表
  - 实现链表、栈、队列或环形缓冲区
  - 使用 pthread 创建线程、互斥锁和条件变量
  - 识别竞态条件和死锁
  - 编译并运行带 AddressSanitizer 的程序

  ## 1. C 语言内存管理（2 小时）

  先复习四类存储区域：

  代码区
  全局/静态区
  栈区
  堆区

  重点练习：

  int *p = malloc(10 * sizeof(*p));
  if (p == NULL) {
      return -1;
  }

  free(p);
  p = NULL;

  掌握：

  malloc()
  calloc()
  realloc()
  free()
  sizeof

  重点理解：

  - malloc 分配的内容未初始化
  - calloc 分配后会清零
  - realloc 可能改变内存地址
  - free() 后不能继续访问原指针
  - 每次成功分配必须有明确的释放责任
  - sizeof(ptr) 不等于指针指向对象的大小

  建议故意制造并观察这些错误：

  int *p = malloc(3 * sizeof(*p));
  p[3] = 10;       // 越界
  free(p);
  printf("%d\n", p[0]);  // Use-after-free
  free(p);         // double free

  使用工具检查：

  gcc -g -O0 -Wall -Wextra \
      -fsanitize=address,undefined \
      -fno-omit-frame-pointer \
      test_memory.c -o test_memory

  ./test_memory

  如果安装了 Valgrind：

  valgrind --leak-check=full ./test_memory

  ## 2. 函数指针和回调（1.5 小时）

  先掌握基本语法：

  int add(int a, int b);

  int (*operation)(int, int);
  operation = add;

  int result = operation(5, 3);

  实现一个计算器：

  typedef int (*binary_op_t)(int, int);

  int calculate(int a, int b, binary_op_t op) {
      return op(a, b);
  }

  然后学习回调：

  typedef void (*event_handler_t)(const char *message);

  void process_event(const char *message, event_handler_t handler) {
      handler(message);
  }

  重点理解：

  - 函数指针类型必须匹配参数和返回值
  - 回调函数可以把“处理逻辑”传给通用函数
  - void *context 可以传递用户自定义上下文
  - 函数指针本身不负责内存管理

  建议实现一个简单分发表：

  typedef void (*command_func_t)(void);

  struct command {
      const char *name;
      command_func_t handler;
  };

  这类设计在驱动、命令行解析和事件系统中很常见。

  ## 3. 数据结构复习（2 小时）

  建议按以下顺序：

  ### 链表

  实现：

  struct node {
      int value;
      struct node *next;
  };

  练习：

  - 头插
  - 尾插
  - 删除指定节点
  - 查找
  - 遍历
  - 释放整个链表

  ### 栈和队列

  使用数组实现：

  #define QUEUE_CAPACITY 16

  struct queue {
      int data[QUEUE_CAPACITY];
      size_t head;
      size_t tail;
      size_t count;
  };

  练习：

  - queue_push
  - queue_pop
  - 判断空和满
  - 处理容量边界

  ### 环形缓冲区

  重点理解：

  head：写入位置
  tail：读取位置
  满：count == capacity
  空：count == 0

  环形缓冲区非常适合串口、网络接收和生产者/消费者场景。

  ### 哈希表

  如果时间充足，再实现简单的字符串哈希表，重点理解：

  - 哈希函数
  - 冲突处理
  - 链地址法
  - 键和值的所有权

  每种结构都要明确：

  谁分配？
  谁修改？
  谁释放？
  是否允许 NULL？
  是否线程安全？

  ## 4. pthread 并发基础（2 小时）

  创建线程：

  #include <pthread.h>

  void *worker(void *arg) {
      return NULL;
  }

  pthread_t tid;
  pthread_create(&tid, NULL, worker, NULL);
  pthread_join(tid, NULL);

  重点学习：

  pthread_create()
  pthread_join()
  pthread_mutex_init()
  pthread_mutex_lock()
  pthread_mutex_unlock()
  pthread_cond_wait()
  pthread_cond_signal()

  先观察竞态条件：

  int counter = 0;

  void *worker(void *arg) {
      for (int i = 0; i < 100000; i++) {
          counter++;
      }
      return NULL;
  }

  两个线程同时执行后，结果通常小于预期。

  使用互斥锁修复：

  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

  pthread_mutex_lock(&lock);
  counter++;
  pthread_mutex_unlock(&lock);

  编译时必须链接 pthread：

  gcc -Wall -Wextra -g counter.c -o counter -pthread

  ## 5. 条件变量和生产者/消费者（1.5 小时）

  使用前面实现的队列，构造：

  生产者线程 → 放入任务
  消费者线程 → 取出任务并执行

  队列为空时，消费者等待：

  while (queue_is_empty(&queue)) {
      pthread_cond_wait(&not_empty, &mutex);
  }

  生产者放入任务后通知：

  pthread_cond_signal(&not_empty);

  必须牢记：

  - pthread_cond_wait() 必须在持有互斥锁时调用
  - 条件判断要使用 while，不要使用 if
  - 等待前检查条件，唤醒后重新检查条件
  - 线程退出前要正确 join
  - 共享数据的读写必须有同步策略

  ## 6. 死锁、原子变量和 volatile（1 小时）

  制造死锁：

  线程 A：先锁 lock1，再锁 lock2
  线程 B：先锁 lock2，再锁 lock1

  解决方式：

  - 所有线程按固定顺序加锁
  - 缩短持锁时间
  - 尽量避免嵌套锁
  - 必要时使用 pthread_mutex_trylock

  学习原子操作：

  #include <stdatomic.h>

  atomic_int counter;
  atomic_fetch_add(&counter, 1);

  要区分：

  mutex：保护临界区和复杂数据结构
  atomic：保护简单共享变量
  volatile：告诉编译器变量可能被外部改变，不提供线程同步

  volatile 不能替代互斥锁或原子变量。

  ## 7. 综合项目：线程安全任务队列

  建议目录：

  hello-c/
  ├── include/
  │   ├── queue.h
  │   └── logger.h
  ├── src/
  │   ├── main.c
  │   ├── queue.c
  │   └── logger.c
  └── Makefile

  功能要求：

  - 主线程创建任务
  - 生产者线程将任务放入队列
  - 消费者线程从队列取任务
  - 每个任务包含函数指针和参数
  - 执行过程记录日志
  - 程序退出时释放所有堆内存
  - 使用互斥锁和条件变量保证线程安全

  任务结构可以设计为：

  typedef void (*task_func_t)(void *arg);

  struct task {
      task_func_t func;
      void *arg;
  };

  编译：

  gcc -Wall -Wextra -g -O0 \
      -fsanitize=address,undefined \
      -pthread \
      src/*.c -Iinclude -o task_queue

  ## 今日验收清单

  - [ ] 能解释栈、堆、静态区的区别
  - [ ] 能正确使用 malloc/calloc/realloc/free
  - [ ] 能用 ASan 定位一次内存越界
  - [ ] 能写出函数指针和回调函数
  - [ ] 能实现链表或环形队列
  - [ ] 能创建并等待 pthread
  - [ ] 能用互斥锁修复竞态条件
  - [ ] 能用条件变量实现生产者/消费者
  - [ ] 能解释死锁产生原因和解决方法
  - [ ] 能区分 mutex、atomic 和 volatile
  - [ ] 能完成线程安全任务队列

  推荐每天复习时坚持一个原则：

  先明确对象的生命周期，
  再明确数据的所有权，
  最后设计线程之间的访问规则。

  这三个问题能覆盖 C 程序中大部分内存和并发故障。
