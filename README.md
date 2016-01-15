## ScanZBufferAlgorithm

### 项目说明

计算机图形学的第一个大作业，自己动手实现扫面线Zbuffer算法

### 开发环境

VS2013 MFC

外部库: OpenMesh - 用来读取和管理OBJ模型结构

OBJ_Models文件夹为测试用的OBJ模型文件

### 关于扫描线ZBuffer算法实现的说明：

1. 基于MFC实现界面，可以通过选项命令读取模型文件

2. Utility.h 定义了工程的全局头文件

3. ScanLineZBufferView 实现视图界面命令，绘制模型在OnDraw中调用，实现模型拖动旋转

4. ScanLineZBufferDoc实现模型文件的打开

5. ScanLineZBufferCore中定义ZBuffer的数据结构和核心算法。

6. 模型的读取和遍历使用OpenMesh来实现

7. 左下角StatusBar，显示模型的顶点和面的数量信息

8. 绘制时候采用双缓冲区交替绘制，对于模型点数不是特别的多的模型，在Release编译下，运行流畅。

9. 扫描线ZBuffer的数据结构与PPT中不太一样，主要参考了网上的一种写法，把活化边表拆分开来，活化边表只存储一半的边，并且对应的活化边表是记录在相对应的活化多边形下。这样通过当前活化边表对于x的方向排序，每两个活化边表中边就形成一个边对。这样实现代码简洁。