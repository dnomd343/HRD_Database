#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "HRD_analy.h"

void HRD_analy::Output_Detail(string File_name) { // 输出分析结果到文件
    unsigned int i, j, k;
    ofstream output;
    vector <Case_cal *> case_list;
    if (quiet == false) {
        cout << "Output into: " << File_name << "...";
    }
    output.open(File_name);
    output << "[Min_solution_step]" << endl;
    output << min_solution_step << endl;
    output << "[Min_solution_case]" << endl;
    output << "num: " << min_solution_num << endl;
    for (i = 0; i < min_solution_case.size(); i++) {
        output << Change_str(min_solution_case[i]) << endl;
    }
    output << "[Farthest_step]" << endl;
    output << farthest_step << endl;
    output << "[Farthest_case]" << endl;
    output << "num: " << farthest_num << endl;
    for (i = 0; i < farthest_case.size(); i++) {
        output << Change_str(farthest_case[i]) << endl;
    }
    output << "[Solution]" << endl;
    output << "num: " << solution_num << endl;
    for (i = 0; i < solution_case.size(); i++) {
        output << Change_str(solution_case[i]) << "(" << solution_step[i] << ")" << endl;
    }
    output << "[Layer_Size]" << endl;
    for (i = 0; i < Layer.size(); i++) {
        output << i << " -> " << Layer[i].size() << endl;
    }
    output << "[Layer]" << endl;
    for (i = 0; i < Layer.size(); i++) {
        for (j = 0; j < Layer[i].size(); j++) {
            output << "(" << i << "," << j << ") -> ";
            output << Change_str((*Layer[i][j]).code) << endl;
        }
    }
    output << "[Next]" << endl;
    for (i = 0; i < Layer.size(); i++) {
        for (j = 0; j < Layer[i].size(); j++) {
            case_list = (*(*Layer[i][j]).adjacent).next_case;
            output << "(" << i << "," << j << ") ->";
            for (k = 0; k < case_list.size(); k++) {
                output << " (" << (*case_list[k]).layer_num;
                output << "," << (*case_list[k]).layer_index << ")";
            }
            output << endl;
        }
    }
    output << "[Source]" << endl;
    for (i = 0; i < Layer.size(); i++) {
        for (j = 0; j < Layer[i].size(); j++) {
            case_list = (*(*Layer[i][j]).adjacent).source_case;
            output << "(" << i << "," << j << ") <-";
            for (k = 0; k < case_list.size(); k++) {
                output << " (" << (*case_list[k]).layer_num;
                output << "," << (*case_list[k]).layer_index << ")";
            }
            output << endl;
        }
    }
    output.close();
    if (quiet == false) {
        cout << "done" << endl;
    }
}

void HRD_analy::Analyse_Case(unsigned long long code) { // 分析输入编码的各种参数
    vector < vector <bool> > solution_flag;
    vector <unsigned long long> temp;
    unsigned int i, j, k;
    farthest_step = -1; // 初始化farthest
    farthest_num = 0;
	farthest_case.clear();
    min_solution_step = -1; // 初始化min_solution
    min_solution_num = 0;
    min_solution_case.clear();
	solution_num = 0; // 初始化solution
	solution_case.clear();
    solution_step.clear();
    if (Check_Code(code) == false) {return;} // 若编码错误则退出
    Calculate(code); // 计算分层数据
    solution_flag.resize(Layer.size()); // 同步Layer的结构
    for (i = 0; i < solution_flag.size(); i++) {
        solution_flag[i].resize(Layer[i].size());
    }
    // 获取最远端情况
    farthest_step = Layer.size() - 1; // 计算最远布局的步数
    for (i = 0; i < Layer[farthest_step].size(); i++) { // 找到所有最远的布局
        farthest_case.push_back((*Layer[farthest_step][i]).code);
    }
    farthest_num = farthest_case.size();
	sort(farthest_case.begin(), farthest_case.end());  //得到的结果进行排序
    // 获取最少步解
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
	sort(min_solution_case.begin(), min_solution_case.end()); // 得到的结果进行排序
    // 获取全部解
    vector <Case_cal *> case_list;
    solution_case = min_solution_case; // 同步最少步解到所有解序列中
    for (i = 0; i < solution_case.size(); i++) { // 初始化已知部分的solution_step
        solution_step.push_back(min_solution_step);
    }
    for (i = 0; i < Layer.size() - 1; i++) { // 遍历除最后一层外的所有层
        for (j = 0; j < Layer[i].size(); j++) { // 遍历层内元素
            if (solution_flag[i][j] == true) { // 若该元素被标识
                case_list = (*(*Layer[i][j]).adjacent).next_case;
                for (k = 0; k < case_list.size(); k++) { // 遍历其下一步
                    solution_flag[i + 1][(*case_list[k]).layer_index] = true; // 标识
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
		sort(temp.begin(), temp.end()); // 将得到的结果进行排序
		for (k = 0; k < temp.size(); k++) { // 将temp内容加入solution_case中
			solution_case.push_back(temp[k]);
		}
    }
    solution_num = solution_case.size();
    if (quiet == true) {return;} // 若quiet为true则不输出
    cout << "---------------------------" << endl;
    cout << "farthest_step = " << farthest_step << endl;
    cout << "farthest_num = " << farthest_num << endl;
    cout << "farthest_case -> " << endl;
    for (i = 0; i < farthest_case.size(); i++) {
        cout << "  " << Change_str(farthest_case[i]) << endl;
    }
    cout << "---------------------------" << endl;
    cout << "min_solution_step = " << min_solution_step << endl;
    cout << "min_solution_num = " << min_solution_num << endl;
    cout << "min_solution_case -> " << endl;
    for (i = 0; i < min_solution_case.size(); i++) {
        cout << "  " << Change_str(min_solution_case[i]) << endl;
    }
    cout << "---------------------------" << endl;
    cout << "solution_num = " << solution_num << endl;
    cout << "solution_case(solution_step) -> " << endl;
    for (i = 0; i < solution_case.size(); i++) {
        cout << "  " << Change_str(solution_case[i]) << "(" << solution_step[i] << ")" << endl;
    }
    cout << "---------------------------" << endl;
}

void HRD_analy::Free_Data() { // 释放上一次的计算结果
    unsigned int i, j;
    for (i = 0; i < Layer.size(); i++) { // 释放Layer中指向的全部节点
        for (j = 0; j < Layer[i].size(); j++) {
            delete (*Layer[i][j]).adjacent;
            delete Layer[i][j];
        }
    }
    Layer.clear(); // 清空层数据
    for (i = 0; i < 0x10000; i++) { // 清空哈希表
        Layer_hash[i].clear();
    }
}

void HRD_analy::Calculate(unsigned long long code) {
    Free_Data(); // 初始化数据结构
    Case_cal *start = new Case_cal;
    (*start).adjacent = new Case_near;
    if (Parse_Code(*start, code) == false) { // 若输入编码错误 退出
        delete (*start).adjacent;
        delete start;
        return;
    }
    Layer.resize(1); // 创建第0层
    Layer[0].push_back(start); // 加入根节点
    (*start).layer_num = (*start).layer_index = 0; // 初始化根节点编号
    Layer_hash[0xffff & ((*start).code >> 16)].push_back(start); // 根节点加入哈希表
    now_move_num = now_move_index = 0; // 从根节点开始运算
    while (1 == 1) { // 创建死循环
        if (now_move_index == 0) { // 若在计算层的第一个元素
            Layer.resize(Layer.size() + 1); // 则新增一层
        }
        now_move_case = Layer[now_move_num][now_move_index]; // 记录当前正在查找的节点
        Find_Next_Case(*now_move_case); // 寻找节点的子布局
        if (now_move_index == Layer[now_move_num].size() - 1) { // 若在层的最后一个元素
            if (Layer[now_move_num + 1].size() == 0) { // 若下一层是空的
                break; // 已全部搜索完毕 退出搜索循环
            }
            now_move_num++; // 计算目标移到下一层第一个元素
            now_move_index = 0;
            if (quiet == false) {
                cout << now_move_num << " -> " << Layer[now_move_num].size() << endl;
            }
        } else { // 不是最后一个元素
            now_move_index++; // 计算目标移到下一元素
        }
    }
    Layer.pop_back(); // 移除最后的空层
}

void HRD_analy::Add_Case(Case_cal *dat) { // 将计算得到的节点加入层级中
    unsigned int i, x, y;
    Case_cal *repeat_case;
    int hash_index = 0xffff & ((*dat).code >> 16); // 取得哈希索引
    for (i = 0; i < Layer_hash[hash_index].size(); i++) { // 遍历索引内容
        repeat_case = Layer_hash[hash_index][i];
        if ((*repeat_case).code == (*dat).code) { // 发现重复
            if ((*repeat_case).layer_num == now_move_num + 1) { // 若发现的目标比现在多一层
                (*(*repeat_case).adjacent).source_case.push_back(now_move_case); // 记录父节点信息
                (*(*now_move_case).adjacent).next_case.push_back(repeat_case); // 记录子节点信息
                for (x = 0; x < 4; x++) { // 遍历freeze表
                    for (y = 0; y < 5; y++) {
                        if ((*dat).freeze[x][y] == true) { // 将输入表合并到原先的表上
                            (*repeat_case).freeze[x][y] = true;
                        }
                    }
                }
            }
            delete dat; // 销毁节点
            return; // 退出
        }
    }
    (*dat).adjacent = new Case_near; // 初始化节点的相邻布局结构
    (*(*dat).adjacent).source_case.push_back(now_move_case); // 记录父节点信息
    (*(*now_move_case).adjacent).next_case.push_back(dat); // 记录子节点信息
    (*dat).layer_num = now_move_num + 1; // 记录节点的层编号
    (*dat).layer_index = Layer[now_move_num + 1].size(); // 记录节点在层中的编号
    Layer[now_move_num + 1].push_back(dat); // 加入层级结构中
    Layer_hash[hash_index].push_back(dat); // 加入哈希索引
}

void HRD_analy::Find_Next_Case(Case_cal &dat_raw) { // 找到下一步移动的情况(一步可以为同一块多次移动) 结果聚集到Add_Case中
    int num, x, y, i, j;
    bool addr[4][5]; // 在Find_Sub_Case深搜中用于剪枝
    Case_cal dat = dat_raw;
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

void HRD_analy::Find_Sub_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]) { // 找到下一个单格移动的情况
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

void HRD_analy::Build_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]) { // 实现移动并将结果发送到Add_Case
    if (addr[x][y] == true) { // 重复
        return; // 退出
    } else {
        addr[x][y] = true; // 加入位置数据
    }
    Case_cal *dat_mod = new Case_cal; // 新建对象 在Add_Case中加入层中或被释放
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

bool HRD_analy::Check_Code(unsigned long long code) {
    Case_cal dat;
    return Parse_Code(dat, code);
}

void HRD_analy::Get_Code(Case_cal &dat) { // 获取编码并存储在dat.code 输入数据必须无误
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

bool HRD_analy::Parse_Code(unsigned long long Code) {
    Parse_dat.layer_num = Parse_dat.layer_index = 0;
    return Parse_Code(Parse_dat, Code);
}

bool HRD_analy::Parse_Code(Case_cal &dat, unsigned long long Code) { // 解析编码 返回false表示编码有误
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

string HRD_analy::Change_str(unsigned long long dat) { // 将数字转化为文本编码
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

unsigned long long HRD_analy::Change_int (char *str) { // 将文本编码转化为数字(传入9位字符串)
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
