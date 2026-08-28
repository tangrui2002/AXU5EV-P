## 今日目标
  完成一个小型 C 项目：
  hello-c/
  ├── include/
  │   └── math_utils.h
  ├── src/
  │   ├── main.c
  │   └── math_utils.c
  ├── Makefile
  └── CMakeLists.txt 
  最终得到：
  - GCC 编译的本机可执行文件
  - Make 构建版本
  - CMake 构建版本
  - ARM 交叉编译版本
  - 能解释编译、链接、构建和交叉编译的区别

        总结：mkdir -p hello-c/{include,src}  创建项目目录

  ## 1. GCC 基础：编译器到底做了什么（1.5 小时）

  学习四个阶段：

  预处理 → 编译 → 汇编 → 链接

  练习：

  gcc -E src/main.c -o main.i
        总结：预处理，编译第一步。
            做了什么：1.将头文件拷贝到源文件；2.展开所有宏定义；3.处理条件编译。
            -o：制定输出文件名
            输出：.i文件。内容比源文件庞大的多，因为展开了头文件。
  gcc -S src/main.c -o main.s
        总结：编译为汇编，第二步。
            做了什么：将预处理后的代码翻译为汇编语言。
            输出：.s文件，纯文本的汇编文件。
  gcc -c src/main.c -o main.o
        总结：汇编（不链接），将汇编代码（.s）翻译为二进制。
            输出：.o文件，二进制文件，不能独立运行，因为没有标准库和其他.o文件进行链接。
  gcc src/main.c src/math_utils.c -o hello
        总结：完整的 编译+链接
            链接：将两个临时的.o文件合并，最终生成一个可执行文件hello。
  重点掌握：

  gcc -Wall -Wextra -g -O0
  gcc -Iinclude ...
  gcc -DDEBUG ...
  gcc -L/path -lname ...
        总结：-Wall （Warnings all）开启常用警告
            -Wextra （Warnings extra）开启额外警告，比-Wall更细致
            -g (Generate debug info) 在可执行文件中嵌入调试信息（如变量名，行号，函数名）
            -Iinclude  指定头文件搜索路径
            -DDEBUG  预定义宏（编译期开关）
                例：
                #ifdef DEBUG
                    printf("调试信息：变量 x = %d\n", x);  // 只有定义了 DEBUG 才打印
                #endif
            -L/路径 -l库名     链接外部库（链接阶段）
  理解：

  - .c：源文件
  - .i：预处理结果
  - .s：汇编代码
  - .o：目标文件
  - 可执行文件：链接后的结果
  - -I、-L、-l 分别用于头文件、库目录和库文件

  建议马上练习：

  ./hello
  file hello
  ldd hello

  ## 2. Make：自动化管理编译依赖（1.5 小时）

  先手写最小 Makefile：

  CC := gcc     #指定编译工具gcc
  CFLAGS := -Wall -Wextra -g -Iinclude      #指定编译选项

  TARGET := hello       #编译目标文件名
  SRCS := src/main.c src/math_utils.c       #列出所有源文件
  OBJS := $(SRCS:.c=.o)     #把 SRCS 中所有 .c 后缀替换成 .o

  $(TARGET): $(OBJS)        #依赖关系，hello 依赖于 src/main.o 和 src/math_utils.o
  	$(CC) $(OBJS) -o $@     
        #等价于gcc src/main.o src/math_utils.o -o hello

  src/%.o: src/%.c          
        # %是通配符，意思是“任何 src/ 目录下的 .o 文件，都依赖 src/ 下同名的 .c 文件”
  	$(CC) $(CFLAGS) -c $< -o $@     
        #等价于gcc -Wall -Wextra -g -Iinclude -c src/main.c -o src/main.o

  .PHONY: all clean     
        #声明all和clean是伪目标，即告诉Make它们不代表真实存在的文件名
        #如果你不声明，而当前目录恰好有一个名为 clean 的文件，执行 make clean 会误以为文件已存在且没有依赖，从而什么都不做。.PHONY 强制 Make 每次都执行下面的命令。

  all: $(TARGET)

  clean:
  	rm -f $(OBJS) $(TARGET)     #清除所有编译产物

  练习命令：

  make
        #增量编译，只更新改动过的部分,会生成新的 .o 和 hello
  make clean
        #删除所有编译生成的临时文件和目标文件
  make -n
        #预览即将执行的命令，但不实际运行
  make -j$(nproc)
        #全核并行编译，速度拉满, 内容和make一样

  重点理解自动变量：

  - $@：目标文件
  - $<：第一个依赖
  - $^：全部依赖

  然后增加 Debug/Release：

  CFLAGS_DEBUG := -Wall -Wextra -g -O0 -Iinclude
  CFLAGS_RELEASE := -Wall -Wextra -O2 -DNDEBUG -Iinclude

  学习重点不是记语法，而是理解：

  > 目标文件只在源文件或头文件变化后重新编译。

  ## 3. CMake：跨平台生成构建系统（2 小时）

  编写 CMakeLists.txt：
#CMakeLists.txt
        #声明CMake最低版本
  cmake_minimum_required(VERSION 3.16)
        #CMake初始化（项目名称  版本号  语言
  project(hello_c VERSION 1.0 LANGUAGES C)
        ）
        #指定编译器C11标准
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_C_STANDARD_REQUIRED ON)
        #定义一个可执行文件目标（Target）名为 hello，并由这两个源文件构建而成
  add_executable(hello
      src/main.c
      src/math_utils.c
  )
        #为构建目标 hello 添加头文件搜索路径
  target_include_directories(hello PRIVATE include)
        #为构建目标 hello 添加编译选项
  target_compile_options(hello PRIVATE
      -Wall
      -Wextra
  )

  推荐使用独立构建目录：

  cmake -S . -B build
        #配置阶段（生成Makefile）
        #-S : Source(源码目录)缩写， . 表示当前目录（即CMakeLists.txt 所在的位置）。
        #-B build : -B是Build的缩写， 把所有临时文件和生成的 Makefile 都放到 build 这个文件夹里。
        #干了什么：创建了 build 文件夹，读取 CMakeLists.txt，分析编译需求（头文件路径、源文件、编译选项），最后在 build 文件夹里生成了一个 Makefile
  cmake --build build
        #构建阶段（执行编译）
        #--build ： Cmake通用构建命令，不关心你底层用的是 make、ninja 还是 Visual Studio。
        #build ： 指定build文件夹。
        #干了什么： 找到上一步生成的 Makefile，然后执行 make 命令（编译、链接，生成可执行文件hello）。
  ./build/hello

  Debug 和 Release：
#Debug:
  cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
  cmake --build build-debug
        #-DCMAKE_BUILD_TYPE=Debug : -D 用于定义 CMake 变量，这里把 CMAKE_BUILD_TYPE 设置为 Debug，
            它会在生成的 Makefile 中，默认给编译器加上-g -O0,
            等价于手写 Makefile 时定义的 CFLAGS_DEBUG := -Wall -Wextra -g -O0 -Iinclude。
#Release（发布版）:
  cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
  cmake --build build-release
        #-DCMAKE_BUILD_TYPE=Release ： 把 CMAKE_BUILD_TYPE 设置为 Debug，
            等价于之前定义的 CFLAGS_RELEASE := -Wall -Wextra -O2 -DNDEBUG -Iinclude
  重点掌握几个概念：

  - project()
  - add_executable()
  - add_library()
  - target_include_directories()
  - target_compile_options()
  - target_link_libraries()
  - cmake -S -B
  - cmake --build

  理解 CMake 的定位：
  > CMake 通常不直接编译，而是生成 Makefiles、Ninja 文件或 IDE 工程。
        #真正工作的还是gcc
        
  ## 4. 库和链接：把 GCC、Make、CMake 串起来（1 小时）

  增加一个静态库：

  add_library(math_utils STATIC src/math_utils.c)
        #创建静态库(把 math_utils.c 打包成 .a 文件)
  target_include_directories(math_utils PUBLIC include)
        #声明库，如果链接，会自动获得 include 目录的搜索权限
  add_executable(hello src/main.c)
  target_link_libraries(hello PRIVATE math_utils)
        #链接，将hello和math_utils库链接
  本阶段理解：

  - 静态库：.a
        #加载到可执行文件中
  - 动态库：.so
        #不加载，运行时再加载
  - 编译阶段和链接阶段的区别
  - PUBLIC、PRIVATE 的基本含义
        #PUBLIC：公共库， PRIVATE：私有库
  使用命令检查：

  file build/hello
  nm build/hello

  ## 5. 交叉编译：为 ARM 目标板生成程序（2 小时）

  先确认交叉工具链：

  which aarch64-linux-gnu-gcc
  aarch64-linux-gnu-gcc --version

  如果目标是 64 位 Zynq MPSoC Linux，通常使用：

  aarch64-linux-gnu-gcc

  如果是 32 位 ARM，则可能使用：

  arm-linux-gnueabihf-gcc

  直接交叉编译：

  aarch64-linux-gnu-gcc \
      -Wall -Wextra \
      -Iinclude \
      src/main.c src/math_utils.c \
      -o hello-arm64
        #语法和gcc相同，只是编译器本身换成了专门生成 ARM 机器码的版本
  检查结果：

  file hello-arm64

  应看到类似：

  ELF 64-bit LSB executable, ARM aarch64

  注意：本机生成的程序不能直接在 ARM 板上运行，必须使用目标架构工具链，并匹配目
  标系统的 C 库和 sysroot。

  ## 6. 用 CMake 做交叉编译（重点，1 小时）
        #交叉编译（Cross Compilation）：在 A 平台上编译，但编译出来的程序是为了在 B 平台上运行。
  创建 toolchain-aarch64.cmake：

  set(CMAKE_SYSTEM_NAME Linux)
  set(CMAKE_SYSTEM_PROCESSOR aarch64)
        #指定系统
  set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
  set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
        #指定交叉编译器
  执行：

  cmake -S . -B build-arm64 \
      -DCMAKE_TOOLCHAIN_FILE=toolchain-aarch64.cmake
        #-DCMAKE_TOOLCHAIN_FILE=toolchain-aarch64.cmake 指定toolchain-aarch64.cmake为工具链配置文件
  cmake --build build-arm64
  file build-arm64/hello

  如果工具链带有 sysroot，还应补充：

  set(CMAKE_SYSROOT /path/to/sysroot)
  set(CMAKE_FIND_ROOT_PATH /path/to/sysroot)

  实际项目中，sysroot 应来自目标板 rootfs、SDK 或 Yocto/厂商工具链。

  ## 今日验收清单

  完成以下任务就算达标：

  - [X] 能解释 GCC 四个编译阶段
  - [X] 能使用 gcc -Wall -g 编译程序
  - [X] 能写出支持 make 和 make clean 的 Makefile
  - [X] 能用 CMake 的独立构建目录编译
  - [X] 能生成并链接一个静态库
  - [X] 能区分本机编译和交叉编译
  - [X] 能用 file 确认 ELF 的目标架构
  - [X] 能用 CMake toolchain 文件生成 ARM 程序

  ## 学习顺序建议

  今天按这个顺序最合适：

  GCC 命令
    ↓
  目标文件与链接
    ↓
  Make 依赖管理
    ↓
  CMake 工程管理
    ↓
  交叉工具链
    ↓
  CMake 交叉编译

