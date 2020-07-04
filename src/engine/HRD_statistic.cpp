#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include "HRD_cal.h"
#include "HRD_analy.h"
#include "HRD_statistic.h"

void HRD_statistic::All_Statistic() {
    Find_All_Case();
    Sort_All_Case();
    Output_All_Case("All_Case.txt");
    Output_main_table("main.csv");
    Get_seed();
}

void HRD_statistic::Find_All_Case(string File_name) { // 找到所有编码并输出到文件
    Find_All_Case();
    Output_All_Case(File_name);
}

void HRD_statistic::Get_seed() {
    HRD_cal cal;
    unsigned int i, j;
    string File_name;
    ofstream output_seed;
    vector <unsigned int> case_index;
    unsigned char jiang_num, bing_num, style_num;
    for (jiang_num = 0; jiang_num <= 7; jiang_num++) { // 遍历全部jiang_num-bing_num-style_num组
        for (bing_num = 0; bing_num <= (14 - jiang_num * 2); bing_num++) {
            for (style_num = 0; style_num <= jiang_num; style_num++){
                cout << int(jiang_num) << "-" << int(bing_num) << "-" << int(style_num) << " -> ";
                case_index.clear();
                for (i = 0; i < All_Case.size(); i++) { // 在全部布局中找到符合当前要求的布局
                    if ((*All_Case[i]).jiang_num == jiang_num && (*All_Case[i]).bing_num == bing_num && (*All_Case[i]).style_num == style_num && (*All_Case[i]).group_index == 0) {
                        case_index.push_back(i);
                    }
                }
                cout << case_index.size() << endl;
                if (case_index.size() == 0) {continue;}
                for (i = 0; i < case_index.size() - 1; i++) {
                    for (j = 0; j < case_index.size() - i - 1; j++) {
                        if ((*All_Case[case_index[j]]).group_num > (*All_Case[case_index[j + 1]]).group_num) {
                            swap(case_index[j], case_index[j + 1]);
                        }
                    }
                }
                File_name = to_string(jiang_num) + "-" + to_string(bing_num) + "-" + to_string(style_num) + ".txt";
                output_seed.open(File_name);
                for (i = 0; i < case_index.size(); i++) { // 录入找到布局的编码
                    output_seed << cal.Change_str((*All_Case[case_index[i]]).code);
                    if (i != case_index.size() - 1) { // 如果不是最后一组就输出回车
                        output_seed << endl;
                    }
                }
                output_seed.close();
            }
        }
    }
}

void HRD_statistic::Sort_All_Case() { // 整理所有布局
    HRD_analy analy;
    unsigned int i, num;
    unsigned char jiang_num, bing_num, style_num;
    for (i = 0; i < All_Case.size(); i++) { // 遍历全部布局
        analy.Parse_Code((*All_Case[i]).code); // 解析布局
        jiang_num = bing_num = style_num = 0;
        for (num = 1; num < 15; num++) { // 遍历所有棋子
            switch (analy.Parse_dat.type[num]) {
                case 1: // 2 * 1
                    jiang_num++;
                    style_num++;
                    break;
                case 2: // 1 * 2
                    jiang_num++;
                    break;
                case 3: // 1 * 2
                    bing_num++;
                    break;
                default: // space
                    continue;
            }
        }
        (*All_Case[i]).jiang_num = jiang_num;
        (*All_Case[i]).bing_num = bing_num;
        (*All_Case[i]).style_num = style_num;
        if (i % 1000000 == 0) { // 显示计算进度
            cout << i << "/" << All_Case.size() << endl;
        }
    }
    vector <Case_group *> case_res;
    vector <unsigned int> case_index;
    vector <unsigned long long> case_code;
    for (jiang_num = 0; jiang_num <= 7; jiang_num++) { // 遍历全部jiang_num-bing_num-style_num组
        for (bing_num = 0; bing_num <= (14 - jiang_num * 2); bing_num++) {
            for (style_num = 0; style_num <= jiang_num; style_num++){
                cout << int(jiang_num) << "-" << int(bing_num) << "-" << int(style_num);
                case_index.clear();
                case_code.clear();
                for (i = 0; i < All_Case.size(); i++) { // 在全部布局中找到符合当前要求的布局
                    if ((*All_Case[i]).jiang_num == jiang_num && (*All_Case[i]).bing_num == bing_num && (*All_Case[i]).style_num == style_num) {
                        case_index.push_back(i); // 记录找到布局的索引
                    }
                }
                for (i = 0; i < case_index.size(); i++) { // 录入找到布局的编码
                    case_code.push_back((*All_Case[case_index[i]]).code);
                }
                cout << " (" << case_code.size() << ") ...";
                case_res = Split_Group(case_code); // 进行群组分割
                for (i = 0; i < case_res.size(); i++) { // 遍历分割结果
                    (*All_Case[case_index[i]]).group_num = (*case_res[i]).group_num; // 记录群组分类结果
                    (*All_Case[case_index[i]]).group_index = (*case_res[i]).group_index;
                    delete case_res[i]; // 释放内存
                }
                cout << "OK" << endl;
            }
        }
    }
}

vector <HRD_statistic::Case_group *> HRD_statistic::Split_Group(vector <unsigned long long> input_dat) { // 将输入的数据进行群组分割 输入的数据必须是一个或多个群的集合 输入的数据必须从大到小排列
    unsigned int i, j;
    HRD_cal cal;
    list <Case_group *> case_list; // 计算中间序列
    vector <Case_group *> output_dat; // 储存计算结果
    vector <unsigned long long> dat;
    list <Case_group *>::iterator it; // 声明迭代器
    vector < vector <Case_group *> > groups; // 记录群组数组
    for (i = 0; i < input_dat.size(); i++) { // 记录输入的编码
        Case_group *temp = new Case_group;
        (*temp).id = i;
        (*temp).code = input_dat[i];
        case_list.push_back(temp);
    }
    while (case_list.size() != 0) { // 循环分割群组
        it = case_list.begin(); // 定位当前序列起始点
        dat = cal.Calculate_All((*(*it)).code); // 计算起始点编码所在的整个群
        sort(dat.begin(), dat.end()); // 整理顺序
        groups.resize(groups.size() + 1); // groups增加一组
        for (i = 0; i < dat.size(); i++) { // 遍历搜索到的整个群
            while (it != case_list.end()) { // 直到序列的结尾
                if (dat[i] == (*(*it)).code) { // 在序列中找到
                    groups[groups.size() - 1].push_back(*it); // 记录找到的编码
                    case_list.erase(it++); // 删除序列中当前的编码 并指向序列中下一个编码
                    break; // 退出循环
                }
                it++; // 指向序列中下一个编码
            }
        }
    }
    if (groups.size() == 0) {return output_dat;} // 如果找到0个群组则退出
    for (i = 0; i < groups.size() - 1; i++) { // 根据群的大小进行冒泡排序
        for (j = 0; j < groups.size() - i - 1; j++) {
            if (groups[j].size() < groups[j + 1].size()) {
                swap(groups[j], groups[j + 1]);
            }
        }
    }
    output_dat.resize(input_dat.size()); // 声明返回数组的长度
    for (i = 0; i < groups.size(); i++) { // 遍历所有群
        for (j = 0; j < groups[i].size(); j++) { // 遍历群中的所有元素
            (*groups[i][j]).group_num = i;
            (*groups[i][j]).group_index = j;
            output_dat[(*groups[i][j]).id] = groups[i][j]; // 记录元素到返回数组
        }
    }
    return output_dat;
}

void HRD_statistic::Output_All_Case(string File_name) { // 输出全部编码
    unsigned int i;
    HRD_cal cal;
    ofstream output;
    output.open(File_name);
    for (i = 0; i < All_Case.size(); i++) { // 遍历全部布局
        output << cal.Change_str((*All_Case[i]).code);
        if (i != All_Case.size() - 1) { // 如果不是最后一组就输出回车
            output << endl;
        }
        if (i % 100000 == 0) { // 显示输出进度
            cout << i << "/" << All_Case.size() << endl;
        }
    }
    output.close();
}

void HRD_statistic::Output_main_table(string File_name) { // 输出全部编码及分类
    unsigned int i;
    HRD_cal cal;
    ofstream output;
    output.open(File_name);
    for (i = 0; i < All_Case.size(); i++) { // 遍历全部布局
        output << (*All_Case[i]).id << ",";
        output << cal.Change_str((*All_Case[i]).code) << ",";
        output << int((*All_Case[i]).jiang_num) << ",";
        output << int((*All_Case[i]).bing_num) << ",";
        output << int((*All_Case[i]).style_num) << ",";
        output << (*All_Case[i]).group_num << ",";
        output << (*All_Case[i]).group_index;
        if (i != All_Case.size() - 1) { // 如果不是最后一组就输出回车
            output << endl;
        }
        if (i % 100000 == 0) { // 显示输出进度
            cout << i << "/" << All_Case.size() << endl;
        }
    }
    output.close();
}
void HRD_statistic::Find_All_Case() { // 找到所有编码
    unsigned long long i, n, Code;
    unsigned int num = 0;
    HRD_cal cal;
    All_Case.clear();
    for (n = 0; n < 0xF; n++) { // n为2 * 2块位置 亦编码左起第1位
        if (n % 4 == 3) {continue;} // 排除2 * 2块不可能在的位置
        for (i = 0; i <= 0xFFFFFFFF; i++) { // 遍历编码低32位
            Code = (n << 32) | i; // 生成Code
            if (cal.Check_Code(Code) == true) { // 检查编码是否正确
                Case *temp = new Case;
                (*temp).code = Code;
                (*temp).id = num;
                All_Case.push_back(temp); // 记录搜到的编码
                num++;
            }
            if (i % 0x1000000 == 0) { // 输出搜索进度
                cout << n << ": " << (i / 0x1000000) + 1 << "/256";
                cout << " -> " << num << endl;
            }
        }
    }
    cout << "Total: " << num << endl;
}
