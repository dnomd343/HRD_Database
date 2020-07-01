#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include "HRD_cal.h"
#include "HRD_analy.h"
using namespace std;

ifstream File_Input;
ofstream File_Output;

struct Case {
    unsigned int id;
    unsigned long long code;
    unsigned char jiang_num;
    unsigned char bing_num;
    unsigned char style_num;
    unsigned int group_num;
    unsigned int group_index;
};
struct Case_group {
    unsigned int id;
    unsigned long long code;
    unsigned int group_num;
    unsigned int group_index;
};
vector <Case *> All_Case;

void Find_All_Case();
void Output();
void Sort_All_Case();
vector <Case_group *> Split_Group(vector <unsigned long long> dat);

void Input() {
    unsigned int num = 0;
	char str[10];
    HRD_analy analy;

	File_Input.open("All_Case.txt");
    while (File_Input.eof() != true) {
        File_Input >> str;
        Case *temp = new Case;
        (*temp).code = analy.Change_int(str);
        (*temp).id = num;
        All_Case.push_back(temp);
        num++;
    }
	File_Input.close();
}

int main() {
	cout << "Klotski statistician by Dnomd343" << endl;

	Find_All_Case();
    //Input();
    //cout << "Input OK" << endl;

    Sort_All_Case();

    cout << "Output..." << endl;
    Output();
    cout << "bye" << endl;
	return 0;
}

void Sort_All_Case() {
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
    cout << "split group..." << endl;

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

vector <Case_group *> Split_Group(vector <unsigned long long> input_dat) {
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

void Output() {
    unsigned int i;
    HRD_cal cal;
    File_Output.open("All_Case.txt");
    for (i = 0; i < All_Case.size(); i++) {
        File_Output << cal.Change_str((*All_Case[i]).code);
        if (i != All_Case.size() - 1) {
            File_Output << endl;
        }
    }
    File_Output.close();
    File_Output.open("main.csv");
    for (i = 0; i < All_Case.size(); i++) {
        File_Output << (*All_Case[i]).id << ",";
        File_Output << cal.Change_str((*All_Case[i]).code) << ",";
        File_Output << int((*All_Case[i]).jiang_num) << ",";
        File_Output << int((*All_Case[i]).bing_num) << ",";
        File_Output << int((*All_Case[i]).style_num) << ",";
        File_Output << (*All_Case[i]).group_num << ",";
        File_Output << (*All_Case[i]).group_index;
        if (i != All_Case.size() - 1) {
            File_Output << endl;
        }
        if (i % 100000 == 0) {
            cout << i << "/" << All_Case.size() << endl;
        }
    }
    File_Output.close();
}
void Find_All_Case() {
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

