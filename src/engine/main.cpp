#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include "HRD_cal.h"
#include "HRD_analy.h"
#include "HRD_group.h"
#include "HRD_statistic.h"
using namespace std;

ofstream File_Output;
unsigned long long checked_code;

bool code_check(string str) {
    HRD_cal cal;
    char code_str[10];
    if (str.length() > 9) { // 超过九位 错误
        return false;
    } else {
        str.resize(9, '0'); // 剩余的补0
    }
    for (unsigned int i = 0; i < str.length(); i++) {
        code_str[i] = str[i];
    }
    code_str[9] = '\0';
    checked_code = cal.Change_int(code_str);
    return cal.Check_Code(checked_code);
}

void show_case(string str) {
    unsigned long long code;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_analy analy;
    char output_char[3] = "&%";
    cout << endl;
    cout << "Code: " << analy.Change_str(code) << endl;
    analy.Output_Graph(code, 3, 1, output_char);
    cout << endl;
}

void show_case(string str, string width) {
    unsigned long long code;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_analy analy;
    int square_width;
    char output_char[3] = "&%";
    istringstream is(width); // 借用字符串流对象将string转为int
    is >> square_width;
    cout << endl;
    cout << "Code: " << analy.Change_str(code) << endl;
    analy.Output_Graph(code, square_width, 1, output_char);
    cout << endl;
}

void cal_case(string str) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    HRD_cal cal;
    code = checked_code;
    cout << "Start code: " << cal.Change_str(code) << endl;
    dat = cal.Calculate(code);
    if (dat.size() == 0) {
        cout << "no solution" << endl;
    } else {
        cout << "At least need " << dat.size() - 1 << " step:" << endl;
    }
    for (unsigned int i = 0; i < dat.size(); i++) {
        cout << i << ": " << cal.Change_str(dat[i]) << endl;
    }
    cout << endl;
}

void cal_case(string str, string File_name) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    HRD_cal cal;
    code = checked_code;
    cout << "Start code: " << cal.Change_str(code) << endl;
    dat = cal.Calculate(code);
    if (dat.size() == 0) {
        cout << "no solution" << endl;
    } else {
        cout << "At least need " << dat.size() - 1 << " step" << endl;
    }
    File_Output.open(File_name);
    cout << "Data save at " << File_name << endl;
    if (dat.size() == 0) {
        File_Output << "-1";
    } else {
        File_Output << dat.size() - 1 << endl;
        for (unsigned int i = 0; i < dat.size(); i++) {
            File_Output << cal.Change_str(dat[i]);
            if (i + 1 != dat.size()) {
                File_Output << endl;
            }
        }
    }
    cout << endl;
    File_Output.close();
}

void cal_target(string str_1, string str_2) {
    unsigned long long code, target;
    vector <unsigned long long> dat;
    if (code_check(str_1) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    if (code_check(str_2) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    target = checked_code;
    HRD_cal cal;
    cout << "Start code: " << cal.Change_str(code) << endl;
    cout << "Target code: " << cal.Change_str(target) << endl;
    dat = cal.Calculate(code, target);
    if (dat.size() == 0) {
        cout << "no solution" << endl;
    } else {
        cout << "At least need " << dat.size() - 1 << " step:" << endl;
    }
    for (unsigned int i = 0; i < dat.size(); i++) {
        cout << i << ": " << cal.Change_str(dat[i]) << endl;
    }
    cout << endl;
}

void cal_target(string str_1, string str_2, string File_name) {
    unsigned long long code, target;
    vector <unsigned long long> dat;
    if (code_check(str_1) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    if (code_check(str_2) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    target = checked_code;
    HRD_cal cal;
    cout << "Start code: " << cal.Change_str(code) << endl;
    cout << "Target code: " << cal.Change_str(target) << endl;
    dat = cal.Calculate(code, target);
    if (dat.size() == 0) {
        cout << "no solution" << endl;
    } else {
        cout << "At least need " << dat.size() - 1 << " step" << endl;
    }
    File_Output.open(File_name);
    cout << "Data save at " << File_name << endl;
    if (dat.size() == 0) {
        File_Output << "-1";
    } else {
        File_Output << dat.size() - 1 << endl;
        for (unsigned int i = 0; i < dat.size(); i++) {
            File_Output << cal.Change_str(dat[i]);
            if (i + 1 != dat.size()) {
                File_Output << endl;
            }
        }
    }
    cout << endl;
    File_Output.close();
}

void cal_group(string str) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    HRD_cal cal;
    code = checked_code;
    cout << "Start code: " << cal.Change_str(code) << endl;
    dat = cal.Calculate_All(code);
    cout << "Group size: " << dat.size() << endl;
    cout << endl;
}

void cal_group(string str, string File_name) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    HRD_cal cal;
    code = checked_code;
    cout << "Start code: " << cal.Change_str(code) << endl;
    dat = cal.Calculate_All(code);
    sort(dat.begin(), dat.end());
    cout << "Group size: " << dat.size() << endl;
    File_Output.open(File_name);
    cout << "Data save at " << File_name << endl;
    for (unsigned int i = 0; i < dat.size(); i++) {
        File_Output << cal.Change_str(dat[i]);
        if (i + 1 != dat.size()) {
            File_Output << endl;
        }
    }
    cout << endl;
    File_Output.close();
}

void analy_case(string str, bool quiet) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    HRD_analy analy;
    code = checked_code;
    cout << "Start code: " << analy.Change_str(code) << endl;
    analy.quiet = quiet;
    analy.Analyse_Case(code);
    cout << endl;
    analy.Free_Data();
}

void analy_case(string str, string File_name, bool quiet) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    HRD_analy analy;
    code = checked_code;
    cout << "Start code: " << analy.Change_str(code) << endl;
    analy.quiet = quiet;
    analy.Analyse_Case(code);
    if (quiet == true) {
        cout << "Output into " << File_name << "...";
    }
    analy.Output_Detail(File_name);
    if (quiet == true) {
        cout << "done" << endl;
    }
    cout << endl;
    analy.Free_Data();
}

void analy_group(string str, string File_name_1, string File_name_2, bool is_all) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    HRD_group group;
    code = checked_code;
    cout << "Start code: " << group.Change_str(code) << endl;
    group.Batch_Analyse(code, File_name_1, File_name_2, is_all);
    cout << "Data save at " << File_name_1 << " and " << File_name_2 << endl;
    cout << endl;
}

void analy_multi_group(string str, string File_name_1, string File_name_2, bool is_all) {
    HRD_group group;
    if (group.Multi_Analyse(str, File_name_1, File_name_2, is_all) == false) {
        cout << endl;
        return;
    }
    cout << "Data save at " << File_name_1 << " and " << File_name_2 << endl;
    cout << endl;
}

void find_all() {
    HRD_statistic statistic;
    cout << "Warning: It will consume nearly 2GB of memory, please make sure there is enough memory!" << endl;
    cout << "Press ENTER to start...";
    cin.get();
    statistic.All_Statistic();
    cout << "Data save at All_Case.txt, main.csv, *-*-*.txt" << endl;
    cout << endl;
}

void find_all_code(string File_name) {
    HRD_statistic statistic;
    cout << "Warning: It will consume nearly 2GB of memory, please make sure there is enough memory!" << endl;
    cout << "Press ENTER to start...";
    cin.get();
    statistic.Find_All_Case(File_name);
    cout << "Data save at " << File_name << endl;
    cout << endl;
}

void show_help() {
    cout << "(version: v1.1)" << endl;
    cout << "Usage of HRD_engine:" << endl;
    cout << endl;
    cout << "  --show <code> [square_width]" << endl;
    cout << "    Purpose: Visualize the <code>" << endl;
    cout << "    eg: ./engine --show 1A9BF0C00" << endl;
    cout << "        ./engine --show 1A9BF0C00 4" << endl;
    cout << endl;
    cout << "  --cal <code> [file_name]" << endl;
    cout << "    Purpose: Find the minimum step solution of <code>" << endl;
    cout << "    eg: ./engine --cal 1A9BF0C00" << endl;
    cout << "        ./engine --cal 1A9BF0C00 demo.txt" << endl;
    cout << endl;
    cout << "  --cal-target <code> <target> [file_name]" << endl;
    cout << "    Purpose: Find the shortest path from <code> to <target>" << endl;
    cout << "    eg: ./engine --cal-target 4FEA13400 43EA73400" << endl;
    cout << "        ./engine --cal-target 4FEA13400 43EA73400 demo.txt" << endl;
    cout << endl;
    cout << "  --group <code> [file_name]" << endl;
    cout << "    Purpose: Find all elements of the group where <code> located" << endl;
    cout << "    eg: ./engine --group 4FEA13400" << endl;
    cout << "        ./engine --group 4FEA13400 demo.txt" << endl;
    cout << endl;
    cout << "  --analy <code> [file_name]" << endl;
    cout << "    Purpose: Detailed analysis of the <code> " << endl;
    cout << "    eg: ./engine --analy 1A9BF0C00" << endl;
    cout << "        ./engine --analy 1A9BF0C00 demo.txt" << endl;
    cout << endl;
    cout << "  --analy-quiet <code> <file_name>" << endl;
    cout << "    Purpose: The same function as --analy, but doesn't show the specific process" << endl;
    cout << "    eg: ./engine --analy-quiet 1A9BF0C00 demo.txt" << endl;
    cout << endl;
    cout << "  --analy-group <code> <file_name_farthest> <file_name_solution>" << endl;
    cout << "    <file_name_farthest>: As the output file of \"farthest\"" << endl;
    cout << "    <file_name_solution>: As the output file of \"solution\"" << endl;
    cout << "    Purpose: Analyze the whole group where <code> located" << endl;
    cout << "    eg: ./engine --analy-group 1A9BF0C00 farthest.csv solution.csv" << endl;
    cout << endl;
    cout << "  --analy-group-integral <code> <file_name_farthest> <file_name_farthest>" << endl;
    cout << "    Purpose: The same function as --analy-group, but all solution case will be output" << endl;
    cout << "    eg: ./engine --analy-group-integral 1A9BF0C00 farthest.csv solution.csv" << endl;
    cout << endl;
    cout << "  --analy-multi-group <input_file_name> <file_name_farthest> <file_name_farthest>" << endl;
    cout << "    <input_file_name>: As the input file of seeds" << endl;
    cout << "    <file_name_farthest>: As the output file of \"farthest\"" << endl;
    cout << "    <file_name_farthest>: As the output file of \"solution\"" << endl;
    cout << "    Purpose: Analyze the whole group where each seed located" << endl;
    cout << "    eg: ./engine --analy-multi-group 5-4-0.txt farthest.csv solution.csv" << endl;
    cout << endl;
    cout << "  --analy-multi-group-integral <input_file_name> <file_name_farthest> <file_name_farthest>" << endl;
    cout << "    Purpose: The same function as --analy-multi-group, but all solution case will be output" << endl;
    cout << "    eg: ./engine --analy-multi-group-integral 5-4-0.txt farthest.csv solution.csv" << endl;
    cout << endl;
    cout << "  --all" << endl;
    cout << "    Purpose: Find all the cases of klotski with detail" << endl;
    cout << "    eg: ./engine --all" << endl;
    cout << endl;
    cout << "  --all-code <file_name>" << endl;
    cout << "    Purpose: Find all the code of klotski" << endl;
    cout << "    eg: ./engine --all-code All_Case.txt" << endl;
    cout << endl;
    cout << "  --help" << endl;
    cout << "    Purpose: Display instructions for use" << endl;
    cout << "    eg: ./engine --help" << endl;
    cout << endl;
    cout << "More details: https://github.com/dnomd343/HRD_Database" << endl;
    cout << endl;
}

void parameter_err() {
    cout << "parameter error" << endl;
    cout << endl;
}

int main(int argc, char* argv[]) {
    cout << endl << "Klotski engine by Dnomd343" << endl;
    if (argc <= 1) {
        cout << "no parameter" << endl;
        cout << "You can try \"--help\" for more information" << endl;
        cout << endl;
        return 0;
    }
    string parameter;
    parameter = argv[1];
    if (parameter == "--show") {
        if (argc == 3) {
            show_case(argv[2]);
        } else if (argc == 4) {
            show_case(argv[2], argv[3]);
        } else {
            parameter_err();
        }
    } else if (parameter == "--cal") {
        if (argc == 3) {
            cal_case(argv[2]);
        } else if (argc == 4) {
            cal_case(argv[2], argv[3]);
        } else {
            parameter_err();
        }
    } else if (parameter == "--cal-target") {
        if (argc == 4) {
            cal_target(argv[2], argv[3]);
        } else if (argc == 5) {
            cal_target(argv[2], argv[3], argv[4]);
        } else {
            parameter_err();
        }
    } else if (parameter == "--group") {
        if (argc == 3) {
            cal_group(argv[2]);
        } else if (argc == 4) {
            cal_group(argv[2], argv[3]);
        } else {
            parameter_err();
        }
    } else if (parameter == "--analy") {
        if (argc == 3) {
            analy_case(argv[2], false);
        } else if (argc == 4) {
            analy_case(argv[2], argv[3], false);
        } else {
            parameter_err();
        }
    } else if (parameter == "--analy-quiet") {
        if (argc == 4) {
            analy_case(argv[2], argv[3], true);
        } else {
            parameter_err();
        }
    } else if (parameter == "--analy-group") {
        if (argc == 5) {
            analy_group(argv[2], argv[3], argv[4], false);
        } else {
            parameter_err();
        }
    } else if (parameter == "--analy-group-integral") {
        if (argc == 5) {
            analy_group(argv[2], argv[3], argv[4], true);
        } else {
            parameter_err();
        }
    } else if (parameter == "--analy-multi-group") {
        if (argc == 5) {
            analy_multi_group(argv[2], argv[3], argv[4], false);
        } else {
            parameter_err();
        }
    } else if (parameter == "--analy-multi-group-integral") {
        if (argc == 5) {
            analy_multi_group(argv[2], argv[3], argv[4], true);
        } else {
            parameter_err();
        }
    } else if (parameter == "--all") {
        if (argc == 2) {
            find_all();
        } else {
            parameter_err();
        }
    } else if (parameter == "--all-code") {
        if (argc == 3) {
            find_all_code(argv[2]);
        } else {
            parameter_err();
        }
    } else if (parameter == "--help") {
        show_help();
    } else {
        cout << "unknow parameter" << endl;
        cout << "You can try \"--help\" for more information" << endl;
        cout << endl;
    }
    return 0;
}
