## 华容道数据库计划

### 简述

这是关于华容道性质的研究项目，目标是找到所有华容道布局并分析它们之间的关联，同时取得各方面的参数，并将收集到的资料建成数据库加以整理。

经典华容道游戏很难在数学上建模，也无法在数学证明上得到其具体性质；研究它的统计规律只能使用计算机进行穷举，遍历所有可能的布局，分别算出它们的各种数据，最后再进行整合统计。

因此，这个项目将通过穷举的方式来得到全部布局的详细信息并录入数据库进行分类整理，再而进行统计分析。


### 找到所有可能的情况

在搜索全部可能布局的问题上，纯粹的数学计算在这里毫无用武之地：不同的棋子有不同的排列方式，根本无法进行可行的排列组合计算；因此，搜索过程也只能暴力穷举，将棋子的全排列逐个验证，剔除无效的排列后将得到全部合法的布局。

为了完善地进行统计，我先定义了华容道的布局规则，移动规则及各种延伸概念，给出了一套完整的[严格定义](./docs/klotski.md)，此后的一切计算和统计将会在这个框架下进行。

经过统计，符合定义的华容道布局共有29334498种，这个数量级确实超出了个人的直观感受，但多次验证都证明这个数据是正确的；另一方面，千万级别的数据量也会对计算过程造成不小的挑战，甚至连数据库的大小都可能超出预期。


### 具体要统计什么数据

此前，大部分华容道的编程都是为了得到最少步解法，然而仅这一个数据对研究远远不够；在经过整理预判后，选定了以下几个数据作为主要统计目标：

+ 解的最少步数

+ 最少步解的个数

+ 全部最少步解

+ 所有解的个数

+ 最远步数

+ 最远布局的个数

+ 全部最远布局

+ 衍生布局的个数

另一方面就是布局之间的关联，在[严格定义](./docs/klotski.md)中给出了`群`的概念，它是一个封闭的集合，内部任何布局无论如何移动，其结果仍然会落在集合中，也即全部情况组合成了多个孤立的群。

综上，要统计的数据主要是：

1. 全部群的具体信息；

2. 全部布局各自的数据；


### 计算时间问题

在华容道算法上，鉴于其数学复杂性，我们可选的也只能是无方向性的暴力搜索；绝大多数相关的程序也都使用了BFS方式进行搜索，通过哈希索引或者树结构优化计算速度，在计算过程中，单纯求解一般会经过约1000 \~ 100000个中间布局，得到结果后进行回溯；整个计算流程耗费的时间不等，对于常见的横刀立马布局，网络上流传的算法计算时间普遍在50ms \~ 800ms，个别优化较好的能下探到20ms附近；但即使做到这个速度，在千万级数据量面前都是相形见绌，计算时间和费用都是问题。

以上的讨论只是针对计算最少步数的，而现在要统计的还有其他数据，特别是最远布局，得到这个数据的计算量一般是单纯求解的数倍，这些都会严重拖慢计算进度。

为了在个人可行的范围内得到统计结果，只能尽可能地提升效率；目前这套计算引擎使用c++编写，尽可能地在底层提高计算效率，而最重要的提升还是在算法上。

考虑到现在要做的是计算全部情况，而不是计算单个布局，可以通过转变计算方式来加快速度；这个方法的提升在于借助了群的性质，每次只针对一个群做运算：在华容道求解的算法中，耗时最大的就是寻找下一步和剪枝，而如果计算局限在一个群中，我们可以通过构建它们之间的关系网（即定义中的网模型），通过指针免除掉寻找下一步的问题，进而将找到的节点内存地址加入线性表中并进行标记，省去了剪枝的过程，通过这样子的计算速率可以大大提高。


### 编码

为了方便研究，每个华容道布局都有一个唯一的编码，它是一个九位长度的十六进制数，且编码和布局之间是一一对应的关系。

在设计上，编码亦可通过口算直接摆出布局样式，也可反过来根据布局算出编码，整个过程可以不需要借助计算机计算，稍微理解进制的都可以轻松做到。

考虑到大多数情况下，编码最后都会补0到九位，因此规定了编码后方的0可以省略；如横刀立马的编码是1A9BF0C00，可简记为1A9BF9C。

详细内容会在[编码规则](./docs/klotski_code.md)部分中讨论。


### 编码可视化

对大多数人来说，纯粹的编码并不直观，而另一方面又需要编码的简洁性来辅助研究；因此我编写了一个编码可视化的小工具，可以根据布局计算编码或根据编码来显示布局样式。

![visual_tool](./docs/images/visual_tool.png)

工具介绍

+ 通过鼠标拖拽画出棋子

+ 绘制时违反规则会出现提示

+ 通过单击删除棋子

+ 下方文本框输入编码自动生成布局

+ 编码输入错误会标红警告

+ 双击文本框可清空内容

+ 清除按钮可清空上方内容

+ 完成按钮可补0生成九位编码

下载

[Github仓库](./res/HRD_Virual.exe)

[备用链接](https://hrd.dnomd343.top/download/HRD_Virual.exe)


### 计算引擎

计算引擎使用c++编写，负责全部计算工作，同时，它也可以用于最短路径/最远布局/解的个数等其他方面的运算。

源码编译后是命令行程序，需输入指定命令进行运算工作，具体[使用方法](./docs/usage.md)将在这篇文档中阐述。

引擎封装了多个类实现不同功能，如果你有其他需求可以直接调用它的封装函数实现，这是它库函数的[使用演示](./docs/demo.md)。


### 如何运行这个项目
...


### 数据库使用
...


### 关于开源

本项目所有源码以[MIT协议](./LICENSE)开源，你可以不受任何限制地使用，但保留作者版权；


*by Dnomd343*

*2020.7.7*