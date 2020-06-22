#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

ifstream File_Input;
ofstream File_Output;

struct Case_struct {
    bool freeze[4][5]; // true -> no move ; false -> can move
    unsigned char status[4][5]; // 0xFF -> undefined ; 0xFE -> space
    unsigned char type[15]; // 0 -> 2 * 2 ; 1 -> 2 * 1 ; 2 -> 1 * 2 ; 3 -> 1 * 1
    unsigned long long code;
};
vector <vector <Case_struct *> > Layer; // 储存全部层数据的节点
vector <vector <vector <int> > > Hash; // 哈希表
vector <vector <vector <int> > > Layer_Next; // 子节点数据
vector <vector <vector <int> > > Layer_Source; // 父节点数据
vector <int> int_vector;
int layer_num, layer_index; // layer_num: 当前扫描节点的层编号  layer_index: 当前扫描节点的层中编号

// 布局的基本参数
int min_solution_step; // 最少的步数
int min_solution_num; // 最少步解的个数
vector <unsigned long long> min_solution_case; // 所有最少步解

vector <unsigned int> solution_step; // 所有解对应的步数
int solution_num; // 解的个数
vector <unsigned long long> solution_case; // 所有解

int farthest_step; // 最远布局的步数
int farthest_num; // 最远布局的个数
vector <unsigned long long> farthest_case; // 所有最远的布局

void debug(Case_struct &dat);
unsigned long long Change_int (char str[10]);
string Change_str(unsigned long long dat);
bool Parse_Code(Case_struct &dat, unsigned long long Code);
void Get_Code(Case_struct &dat);
void Find_Sub_Case(Case_struct &dat, int &num, int x, int y, bool addr[4][5]);
void Build_Case(Case_struct &dat, int &num, int x, int y, bool addr[4][5]);
void Find_Next_Case(Case_struct &dat_raw);
void Add_Case(Case_struct *dat);
void Calculate(unsigned long long code);
void Free_Data();
void Analyse_Case(unsigned long long code);
void Sort(vector <unsigned long long> &dat);

int main() {
    cout << "Klotski Calculator by Dnomd343" << endl;
    cout << "start" << endl;
    //Calculate(0x4FEA13400); // 0x1A9BF0C00 0x2CF519C00 0x652D7F000 0x2B1877C00
    struct Case_struct {
        unsigned int id;
        unsigned long long code;
        int min_solution_step;
        int min_solution_num;
        vector <unsigned long long> min_solution_case;
        vector <unsigned int> solution_step;
        int solution_num;
        vector <unsigned long long> solution_case;
        int farthest_step;
        int farthest_num;
        vector <unsigned long long> farthest_case;
    };
    vector <Case_struct> Cases;
    Case_struct empty_case;
    char str[9];
    int i, j;
    File_Input.open("test.txt");
    while (File_Input.eof() != true) {
		File_Input >> str;
        Cases.push_back(empty_case);
        Cases[Cases.size() - 1].id = Cases.size() - 1;
		Cases[Cases.size() - 1].code = Change_int(str);
	}
    File_Input.close();
    for (i = 0; i < Cases.size(); i++) {
		cout << Change_str(Cases[i].code) << "...";
		Analyse_Case(Cases[i].code);
		Cases[i].min_solution_step = min_solution_step;
		Cases[i].solution_num = solution_num;
		Cases[i].min_solution_num = min_solution_num;
		Cases[i].solution_case = solution_case;
		Cases[i].solution_step = solution_step;
		Cases[i].min_solution_case = min_solution_case;
		Cases[i].farthest_step = farthest_step;
		Cases[i].farthest_num = farthest_num;
		Cases[i].farthest_case = farthest_case;
		cout << "OK -> " << i + 1 << "/" << Cases.size() << endl;
	}
    cout << "Output farthest.csv...";
	File_Output.open("farthest.csv");
	File_Output << "id,farthest_step,farthest_num,farthest_case";
	for (i = 0; i < Cases.size(); i++) {
		File_Output << endl;
		File_Output << Cases[i].id << ",";
		File_Output << Cases[i].farthest_step << ",";
		File_Output << Cases[i].farthest_num << ",";
		for (j = 0; j < Cases[i].farthest_case.size(); j++) {
			File_Output << Change_str(Cases[i].farthest_case[j]);
			if (j != Cases[i].farthest_case.size() - 1) {File_Output << "-";}
		}
	}
	File_Output.close();
	cout << "OK" << endl;
	cout << "Output min_solution.csv...";
	File_Output.open("min_solution.csv");
	File_Output << "id,min_solution_step,min_solution_num,min_solution_case";
	for (i = 0; i < Cases.size(); i++) {
		File_Output << endl;
		File_Output << Cases[i].id << ",";
		File_Output << Cases[i].min_solution_step << ",";
		File_Output << Cases[i].min_solution_num << ",";
		for (j = 0; j < Cases[i].min_solution_case.size(); j++) {
			File_Output << Change_str(Cases[i].min_solution_case[j]);
			if (j != Cases[i].min_solution_case.size() - 1) {File_Output << "-";}
		}
	}
	File_Output.close();
	cout << "OK" << endl;
	cout << "Output solution.csv...";
	File_Output.open("solution.csv");
	File_Output << "id,solution_num,solution_case";
	for (i = 0; i < Cases.size(); i++) {
		File_Output << endl;
		File_Output << Cases[i].id << ",";
		File_Output << Cases[i].solution_num << ",";
		for (j = 0; j < Cases[i].solution_case.size(); j++) {
			File_Output << Change_str(Cases[i].solution_case[j]);
			File_Output << "(" << Cases[i].solution_step[j] << ")";
			if (j != Cases[i].solution_case.size() - 1) {File_Output << "-";}
		}
	}
	File_Output.close();
	cout << "OK" << endl;
	cout << "All Done!" << endl;

    cout << "bye..." << endl;
    return 0;
}

void Analyse_Case(unsigned long long code) { // 分析输入编码的各种参数 (输入编码必须无误)
    vector < vector <bool> > solution_flag;
    vector <unsigned long long> temp;
    int i, j, k;
    farthest_step = -1; // 初始化farthest
    farthest_num = 0;
	farthest_case.clear();
    min_solution_step = -1; // 初始化min_solution
    min_solution_num = 0;
    min_solution_case.clear();
	solution_num = 0; // 初始化solution
	solution_case.clear();
    solution_step.clear();
    Calculate(code); // 计算分层数据
    solution_flag.resize(Layer.size()); // 同步Layer的结构
    for (i = 0; i < solution_flag.size(); i++) {
        solution_flag[i].resize(Layer[i].size());
    }

    farthest_step = Layer.size() - 1; // 计算最远布局的步数
    for (i = 0; i < Layer[farthest_step].size(); i++) { // 找到所有最远的布局
        farthest_case.push_back((*Layer[farthest_step][i]).code);
    }
    farthest_num = farthest_case.size();
	Sort(farthest_case);  //得到的结果进行排序

    for (i = 0; i < Layer.size(); i++) {
        for (j = 0; j < Layer[i].size(); j++) {
            if (((*Layer[i][j]).code >> 32) == 0xD) { // 2 * 2块在出口位置
                min_solution_step = i; // 找到最少步数
                j = Layer[i].size(); // 跳出两层循环
                i = Layer.size();
            }
        }
    }
    if (min_solution_step == -1) {return;} // 无解则退出
    for (i = 0; i < Layer[min_solution_step].size(); i++) { // 遍历最少步所在层
        if (((*Layer[min_solution_step][i]).code >> 32) == 0xD) { // 判断是否为解
            min_solution_case.push_back((*Layer[min_solution_step][i]).code); // 加入最少步解序列中
            solution_flag[min_solution_step][i] = true; // 标识
        }
    }
    min_solution_num = min_solution_case.size();
	Sort(min_solution_case); // 得到的结果进行排序
    
    solution_case = min_solution_case; // 同步最少步解到所有解序列中
    for (i = 0; i < solution_case.size(); i++) { // 初始化已知部分的solution_step
        solution_step.push_back(min_solution_step);
    }
    for (i = 0; i < Layer.size() - 1; i++) { // 遍历除最后一层外的所有层
        for (j = 0; j < Layer[i].size(); j++) { // 遍历层内元素
            if (solution_flag[i][j] == true) { // 若该元素被标识
                for (k = 0; k < Layer_Next[i][j].size(); k++) { // 遍历其下一步
                    solution_flag[i + 1][Layer_Next[i][j][k]] = true; // 标识
                }
            }
        }
		temp.clear();
        for (j = 0; j < Layer[i + 1].size(); j++) { // 遍历下一层内元素
            if (solution_flag[i + 1][j] == false) { // 得到未被标识的元素
                if (((*Layer[i + 1][j]).code >> 32) == 0xD) { // 若为解的布局
					temp.push_back((*Layer[i + 1][j]).code); // 先加入到temp中方便排序
                    solution_step.push_back(i + 1);
                    solution_flag[i + 1][j] = true; // 标识
                }
            }
        }
		Sort(temp); // 将得到的结果进行排序
		for (k = 0; k < temp.size(); k++) { // 将temp内容加入solution_case中
			solution_case.push_back(temp[k]);
		}
    }
    solution_num = solution_case.size();
}

void Sort(vector <unsigned long long> &dat) { // 将输入的vector排序 (从小到大)
	unsigned int i, j;
	if (dat.size() == 0) {return;} // 空的则退出
	for (i = 0; i < dat.size() - 1; i++) { // 冒泡排序
		for (j = 0; j < dat.size() - 1 - i; j++) {
			if (dat[j] >= dat[j + 1]) {
                swap(dat[j], dat[j + 1]);
            }
		}
	}
}

void Free_Data() { //释放数据
    for (int i = 0; i < Layer.size(); i++) { // 释放Layer中指向的全部节点
        for (int j = 0; j < Layer[i].size(); j++) {
            delete Layer[i][j];
        }
    }
    Layer.clear(); // 清空层数据
    Hash.clear(); // 清空哈希表
    Layer_Next.clear(); // 清空子节点标识
    Layer_Source.clear(); // 清空父节点标识
}

void Calculate(unsigned long long code) { // 计算输入编码的全部层数据
    Free_Data(); // 释放上一次计算的数据
    Case_struct *dat = new Case_struct;
    vector <Case_struct *> empty_layer;
    vector <vector <int> > int_2nd_vector;
    vector <vector <int> > hash_layer;
    hash_layer.resize(0x100); // 单层哈希索引设定为8位
    Parse_Code(*dat, code); // 解译输入编码
    Layer.push_back(empty_layer); // 添加首层
    Hash.push_back(hash_layer);
    Layer_Next.push_back(int_2nd_vector);
    Layer_Source.push_back(int_2nd_vector);
    Layer[0].push_back(dat); // 添加根节点
    Hash[0][0xff & (code >> 24)].push_back(0);
    Layer_Next[0].push_back(int_vector);
    Layer_Source[0].push_back(int_vector);
    layer_num = layer_index = 0; // 定义入口为根节点
    while (1 == 1) { // 创建死循环
        if (layer_index == 0) { // 若在计算层的第一个元素
            Layer.push_back(empty_layer); // 则新增一层
            Hash.push_back(hash_layer);
            Layer_Next.push_back(int_2nd_vector);
            Layer_Source.push_back(int_2nd_vector);
        }
        Find_Next_Case(*Layer[layer_num][layer_index]); // 寻找子布局
        if (layer_index == Layer[layer_num].size() - 1) { // 若在层的最后一个元素
            if (Layer[layer_num + 1].size() == 0) { // 若下一层是空的
                break; // 已全部搜索完毕 退出搜索循环
            }
            layer_num++; // 计算目标移到下一层第一个元素
            layer_index = 0;
        } else { // 不是最后一个元素
            layer_index++; // 计算目标移到下一元素
        }
    }
    Layer.pop_back(); // 移除最后的空层
    Hash.pop_back();
    Layer_Next.pop_back();
    Layer_Source.pop_back();
}

void Add_Case(Case_struct *dat) { // 新节点若不重复即可以加入
    int x, y, k, num;
    int hash_index = (0xff & ((*dat).code >> 24)); // 取编码低24 ~ 32位作为哈希索引
    num = layer_num; // 扫描目标为当前计算所在层
    for (k = 0; k < Hash[num][hash_index].size(); k++) { // 遍历对应索引
        if ((*Layer[num][Hash[num][hash_index][k]]).code == (*dat).code) { // 若发现重复
            delete dat; // 释放不加入的节点
            return; // 退出
        }
    }
    num++; // 向下一层
    for (k = 0; k < Hash[num][hash_index].size(); k++) { // 遍历对应索引
        if ((*Layer[num][Hash[num][hash_index][k]]).code == (*dat).code) { // 若发现重复
            for (x = 0; x < 4; x++) { // 遍历freeze表
                for (y = 0; y < 5; y++) {
                    if ((*dat).freeze[x][y] == true) { // 将输入表合并到原先的表上
                        (*Layer[num][Hash[num][hash_index][k]]).freeze[x][y] = true;
                    }
                }
            }
            Layer_Next[layer_num][layer_index].push_back(Hash[num][hash_index][k]); // 添加子节点数据
            Layer_Source[num][Hash[num][hash_index][k]].push_back(layer_index); // 添加父节点数据
            delete dat; // 释放不加入的节点
            return; // 退出
        }
    }
    Hash[layer_num + 1][0xff & ((*dat).code >> 24)].push_back(Layer[layer_num + 1].size()); // 添加索引
    Layer[layer_num + 1].push_back(dat); // 新增布局到Layer对应层中
    Layer_Next[layer_num + 1].push_back(int_vector); // 新建子节点
    Layer_Source[layer_num + 1].push_back(int_vector); // 新建父节点
    Layer_Next[layer_num][layer_index].push_back(Layer[layer_num + 1].size() - 1); // 添加子节点数据
    Layer_Source[layer_num + 1][Layer_Source[layer_num + 1].size() - 1].push_back(layer_index); // 添加父节点数据
}

void Find_Next_Case(Case_struct &dat_raw) { // 找到下一步移动的情况(一步可以为同一块多次移动) 结果聚集到Add_Case中
    int num, x, y, i, j;
    bool addr[4][5]; // 在Find_Sub_Case深搜中用于剪枝
    Case_struct dat = dat_raw;
    for (y = 0; y < 5; y++) { // 仅保留空格位置的freeze为true
        for (x = 0; x < 4; x++) {
            if (dat.status[x][y] != 0xFE && dat.freeze[x][y] == true) { // 不为空格但freeze为true
                dat.freeze[x][y] = false; // 重置为false
            }
        }
    }
    for (y = 0; y < 5; y++) { // 遍历整个棋盘
        for (x = 0; x < 4; x++) {
            if (dat_raw.freeze[x][y] == true) {continue;} // 遇到freeze为true的跳过
            num = dat.status[x][y]; // 统一修改(x, y)块 减少代码量
            dat.status[x][y] = 0xFE;
            dat.freeze[x][y] = true;
            for (i = 0; i < 4; i++) { // 初始化
                for (j = 0; j < 5; j++) {
                    addr[i][j] = false;
                }
            }
            addr[x][y] = true; // 加入当前块 防止重复查询
            switch (dat.type[num]) {
                case 0: // 2 * 2
                    dat_raw.freeze[x + 1][y] 
                        = dat_raw.freeze[x][y + 1] = dat_raw.freeze[x + 1][y + 1] = true;
                    dat.status[x + 1][y] = dat.status[x][y + 1] = dat.status[x + 1][y + 1] = 0xFE;
                    dat.freeze[x + 1][y] = dat.freeze[x][y + 1] = dat.freeze[x + 1][y + 1] = true;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x + 1][y] = dat.status[x][y + 1] = dat.status[x + 1][y + 1] = num;
                    dat.freeze[x + 1][y] = dat.freeze[x][y + 1] = dat.freeze[x + 1][y + 1] = false;
                    break;
                case 1: // 2 * 1
                    dat_raw.freeze[x + 1][y] = true;
                    dat.status[x + 1][y] = 0xFE;
                    dat.freeze[x + 1][y] = true;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x + 1][y] = num;
                    dat.freeze[x + 1][y] = false;
                    break;
                case 2: // 1 * 2
                    dat_raw.freeze[x][y + 1] = true;
                    dat.status[x][y + 1] = 0xFE;
                    dat.freeze[x][y + 1] = true;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x][y + 1] = num;
                    dat.freeze[x][y + 1] = false;
                    break;
                case 3: // 1 * 1
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    break;
            }
            dat.status[x][y] = num; // 复原统一修改的块
            dat.freeze[x][y] = false;
        }
    }
}

void Find_Sub_Case(Case_struct &dat, int &num, int x, int y, bool addr[4][5]) { // 找到下一个单格移动的情况
    switch (dat.type[num]) {
        case 0: // 2 * 2
            if (y != 0) { // 不在最上面
                if (dat.status[x][y - 1] == 0xFE && dat.status[x + 1][y - 1] == 0xFE) { // 上面为空
                    Build_Case(dat, num, x, y - 1, addr);
                }
            }
            if (y != 3) { // 不在最下面
                if (dat.status[x][y + 2] == 0xFE && dat.status[x + 1][y + 2] == 0xFE) { // 下面为空
                    Build_Case(dat, num, x, y + 1, addr);
                }
            }
            if (x != 0) { // 不在最左边
                if (dat.status[x - 1][y] == 0xFE && dat.status[x - 1][y + 1] == 0xFE) { // 左边为空
                    Build_Case(dat, num, x - 1, y, addr);
                }
            }
            if (x != 2) { // 不在最右边
                if (dat.status[x + 2][y] == 0xFE && dat.status[x + 2][y + 1] == 0xFE) { // 右边为空
                    Build_Case(dat, num, x + 1, y, addr);
                }
            }
            break;
        case 1: // 2 * 1
            if (y != 0) { // 不在最上面
                if (dat.status[x][y - 1] == 0xFE && dat.status[x + 1][y - 1] == 0xFE) { // 上面为空
                    Build_Case(dat, num, x, y - 1, addr);
                }
            }
            if (y != 4) { // 不在最下面
                if (dat.status[x][y + 1] == 0xFE && dat.status[x + 1][y + 1] == 0xFE) { // 下面为空
                    Build_Case(dat, num, x, y + 1, addr);
                }
            }
            if (x != 0) { // 不在最左边
                if (dat.status[x - 1][y] == 0xFE) { // 左边为空
                    Build_Case(dat, num, x - 1, y, addr);
                }
            }
            if (x != 2) { // 不在最右边
                if (dat.status[x + 2][y] == 0xFE) { // 右边为空
                    Build_Case(dat, num, x + 1, y, addr);
                }
            }
            break;
        case 2: // 1 * 2
            if (y != 0) { // 不在最上面
                if (dat.status[x][y - 1] == 0xFE) { // 上面为空
                    Build_Case(dat, num, x, y - 1, addr);
                }
            }
            if (y != 3) { // 不在最下面
                if (dat.status[x][y + 2] == 0xFE) { // 下面为空
                    Build_Case(dat, num, x, y + 1, addr);
                }
            }
            if (x != 0) { // 不在最左边
                if (dat.status[x - 1][y] == 0xFE && dat.status[x - 1][y + 1] == 0xFE) { // 左边为空
                    Build_Case(dat, num, x - 1, y, addr);
                }
            }
            if (x != 3) { // 不在最右边
                if (dat.status[x + 1][y] == 0xFE && dat.status[x + 1][y + 1] == 0xFE) { // 右边为空
                    Build_Case(dat, num, x + 1, y, addr);
                }
            }
            break;
        case 3: // 1 * 1
            if (y != 0) { // 不在最上面
                if (dat.status[x][y - 1] == 0xFE) { // 上面为空
                    Build_Case(dat, num, x, y - 1, addr);
                }
            }
            if (y != 4) { // 不在最下面
                if (dat.status[x][y + 1] == 0xFE) { // 下面为空
                    Build_Case(dat, num, x, y + 1, addr);
                }
            }
            if (x != 0) { // 不在最左边
                if (dat.status[x - 1][y] == 0xFE) { // 左边为空
                    Build_Case(dat, num, x - 1, y, addr);
                }
            }
            if (x != 3) { // 不在最右边
                if (dat.status[x + 1][y] == 0xFE) { // 右边为空
                    Build_Case(dat, num, x + 1, y, addr);
                }
            }
            break;
    }
}

void Build_Case(Case_struct &dat, int &num, int x, int y, bool addr[4][5]) { // 实现移动并将结果发送到Add_Case
    if (addr[x][y] == true) { // 重复
        return; // 退出
    } else {
        addr[x][y] = true; // 加入位置数据
    }
    Case_struct *dat_mod = new Case_struct; // 新建对象 在Add_Case中加入层中或被释放
    *dat_mod = dat;
    switch ((*dat_mod).type[num]) { // 注入移动后的信息
        case 0: // 2 * 2
            (*dat_mod).status[x][y] = (*dat_mod).status[x][y + 1] 
                = (*dat_mod).status[x + 1][y] = (*dat_mod).status[x + 1][y + 1] = num;
            break;
        case 1: // 2 * 1
            (*dat_mod).status[x][y] = (*dat_mod).status[x + 1][y] = num;
            break;
        case 2: // 1 * 2
            (*dat_mod).status[x][y] = (*dat_mod).status[x][y + 1] = num;
            break;
        case 3: // 1 * 1
            (*dat_mod).status[x][y] = num;
            break;
    }
    Get_Code(*dat_mod); // 更新移动后的编码
    Add_Case(dat_mod); // 发送给Add_Case
    Find_Sub_Case(dat, num, x, y, addr); // 递归搜索
}

void Get_Code(Case_struct &dat) { // 获取编码并存储在dat.code 输入数据必须无误
    bool temp[4][5]; // 用于临时标记
    int x, y, num;
    dat.code = 0;
    for (x = 0; x < 4; x++) { // 初始化temp
        for (y = 0; y < 5; y++) {
            temp[x][y] = false;
        }
    }
    num = 0;
    for (y = 0; y < 5; y++) { // 遍历20个格
        for (x = 0; x < 4; x++) {
            if (temp[x][y] == true) {continue;} // 该格已被占用
            if (dat.status[x][y] == 0xFE) { // space
                num++;
                dat.code <<= 2;
                continue;
            }
            switch (dat.type[dat.status[x][y]]) { // type -> 0 / 1 / 2 / 3
                case 0: // 2 * 2
                    dat.code |= (x + y * 4) << (num * 2); // 写入2 * 2块位置
                    temp[x][y + 1] = temp[x + 1][y] = temp[x + 1][y + 1] = true; // 标记占用
                    break;
                case 1: // 2 * 1
                    num++;
                    dat.code <<= 2;
                    dat.code |= 1; // 01
                    temp[x + 1][y] = true; // 标记占用
                    break;
                case 2: // 1 * 2
                    num++;
                    dat.code <<= 2;
                    dat.code |= 2; // 10
                    temp[x][y + 1] = true; // 标记占用
                    break;
                case 3: // 1 * 1
                    num++;
                    dat.code <<= 2;
                    dat.code |= 3; // 11
                    break;
            }
        }
    }
    dat.code <<= (16 - num) * 2; // 左移使编码占满低36位
    dat.code &= 0xFFFFFFFFF; // 清除高28位内容
}

bool Parse_Code(Case_struct &dat, unsigned long long Code) { // 解析编码 返回false表示编码有误
    unsigned char range[16]; // dat低32位分16组
    int i, x, y, num, space_num = 0;
    dat.code = Code;
    for (x = 0; x < 4; x++) { // 初始化status和freeze
        for (y = 0; y < 5; y++) {
            dat.status[x][y] = 0xFF;
            dat.freeze[x][y] = false;
        }
    }
    for (i = 0; i < 15; i++) { // 初始化type
        dat.type[i] = 0xFF;
    }
    num = 0;
    for (i = 15; i >= 0; i--) { // 载入排列到range
        range[i] = Code & 0x3  ;
        if (range[i] == 0) {num++;}
        Code >>= 2;
    }
    if (num < 2) {return false;} // 0的个数低于两个出错
    if (Code > 14) {return false;} // 排除越界情况
    if (Code % 4 == 3) {return false;}
    dat.type[0] = 0; // 载入2 * 2方块
    x = Code % 4;
    y = Code / 4;
    dat.status[x][y] = dat.status[x + 1][y] = dat.status[x][y + 1] = dat.status[x + 1][y + 1] = 0;

    num = x = y = 0;
    for (i = 0; i < 16; i++) {
        while (dat.status[x][y] != 0xFF) { // 找到下一个未填入的位置
            if (++x == 4) {
                x = 0;
                if (++y == 5) { // 已填满20个空位 越界
                    if (space_num < 2) {return false;} // 空格低于两个 出错
                    for (num = i; num < 16; num++) { // 检查余下编码是否为0
                        if (range[num] != 0) {return false;} // 出现非0 编码错误
                    }
                    return true; // 全为0 编码正确
                }
            }
        }
        switch (range[i]) { // 分别处理四种情况
            case 0: // space
                space_num++;
                dat.status[x][y] = 0xFE;
                dat.freeze[x][y] = true; // 空格标记为不可移动
                break;
            case 1: // 2 * 1
                if (x == 3) {return false;} // 越界出错
                if (dat.status[x + 1][y] != 0xFF) {return false;} // 方块重叠
                num++;
                dat.type[num] = 1;
                dat.status[x][y] = dat.status[x + 1][y] = num;
                break;
            case 2: // 1 * 2
                if (y == 4) {return false;} // 越界出错
                if (dat.status[x][y + 1] != 0xFF) {return false;} // 方块重叠
                num++;
                dat.type[num] = 2;
                dat.status[x][y] = dat.status[x][y + 1] = num;
                break;
            case 3: // 1 * 1
                num++;
                dat.type[num] = 3;
                dat.status[x][y] = num;
                break;
        }
    }
    return true; // 20格恰好被填满
}

string Change_str(unsigned long long dat) { // 将数字转化为文本编码
    string str;
    str.resize(9); // 修改其长度为9位
    for (int i = 8; i >= 0; i--) { // 将每一位从数值转为ASCII码
        if ((dat & 0xF) <= 9) { // 0 ~ 9
            str[i] = (dat & 0xF) + 48;
        } else { // A ~ F
            str[i] = (dat & 0xF) + 55;
        }
        dat >>= 4;
    }
    return str;
}

unsigned long long Change_int (char *str) { // 将文本编码转化为数字(传入9位字符串)
    unsigned long long dat = 0;
    for (int i = 0; i < 9; i++) { // 将每一位从ASCII码转为数值
        dat <<= 4;
        if (str[i] >= 48 && str[i] <= 57) { // 0 ~ 9
            dat |= str[i] - 48;
        } else if (str[i] >= 65 && str[i] <= 70) { // A ~ F
            dat |= str[i] - 55;
        } else if (str[i] >= 97 && str[i] <= 102) { // a ~ f
            dat |= str[i] - 87;
        }
	}
    return dat;
}

void debug(Case_struct &dat) {
    cout << "status" << endl;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 4; x++) {
            if (dat.status[x][y] <= 9) { // 0 ~ 9
                cout << int(dat.status[x][y]) << " ";
            } else if (dat.status[x][y] <= 0xE) { // A ~ E
                cout << char(dat.status[x][y] + 55) << " ";
            } else if (dat.status[x][y] == 0xFE) { // space
                cout << ". ";
            } else if (dat.status[x][y] == 0xFF) { // undefined
                cout << "* ";
            } else { // error
                cout << "! ";
            }
        }
        cout << endl;
    }
    cout << "freeze" << endl;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 4; x++) {
            if (dat.freeze[x][y] == true) {
                cout << "x ";
            } else {
                cout << "- ";
            }
        }
        cout << endl;
    }
    cout << "type" << endl;
    for (int i = 0; i < 15; i++) {
        if (i < 10) {
            cout << i;
        } else {
            cout << char(i + 55);
        }
        cout << " -> ";
        if (dat.type[i] == 0) {
            cout << "2 * 2" << endl;
        } else if (dat.type[i] == 1) {
            cout << "2 * 1" << endl;
        } else if (dat.type[i] == 2) {
            cout << "1 * 2" << endl;
        } else if (dat.type[i] == 3) {
            cout << "1 * 1" << endl;
        } else {
            cout << "undefined" << endl;
        }
    }
    cout << "code: " << Change_str(dat.code) << endl;
}