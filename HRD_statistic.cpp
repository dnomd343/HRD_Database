#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include "HRD_cal.h"
#include "HRD_analy.h"
#include "HRD_statistic.h"

void HRD_statistic::Find_All_Case(string File_name) {
    Find_All_Case();
    Output_All_Case(File_name);
}

void HRD_statistic::Make_main_table(string File_name) {
    Find_All_Case();
    Sort_All_Case();
    Output_main_table(File_name);
}

void HRD_statistic::Sort_All_Case() {
    HRD_analy analy;
    unsigned int i, num;
    unsigned char jiang_num, bing_num, style_num;
    for (i = 0; i < All_Case.size(); i++) {
        analy.Parse_Code((*All_Case[i]).code);
        jiang_num = bing_num = style_num = 0;
        for (num = 0; num < 15; num++) {
            switch (analy.Parse_dat.type[num]) {
                case 1:
                    jiang_num++;
                    style_num++;
                    break;
                case 2:
                    jiang_num++;
                    break;
                case 3:
                    bing_num++;
                    break;
                default:
                    continue;
            }
        }
        (*All_Case[i]).jiang_num = jiang_num;
        (*All_Case[i]).bing_num = bing_num;
        (*All_Case[i]).style_num = style_num;
        if (i % 1000000 == 0) {
            cout << i << "/" << All_Case.size() << endl;
        }
    }
    vector <Case_group *> case_res;
    vector <unsigned int> case_index;
    vector <unsigned long long> case_code;
    for (jiang_num = 0; jiang_num <= 7; jiang_num++) {
        for (bing_num = 0; bing_num <= (14 - jiang_num * 2); bing_num++) {
            for (style_num = 0; style_num <= jiang_num; style_num++){
                cout << int(jiang_num) << "-" << int(bing_num) << "-" << int(style_num);
                case_index.clear();
                case_code.clear();
                for (i = 0; i < All_Case.size(); i++) {
                    if ((*All_Case[i]).jiang_num == jiang_num && (*All_Case[i]).bing_num == bing_num && (*All_Case[i]).style_num == style_num) {
                        case_index.push_back(i);
                    }
                }
                for (i = 0; i < case_index.size(); i++) {
                    case_code.push_back((*All_Case[case_index[i]]).code);
                }
                cout << " (" << case_code.size() << ") ...";
                case_res = Split_Group(case_code);
                for (i = 0; i < case_res.size(); i++) {
                    (*All_Case[case_index[i]]).group_num = (*case_res[i]).group_num;
                    (*All_Case[case_index[i]]).group_index = (*case_res[i]).group_index;
                    delete case_res[i];
                }
                cout << "OK" << endl;
            }
        }
    }
}

vector <HRD_statistic::Case_group *> HRD_statistic::Split_Group(vector <unsigned long long> input_dat) {
    unsigned int i, j;
    HRD_cal cal;
    list <Case_group *> case_list;
    list <Case_group *> case_list_bak;
    vector <Case_group *> output_dat;
    vector <unsigned long long> dat;
    list <Case_group *>::iterator it;
    vector < vector <Case_group *> > groups;
    for (i = 0; i < input_dat.size(); i++) {
        Case_group *temp = new Case_group;
        (*temp).id = i;
        (*temp).code = input_dat[i];
        case_list.push_back(temp);
        case_list_bak.push_back(temp);
    }
    while (case_list.size() != 0) {
        it = case_list.begin();
        dat = cal.Calculate_All((*(*it)).code);
        sort(dat.begin(), dat.end());
        groups.resize(groups.size() + 1);
        for (i = 0; i < dat.size(); i++) {
            while (it != case_list.end()) {
                if (dat[i] == (*(*it)).code) {
                    groups[groups.size() - 1].push_back(*it);
                    case_list.erase(it++);
                    break;
                }
                it++;
            }
        }
    }
    if (groups.size() == 0) {return output_dat;}
    for (i = 0; i < groups.size() - 1; i++) {
		for (j = 0; j < groups.size() - i - 1; j++) {
			if (groups[j].size() < groups[j + 1].size()) {
				swap(groups[j], groups[j + 1]);
			}
		}
	}
    output_dat.resize(input_dat.size());
    for (i = 0; i < groups.size(); i++) {
        for (j = 0; j < groups[i].size(); j++) {
            (*groups[i][j]).group_num = i;
            (*groups[i][j]).group_index = j;
            output_dat[(*groups[i][j]).id] = groups[i][j];
        }
    }
    return output_dat;
}

void HRD_statistic::Output_All_Case(string File_name) {
    unsigned int i;
    HRD_cal cal;
    ofstream output;
    output.open(File_name);
    for (i = 0; i < All_Case.size(); i++) {
        output << cal.Change_str((*All_Case[i]).code);
        if (i != All_Case.size() - 1) {
            output << endl;
        }
        if (i % 100000 == 0) {
            cout << i << "/" << All_Case.size() << endl;
        }
    }
    output.close();
}

void HRD_statistic::Output_main_table(string File_name) {
    unsigned int i;
    HRD_cal cal;
    ofstream output;
    output.open(File_name);
    for (i = 0; i < All_Case.size(); i++) {
        output << (*All_Case[i]).id << ",";
        output << cal.Change_str((*All_Case[i]).code) << ",";
        output << int((*All_Case[i]).jiang_num) << ",";
        output << int((*All_Case[i]).bing_num) << ",";
        output << int((*All_Case[i]).style_num) << ",";
        output << (*All_Case[i]).group_num << ",";
        output << (*All_Case[i]).group_index;
        if (i != All_Case.size() - 1) {
            output << endl;
        }
        if (i % 100000 == 0) {
            cout << i << "/" << All_Case.size() << endl;
        }
    }
    output.close();
}
void HRD_statistic::Find_All_Case() {
	unsigned long long i, n, Code;
	unsigned int num = 0;
	HRD_cal cal;
	All_Case.clear();
    for (n = 0; n < 0xF; n++) {
        if (n % 4 == 3) {continue;}
        for (i = 0; i <= 0xFFFFFFFF; i++) {
            Code = (n << 32) | i;
            if (cal.Check_Code(Code) == true) {
                Case *temp = new Case;
                (*temp).code = Code;
                (*temp).id = num;
                All_Case.push_back(temp);
                num++;
            }
            if (i % 0x1000000 == 0) {
                cout << n << ": " << (i / 0x1000000) + 1 << "/256";
                cout << " -> " << num << endl;
            }
        }
    }
    cout << "Total: " << num << endl;
}

