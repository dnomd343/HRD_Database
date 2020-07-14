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
vector <unsigned int> int_vector;  //空vector 给Layer用
list <unsigned int> Hash[0x10000];  //哈希索引表
unsigned int Now_Move;  //目前正在进行计算的布局编号
vector <unsigned int> List;  //所有情况的队列
vector <unsigned int> Layer_Num;  //所在层的编号
vector <unsigned int> Layer_Index;  //层中所属的编号
vector <list <unsigned int> > Source;  //父布局编号
vector <vector <unsigned int> > Layer;  //分层
vector <vector <vector <unsigned int> > > Layer_Next;  //分层链接
vector <bool> bool_vector;  //空vector 给solution_flag用
vector <vector <bool> > solution_flag;  //求不同步数解的标识

//布局的基本参数
int group_size;  //整个队列组的大小
int min_solution_step;  //最少的步数
int solution_num;  //解的个数
int min_solution_num;  //最少步解的个数
vector <unsigned int> solution_case;  //所有解
vector <unsigned int> solution_step;  //所有解对应的步数
vector <unsigned int> min_solution_case;  //所有最少步解
int farthest_step;  //最远布局的步数
int farthest_num;  //最远布局的个数
vector <unsigned int> farthest_case;  //所有最远的布局

void debug();
void Output_Graph(unsigned int Code);
string Change_str(unsigned int dat);
unsigned int Change_int(char str[8]);
bool Check(unsigned int Code);
void Analyse_Code(unsigned int Code);
unsigned int Get_Code();
bool Check_Empty(unsigned char address,unsigned char dir,unsigned char num);
void Fill_Block(unsigned char addr, unsigned char style, unsigned char filler);
void Move_Block(unsigned char num,unsigned char dir_1,unsigned char dir_2);
void Find_Next();
void Add_Case(unsigned int Code);
void Calculate(unsigned int Start_Code);
void Split_Layer();
vector <unsigned int> Sort(vector <unsigned int> dat);
void Analyse_Case(unsigned int Start_Code);
void Output_Detail(unsigned int Code);
vector <unsigned int> Split_Group(vector <unsigned int> dat);

int main() {
	unsigned int i, j, k, index;
	struct Case_struct {
		unsigned int id;
		char Short_Code[5];
		unsigned int Code;
		unsigned int Style;
		unsigned int Group;
		int group_size;
		int min_solution_step;
		int solution_num;
		int min_solution_num;
		vector <unsigned int> solution_case;
		vector <unsigned int> solution_step;
		vector <unsigned int> min_solution_case;
		int farthest_step;
		int farthest_num;
		vector <unsigned int> farthest_case;
	};

	Case_struct empty_case;
	vector <Case_struct> All_Case;

	cout << "Find all cases..." << endl;
	for (i = 0; i < 0xFFFFFFF; i += 4) {
		if (Check(i) == true) {
			All_Case.push_back(empty_case);
			index = All_Case.size() - 1;
			All_Case[index].Code = i;
			All_Case[index].id = index;
			k = 0;
			for (j = 1; j <= 5; j++) {
				if (block[j].style == 1) {k++;}
			}
			All_Case[index].Style = k;
			All_Case[All_Case.size() - 1].Short_Code[4] = 0;
			All_Case[All_Case.size() - 1].Short_Code[3] = index % 26;
			index = (index - All_Case[All_Case.size() - 1].Short_Code[3]) / 26;
			All_Case[All_Case.size() - 1].Short_Code[2] = index % 26;
			index = (index - All_Case[All_Case.size() - 1].Short_Code[2]) / 26;
			All_Case[All_Case.size() - 1].Short_Code[1] = index % 26;
			index = (index - All_Case[All_Case.size() - 1].Short_Code[1]) / 26;
			All_Case[All_Case.size() - 1].Short_Code[0] = index % 26;
			for (j = 0; j <= 3; j++) {
				All_Case[All_Case.size() - 1].Short_Code[j] += 97;
			}
		}
		if (i % 0x1000000 == 0) {cout << (i >> 24) + 1 << "/16" << endl;}
	}
	cout << "OK -> Size of All_Case: " << All_Case.size() << endl;
	vector <unsigned int> dat;
	vector <unsigned int> Group_Index;
	for (k = 0; k <= 5; k++) {
		dat.clear();
		Group_Index.clear();
		for (i = 0; i < All_Case.size(); i++) {
			if (All_Case[i].Style == k) {
				dat.push_back(All_Case[i].Code);
			}
		}
		cout << "Style_" << k << " with " << dat.size() << " cases" << endl;
		Group_Index = Split_Group(dat);
		j = 0;
		for (i = 0; i < All_Case.size(); i++) {
			if (All_Case[i].Style == k) {
				All_Case[i].Group = Group_Index[j];
				j++;
			}
		}
	}
	cout << "Output main.csv...";
	File_Output.open("main.csv");
	File_Output << "id,short_code,code,style,group";
	for (i = 0; i < All_Case.size(); i++) {
		File_Output << endl;
		File_Output << All_Case[i].id << ",";
		File_Output << All_Case[i].Short_Code << ",";
		File_Output << Change_str(All_Case[i].Code) << ",";
		File_Output << All_Case[i].Style << ",";
		File_Output << All_Case[i].Group;
	}
	File_Output.close();
	cout << "OK" << endl;
	cout << "Start analyse all cases..." << endl;
	for (i = 0; i < All_Case.size(); i++) {
		cout << Change_str(All_Case[i].Code) << "...";
		Analyse_Case(All_Case[i].Code);
		All_Case[i].group_size = group_size;
		All_Case[i].min_solution_step = min_solution_step;
		All_Case[i].solution_num = solution_num;
		All_Case[i].min_solution_num = min_solution_num;
		All_Case[i].solution_case = solution_case;
		All_Case[i].solution_step = solution_step;
		All_Case[i].min_solution_case = min_solution_case;
		All_Case[i].farthest_step = farthest_step;
		All_Case[i].farthest_num = farthest_num;
		All_Case[i].farthest_case = farthest_case;
		cout << "OK -> " << i + 1 << "/" << All_Case.size() << endl;
	}
	cout << "Output group.csv...";
	File_Output.open("group.csv");
	File_Output << "id,group_id,group_size";
	for (i = 0; i < All_Case.size(); i++) {
		File_Output << endl;
		File_Output << All_Case[i].id << ",";
		File_Output << All_Case[i].Style << "-" << All_Case[i].Group << ",";
		File_Output << All_Case[i].group_size;
	}
	File_Output.close();
	cout << "OK" << endl;
	cout << "Output farthest.csv...";
	File_Output.open("farthest.csv");
	File_Output << "id,farthest_step,farthest_num,farthest_case";
	for (i = 0; i < All_Case.size(); i++) {
		File_Output << endl;
		File_Output << All_Case[i].id << ",";
		File_Output << All_Case[i].farthest_step << ",";
		File_Output << All_Case[i].farthest_num << ",";
		for (j = 0; j < All_Case[i].farthest_case.size(); j++) {
			File_Output << Change_str(All_Case[i].farthest_case[j]);
			if (j != All_Case[i].farthest_case.size() - 1) {File_Output << "-";}
		}
	}
	File_Output.close();
	cout << "OK" << endl;
	cout << "Output min_solution.csv...";
	File_Output.open("min_solution.csv");
	File_Output << "id,min_solution_step,min_solution_num,min_solution_case";
	for (i = 0; i < All_Case.size(); i++) {
		File_Output << endl;
		File_Output << All_Case[i].id << ",";
		File_Output << All_Case[i].min_solution_step << ",";
		File_Output << All_Case[i].min_solution_num << ",";
		for (j = 0; j < All_Case[i].min_solution_case.size(); j++) {
			File_Output << Change_str(All_Case[i].min_solution_case[j]);
			if (j != All_Case[i].min_solution_case.size() - 1) {File_Output << "-";}
		}
	}
	File_Output.close();
	cout << "OK" << endl;
	cout << "Output solution.csv...";
	File_Output.open("solution.csv");
	File_Output << "id,solution_num,solution_case";
	for (i = 0; i < All_Case.size(); i++) {
		File_Output << endl;
		File_Output << All_Case[i].id << ",";
		File_Output << All_Case[i].solution_num << ",";
		for (j = 0; j < All_Case[i].solution_case.size(); j++) {
			File_Output << Change_str(All_Case[i].solution_case[j]);
			File_Output << "(" << All_Case[i].solution_step[j] << ")";
			if (j != All_Case[i].solution_case.size() - 1) {File_Output << "-";}
		}
	}
	File_Output.close();
	cout << "OK" << endl;
	cout << "All Done!" << endl;
    return 0;
}

vector <unsigned int> Split_Group(vector <unsigned int> dat) {  //分离群组，要求输入的数据从小到大排列
	unsigned int i, j;
	vector <unsigned int> temp;
	vector <vector <unsigned int> > Group_Sorted;
	cout << "Input dat with " << dat.size() << " cases" << endl;
	cout << "Start to split the group" << endl;
	while (1 == 1) {
		if (dat.size() == 0) {break;}
		cout << "Try to find the group of " << Change_str(dat[0]) << "...";
		Calculate(dat[0]);
		cout << "OK -> " << List.size() << " members had been found!" << endl;
		cout << "Try to kick them out...";
		for (i = 0; i < List.size(); i++) {
			for (j = 0; j < dat.size(); j++) {
				if (List[i] == dat[j]) {dat[j] = 0;}
			}
		}
		cout << "OK" << endl;
		cout << "Neaten the group...";
		for (i = 0; i < List.size() - 1; i++) {
			for (j = 0; j < List.size() - 1; j++) {
				if (List[j] >= List[j + 1]) {swap(List[j], List[j + 1]);}
			}
		}
		Group_Sorted.push_back(List);
		cout << "OK" << endl;
		cout << "Refresh dat...";
		temp.clear();
		for (i = 0; i < dat.size(); i++) {
			if (dat[i] != 0) {
				temp.push_back(dat[i]);
			}
		}
		dat.clear();
		dat = temp;
		cout << "OK -> Size of dat:" << dat.size() << endl;
	}
	cout << "Neaten the Group_Sorted...";
	for (i = 0; i < Group_Sorted.size() - 1; i++) {
		for (j = 0; j < Group_Sorted.size() - 1; j++) {
			if (Group_Sorted[j].size() < Group_Sorted[j + 1].size()) {
				swap(Group_Sorted[j], Group_Sorted[j + 1]);
			}
		}
	}
	cout << "OK -> Size of Group_Sorted: " << Group_Sorted.size() << endl;
	dat.clear();
	temp.clear();
	for (i = 0; i < Group_Sorted.size(); i++) {
		for (j = 0; j < Group_Sorted[i].size(); j++) {
			dat.push_back(Group_Sorted[i][j]);
			temp.push_back(i);
		}
	}
	cout << "Record the Group_num...";
	for (i = 0; i < dat.size() - 1; i++) {
		for (j = 0; j < dat.size() - 1; j++) {
			if (dat[j] >= dat[j + 1]) {
				swap(dat[j], dat[j + 1]);
				swap(temp[j], temp[j + 1]);
			}
		}
	}
	cout << "OK" << endl;
	return temp;
}
void Output_Detail(unsigned int Code) {  //输出某一布局的具体数据
    unsigned int i, j, layer;
    Analyse_Case(Code);  //进行分析
    cout << "Code: " << Change_str(Code) << endl << endl;
    cout << "farthest_step: " << farthest_step << endl;
    cout << "farthest_num: " << farthest_num << endl;
    cout << "farthest_case: ";
    for (i = 0; i < farthest_case.size(); i++) {
         cout << Change_str(farthest_case[i]) << " ";
    }
    cout << endl << endl;
    cout << "min_solution_step: " << min_solution_step << endl;
    cout << "min_solution_num: " << min_solution_num << endl;
    cout << "min_solution_case: ";
    for (i = 0; i < min_solution_case.size(); i++) {
         cout << Change_str(min_solution_case[i]) << " ";
    }
    cout << endl << endl;
    cout << "solution_num: " << solution_num << endl;
    cout << "solution_case(solution_step): ";
    for (i = 0; i < solution_case.size(); i++) {
         cout << Change_str(solution_case[i]) << "(" << solution_step[i] << ") ";
    }
    cout << endl << endl;
    string File_Name;
    File_Name = Change_str(Code) + ".txt";
    cout << "Data save at " << File_Name << endl << endl;
    File_Output.open(File_Name.c_str());
    File_Output << "[Group_size]" << endl << group_size << endl;
    File_Output << "[Min_Solution_step]" << endl << min_solution_step << endl;
    File_Output << "[Farthest_step]" << endl << farthest_step << endl;
    File_Output << "[Min_Solution]" << endl;
    for (i = 0; i < min_solution_case.size(); i++) {
        File_Output << Change_str(min_solution_case[i]) << endl;
    }
    File_Output << "[Farthest]" << endl;
    for (i = 0; i < farthest_case.size(); i++) {
        File_Output << Change_str(farthest_case[i]) << endl;
    }
    File_Output << "[Solution]" << endl;
    for (i = 0; i < solution_case.size(); i++) {
        File_Output << Change_str(solution_case[i]) << " (" << solution_step[i] << ")" << endl;
    }
    File_Output << "[List]" << endl;
    for (i = 0; i < List.size(); i++) {
        File_Output << i << " -> " << Change_str(List[i]) << " (" << Layer_Num[i] << "," << Layer_Index[i] << ")" << endl;
    }
    File_Output << "[Layer]" << endl;
    for (layer = 0; layer < Layer.size(); layer++) {
        for (i = 0; i < Layer_Next[layer].size(); i++) {
            File_Output << "(" << layer << "," << i << ")" << " -> ";
            for (j = 0; j < Layer_Next[layer][i].size(); j++) {
                File_Output << "(" << (layer + 1) << "," << Layer_Next[layer][i][j] << ") ";
            }
            File_Output << endl;
        }
    }
    list<unsigned int>::iterator poi;
    File_Output << "[Source]" << endl;
    for (i = 0; i < List.size(); i++){
        File_Output << i << " -> " << Change_str(List[i]) << " (" << Layer_Num[i] << ")";
        poi = Source[i].begin();
        while (poi != Source[i].end()) {
            File_Output << "  [" << *poi << "]";
            ++poi;
        }
        File_Output<<endl;
    }
    File_Output.close();
}

void Analyse_Case(unsigned int Code) {  //对一个布局进行分析
	unsigned int i, j, k;
	vector <unsigned int> temp;
    min_solution_step = -1;  //初始化
	solution_num = 0;
    min_solution_num = 0;
    farthest_step = -1;
    farthest_num = 0;
	solution_case.clear();
    solution_step.clear();
	min_solution_case.clear();
	farthest_case.clear();
    Layer.clear();
    Layer_Next.clear();
    Layer_Index.clear();
    solution_flag.clear();
	if (Check(Code) == false) {return;}  //若输入编码无效则退出
	Calculate(Code);  //通过计算建立队列表
    Split_Layer();  //分层并计算链接

	group_size = List.size();  //得到整个队列树大小
    farthest_step = Layer_Num[Layer_Num.size() - 1];  //计算最远布局的步数
    for (i = 0; i < Layer[farthest_step].size(); i++) {  //找到所有最远的布局
        farthest_case.push_back(Layer[farthest_step][i]);
    }
    farthest_num = farthest_case.size();
	farthest_case = Sort(farthest_case);  //得到的结果进行排序

    for (i = 0; i < List.size(); i++) {
        if ((0xF & (List[i] >> 24)) == 0xD) {
            min_solution_step = Layer_Num[i];  //找到最少步数
            break;
        }
    }
    if (min_solution_step == -1) {return;}  //无解则退出
    for (i = 0; i < Layer[min_solution_step].size(); i++) {  //遍历最少步所在层
        if ((0xF & (Layer[min_solution_step][i] >> 24)) == 0xD) {  //判断是否为解
            min_solution_case.push_back(Layer[min_solution_step][i]);
            solution_flag[min_solution_step][i] = true;  //标识
        }
    }
    min_solution_num = min_solution_case.size();
	min_solution_case = Sort(min_solution_case);  //得到的结果进行排序

    solution_case = min_solution_case;
    for (i = 0; i < solution_case.size(); i++) {  //初始化已知部分的solution_step
        solution_step.push_back(min_solution_step);
    }
    for (i = 0; i < Layer.size() - 1; i++) {  //遍历除最后一层外的所有层
        for (j = 0; j < Layer[i].size(); j++) {  //遍历层内元素
            if (solution_flag[i][j] == true) {  //若该元素被标识
                for (k = 0; k < Layer_Next[i][j].size(); k++) {  //遍历其下一步
                    solution_flag[i + 1][Layer_Next[i][j][k]] = true;  //标识
                }
            }
        }
		temp.clear();  //初始化temp
        for (j = 0; j < Layer[i + 1].size(); j++) {  //遍历下一层内元素
            if (solution_flag[i + 1][j] == false) {  //得到未被标识的元素
                if ((0xF & (Layer[i + 1][j] >> 24)) == 0xD) {  //若为解的布局
                    //solution_case.push_back(Layer[i + 1][j]);
					temp.push_back(Layer[i + 1][j]);  //为了方便排序，先加入到temp中
                    solution_step.push_back(i + 1);
                    solution_flag[i + 1][j] = true;  //进行标识
                }
            }
        }
		temp = Sort(temp);  //将得到的数据进行处理
		for (k = 0; k < temp.size(); k++) {  //将temp内容加入solution_case中
			solution_case.push_back(temp[k]);
		}
    }
    solution_num = solution_case.size();
}

vector <unsigned int> Sort(vector <unsigned int> dat) {  //将输入的vector排序为从小到大
	unsigned int i, j;
	if (dat.size() == 0) {return dat;}  //空的则退出
	for (i = 0; i < dat.size() - 1; i++) {  //冒泡排序
		for (j = 0; j < dat.size() - 1; j++) {
			if (dat[j] >= dat[j + 1]) {swap(dat[j], dat[j + 1]);}
		}
	}
	return dat;
}

void Split_Layer() {  //分割得到不同层并进行链接
	vector <vector <unsigned int> > temp;
	int i, num, index;
	num = -1;
	index = 0;
	for (i = 0; i < Layer_Num.size(); i++) {  //遍历全部布局
		if (Layer_Num[i] != num) {  //到了新的层
            solution_flag.push_back(bool_vector);
			Layer.push_back(int_vector);
			Layer_Next.push_back(temp);
			num = Layer_Num[i];
			index = 0;
		}
        solution_flag[num].push_back(false);
		Layer_Index.push_back(index);
		index++;
		Layer[num].push_back(List[i]);
		Layer_Next[num].push_back(int_vector);  //初始化链接数据
	}

	list <unsigned int>::iterator poi;  //声明迭代器
	for (i = 1; i < List.size(); i++) {  //遍历除根布局外的额全部布局
		poi = Source[i].begin();
		while (poi != Source[i].end()) {  //遍历其上一步
			Layer_Next[Layer_Num[*poi]][Layer_Index[*poi]].push_back(Layer_Index[i]);  //加入链接表中
			++poi;
		}
	}
}

void Calculate(unsigned int Start_Code) {  //启动计算引擎
	unsigned int i;
	for (i = 0; i <= 0xFFFF; i++) {Hash[i].clear();}  //初始化
	List.clear();
	Layer_Num.clear();
	Source.clear();
	Hash[(Start_Code >> 4) & 0xFFFF].push_back(0);  //加入初始布局
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

void Add_Case(unsigned int Code) {  //将计算结果加入队列
    list <unsigned int>::iterator poi;  //定义迭代器
	poi = Hash[(Code >> 4) & 0xFFFF].begin();  //设置poi为索引表的起始点
	while (poi != Hash[(Code >> 4) & 0xFFFF].end()) {  //遍历索引表
		if (Code == List[*poi]) {  //若发现重复
            if ((Layer_Num[*poi] - Layer_Num[Now_Move]) == 1) {  //若高一层
				Source[*poi].push_back(Now_Move);  //加入父节点列表
			}
            return;  //重复 退出
        }
		++poi;
	}
	Hash[(Code >> 4) & 0xFFFF].push_back(List.size());  //将计算结果添加至索引表
	List.push_back(Code);  //将计算结果加入队列
	Layer_Num.push_back(Layer_Num[Now_Move] + 1);  //添加对应的层数
	Source.push_back(int_list);  //初始化其父节点列表
	Source[Source.size() - 1].push_back(Now_Move);  //将正在进行搜索的布局添加为父节点
}

void Move_Block(unsigned char num, unsigned char dir_1, unsigned char dir_2) {  //按要求移动块并将移动后的编码传给Add_Case
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

void Fill_Block(unsigned char addr, unsigned char style, unsigned char filler) {  //用指定内容填充table中指定的位置
	if (style == 0) {table[addr] = table[addr + 1] = table[addr + 4] = table[addr + 5] = filler;}  //2*2
	else if (style == 1) {table[addr] = table[addr + 1] = filler;}  //2*1
	else if (style == 2) {table[addr] = table[addr + 4] = filler;}  //1*2
	else if (style == 3) {table[addr] = filler;}  //1*1
}

bool Check_Empty(unsigned char address, unsigned char dir, unsigned char num) {  //判断指定位置是否为空格 若不是空格或者无效返回false
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

void Analyse_Code(unsigned int Code) {  //解译编码到 table[20] block[10] space[2]中
	unsigned char i, addr, style;
	unsigned char num_space = 0, num_type_1 = 0, num_type_2 = 5;
	space[0] = space[1] = 0xFF;  //初始化
	for (i = 0; i < 20; i++) {table[i] = 0xFF;}
	for (i = 0; i <= 9; i++) {
		block[i].address = 0xFF;
		block[i].style = 0xFF;
	}

	block[0].address = 0xF & (Code >> 24);  //开始解译
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
		style = 0x3 & (Code >> (22 - i * 2));  //0:space  1:2*1  2:1*2  3:1*1
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

bool Check(unsigned int Code) {  //检查编码是否合法 正确返回true 错误返回false
	bool temp[20];
	unsigned char addr, i;
	Analyse_Code(Code);

	for (i = 0; i < 20; i++) {
        temp[i] = false;  //初始化
		if (table[i] > 10) {return false;}  //检查table内容是否合法
	}

	if (block[0].style != 0) {return false;}  //检查2*2块
	for (i = 1; i <= 5; i++) {  //检查2*1与1*2块
		if ((block[i].style != 1) && (block[i].style != 2)) {
            return false;
        }
	}
	for (i = 6; i <= 9; i++) {  //检查1*1块
		if (block[i].style != 3) {
            return false;
        }
	}
	for (i = 0; i <= 1; i++) { //检查空格
		if (space[i] > 19) {
			return false;
		} else {
			temp[space[i]] = true;
		}
	}

	addr = block[0].address;  //检查2*2块
	if ((addr > 14) || (addr % 4 == 3)) {return false;}
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

string Change_str(unsigned int dat) {  //将编码数据转化为字符
	unsigned char i, bit;
	string str = "";
	for (i = 0; i < 7; i++) {
		bit = 0xF & dat >> (6 - i) * 4;  //分离单个十六进制位
		if ((bit >= 0) && (bit <= 9)) {str += bit + 48;}  //0~9
		if ((bit >= 0xA) && (bit <= 0xF)) {str += bit + 55;}  //A~F
	}
	return str;
}

unsigned int Change_int(char str[8]) {  //将编码字符转化为int
	unsigned int i, dat = 0;
	for (i = 0; i < 7; i++) {
        if ((str[i] >= 48) && (str[i] <= 57)) {dat |= (str[i] - 48) << (24 - i * 4);}  //0~9
		if ((str[i] >= 65) && (str[i] <= 70)) {dat |= (str[i] - 55) << (24 - i * 4);}  //A~F
		if ((str[i] >= 97) && (str[i] <= 102)) {dat |= (str[i] - 87) << (24 - i * 4);}  //a~f
	}
	return dat;
}

void Output_Graph(unsigned int Code) {  //命令行下图形化输出布局图
	const unsigned char Square_Width = 4;  //方块宽度
	const string Square_str = "[]";  //or "u2588"
	unsigned int i, k, m;
	unsigned int start_x, start_y, end_x, end_y;
	bool Graph[(Square_Width + 1) * 4 + 1][(Square_Width + 1) * 5 + 1];  //点阵方式储存输出图像
	if (Check(Code) == false) {
		cout << "The Code is Wrong!" << endl;
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

	for (i = 1; i <= (Square_Width + 1) * 4 + 3; i++) {cout << Square_str;}  //显示上边框
	cout << endl << Square_str;
	for (k = 0; k <= (Square_Width + 1) * 5; k++){  //显示图像内容
		for (i = 0; i <= (Square_Width + 1) * 4; i++) {
			if (Graph[i][k] == false){
				cout << "  ";
			} else {
				cout << Square_str;
			}
		}
		cout << Square_str << endl << Square_str;  //显示左右边框
	}
	for (i = 1; i <= (Square_Width + 1) * 4 + 2; i++) {cout << Square_str;}  //显示下边框
	cout<<endl;
}

void debug() {  //输出当前 table[20] block[10] space[2]的状态
	unsigned int i;
	for (i = 0; i < 20; i++) {  //输出table状态
		if (table[i] != 0xA) {
			cout << int(table[i]) << " ";
		} else {
			cout << "A ";
		}
		if (i == 3) {cout << "    00 01 02 03" << endl;}
		if (i == 7) {cout << "    04 05 06 07" << endl;}
		if (i == 11) {cout << "    08 09 10 11" << endl;}
		if (i == 15) {cout << "    12 13 14 15" << endl;}
		if (i == 19) {cout << "    16 17 18 19" << endl;}
	}
    cout << endl;
	cout << "space[0] -> address=" << int(space[0]) << endl;  //输出space状态
	cout << "space[1] -> address=" << int(space[1]) << endl;
    cout << endl;
	for (i = 0; i <= 9; i++) {  //输出block状态
		cout << "block[" << i << "] -> address=" << int(block[i].address);
        cout << " style=" << int(block[i].style) << endl;
	}
}
