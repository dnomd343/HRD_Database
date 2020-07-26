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
    bool flag;
    char code_str[10];
    if (str.length() > 9) { // 超过九位 错误
        return false;
    } else {
        str.resize(9, '0'); // 剩余的补0
    }
    for (unsigned int i = 0; i < str.length(); i++) {
        flag = false;
        if (str[i] >= 48 && str[i] <= 57) { // 0 ~ 9
            flag = true;
        } else if (str[i] >= 65 && str[i] <= 70) { // A ~ F
            flag = true;
        } else if (str[i] >= 97 && str[i] <= 102) { // a ~ f
            flag = true;
        }
        if (!flag) {return false;} // 存在非法字符 错误
        code_str[i] = str[i];
    }
    code_str[9] = '\0';
    checked_code = cal.Change_int(code_str);
    return cal.Check_Code(checked_code);
}

void show_case(string code_str) {
    unsigned long long code;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_analy analy;
    char output_char[3] = "&%";
    cout << endl;
    cout << "Code: " << analy.Change_str(code) << endl;
    analy.Output_Graph(code, 4, 1, output_char);
    cout << endl;
}

void show_case(string code_str, string width) {
    unsigned long long code;
    if (code_check(code_str) == false) {
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

void cal_case(string code_str) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
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

void cal_case(string code_str, string File_name) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_cal cal;
    if (File_name == "%") {File_name = cal.Change_str(code) + ".txt";}
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

void cal_path(string code_str, string File_name) {
    unsigned long long code;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_analy analy;
    analy.quiet = true;
    if (File_name == "%") {File_name = analy.Change_str(code) + ".txt";}
    cout << "Start code: " << analy.Change_str(code) << endl;
    analy.Analyse_Case(code);
    if (analy.min_solution_step == -1) {
        cout << "no solution" << endl;
        return;
    } else {
        cout << "At least need " << analy.min_solution_step << " step" << endl;
    }
    cout << "min-step solution case(" << analy.min_solution_num <<"):" << endl;
    for (unsigned int i = 0; i < analy.min_solution_case.size(); i++) {
        cout << "  " << analy.Change_str(analy.min_solution_case[i]) << endl;
    }
    analy.Output_Path(analy.min_solution_case, File_name);
    cout << "Data save at " << File_name << endl;
}

void cal_solution_path(string code_str, string File_name) {
    unsigned long long code;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_analy analy;
    analy.quiet = true;
    if (File_name == "%") {File_name = analy.Change_str(code) + ".txt";}
    cout << "Start code: " << analy.Change_str(code) << endl;
    analy.Analyse_Case(code);
    if (analy.min_solution_step == -1) {
        cout << "no solution" << endl;
        return;
    }
    cout << "solution case(" << analy.solution_num <<"):" << endl;
    for (unsigned int i = 0; i < analy.solution_case.size(); i++) {
        cout << "  " << analy.Change_str(analy.solution_case[i]) << "(" << analy.solution_step[i] << ")" << endl;
    }
    analy.Output_Path(analy.solution_case, File_name);
    cout << "Data save at " << File_name << endl;
}

void cal_farthest_path(string code_str, string File_name) {
    unsigned long long code;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_analy analy;
    analy.quiet = true;
    if (File_name == "%") {File_name = analy.Change_str(code) + ".txt";}
    cout << "Start code: " << analy.Change_str(code) << endl;
    analy.Analyse_Case(code);
    cout << "farthest case(" << analy.farthest_num <<"):" << endl;
    for (unsigned int i = 0; i < analy.farthest_case.size(); i++) {
        cout << "  " << analy.Change_str(analy.farthest_case[i]) << endl;
    }
    analy.Output_Path(analy.farthest_case, File_name);
    cout << "Data save at " << File_name << endl;
}

void cal_target(string code_str, string target_str) {
    unsigned long long code, target;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    if (code_check(target_str) == false) {
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

void cal_target(string code_str, string target_str, string File_name) {
    unsigned long long code, target;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    if (code_check(target_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    target = checked_code;
    HRD_cal cal;
    if (File_name == "%") {File_name = cal.Change_str(code) + "_" + cal.Change_str(target) + ".txt";}
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

void cal_target_path(string code_str, vector <string> target_dat, string File_name) {
    unsigned long long code;
    vector <unsigned long long> target;
    HRD_analy analy;
    analy.quiet = true;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    for (unsigned int i = 0; i < target_dat.size(); i++) {
        if (code_check(target_dat[i]) == false) {
            cout << "target code error" << endl;
            cout << endl;
            return;
        }
        target.push_back(checked_code);
    }
    if (File_name == "%") {File_name = analy.Change_str(code) + ".txt";}
    cout << "Start code: " << analy.Change_str(code) << endl;
    cout << "Target code:" << endl;
    for (unsigned int i = 0; i < target.size(); i++) {
        cout << "  " << analy.Change_str(target[i]) << endl;
    }
    analy.Analyse_Case(code);
    if (analy.Output_Path(target, File_name)) {
        cout << "Data save at " << File_name << endl;
    } else {
        cout << "Target not found" << endl;
    }
    cout << endl;
}

void cal_group(string code_str) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
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

void cal_group(string code_str, string File_name) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_cal cal;
    if (File_name == "%") {File_name = cal.Change_str(code) + ".txt";}
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

void cal_group_path(string code_str, string File_name) {
    unsigned long long code;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_analy analy;
    analy.quiet = true;
    if (File_name == "%") {File_name = analy.Change_str(code) + ".txt";}
    cout << "Start code: " << analy.Change_str(code) << endl;
    analy.Analyse_Case(code);
    cout << "stop_point_num: " << analy.stop_point_num << endl;
    analy.Output_All_Path(File_name);
    cout << "Data save at " << File_name << endl;
}

void analy_case(string code_str, bool quiet) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
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

void analy_case(string code_str, string File_name, bool quiet) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_analy analy;
    if (File_name == "%") {File_name = analy.Change_str(code) + ".txt";}
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

void analy_group(string code_str, string File_name_1, string File_name_2, bool is_all) {
    unsigned long long code;
    vector <unsigned long long> dat;
    if (code_check(code_str) == false) {
        cout << "code error" << endl;
        cout << endl;
        return;
    }
    code = checked_code;
    HRD_group group;
    if (File_name_1 == "%") {File_name_1 = "farthest_" + group.Change_str(code) + ".csv";}
    if (File_name_2 == "%") {File_name_2 = "solution_" + group.Change_str(code) + ".csv";}
    cout << "Start code: " << group.Change_str(code) << endl;
    group.Batch_Analyse(code, File_name_1, File_name_2, is_all);
    cout << "Data save at " << File_name_1 << " and " << File_name_2 << endl;
    cout << endl;
}

void analy_multi_group(string File_name_seed, string File_name_1, string File_name_2, bool is_all) {
    HRD_group group;
    if (File_name_seed.substr(File_name_seed.length() - 4) == ".txt") {
    	File_name_seed = File_name_seed.substr(0, File_name_seed.length() - 4);
        if (File_name_1 == "%") {File_name_1 = "farthest_" + File_name_seed + ".csv";}
        if (File_name_2 == "%") {File_name_2 = "solution_" + File_name_seed + ".csv";}
        File_name_seed = File_name_seed + ".txt";
    } else {
        if (File_name_1 == "%") {File_name_1 = "farthest_" + File_name_seed + ".csv";}
        if (File_name_2 == "%") {File_name_2 = "solution_" + File_name_seed + ".csv";}
    }
    if (group.Multi_Analyse(File_name_seed, File_name_1, File_name_2, is_all) == false) {
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
    if (File_name == "%") {File_name = "All_Case.txt";}
    statistic.Find_All_Case(File_name);
    cout << "Data save at " << File_name << endl;
    cout << endl;
}

void show_help() {
    cout << "(version: v1.2)" << endl;
    cout << "Usage of HRD_engine:" << endl;
    cout << endl;
    cout << "  ~ 'code' will automatically fill '0' to 9 bits" << endl;
    cout << endl;
    cout << "  ~ use '%' to automatically generate 'file_name'" << endl;
    cout << endl;
    cout << "  --show <code> [square_width]" << endl;
    cout << "    Purpose: Visualize the <code>" << endl;
    cout << "    eg: ./engine --show 1A9BF0C00" << endl;
    cout << "        ./engine --show 1A9BF0C00 5" << endl;
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
    cout << "  --cal-path <code> <file_name>" << endl;
    cout << "    Purpose: Find all of the minimum step solution path of <code>" << endl;
    cout << "    eg: ./engine --cal-path 4FEA13400 demo.txt" << endl;
    cout << endl;
    cout << "  --cal-solution-path <code> <file_name>" << endl;
    cout << "    Purpose: Find all of the solution path of <code>" << endl;
    cout << "    eg: ./engine --cal-solution-path 1A9BF0C00 demo.txt" << endl;
    cout << endl;
    cout << "  --cal-farthest-path <code> <file_name>" << endl;
    cout << "    Purpose: Find all of the farthest path of <code>" << endl;
    cout << "    eg: ./engine --cal-farthest-path 4FEA13400 demo.txt" << endl;
    cout << endl;
    cout << "  --cal-target-path <code> <target_1> ... <target_n> <file_name>" << endl;
    cout << "    Purpose: Find all of the shortest path from <code> to <target_1>...<target_n>" << endl;
    cout << "    eg: ./engine --cal-target-path 1A9BF0C00 DAAF4CC00 AE2F2B400 demo.txt" << endl;
    cout << endl;
    cout << "  --group <code> [file_name]" << endl;
    cout << "    Purpose: Find all elements of the group where <code> located" << endl;
    cout << "    eg: ./engine --group 4FEA13400" << endl;
    cout << "        ./engine --group 4FEA13400 demo.txt" << endl;
    cout << endl;
    cout << "  --group-path <code> <file_name>" << endl;
    cout << "    Purpose: Find all of the path in the group where <code> located" << endl;
    cout << "    eg: ./engine --group-path 1A9BF0C00 demo.txt" << endl;
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
    } else if (parameter == "--cal-path") {
        if (argc == 4) {
            cal_path(argv[2], argv[3]);
        } else {
            parameter_err();
        }
    } else if (parameter == "--cal-solution-path") {
        if (argc == 4) {
            cal_solution_path(argv[2], argv[3]);
        } else {
            parameter_err();
        }
    } else if (parameter == "--cal-farthest-path") {
        if (argc == 4) {
            cal_farthest_path(argv[2], argv[3]);
        } else {
            parameter_err();
        }
    } else if (parameter == "--cal-target-path") {
        if (argc >= 5) {
            vector <string> target;
            for (int i = 3; i < argc - 1; i++) {
                target.push_back(argv[i]);
            }
            cal_target_path(argv[2], target, argv[argc - 1]);
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
    } else if (parameter == "--group-path") {
        if (argc == 4) {
            cal_group_path(argv[2], argv[3]);
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
