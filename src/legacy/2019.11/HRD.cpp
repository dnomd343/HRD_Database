#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <fstream>
using namespace std;

ifstream File_Input;
ofstream File_Output;
const unsigned char Up = 1, Down = 2, Left = 3, Right = 4;

struct block_struct {
	unsigned char address;  //0~19
	unsigned char style;  //0:2*2  1:2*1  2:1*2  3:1*1
};
unsigned char table[20];  //0~9:block[?]  0xA:space  0xFF:empty
unsigned char space[2];  //space[0~1]的位置
struct block_struct block[10];  //0:2*2  1~5:1*2,2*1  6~9:1*1

list <unsigned int> int_list;  //空列表 给Source用
list <unsigned int> Hash[0x10000];  //哈希索引表
unsigned int Now_Move;  //目前正在进行计算的布局编号

vector <unsigned int> List;  //所有情况的队列
vector <unsigned int> Layer_Num;  //所在层的编号
vector <list <unsigned int> > Source;  //父布局编号

int group_size;  //整个队列组的大小
int min_steps;  //距离解的最少步骤
int farthest_steps;  //到达最远布局的步数
vector <unsigned int> Solutions;  //所有解
vector <unsigned int> min_Solutions;  //所有最少步解
vector <unsigned int> Solutions_path;  //最少步解法
vector <unsigned int> farthest_cases;  //最远的布局解法

int target_steps;  //到达目标布局的最少步骤
vector <unsigned int> target_path;  //到达目标布局的最少步解法
vector <vector <unsigned int> > target_data;  //target_data[Layer_num][num]
vector <vector <unsigned int> > target_source;  //target_source[next_index]

vector <unsigned int> all_data;  //所有布局的编码
vector <vector <unsigned int> > style_data;  //style_0 ~ style_5下的所有编码
vector <vector <vector <unsigned int> > > group_data;  //group[style][grout_index]下的所有编码

void debug();
void Find_All_Case();
void Data_Output();
unsigned int Change_int (char str[8]);
string Change_str (unsigned int dat);
void Output_Graph (unsigned int Code);
void Analyse_Code (unsigned int Code);
void Add_Case (unsigned int Code);
void Calculate (unsigned int Start_Code);
bool Check (unsigned int Code);
unsigned int Get_Code();
void Find_Next();
bool Check_Empty (unsigned char address,unsigned char dir,unsigned char num);
void Move_Block (unsigned char num,unsigned char dir_1,unsigned char dir_2);
void Fill_Block (unsigned char addr, unsigned char style, unsigned char filler);
vector <unsigned int> Search_Path (unsigned int target_num);
void Analyse_Case (unsigned int Start_Code);
void Analyse_target (unsigned int target);

int main(){
	Analyse_Case(0x4FEA134);
	Analyse_target(0x4F2E9C4);
	return 0;

	cout<<"Welcome to HRD-Calculator by Dnomd343"<<endl;
	cout<<"Please Input the Code:";
}

vector <unsigned int> Search_Path (unsigned int target_num) {  //搜索到达目标布局的一条最短路径 返回vector类
	vector <unsigned int> path;
	int temp = -1;
	path.push_back(target_num);  //路径中加入目标布局
	while (temp != 0) {
		temp = path[path.size() - 1];
		path.push_back(*Source[temp].begin());
	}
	path.pop_back();  //去掉重复的根布局
	temp = path.size() / 2;  //反置整个路径
	for (int i = 0; i < temp; i++) {
		swap(path[i], path[path.size() - i - 1]);
	}
	for (int  i = 0; i < path.size(); i++){  //将序号改成布局的编码
		path[i] = List[path[i]];
	}
	return path;
}

void Analyse_target (unsigned int target) {
	bool get_it = false;
	unsigned int temp, target_num;
	list <unsigned int>::iterator poi;
	target_steps = -1;
	target_path.clear();
	web_target.clear();
	web_target_source.clear();
	for (int i = 0; i < List.size(); i++){  //搜索target的编号到target_num
		if (List[i] == target){
			get_it = true;
			target_num = i;
			target_steps = Layer_Num[i];
			break;
		}
	}
	if (get_it == false) {return;}  //找不到这个target 退出
	target_path = Search_Path(target_num);

}

void Analyse_Case (unsigned int Start_Code) {  //对一个布局进行分析
	unsigned int i, first_solution;
	bool get_it = false;
	if (Check(Start_Code) == false) {return;}
	Calculate(Start_Code);  //通过计算建立队列表
	min_steps = -1;
	farthest_steps = -1;
	group_size=List.size();
	Solutions.clear();
	min_Solutions.clear();
	farthest_cases.clear();
	Solutions_path.clear();

	for (i = 0; i < List.size(); i++) {  //遍历队列中所有元素
		Analyse_Code(List[i]);
		if (block[0].address == 13) {  //若当前布局为有效解
			if (get_it == false){
				min_steps = Layer_Num[i];  //第一个找到的解为最少步解
				first_solution = i;
				get_it = true;
			}
			Solutions.push_back(List[i]);
			if (Layer_Num[i] == min_steps) {
				min_Solutions.push_back(List[i]);
			}
		}
	}

	farthest_steps=Layer_Num[Layer_Num.size()-1];  //计算最远布局的步数
	for (i = Layer_Num.size() - 1; i > 0; i--) {  //找到所有最远的布局
		if(Layer_Num[i] != farthest_steps) {break;}
		farthest_cases.push_back(List[i]);
	}
	if (min_steps != -1) {Solutions_path = Search_Path(first_solution);}
}

void Calculate (unsigned int Start_Code) {  //启动计算引擎
	unsigned int i;
	for (i = 0; i <= 0xFFFF; i++) {Hash[i].clear();}  //初始化
	List.clear();
	Layer_Num.clear();
	Source.clear();

	Hash[(Start_Code>>4) & 0xFFFF].push_back(0);  //加入初始布局
	List.push_back(Start_Code);
	Layer_Num.push_back(0);
	Source.push_back(int_list);
	Now_Move = 0;  //搜索目标指向根节点
	while (Now_Move != List.size()) {  //进行广度优先搜索
		Analyse_Code(List[Now_Move]);  //解析目标布局
		Find_Next();  //根据解析结果搜索所有子布局
		Now_Move++;
	}
}

void Add_Case (unsigned int Code) {  //将计算结果加入队列
    list <unsigned int>::iterator poi;  //定义迭代器
	poi = Hash[(Code>>4) & 0xFFFF].begin();  //设置poi为索引表的起始点
	while (poi != Hash[(Code>>4) & 0xFFFF].end()) {  //遍历索引表
		if (Code == List[*poi]) {  //若发现重复
            if ((Layer_Num[*poi] - Layer_Num[Now_Move]) == 1) {  //若高一层
				Source[*poi].push_back(Now_Move);  //加入父节点列表
			}
            return;  //重复 退出
        }
		++poi;
	}
	Hash[(Code>>4) & 0xFFFF].push_back(List.size());  //将计算结果添加至索引表
	List.push_back(Code);  //将计算结果加入队列
	Layer_Num.push_back(Layer_Num[Now_Move] + 1);  //添加对应的层数
	Source.push_back(int_list);  //初始化其父节点列表
	Source[Source.size()-1].push_back(Now_Move);  //将正在进行搜索的布局添加为父节点
}

void Fill_Block (unsigned char addr, unsigned char style, unsigned char filler) {  //用指定内容填充table中指定的位置
	if (style == 0) {table[addr] = table[addr + 1] = table[addr + 4] = table[addr + 5] = filler;}  //2*2
	else if (style == 1) {table[addr] = table[addr + 1] = filler;}  //2*1
	else if (style == 2) {table[addr] = table[addr + 4] = filler;}  //1*2
	else if (style == 3) {table[addr] = filler;}  //1*1
}

void Move_Block (unsigned char num, unsigned char dir_1, unsigned char dir_2) {  //按要求移动块并将移动后的编码传给Add_Case
	unsigned char i, addr, addr_bak;
	addr = block[num].address;
	addr_bak = addr;

	if (dir_1 == Up) {addr -= 4;}  //第一次移动
	else if (dir_1==Down) {addr += 4;}
	else if (dir_1==Left) {addr--;}
	else if (dir_1==Right) {addr++;}
	if (dir_2 == Up) {addr -= 4;}  //第二次移动
	else if (dir_2 == Down) {addr += 4;}
	else if (dir_2 == Left) {addr--;}
	else if (dir_2 == Right) {addr++;}

	Fill_Block(addr_bak, block[num].style, 0xA);  //修改 table为移动后的状态
	Fill_Block(addr, block[num].style, num);
	block[num].address = addr;

	Add_Case(Get_Code());  //生成编码并赋予Add_Case

	block[num].address = addr_bak;
	Fill_Block(addr, block[num].style, 0xA);  //还原 table原来的状态
	Fill_Block(addr_bak, block[num].style, num);
}

void Find_Next() {  //寻找所有移动的方式并提交给Move_Block
	bool Can_Move[10];
    unsigned char i, addr;
	for (i = 0; i < 10; i++) {Can_Move[i] = false;}
	for (i = 0; i <= 1; i++) {  //寻找位于空格周围的所有块
		addr = space[i];
		if (addr > 3)
			if (table[addr - 4] != 0xA) {Can_Move[table[addr - 4]] = true;}
		if (addr < 16)
			if (table[addr + 4] != 0xA) {Can_Move[table[addr + 4]] = true;}
		if (addr % 4 != 0)
			if (table[addr - 1] != 0xA) {Can_Move[table[addr - 1]] = true;}
		if (addr % 4 != 3) {
			if (table[addr + 1] != 0xA) {Can_Move[table[addr + 1]] = true;}}
	}

	for (i = 0; i <= 9; i++) {
		if (Can_Move[i] == true) {  //若该块可能可以移动
			addr = block[i].address;
			if (block[i].style == 0) {  //2*2
				if ((Check_Empty(addr, Up, 1) == true) &&
					(Check_Empty(addr + 1,Up, 1) == true)) {Move_Block(i, Up, 0);}

				if ((Check_Empty(addr, Down, 2) == true) &&
					(Check_Empty(addr + 1,Down, 2) == true)) {Move_Block(i, Down, 0);}

				if ((Check_Empty(addr, Left, 1) == true) &&
					(Check_Empty(addr + 4, Left, 1) == true)) {Move_Block(i, Left, 0);}

				if ((Check_Empty(addr, Right, 2) == true) &&
					(Check_Empty(addr + 4, Right, 2) == true)) {Move_Block(i, Right, 0);}
			}
			else if (block[i].style == 1) {  //2*1
				if ((Check_Empty(addr, Up, 1) == true) &&
					(Check_Empty(addr + 1, Up, 1) == true)) {Move_Block(i, Up, 0);}

				if ((Check_Empty(addr, Down, 1) == true) &&
					(Check_Empty(addr + 1, Down, 1) == true)) {Move_Block(i, Down, 0);}

				if (Check_Empty(addr, Left, 1) == true) {
					Move_Block(i, Left, 0);
					if (Check_Empty(addr, Left, 2) == true) {Move_Block(i, Left, Left);}
				}

				if (Check_Empty(addr, Right, 2) == true) {
					Move_Block(i, Right, 0);
					if (Check_Empty(addr, Right, 3) == true) {Move_Block(i, Right, Right);}
				}
			}
			else if (block[i].style == 2) {  //1*2
				if (Check_Empty(addr, Up, 1) == true) {
					Move_Block(i, Up, 0);
					if (Check_Empty(addr, Up, 2) == true) {Move_Block(i, Up, Up);}
				}

				if (Check_Empty(addr, Down, 2) == true) {
					Move_Block(i, Down, 0);
					if (Check_Empty(addr, Down, 3) == true) {Move_Block(i, Down, Down);}
				}

				if ((Check_Empty(addr, Left, 1) == true) &&
					(Check_Empty(addr + 4, Left, 1) == true)) {Move_Block(i, Left, 0);}

				if ((Check_Empty(addr, Right, 1) == true) &&
					(Check_Empty(addr + 4, Right, 1) == true)) {Move_Block(i, Right, 0);}
			}
			else if (block[i].style == 3) {  //1*1
				if (Check_Empty(addr, Up, 1) == true) {
					Move_Block(i, Up, 0);
					if (Check_Empty(addr - 4, Up, 1) == true) {Move_Block(i, Up, Up);}
					if (Check_Empty(addr - 4, Left, 1) == true) {Move_Block(i, Up, Left);}
					if (Check_Empty(addr - 4, Right, 1) == true) {Move_Block(i, Up, Right);}
				}

				if (Check_Empty(addr, Down, 1) == true) {
					Move_Block(i, Down, 0);
					if (Check_Empty(addr + 4, Down, 1) == true) {Move_Block(i, Down, Down);}
					if (Check_Empty(addr + 4, Left, 1) == true) {Move_Block(i, Down, Left);}
					if (Check_Empty(addr + 4, Right, 1) == true) {Move_Block(i, Down, Right);}
				}

				if (Check_Empty(addr, Left, 1) == true) {
					Move_Block(i, Left, 0);
					if (Check_Empty(addr - 1, Up, 1) == true) {Move_Block(i, Left, Up);}
					if (Check_Empty(addr - 1, Down, 1) == true) {Move_Block(i, Left, Down);}
					if (Check_Empty(addr - 1, Left, 1) == true) {Move_Block(i, Left, Left);}
				}

				if (Check_Empty(addr, Right, 1) == true) {
					Move_Block(i, Right, 0);
					if (Check_Empty(addr + 1, Up, 1) == true) {Move_Block(i, Right, Up);}
					if (Check_Empty(addr + 1, Down, 1) == true) {Move_Block(i, Right, Down);}
					if (Check_Empty(addr + 1, Right, 1) == true) {Move_Block(i, Right, Right);}
				}
			}
		}
	}
}

bool Check_Empty (unsigned char address, unsigned char dir, unsigned char num) {  //判断指定位置是否为空格 若不是空格或者无效返回false
	unsigned char x, y, addr;
	if (address > 19) {return false;}  //输入位置不存在

	x = address % 4;
	y = (address - x) / 4;
	if (dir == Up) {  //上方
		if (y < num) {return false;}
		addr = address - num * 4;
	}
	if (dir == Down) {  //下方
		if (y + num > 4) {return false;}
		addr = address + num * 4;
	}
	if (dir == Left) {  //左侧
		if (x < num) {return false;}
		addr = address - num;
	}
	if (dir == Right) {  //右侧
		if(x + num > 3){return false;}
		addr = address + num;
	}

	if (table[addr] == 0xA) {
		return true;
	} else {
		return false;
	}
}

unsigned int Get_Code() {  //生成编码
	bool temp[20];
	unsigned int Code = 0;
	unsigned char i, addr, style;

	for (i = 0; i < 20; i++) {temp[i] = false;}  //初始化
	temp[block[0].address] = temp[block[0].address + 1] =
		temp[block[0].address + 4] = temp[block[0].address + 5] = true;

	Code |= block[0].address<<24;  //2*2块的位置
	addr = 0;
	for (i = 1; i <= 11; i++) {
		while(temp[addr] == true){  //找到下一个未填充的空格
			if (addr < 19) {
				addr++;
			} else {
				return 0;
			}
		}
		if (table[addr] == 0xA) {  //空格
			temp[addr] = true;
		} else {
			style = block[table[addr]].style;
			if (style == 1) {  //2*1
				temp[addr] = temp[addr + 1] = true;
				Code |= 1<<(24 - i * 2);
			}
			else if (style == 2) {  //1*2
				temp[addr] = temp[addr + 4] = true;
				Code |= 2<<(24 - i * 2);
			}
			else if (style == 3) {  //1*1
				temp[addr] = true;
				Code |= 3<<(24 - i * 2);
			}
		}
	}
	return Code;
}

void Analyse_Code (unsigned int Code) {  //解译编码到 table[20] block[10] space[2]中
	unsigned char i, addr, style;
	unsigned char num_space = 0, num_type_1 = 0, num_type_2 = 5;
	space[0] = space[1] = 0xFF;  //初始化
	for (i = 0; i < 20; i++) {table[i] = 0xFF;}
	for (i = 0; i <= 9; i++) {
		block[i].address = 0xFF;
		block[i].style = 0xFF;
	}

	block[0].address = 0xF & (Code>>24);  //开始解译
	if (block[0].address > 14) {goto err;}  //2*2块越界 退出
	block[0].style = 0;  //设置2*2块的参数
	Fill_Block(block[0].address, 0, 0x0);
	addr = 0;
	for (i = 0; i < 11; i++) {  //遍历 10个块
		while (table[addr] != 0xFF){  //向下搜索空块
			if (addr < 19) {
				addr++;
			} else {
				break;
			}
		}
		style = 0x3 & (Code>>(22 - i * 2));  //0:space  1:2*1  2:1*2  3:1*1
		if (style == 0) {  //space
			table[addr] = 0xA;
			space[num_space] = addr;
			if (num_space == 0) {num_space++;}
		}
		if (style == 1) {  //2*1
			if (num_type_1 < 5) {num_type_1++;}
			if (addr > 18) {goto err;}  //2*1块越界
			block[num_type_1].style = 1;
			block[num_type_1].address = addr;
			table[addr] = table[addr + 1] = num_type_1;
		}
		if (style == 2) {  //1*2
			if (num_type_1 < 5) {num_type_1++;}
			if (addr > 15) {goto err;}  //1*2块越界
			block[num_type_1].style = 2;
			block[num_type_1].address = addr;
			table[addr] = table[addr + 4] = num_type_1;
		}
		if (style == 3) {  //1*1
			if (num_type_2 < 9) {num_type_2++;}
			block[num_type_2].style = 3;
			block[num_type_2].address = addr;
			table[addr] = num_type_2;
		}
	}
	err:;
}

bool Check (unsigned int Code) {  //检查编码是否合法 正确返回true 错误返回false
	bool temp[20];
	unsigned char addr, i;
	Analyse_Code(Code);

	for (i = 0; i < 20; i++){temp[i] = false;}  //初始化
	for (i = 0; i < 20; i++) {  //检查table内容是否合法
		if (table[i] > 10) {return false;}
	}

	if (block[0].style != 0) {return false;}  //检查2*2块
	for (i = 1; i <= 5; i++) {  //检查2*1与1*2块
		if ((block[i].style != 1) && (block[i].style != 2)) {return false;}
	}
	for (i = 6; i <= 9; i++) {  //检查1*1块
		if (block[i].style != 3) {return false;}
	}
	for (i = 0; i <= 1; i++) { //检查空格
		if (space[i] > 19) {
			return false;
		} else {
			temp[space[i]] = true;
		}
	}

	addr = block[0].address;  //检查2*2块
	if ((addr > 14) || (addr%4 == 3)) {return false;}
	if ((temp[addr] == true) || (temp[addr + 1] == true) ||
		(temp[addr + 4] == true) || (temp[addr + 5] == true)) {return false;}
	temp[addr] = temp[addr + 1] = temp[addr + 4] = temp[addr + 5] = true;
	for (i = 1; i <= 5; i++) {  //检查2*1与1*2块
		addr = block[i].address;
		if (block[i].style == 1) {
			if ((addr > 18) || (addr % 4 == 3)) {return false;}
			if ((temp[addr] == true) || (temp[addr + 1] == true)) {return false;}
			temp[addr] = temp[addr + 1] = true;
		}
		if (block[i].style == 2) {
			if (addr > 15) {return false;}
			if ((temp[addr] == true) || (temp[addr + 4] == true)) {return false;}
			temp[addr] = temp[addr + 4] = true;
		}
	}
	for (i = 6; i <= 9; i++) {  //检查1*1块
		addr = block[i].address;
		if (addr > 19) {return false;}
		if (temp[addr] == true) {return false;}
		temp[addr] = true;
	}
	return true;
}

void Output_Graph (unsigned int Code) {  //命令行下图形化输出布局图
	const unsigned char Square_Width = 4;
	const string Square_str = "[]";  //or "u2588"
	unsigned int i, k, m;
	unsigned int start_x, start_y, end_x, end_y;
	bool Graph[(Square_Width + 1) * 4 + 1][(Square_Width + 1) * 5 + 1];  //点阵方式储存输出图像
	if (Check(Code) == false) {
		cout<<"The Code is Wrong!"<<endl;
		return;
	}

	Analyse_Code(Code);  //先解析编码
	for (k = 0; k <= (Square_Width + 1) * 5; k++) {  //初始化
		for (i = 0; i <= (Square_Width + 1) * 4; i++) {
			Graph[i][k] = false;
		}
	}

	for (m = 0; m <= 9; m++) {  //遍历九个块
		start_x = (block[m].address % 4) * (Square_Width + 1) + 1;  //计算该块的左上角坐标
		start_y = int(block[m].address / 4) * (Square_Width + 1) + 1;
		if ((block[m].style == 0) || (block[m].style == 1)) {  //计算该块的右下角横坐标
			end_x = start_x + Square_Width * 2;  //2*2 or 2*1
		} else {
			end_x = start_x + Square_Width - 1;  //1*2 or 1*1
		}
		if ((block[m].style == 0) || (block[m].style == 2)) {  //计算该块的右下角纵坐标
			end_y = start_y + Square_Width * 2;  //2*2 or 1*2
		} else {
			end_y = start_y + Square_Width - 1;  //2*1 or 1*1
		}
		for (i = start_x; i <= end_x; i++) {  //填充该块覆盖的区域
			for(k = start_y; k <= end_y; k++) {
				Graph[i][k] = true;
			}
		}
	}

	for (i = 1; i <= (Square_Width + 1) * 4 + 3; i++) {cout<<Square_str;}  //显示上边框
	cout<<endl<<Square_str;
	for (k = 0; k <= (Square_Width + 1) * 5; k++){  //显示图像内容
		for (i = 0; i <= (Square_Width + 1) * 4; i++) {
			if (Graph[i][k] == false){
				cout<<"  ";
			} else {
				cout<<Square_str;
			}
		}
		cout<<Square_str<<endl<<Square_str;  //显示左右边框
	}
	for (i = 1; i <= (Square_Width + 1) * 4 + 2; i++) {cout<<Square_str;}  //显示下边框
	cout<<endl;
}

string Change_str (unsigned int dat) {  //将编码数据转化为字符
	unsigned char i, bit;
	string str = "";
	for (i = 0; i < 7; i++) {
		bit = 0xF & dat>>(6 - i)*4;  //分离单个十六进制位
		if ((bit >= 0) && (bit <= 9)) {str += bit + 48;}  //0~9
		if ((bit >= 0xA) && (bit <= 0xF)) {str += bit + 55;}  //A~F
	}
	return str;
}

unsigned int Change_int (char str[8]) {  //将编码字符转化为int
	unsigned int i, dat = 0;
	for (i = 0; i < 7; i++) {
		if ((str[i] >= 48) && (str[i] <= 57)) {dat = dat | (str[i] - 48)<<(24 - i * 4);}  //0~9
		if ((str[i] >= 65) && (str[i] <= 70)) {dat = dat | (str[i] - 55)<<(24 - i * 4);}  //A~F
		if ((str[i] >= 97) && (str[i] <= 102)) {dat = dat | (str[i] - 87)<<(24 - i * 4);}  //a~f
	}
	return dat;
}

void debug() {  //输出当前 table[20] block[10] space[2]的状态
	unsigned int i;
	for (i = 0; i < 20; i++) {
		if (table[i] != 0xA) {
			cout<<int(table[i])<<" ";
		} else {
			cout<<"A ";
		}
		if (i == 3) {cout<<"        00 01 02 03"<<endl;}
		if (i == 7) {cout<<"        04 05 06 07"<<endl;}
		if (i == 11) {cout<<"        08 09 10 11"<<endl;}
		if (i == 15) {cout<<"        12 13 14 15"<<endl;}
		if (i == 19) {cout<<"        16 17 18 19"<<endl;}
	}
	cout<<endl<<"space[0] -> address="<<int(space[0])<<endl;
	cout<<"space[1] -> address="<<int(space[1])<<endl<<endl;
	for (i = 0; i <= 9; i++) {
		cout<<"block["<<i<<"] -> address="<<int(block[i].address)<<" style="<<int(block[i].style)<<endl;
	}
}

void Find_All_Case(){  //Right_File's MD5: 4A0179C6AEF266699A73E41ECB4D87C1
	File_Output.open("All_Case.txt");
	unsigned int i;
	for(i=0;i<0xFFFFFFF;i=i+4){
		if(Check(i)==true){File_Output<<Change_str(i)<<endl;}
	}
	File_Output.close();
}
void Data_Output(){
    unsigned int i;
    list<unsigned int>::iterator poi;
    File_Output.open("Data_Output.txt");
    for(i=0;i<List.size();i++){
        File_Output<<i<<" -> "<<Change_str(List[i])<<" ("<<Layer_Num[i]<<")";
        poi=Source[i].begin();
        while(poi!=Source[i].end()){File_Output<<"  ["<<*poi<<"]";++poi;}
        File_Output<<endl;
    }
    File_Output.close();
}
