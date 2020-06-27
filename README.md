## 库的调用

### HRD_cal

包含头文件"HRD_cal.h"，类名为HRD_cal；

```c++
#include <iostream>
#include <vector>
#include "HRD_cal.h"
using namespace std;

int main() {
    cout << "Klotski fast calculator by Dnomd343" << endl;
    vector <unsigned long long> dat;
    HRD_cal demo;
    
    // 将编码转为文本(参数为unsigned long long 返回string类)
    cout << "4FEA13400 is " << demo.Change_str(0x4FEA13400) << endl;
    cout << "----------------" << endl;

    // 将文本转为编码(参数为*char 返回unsigned long long)
    cout << "1a9bf0c00 is " << hex << demo.Change_int("1A9BF0C00") << dec << endl;
    cout << "----------------" << endl;

    // 计算最少步数(参数为unsigned long long 返回vector类)
    dat = demo.Calculate(0x1A9BF0c00);
    cout << demo.Change_str(0x1A9BF0C00) << "'s solution";
    cout << " need at least " << dat.size() - 1 << " steps" << endl;
    for (unsigned int i = 0; i < dat.size(); i++) {
        cout << i << ": " << demo.Change_str(dat[i]) << endl;
    }
    cout << "----------------" << endl;

    // 计算到某一布局的最短路径(参数均为unsigned long long 分别为起始编码和目标编码 返回vector类)
    dat = demo.Calculate(0x1A9BF0C00, 0x1ABE70C00);
    cout << demo.Change_str(0x1A9BF0C00) << " to " << demo.Change_str(0x1ABE70C00);
    cout << " need at least " << dat.size() - 1 << " steps" << endl;
    for (unsigned int i = 0; i < dat.size(); i++) {
        cout << i << ": " << demo.Change_str(dat[i]) << endl;
    }
    cout << "----------------" << endl;

    // 计算某一布局衍生出的所有布局
    dat = demo.Calculate_All(0x1A9BF0C00);
    cout << demo.Change_str(0x1A9BF0C00) << " can derive " << dat.size() << " cases" << endl;
    cout << "----------------" << endl;

    return 0;
}
```



## 基本定义

### 华容道布局

+ 棋盘大小为4 x 5

+ 棋子为2 x 2，2 x 1（1 x 2），1 x 1三种

+ 棋子间不能重叠，且至少存在两个空格

+ 有且仅有一个2 x 2块，其他类型不限定

  *（一个合法的华容道布局必须满足以上四点）*

  合法华容道布局共有29334498种

### 合法布局举例：

<img src="/images/legal_1+2.png" alt="legal_1+2" style="zoom: 40%;" />

<img src="/images/legal_3+4.png" alt="legal_3+4" style="zoom: 40%;" />


### 非法布局举例：

<img src="/images/illegal_1+2.png" alt="illegal_1+2" style="zoom: 40%;" />

<img src="/images/illegal_3+4.png" alt="illegal_3+4" style="zoom: 40%;" />

**错误原因：**

+ 缺少2 x 2块

+ 仅有一个空格

+ 存在两个2 x 2块

+ 存在3 x 1块




### 布局间的关系

+ 移动原则：棋子只能平行移动，不能进行旋转；

+ **一步**：某一棋子做任意步移动后的结果；

+ **子布局**：某一布局通过一步移动可以得到的布局称为子布局；

+ 性质：布局A是布局B的子布局，同时必有布局B是布局A的子布局；

+ **相邻布局**：两布局互为对方子布局时，两者为相邻布局；


### 步的举例
<img src="/images/step_exp_1.png" alt="step_exp_1" style="zoom: 40%;" />

<img src="/images/step_exp_2.png" alt="step_exp_2" style="zoom:40%;" />

<img src="/images/step_exp_3.png" alt="step_exp_3" style="zoom:40%;" />


### 标准情况

标准布局：存在5个2 x 1（或1 x 2），4个1 x 1棋子的合法华容道布局（363480种）

非标准布局：除标准布局外的全部合法华容道布局（28971018种）



### 编码

合法华容道均有编码，长度9位，每一位是单个16进制数（0\~9与A\~F）；同一布局只能有唯一编码，同一编码亦对应唯一布局，即编码与布局一一对应；

**位置编号**

<img src="/images/address.png" alt="address" style="zoom: 40%;" />

2 x 2棋子的左上角在棋盘中的位置编号有12种情况，对应编码分别为：0、1、2、4、5、6、8、9、A(10)、C(12)、D(13)、E(14)，将其置于编码第一位；剩余8位十六进制位储存其他棋子信息。

其余棋子（空格此时暂时视为棋子）按从左到右，从上到下的顺序排列（取左上角排序）

它们对应的代号（二进制）如下：

| 棋子类型 | 代号 |
| :-: | :-: |
| 空格 | 00 |
| 1 x 2 | 01 |
| 2 x 1 | 10 |
| 1 x 1 | 11 |

十六进制可按位转为二进制，对应关系如下：

| 十六进制 | 二进制 | 十进制 |
| :-: | :-: | :-: |
| 0 | 0000 | 0 |
| 1 | 0001 | 1 |
| 2 | 0010 | 2 |
| 3 | 0011 | 3 |
| 4 | 0100 | 4 |
| 5 | 0101 | 5 |
| 6 | 0110 | 6 |
| 7 | 0111 | 7 |
| 8 | 1000 | 8 |
| 9 | 1001 | 9 |
| A | 1010 | 10 |
| B | 1011 | 11 |
| C | 1100 | 12 |
| D | 1101 | 13 |
| E | 1110 | 14 |
| F | 1111 | 15 |

8个十六进制位相当于32个二进制位，由于每个棋子占用2个二进制位，因此最多储存16个棋子信息；将其依次填入，若有空余则补0填；按此操作即可将布局转化为编码，规定编码最后的0可以省略。

**编码举例**


**例1：**

<img src="/images/code_exp_1.png" alt="exp-1A9BF0C00" style="zoom: 60%;" />

<table style="text-align: center">
	<tr>
		<td>2 x 2</td>
		<td>1</td>
		<td>2</td>
		<td>3</td>
		<td>4</td>
		<td>5</td>
		<td>6</td>
		<td>7</td>
		<td>8</td>
		<td>9</td>
		<td>10</td>
		<td>11</td>
		<td colspan="5">补0</td>
	</tr>
	<tr>
		<td>0001</td>
		<td>10</td>
		<td>10</td>
		<td>10</td>
		<td>01</td>
		<td>10</td>
		<td>11</td>
		<td>11</td>
		<td>11</td>
		<td>00</td>
		<td>00</td>
		<td>11</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
	</tr>
	<tr>
		<td><b>1</b></td>
		<td colspan="2"><b>A</b></td>
		<td colspan="2"><b>9</b></td>
		<td colspan="2"><b>B</b></td>
		<td colspan="2"><b>F</b></td>
		<td colspan="2"><b>0</b></td>
		<td colspan="2"><b>C</b></td>
		<td colspan="2"><b>0</b></td>
		<td colspan="2"><b>0</b></td>
	</tr>
</table>
因此，布局编码为<b>1A9BF0C00</b>，可简写为<b>1A9BF0C</b>


**例2：**

<img src="/images/code_exp_2.png" alt="exp-4FEA13400" style="zoom: 60%;" />

<table style="text-align: center">
	<tr>
		<td>2 x 2</td>
		<td>1</td>
		<td>2</td>
		<td>3</td>
		<td>4</td>
		<td>5</td>
		<td>6</td>
		<td>7</td>
		<td>8</td>
		<td>9</td>
		<td>10</td>
		<td>11</td>
		<td colspan="5">补0</td>
	</tr>
	<tr>
		<td>0100</td>
		<td>11</td>
		<td>11</td>
		<td>11</td>
		<td>10</td>
		<td>10</td>
		<td>10</td>
		<td>00</td>
		<td>01</td>
		<td>00</td>
		<td>11</td>
		<td>01</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
	</tr>
	<tr>
		<td><b>4</b></td>
		<td colspan="2"><b>F</b></td>
		<td colspan="2"><b>E</b></td>
		<td colspan="2"><b>A</b></td>
		<td colspan="2"><b>1</b></td>
		<td colspan="2"><b>3</b></td>
		<td colspan="2"><b>4</b></td>
		<td colspan="2"><b>0</b></td>
		<td colspan="2"><b>0</b></td>
	</tr>
</table>
因此，布局编码为<b>4FEA13400</b>，可简写为<b>4FEA134</b>

**例3：**

<img src="/images/code_exp_3.png" alt="exp-5DC02F800" style="zoom: 60%;" />

<table style="text-align: center">
	<tr>
		<td>2 x 2</td>
		<td>1</td>
		<td>2</td>
		<td>3</td>
		<td>4</td>
		<td>5</td>
		<td>6</td>
		<td>7</td>
		<td>8</td>
		<td>9</td>
		<td>10</td>
		<td>11</td>
		<td>12</td>
		<td>13</td>
		<td colspan="3">补0</td>
	</tr>
	<tr>
		<td>0101</td>
		<td>11</td>
		<td>01</td>
		<td>11</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
		<td>10</td>
		<td>11</td>
		<td>11</td>
		<td>10</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
		<td>00</td>
	</tr>
	<tr>
		<td><b>5</b></td>
		<td colspan="2"><b>D</b></td>
		<td colspan="2"><b>C</b></td>
		<td colspan="2"><b>0</b></td>
		<td colspan="2"><b>2</b></td>
		<td colspan="2"><b>F</b></td>
		<td colspan="2"><b>8</b></td>
		<td colspan="2"><b>0</b></td>
		<td colspan="2"><b>0</b></td>
	</tr>
</table>
因此，布局编码为<b>5DC02F800</b>，可简写为<b>5DC02F8</b>



## 分类

由于共有29334498种布局，将它们从小到大排列，进而得到唯一的id(0 ~ 29334497)；



### 摆列方式分类

将2 x 1（1 x 2）的数量称为jiang_num，1 x 1的数量称为bing_num；

据此可分为64种情况，统计如下：

| jiang_num | bing_num | 数量 |
| :-: | :-: | :-: |
| 0 |  0 |      12 |
| 0 |  1 |     192 |
| 0 |  2 |    1440 |
| 0 |  3 |    6720 |
| 0 |  4 |   21840 |
| 0 |  5 |   52416 |
| 0 |  6 |   96096 |
| 0 |  7 |  137280 |
| 0 |  8 |  154440 |
| 0 |  9 |  137280 |
| 0 | 10 |   96096 |
| 0 | 11 |   52416 |
| 0 | 12 |   21840 |
| 0 | 13 |    6720 |
| 0 | 14 |    1440 |
| 1 |  0 |     256 |
| 1 |  1 |    3584 |
| 1 |  2 |   23296 |
| 1 |  3 |   93184 |
| 1 |  4 |  256256 |
| 1 |  5 |  512512 |
| 1 |  6 |  768768 |
| 1 |  7 |  878592 |
| 1 |  8 |  768768 |
| 1 |  9 |  512512 |
| 1 | 10 |  256256 |
| 1 | 11 |   93184 |
| 1 | 12 |   23296 |
| 2 |  0 |    2138 |
| 2 |  1 |   25656 |
| 2 |  2 |  141108 |
| 2 |  3 |  470360 |
| 2 |  4 | 1058310 |
| 2 |  5 | 1693296 |
| 2 |  6 | 1975512 |
| 2 |  7 | 1693296 |
| 2 |  8 | 1058310 |
| 2 |  9 |  470360 |
| 2 | 10 |  141108 |
| 3 |  0 |    8974 |
| 3 |  1 |   89740 |
| 3 |  2 |  403830 |
| 3 |  3 | 1076880 |
| 3 |  4 | 1884540 |
| 3 |  5 | 2261448 |
| 3 |  6 | 1884540 |
| 3 |  7 | 1076880 |
| 3 |  8 |  403830 |
| 4 |  0 |   20224 |
| 4 |  1 |  161792 |
| 4 |  2 |  566272 |
| 4 |  3 | 1132544 |
| 4 |  4 | 1415680 |
| 4 |  5 | 1132544 |
| 4 |  6 |  566272 |
| 5 |  0 |   24232 |
| 5 |  1 |  145392 |
| 5 |  2 |  363480 |
| 5 |  3 |  484640 |
| 5 |  4 |  363480 |
| 6 |  0 |   14330 |
| 6 |  1 |   57320 |
| 6 |  2 |   85980 |
| 7 |  0 |    3508 |

进而，将2 x 1的数量称为style_num，于是有style_num恒小于或等于jiang_num；

此时可分出203种情况（注意不存在 *jiang_num-bing_num-style_num = 7-0-0* 的情况）：

[203种分类的元素数量](./style_count.md)



### 群

定义：群是有限个不同布局的集合，该集合中全部布局都可以由其中任一布局经过有限次移动得到；

性质：任意一个布局无论如何移动，其结果仍在该群内；

构造：群是封闭的，群中所有元素无序且互异，同时构成一个关系网；

最简性：只需群中任意一个布局，即可复原出群中的所有元素；



### 按群继续分类

对于一个特定的 *jiang_num-bing_num-style_num* 分组，可拆分出n个群；将分出的群按元素数量从大到小排列，若存在元素数量相同的群，则取其中的最小元素排序；对这些群进行编号得0 ~ (n - 1) 共n个群，编号记为group_num；

因而对于某一群，存在一个唯一编号  *jiang_num-bing_num-style_num-group_num* ；由于群中的元素个数是确定的，将其中的元素按编码从小到大排列，其中的元素可得唯一编号group_index；

所以，对于任意布局，可得唯一编号 *jiang_num-bing_num-style_num-group_num-group_index*；



## 层级关系

**最少步数**：当布局A和布局B处于同一个群时，从布局A移动到B所需最少的步数，该数值存在且是确定的；

性质：布局A到B的最少步数与布局B到A的最少步数必定相同；

**最短路径**：从布局A到布局B，所有满足最少步数的路径（最少路径可能不止一条）；

**最远步数**：布局A到它所在群中任一布局均存在一最少步数，其中最大的最少步数称为最远步数；

**最远布局**：布局A到布局B的最少步数为最远步数时，称布局B为布局A的最远布局（一般情况下不止一个）；

（标准华容道中不存在最远布局退化的情况，即最远步数至少为1）

**最少步解**：布局A所在群中存在一布局S，满足2 x 2方块在棋盘的最下方中间（即2 x 2块所在位置编号为13，亦或编码以D开头），记A到S的最少步数为n，此时不存在其他任何满足以上条件的S‘使得最少步数小于n，则称S为布局A的最少步解，n为最少步解的步数（亦简称为最少步）；

**解**：布局A所在群中存在一布局S满足2 x 2方块在棋盘的最下方中间，且有A到S的任意最短路径中均不存在符合2 x 2方块在棋盘的最下方中间的布局，此时称S为布局A的解；

性质：布局的解可能是空集；最少步解属于解的子集（特定条件下两者可以相同）；



**层模型**

表述：存在一起始布局A，从它开始衍生出有限个布局，每个布局抽象为一个节点，同时称A为根节点；由于任一由A衍生出布局到根节点存在一个确定的最少步数，将其称之为到根节点的距离；将所有距离相同的布局称为一个层，层内元素无序且互异，根节点所在层称为第0层，之后依次排列可得有限个层；

记排列的最后一层为第n层，则共有n + 1个层（包括第0层）；

性质1：所有层中的元素集合即为根节点所在的群；

性质2：某一节点所在层数为其到根节点的最少步数；

性质3：若最后一层为第n层，则最远步数为n，且第n层的所有元素均为最远布局；





