## 基本定义

### 华容道布局

+ 棋盘大小为4 x 5

+ 棋子为2 x 2，2 x 1（1 x 2），1 x 1三种

+ 棋子间不能重叠，且至少存在两个空格

+ 有且仅有一个2 x 2块，其他类型不限定

  *（一个合法的华容道布局必须满足以上四点）*

  合法华容道布局共有29334498种


### 合法布局举例：

<img src="/images/legal_1.png" alt="legal_1" style="zoom: 40%;" /><img src="/images/legal_2.png" alt="legal_2" style="zoom:40%;" /><img src="/images/legal_3.png" alt="legal_3" style="zoom:40%;" /><img src="/images/legal_4.png" alt="legal_4" style="zoom:40%;" />


### 非法布局举例：

<img src="/images/illegal_1.png" alt="illegal_1" style="zoom: 40%;" /><img src="/images/illegal_2.png" alt="illegal_2" style="zoom:40%;" /><img src="/images/illegal_3.png" alt="illegal_3" style="zoom:40%;" /><img src="/images/illegal_4.png" alt="illegal_4" style="zoom:40%;" />


### 布局间的关系

+ 移动原则：棋子只能平行移动，不能进行旋转；

+ **一步**：某一棋子做任意步移动后的结果；

+ **子布局**：某一布局通过一步移动可以得到的布局称为子布局；

​			（相对的，布局A是布局B的子布局，同时必有布局B是布局A的子布局）

+ **相邻布局**：两布局互为对方子布局时，两者为相邻布局；

​			（充要条件：布局与其子布局间的关系）


### 步的举例
<img src="/images/step_exp_1.png" alt="step_exp_1" style="zoom: 40%;" />

<img src="/images/step_exp_2.png" alt="step_exp_2" style="zoom:40%;" />

<img src="/images/step_exp_3.png" alt="step_exp_3" style="zoom:40%;" />


### 标准情况

标准布局：存在5个2 x 1（或1 x 2），4个1 x 1棋子的合法华容道布局（363480种）

非标准布局：除标准布局外的全部合法华容道布局（28971018种）



### 编码

合法华容道均有编码，长度9位，每一位是单个16进制数（0~9与A~F）；同一布局只能有唯一编码，同一编码亦对应唯一布局，即编码与布局一一对应；

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
因此，布局编码为**1A9BF0C00**，可简写为**1A9BF0C**

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
因此，布局编码为**4FEA13400**，可简写为**4FEA134**

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
因此，布局编码为**5DC02F800**，可简写为**5DC02F8**