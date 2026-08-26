# 嵌入式 Linux 学习路线清单

目标：嵌入式 Linux / SoC 开发工程师，兼具驱动、应用和 FPGA 协同能力。

## 一、Linux 工具与 C 进阶（2～3 周）

- [X] 熟悉 Linux 命令行和 Shell 基础
- [ ] 使用 Git 管理代码，掌握分支、提交和回退
- [ ] 掌握 GCC、Make、CMake 和交叉编译
- [ ] 使用 GDB、core dump 和日志定位问题
- [ ] 复习 C 语言内存管理、函数指针、数据结构和并发
- [ ] 完成一个 PC 端 TCP/UDP 服务程序
- [ ] 将程序交叉编译并部署到 AXU5EV-P

## 二、Linux 用户态开发（4～6 周）

- [ ] 掌握进程、线程、信号和 IPC
- [ ] 掌握 Socket、epoll、文件系统和 mmap
- [ ] 掌握 mutex、条件变量、信号量和线程安全
- [ ] 学习定时器、配置文件和分级日志
- [ ] 完成“传感器数据采集网关”项目：串口采集、线程处理、TCP 上传、本地存储
- [ ] 为项目加入断线重连、异常处理和性能统计

## 三、嵌入式 Linux 系统与驱动（6～8 周）

建议资料顺序：`course_s0` → `course_s3` → `course_s4` → `course_s5`

- [ ] 理解 BootROM → FSBL → U-Boot → Linux 启动流程
- [ ] 创建并配置 PetaLinux 工程
- [ ] 掌握设备树和根文件系统定制
- [ ] 学习字符设备和 Platform 驱动框架
- [ ] 实践 GPIO、UART、I²C、SPI、中断和 DMA
- [ ] 掌握 ioctl、poll 及用户态/内核态数据交互
- [ ] 完成“自定义 Platform/字符设备驱动 + 用户态测试程序”
- [ ] 能解释设备树节点、probe 流程和中断处理

## 四、PS-PL 协同与硬件加速（6～8 周）

建议资料顺序：`course_s1` → `course_s6` → `course_s2`

- [ ] 复习 Verilog 和 Vivado Block Design
- [ ] 掌握 AXI4-Lite、AXI4-Stream、AXI DMA 和 DDR
- [ ] 理解缓存一致性、PS/PL 数据传输和 PL 中断
- [ ] 使用 Vitis HLS 编写简单加速模块
- [ ] 完成图像滤波或矩阵运算加速项目
- [ ] 对比纯 ARM 与 PL 加速的耗时、吞吐量和资源占用

## 五、端侧视觉与 AI（4～6 周）

- [ ] 学习 V4L2、MIPI 摄像头和 GStreamer
- [ ] 使用 OpenCV 完成采集、预处理和显示
- [ ] 部署 ONNX 或 TFLite 模型
- [ ] 理解模型量化、推理延迟和内存占用
- [ ] 了解 Vitis AI/DPU，并确认工具版本与板卡支持情况
- [ ] 完成“摄像头采集 + 目标检测 + HDMI/网络输出”
- [ ] 在 PC/GPU 上训练模型，在开发板上进行推理

## 六、工程化与求职准备（持续进行）

- [ ] 学习 systemd、启动脚本和看门狗
- [ ] 使用 strace、ftrace、perf、gdbserver 调试和分析性能
- [ ] 为项目补充单元测试、自动化构建和版本说明
- [ ] 了解 Yocto，并理解它与 PetaLinux 的关系
- [ ] 准备至少两个可展示项目：Linux 驱动项目、ARM+FPGA+摄像头/AI 项目
- [ ] 为每个项目整理系统框图、代码、启动步骤、性能指标和问题记录
- [ ] 准备 Linux 启动、设备树、中断、DMA、线程同步和缓存一致性面试题

## 阶段完成标准

- [ ] 能独立搭建、编译、启动和调试嵌入式 Linux 系统
- [ ] 能编写简单驱动并完成用户态调用
- [ ] 能分析 ARM 与 FPGA 协同加速的性能瓶颈
- [ ] 能在面试中清楚说明项目设计取舍和故障定位过程
