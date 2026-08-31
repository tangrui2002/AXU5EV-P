## 今日目标

  完成以下任务：

  - 使用 GDB 设置断点、单步执行和查看变量
  - 定位段错误、空指针、数组越界等问题
  - 配置并生成 Core Dump
  - 使用可执行文件和 Core 文件还原崩溃现场
  - 编写带级别、时间、进程信息的日志
  - 了解 ARM 板上的 gdbserver 远程调试

  ## 1. 准备调试版本（30 分钟）

  编译时保留调试信息：

  gcc -Wall -Wextra -g -Og \
      -fno-omit-frame-pointer \
      src/main.c src/math_utils.c \
      -o hello
        #若报错找不到头文件，添加-Iinclude指定头文件路径
  参数含义：

  - -g：生成 GDB 所需的调试信息
  - -Og：保留较好的调试体验并进行适度优化
  - -O0：几乎不优化，初学时也可以使用
  - -fno-omit-frame-pointer：保留栈帧指针，便于回溯调用栈
        #-fno-omit-frame-pointer：强制保留栈帧指针（FP）。在函数调用时，这个指针用于记录调用链。加上它后，当你用 perf（性能分析工具）或 gdb 发生崩溃时，能够轻松回溯（backtrace）出完整的函数调用栈，而不会丢失关键的调用层级信息。
  检查是否包含调试信息：

  file hello
        #输出结果hello: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=2d6f68291aaeea549783eff464497bd4ee7e2700, for GNU/Linux 3.2.0, with debug_info, not stripped
        #ELF : Linux 下标准的可执行文件格式; 64位；LSB ： 小端； X86-64架构；
        #dynamically linked ： 动态链接 ；interpreter /lib64/ld-linux-x86-64.so.2 ： 动态链接器路径；
        #以及版本标识等等信息
  readelf --debug-dump=info hello | less
        #readelf：Linux 读取和分析 ELF 格式文件工具。
        # --debug-dump=info ： 简写-wi， 提取 ELF 文件中的 .debug_info 节区，包含了源代码中的类型定义、变量声明、函数原型、编译单元等信息。
        
  注意：不要使用 strip hello，否则可能删除调试信息。

  ## 2. GDB 基础操作（2 小时）

  启动 GDB：

  gdb ./hello
        #启动调试
  常用命令：
    >运行控制
  break main
        #可简写为 b main，在 main 函数的入口处设置断点。
  run
        #简写r， 开始运行程序，直到遇到断点或程序崩溃。
  next
        #简写n, 单步逐过程执行（不会停在函数内部）。
  step
        #简写s， 单步逐语句执行（会跳进函数内部）。
  continue
        #简写c， 继续运行
  finish
        #运行完当前函数。
        
    >查看数据 
  print variable（变量）
        #简写p， 打印一次变量的当前值。
  display variable
        #持续显示变量值。
  info locals
        #显示当前函数中所有局部变量的当前值。
  info args
        #显示当前函数接收到的所有实参的值。
        
    >故障回溯及代码定义
  backtrace
        #简写bt， 查看调用栈。显示程序是如何一步步执行到当前这行的（从 main 到当前函数的完整调用链）。程序崩溃时，第一时间执行 bt，能立刻看到是哪个函数、第几行引发的崩溃。
  frame 1
        #切换栈帧。backtrace 会列出多层调用（编号 0, 1, 2...）。frame 1 会把调试视角切换到第 1 层，让你查看该层函数的变量（默认在崩溃的当前帧 0）。
  list
        #列出源代码。默认显示当前执行点附近的 10 行代码。可以快速回顾上下文，不用切出去看源文件。
  quit
        #退出 GDB。如果程序正在运行，GDB 会提示是否终止它

  重点区别：

  - next：执行下一行，不进入函数
  - step：执行下一行，进入函数内部
  - continue：继续运行到下一个断点或异常
  - finish：执行完当前函数并返回
  - bt：查看调用栈

  建议练习：

  1. 在 main 设置断点。
  2. 使用 step 进入 math_utils()。
  3. 用 print 查看参数和返回值。
  4. 用 info locals 查看局部变量。
  5. 在函数返回处使用 finish。

  也可以在启动时直接执行：

  gdb -q -ex 'break main' -ex run ./hello

  ## 3. 制造并定位程序故障（1.5 小时）

  准备一个故意出错的示例，例如：

  int *p = NULL;
  *p = 123;
        #报错Program received signal SIGSEGV, Segmentation fault.
        (gdb) bt
        #0  0x00005555555551a0 in main () at src/main.c:15
        (gdb) info locals 
        x = 5
        y = 3
        result = 8
        p = 0x0
        (gdb) frame 0
        #0  0x00005555555551a0 in main () at src/main.c:15
        15	    *p = 123;
        (gdb) list
        10	    //int a[3] = {1, 2, 3};
        11	    //printf("%d\n", a[10]);
        12	    
        13	    //空指针测试程序
        14	    int *p = NULL;
        15	    *p = 123;
        16	  
        17	    return 0;
        18	}
        (gdb) print p
        $1 = (int *) 0x0
        (gdb) print &p
        $2 = (int **) 0x7fffffffdb28
        (gdb) info registers 
        rax            0x0                 0
        rbx            0x7fffffffdc58      140737488346200
        rcx            0x0                 0
        rdx            0x0                 0
        rsi            0x5555555592a0      93824992252576
        rdi            0x7fffffffd930      140737488345392
        rbp            0x7fffffffdb30      0x7fffffffdb30
        rsp            0x7fffffffdb10      0x7fffffffdb10
        r8             0x7ffff7e03b20      140737352055584
        r9             0x410               1040
        r10            0x1                 1
        r11            0x202               514
        r12            0x1                 1
        r13            0x0                 0
        r14            0x555555557dc0      93824992247232
        r15            0x7ffff7ffd000      140737354125312
        rip            0x5555555551a0      0x5555555551a0 <main+87>
        eflags         0x10246             [ PF ZF IF RF ]
        cs             0x33                51
        ss             0x2b                43
        ds             0x0                 0
        es             0x0                 0
        fs             0x0                 0
        --Type <RET> for more, q to quit, c to continue without paging--q
        Quit
        (gdb) x/i $pc
        => 0x5555555551a0 <main+87>:	movl   $0x7b,(%rax)
        #movl   $0x7b,(%rax)定位到将数据0x7b写入到地址0x0, 导致非法地址写入
        
        
  或者：

  int a[3] = {1, 2, 3};
  printf("%d\n", a[10]);
        #注：GDB 只能在收到信号或断点暂停时显示现场，并不会自动检查 C 数组边界。
  在 GDB 中运行：

  gdb ./crash

  run
  bt
  info locals
  frame 0
  list
  print p
  info registers    #显示 CPU 所有通用寄存器的当前值
  x/i $pc       #以汇编指令的形式，反汇编出程序计数器（PC）当前指向的那条机器码

  定位段错误时，重点关注：

  - Program received signal SIGSEGV
  - 当前停在哪一行
  - bt 显示的调用路径
  - 出错指针或数组下标的值
  - $pc 指向的当前指令

  建议顺便学习：

  watch variable
        #在变量被  写入（修改） 时，程序暂停。
  rwatch variable
        #当指定的变量被  读取  时，程序暂停。
  catch signal SIGSEGV
        #主动捕获段错误信号SIGSEGV。（实际上，GDB 默认就会捕获所有会终止程序的信号，包括 SIGSEGV）
  disassemble /m function_name
        #带源码的混合反汇编。x/i $pc 只看当前这一条。disassemble /m main 会把整个 main 函数的汇编全部倒出来。
  x/16gx address
        #查看从指定地址开始的16 字节，16进制。
        

  ## 4. Core Dump：保存崩溃现场（1.5 小时）

  临时开启 Core Dump：

  ulimit -c unlimited

  查看限制：

  ulimit -a

  运行崩溃程序：

  ./crash

  如果生成了 core 或 core.pid 文件，使用以下命令分析：

  gdb ./crash core

  进入 GDB 后：

  bt
  bt full
  info locals
  info registers
  frame 0
  list
  x/i $pc
  thread apply all bt full

  重点理解：

  > Core 文件保存的是程序崩溃时的内存、寄存器、线程和调用栈信息；它不是完整的可
  > 执行文件，分析时必须提供匹配的可执行文件和调试符号。

  如果没有生成 Core 文件，检查：

  cat /proc/sys/kernel/core_pattern

  某些系统由 systemd-coredump 接管，可以尝试：

  coredumpctl list
  coredumpctl info
  coredumpctl debug

  生产环境还要注意：

  - Core 文件可能很大
  - 可能包含密码、密钥和用户数据
  - 应限制保存目录和访问权限
  - 可使用 coredump_filter 控制内存段是否写入

  ## 5. 日志定位问题（1.5 小时）

  先掌握最基本的输出：

  fprintf(stderr, "[ERROR] open config failed: %s\n", strerror(errno));
  perror("open");

  日志至少应包含：

  时间戳 + 日志级别 + 进程/线程信息 + 文件和行号 + 关键上下文

  例如：

  2026-08-28 14:20:31 [ERROR] pid=1234 main.c:42 open config failed: Permission
  denied

  建议定义日志级别：

  DEBUG   调试细节
  INFO    正常运行信息
  WARN    可恢复异常
  ERROR   操作失败
  FATAL   即将退出的严重错误

  练习内容：

  - 为程序增加 LOG_DEBUG()、LOG_INFO()、LOG_ERROR() 宏
  - 日志输出到 stderr
  - 同时写入文件
  - 记录 errno、参数、状态值和关键路径
  - 在关键操作前后打印日志
  - 避免只打印“失败”，而要打印“哪个操作、哪个对象、为什么失败”

  注意日志缓冲问题：

  fflush(stderr);

  如果输出到普通文件，程序异常退出时，缓冲区内容可能尚未写入文件。

  Linux 服务中还可以学习：

  logger "hello from app"
  journalctl -f
  journalctl -u your-service

  ## 6. GDB、Core 和日志的联合分析（1 小时）

  模拟一次完整排障：

  1. 日志发现某次请求处理失败。
  2. Core 文件显示收到 SIGSEGV。
  3. 使用 bt full 找到调用路径。
  4. 使用 frame、list、print 查看现场变量。
  5. 回到日志确认输入参数和业务状态。
  6. 修复代码后重新运行验证。

  推荐形成固定流程：

  先看日志时间和错误上下文
          ↓
  确认是否有 Core 文件
          ↓
  gdb 程序 Core
          ↓
  bt full / thread apply all bt full
          ↓
  检查当前栈帧、变量和寄存器
          ↓
  复现问题并设置断点或 watchpoint
          ↓
  修复后回归测试

  ## 7. ARM 板远程调试（1 小时）

  目标板上启动：

  gdbserver :1234 ./hello

  主机上使用交叉 GDB：

  aarch64-linux-gnu-gdb ./hello

  在 GDB 中：

  set sysroot /path/to/sysroot
  target remote 192.168.1.100:1234
  break main
  continue

  如果动态库或源码路径不一致，可设置：

  set solib-search-path /path/to/target-libs
  directory /path/to/source

  注意：

  - 主机上的可执行文件必须与目标板运行版本匹配
  - 交叉 GDB、目标程序和动态库的架构及调试符号要对应
  - 目标板上的程序通常不建议包含完整调试符号，可在主机保留带符号副本

  ## 今日验收清单

  - [X] 能使用 break、run、next、step
  - [X] 能用 bt 查看函数调用栈
  - [X] 能用 print、info locals 检查变量
  - [X] 能定位一次空指针或段错误
  - [ ] 能配置 ulimit -c unlimited
  - [ ] 能使用 gdb program core 分析 Core 文件
  - [ ] 能解释为什么 Core 必须匹配可执行文件和符号
  - [ ] 能设计基本日志级别和日志格式
  - [ ] 能使用日志结合调用栈定位问题
  - [ ] 能用 gdbserver 进行 ARM 远程调试

  ## 推荐学习顺序

  调试版本编译
    ↓
  GDB 断点与单步
    ↓
  调用栈和变量分析
    ↓
  Core Dump 配置与分析
    ↓
  日志设计与 errno
    ↓
  GDB + Core + 日志联合排障
    ↓
  ARM 目标板远程调试

  补充一点：完成这条路线后，可以再学习 AddressSanitizer 和 Valgrind，它们对定位
  越界、Use-after-free 和内存泄漏非常有帮助，但应作为 GDB/Core 的补充工具。
