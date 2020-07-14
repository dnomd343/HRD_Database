#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "HRD_cal.h"
#include "HRD_group.h"

ofstream output_farthest;
ofstream output_solution;
ifstream input_seed;

bool HRD_group::Multi_Analyse(string seed_File_name, string name_farthest, string name_solution, bool is_output_solution) {
    HRD_cal cal;
    char str[10];
    vector <unsigned long long> dat;
    vector <unsigned long long> seed; // 储存所有将要计算的布局编码
    input_seed.open(seed_File_name);
    while (input_seed.eof() != true) { // 从外部文件读入列表
        input_seed >> str;
        seed.push_back(Change_int(str));
    }
    for (unsigned int i = 0; i < seed.size(); i++) { // 判断编码正确性
        if (!cal.Check_Code(seed[i])) {
            cout << "input code error" << endl; // 发现错误编码
            return false; // 退出
        }
    }
    Output_solution_case = is_output_solution;
    output_farthest.open(name_farthest);
    output_solution.open(name_solution);
    for (unsigned int i = 0; i < seed.size(); i++) {
        cout << "Start: " << Change_str(seed[i]) << endl;
        dat = cal.Calculate_All(seed[i]); // 得到整个群
        sort(dat.begin(), dat.end()); // 排列
        Analyse_Group(dat); // 分析整个群
    }
    output_farthest.close();
    output_solution.close();
    return true;
}

void HRD_group::Batch_Analyse(unsigned long long seed, string name_farthest, string name_solution, bool is_output_solution) { // 根据群中一个布局分析整个群全部布局的参数 并将结果以csv格式输出到文件
    HRD_cal cal;
    vector <unsigned long long> dat;
    if (!cal.Check_Code(seed)) {return;} // 编码错误 退出
    dat = cal.Calculate_All(seed); // 得到整个群
    sort(dat.begin(), dat.end()); // 排列
    Output_solution_case = is_output_solution;
    output_farthest.open(name_farthest);
    output_solution.open(name_solution);
    Analyse_Group(dat); // 分析整个群
    output_farthest.close();
    output_solution.close();
}

void HRD_group::Analyse_Group(vector <unsigned long long> dat) { // 传入整个群并将结果输出到文件
    unsigned int i, j, k;
    int hash_index;
    vector <Case *> List; // 全组数据
    vector <Case *> List_hash[0x10000]; // 哈希索引
    for (i = 0; i < dat.size(); i++) { // 将数据注入到List中
        Case *temp = new Case;
        temp->Code = dat[i];
        List.push_back(temp);
    }
    for (i = 0; i < List.size(); i++) { // 构建哈希索引表
        hash_index = 0xffff & (List[i]->Code >> 16);
        List_hash[hash_index].push_back(List[i]);
    }
    for (i = 0; i < List.size(); i++) { // 计算全部2 * 2块的位置
        List[i]->addr_2x2 = List[i]->Code >> 32;
    }
    vector <unsigned long long> next_code;
    for (i = 0; i < List.size(); i++) { // 构建关系网
        next_code = Find_Next_Case(List[i]->Code); // 找到全部子布局
        for (j = 0; j < next_code.size(); j++) { // 遍历子布局
            hash_index = 0xffff & (next_code[j] >> 16); // 取得哈希索引
            for (k = 0; List_hash[hash_index].size(); k++) { // 搜索该索引
                if (List_hash[hash_index][k]->Code == next_code[j]) { // 找到目标
                    List[i]->Next.push_back(List_hash[hash_index][k]); // 链接目标位置
                    break; // 退出循环
                }
            }
        }
    }
    Case_detail *result;
    for(i = 0; i < List.size(); i++) { // 遍历整个队列
        for (k = 0; k < List.size(); k++) { // 初始化
            List[k]->Layer_num = -1;
            List[k]->Flag = false;
        }
        result = Analyse_Case(List[i]); // 计算对应布局数据并储存到result中
        Output_detail(result);
        delete result; // 释放内存
        if (i % 13 == 0) {
            cout << i << "/" << List.size() << endl;
        }
    }
    for (i = 0; i < List.size(); i++) { // 释放List数据
        delete List[i];
    }
}

void HRD_group::Output_detail(Case_detail *dat) {
    unsigned int i;
    // farthest
    output_farthest << Change_str(dat->code) << ",";
    output_farthest << dat->farthest_step << ",";
    output_farthest << dat->farthest_num << ",";
    for (i = 0; i < dat->farthest_case.size(); i++) {
        output_farthest << Change_str(dat->farthest_case[i]);
        if (i + 1 != dat->farthest_case.size()) {
            output_farthest << "-";
        }
    }
    output_farthest << endl;
    // solution
    output_solution << Change_str(dat->code) << ",";
    output_solution << dat->min_solution_step << ",";
    output_solution << dat->min_solution_num << ",";
    for (i = 0; i < dat->min_solution_case.size(); i++) {
        output_solution << Change_str(dat->min_solution_case[i]);
        if (i + 1 != dat->min_solution_case.size()) {
            output_solution << "-";
        }
    }
    output_solution << "," << dat->solution_num;
    if (!Output_solution_case) {
        output_solution << endl;
        return;
    }
    output_solution << ",";
    for (i = 0; i < dat->solution_case.size(); i++) {
        output_solution << Change_str(dat->solution_case[i]);
        output_solution << "(" << dat->solution_step[i] << ")";
        if (i + 1 != dat->solution_case.size()) {
            output_solution << "-";
        }
    }
    output_solution << endl;
}

HRD_group::Case_detail* HRD_group::Analyse_Case(Case *start) { // 根据关系网计算布局的参数
    unsigned int i, k;
    vector <Case *> case_list;
    Case_detail *dat = new Case_detail; // dat储存分析结果
    Case_detail_init(*dat); // 初始化
    dat->code = start->Code;
    start->Layer_num = 0; //令入口节点的层级为0
    case_list.push_back(start); // 加入队列中
    i = 0;
    while (i != case_list.size()) { // 找到所有元素后退出
        if ((*case_list[i]).addr_2x2 == 0xD) { // 2 * 2块在出口位置
            if (!case_list[i]->Flag) { // 未被标识
                dat->solution_case.push_back(case_list[i]->Code); // 判定为解
                dat->solution_step.push_back(case_list[i]->Layer_num);
                case_list[i]->Flag = true; // 进行标识
            }
        }
        for (k = 0; k < case_list[i]->Next.size(); k++) { // 检测目标布局的全部子布局
            if ((*case_list[i]->Next[k]).Layer_num == -1) { // 若之前还未被搜索到
                (*case_list[i]->Next[k]).Layer_num = case_list[i]->Layer_num + 1; // 记录层级信息
                case_list.push_back(case_list[i]->Next[k]); // 加入搜索队列
            }
            if (case_list[i]->Flag) { // 若已经标识 则感染下一层的子布局
                if ((*case_list[i]->Next[k]).Layer_num == case_list[i]->Layer_num + 1) { // 若为下一层
                    (*case_list[i]->Next[k]).Flag = true; // 标识
                }
            }
        }
        i++; // 搜索下一个布局
    }
    // 计算最远布局
    dat->farthest_step = case_list[case_list.size() - 1]->Layer_num; // 得到最远步数
    for (int i = case_list.size() - 1; i >= 0; i--) { // 逆向搜索
        if (case_list[i]->Layer_num == dat->farthest_step) { // 如果是最远布局
            dat->farthest_case.push_back(case_list[i]->Code); // 加入记录
        } else {
            break; // 退出搜索
        }
    }
    dat->farthest_num = dat->farthest_case.size(); // 得到最远布局的个数
    sort(dat->farthest_case.begin(), dat->farthest_case.end());
    // 计算解的情况
    dat->solution_num = dat->solution_case.size(); // 得到解的个数
    int step = -1;
    vector < vector <unsigned long long> > temp; // 暂存不同步数解的信息
    for (i = 0; i < dat->solution_step.size(); i++) { // 遍历全部解
        if (step != int(dat->solution_step[i])) { // 发现步数不同
            temp.resize(temp.size() + 1); // temp扩容
            step = dat->solution_step[i]; // 重置步数
        }
        temp[temp.size() - 1].push_back(dat->solution_case[i]); // 数据加入到temp中
    }
    dat->solution_case.clear(); // 清空原数据
    for (i = 0; i < temp.size(); i++) { // 将排序后的数据重新写入
        sort(temp[i].begin(), temp[i].end()); // 排序同一步数的不同解
        if (i == 0) { // 若为最少步数
            dat->min_solution_case = temp[0]; // 记录最少步解的布局
            dat->min_solution_num = temp[0].size(); // 记录最少步解的个数
            dat->min_solution_step = (*dat).solution_step[0]; // 记录最少步数
        }
        for (k = 0; k < temp[i].size(); k++) {
            dat->solution_case.push_back(temp[i][k]); // 写入数据
        }
    }
    return dat;
}

void HRD_group::Case_detail_init(Case_detail &dat) { // 初始化数据
    dat.farthest_step = -1;
    dat.farthest_num = 0;
    dat.farthest_case.clear();
    dat.min_solution_step = -1;
    dat.min_solution_num = 0;
    dat.min_solution_case.clear();
    dat.solution_num = 0;
    dat.solution_case.clear();
    dat.solution_step.clear();
}

vector <unsigned long long> HRD_group::Find_Next_Case(unsigned long long Code) { // 找到下一步移动的情况(一步可以为同一块多次移动)
    int num, x, y, i, j;
    bool addr[4][5]; // 在Find_Sub_Case深搜中用于剪枝
    bool exclude[4][5]; // 排除已搜索过的块
    Case_cal dat;
    Parse_Code(dat, Code);
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 4; x++) {
            if (dat.status[x][y] == 0xFE) { // 目标格为空
                exclude[x][y] = true;
            } else {
                exclude[x][y] = false;
            }
        }
    }
    Next_Case_dat.clear(); // 清空序列
    for (y = 0; y < 5; y++) { // 遍历整个棋盘
        for (x = 0; x < 4; x++) {
            if (exclude[x][y]) {continue;}
            for (i = 0; i < 4; i++) { // 初始化
                for (j = 0; j < 5; j++) {
                    addr[i][j] = false;
                }
            }
            addr[x][y] = true; // 加入当前块 防止重复查询
            num = dat.status[x][y]; // 统一修改(x, y)块
            dat.status[x][y] = 0xFE;
            switch (dat.type[num]) {
                case 0: // 2 * 2
                    exclude[x][y + 1] = exclude[x + 1][y] = exclude[x + 1][y + 1] = true;
                    dat.status[x + 1][y] = dat.status[x][y + 1] = dat.status[x + 1][y + 1] = 0xFE;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x + 1][y] = dat.status[x][y + 1] = dat.status[x + 1][y + 1] = num;
                    break;
                case 1: // 2 * 1
                    exclude[x + 1][y] = true;
                    dat.status[x + 1][y] = 0xFE;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x + 1][y] = num;
                    break;
                case 2: // 1 * 2
                    exclude[x][y + 1] = true;
                    dat.status[x][y + 1] = 0xFE;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x][y + 1] = num;
                    break;
                case 3: // 1 * 1
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    break;
            }
            dat.status[x][y] = num; // 复原统一修改的块
        }
    }
    return Next_Case_dat;
}

void HRD_group::Find_Sub_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]) { // 找到下一个单格移动的情况
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

void HRD_group::Build_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]) { // 实现移动并将结果发送到Add_Case
    if (addr[x][y]) { // 重复
        return; // 退出
    } else {
        addr[x][y] = true; // 加入位置数据
    }
    Case_cal dat_mod = dat; // 新建对象 在Add_Case中加入层中或被释放
    switch (dat_mod.type[num]) { // 注入移动后的信息
        case 0: // 2 * 2
            dat_mod.status[x][y] = dat_mod.status[x][y + 1]
                = dat_mod.status[x + 1][y] = dat_mod.status[x + 1][y + 1] = num;
            break;
        case 1: // 2 * 1
            dat_mod.status[x][y] = dat_mod.status[x + 1][y] = num;
            break;
        case 2: // 1 * 2
            dat_mod.status[x][y] = dat_mod.status[x][y + 1] = num;
            break;
        case 3: // 1 * 1
            dat_mod.status[x][y] = num;
            break;
    }
    Get_Code(dat_mod); // 更新移动后的编码
    Next_Case_dat.push_back(dat_mod.code); // 加入序列
    Find_Sub_Case(dat, num, x, y, addr); // 递归搜索
}

void HRD_group::Get_Code(Case_cal &dat) { // 获取编码并存储在dat.code 输入数据必须无误
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
            if (temp[x][y]) {continue;} // 该格已被占用
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

bool HRD_group::Parse_Code(Case_cal &dat, unsigned long long Code) { // 解析编码 返回false表示编码有误
    unsigned char range[16]; // dat低32位分16组
    int i, x, y, num, space_num = 0;
    dat.code = Code;
    for (x = 0; x < 4; x++) { // 初始化status和freeze
        for (y = 0; y < 5; y++) {
            dat.status[x][y] = 0xFF;
        }
    }
    for (i = 0; i < 15; i++) { // 初始化type
        dat.type[i] = 0xFF;
    }
    num = 0;
    for (i = 15; i >= 0; i--) { // 载入排列到range
        range[i] = Code & 0x3;
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

string HRD_group::Change_str(unsigned long long dat) { // 将数字转化为文本编码
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

unsigned long long HRD_group::Change_int(char *str) { // 将文本编码转化为数字(传入9位字符串)
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
