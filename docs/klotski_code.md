## 编码

合法华容道均有编码，长度9位，每一位是单个16进制数（0\~9与A\~F）；同一布局只能有唯一编码，同一编码亦对应唯一布局，即编码与布局一一对应；

**位置编号**

![address](./docs/images/address.png)

2 x 2棋子的左上角在棋盘中的位置编号有12种情况，对应编码分别为：0、1、2、4、5、6、8、9、A(10)、C(12)、D(13)、E(14)，将其置于编码第一位；剩余8位十六进制位储存其他棋子信息。

其余棋子（空格此时暂时视为棋子）按从左到右，从上到下的顺序排列（取左上角排序）

它们对应的代号（二进制）如下：

| 棋子类型 | 代号 |
| :-: | :-: |
| 空格 | 00 |
| 2 x 1 | 01 |
| 1 x 2 | 10 |
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

## 编码举例

**例1：**

![eg-1A9BF0C00](./docs/images/code_eg_1.png)

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

![eg-4FEA13400](./docs/images/code_eg_2.png)

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

![eg-5DC02F800](./docs/images/code_eg_3.png)

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