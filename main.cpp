#include <iostream>
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
	if (str.length() != 9) {
		return false;
	}
	for (unsigned int i = 0; i < str.length(); i++) {
        code_str[i] = str[i];
	}
    code_str[10] = '\0';
    checked_code = cal.Change_int(code_str);
    return cal.Check_Code(checked_code);
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
    File_Output << dat.size() - 1 << endl;
    for (unsigned int i = 0; i < dat.size(); i++) {
        File_Output << cal.Change_str(dat[i]);
        if (i + 1 != dat.size()) {
            File_Output << endl;
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
    File_Output << dat.size() - 1 << endl;
    for (unsigned int i = 0; i < dat.size(); i++) {
        File_Output << cal.Change_str(dat[i]);
        if (i + 1 != dat.size()) {
            File_Output << endl;
        }
    }
    cout << endl;
    File_Output.close();
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

void find_all(string File_name) {
    HRD_statistic statistic;
    cout << "Warning: It will consume nearly 2GB of memory, please make sure there is enough memory!" << endl;
    cout << "Press ENTER to start...";
    cin.get();
    statistic.Find_All_Case(File_name);
    cout << "Data save at " << File_name << endl;
    cout << endl;
}

void find_all_code(string File_name) {
    HRD_statistic statistic;
    cout << "Warning: It will consume nearly 2GB of memory, please make sure there is enough memory!" << endl;
    cout << "Press ENTER to start...";
    cin.get();
    statistic.Make_main_table(File_name);
    cout << "Data save at " << File_name << endl;
    cout << endl;
}

void show_help() {
    cout << "(version: v0.0)" << endl;
    cout << "Usage of HRD_engine:" << endl;
    cout << endl;
    cout << "  --cal <code> [file_name]" << endl;
    cout << "    [file_name]: for output the result" << endl;
    cout << "    Purpose: find the min step solution from <code>" << endl;
    cout << "    exp: ./engine --cal 1A9BF0C00" << endl;
    cout << "         ./engine --cal 1A9BF0C00 demo.txt" << endl;
    cout << endl;
    cout << "  --cal-target <code> <target> [file_name]" << endl;
    cout << "    [file_name]: for output the result" << endl;
    cout << "    Purpose: find the min step path from <code> to <target>" << endl;
    cout << "    exp: ./engine --cal-target 4FEA13400 43EA73400" << endl;
    cout << "         ./engine --cal-target 4FEA13400 43EA73400 demo.txt" << endl;
    cout << endl;
    cout << "  --group <code> <file_name>" << endl;
    cout << "    <file_name>: for output the result" << endl;
    cout << "    Purpose: find all cases in the group where <code> loacted" << endl;
    cout << "    exp: ./engine --group 4FEA13400 demo.txt" << endl;
    cout << endl;
    cout << "  --analy <code> <file_name>" << endl;
    cout << "    <file_name>: for output the result" << endl;
    cout << "    Purpose: analyse the case with <code> " << endl;
    cout << "    exp: ./engine --analy 1A9BF0C00 demo.txt" << endl;
    cout << endl;
    cout << "  --analy-quiet <code> <file_name>" << endl;
    cout << "    Purpose: the same function with analy but don't show the process" << endl;
    cout << "    exp: ./engine --analy-quiet 1A9BF0C00 demo.txt" << endl;
    cout << endl;
    cout << "  --analy-group <code> <file_name_1> <file_name_2>" << endl;
    cout << "    <file_name_1>: for output the result of \"farthest\"" << endl;
    cout << "    <file_name_2>: for output the result of \"solution\"" << endl;
    cout << "    Purpose: analyse all cases in the group where <code> loacted" << endl;
    cout << "    exp: ./engine --group 1A9BF0C00 farthest.csv solution.csv" << endl;
    cout << endl;
    cout << "  --analy-group-all <code> <file_name_1> <file_name_2>" << endl;
    cout << "    Purpose: the same function with analy-group but more output all solution cases" << endl;
    cout << "    exp: ./engine --group-all 1A9BF0C00 farthest.csv solution.csv" << endl;
    cout << endl;
    cout << "  --all <file_name>" << endl;
    cout << "    Purpose: find all cases from klotski game with detail" << endl;
    cout << "    exp: ./engine --all main.csv" << endl;
    cout << endl;
    cout << "  --all-code <file_name>" << endl;
    cout << "    Purpose: find all cases from klotski game only with code" << endl;
    cout << "    exp: ./engine --all-code All_Case.txt" << endl;
    cout << endl;
    cout << "  --help" << endl;
    cout << "    Purpose: show the usage" << endl;
    cout << "    exp: ./engine --help" << endl;
    cout << endl;
}

int main(int argc, char* argv[]) {
	cout << "Klotski engine by Dnomd343" << endl;
	if (argc <= 1) {
        cout << "no parameter" << endl;
        cout << "You can try to use \"--help\" for more information" << endl;
        cout << endl;
        return 0;
	}
	string parameter;
	parameter = argv[1];
	if (parameter == "--cal") {
		if (argc == 3) {
            cal_case(argv[2]);
		} else if (argc == 4) {
            cal_case(argv[2], argv[3]);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
	} else if (parameter == "--cal-target") {
		if (argc == 4) {
            cal_target(argv[2], argv[3]);
		} else if (argc == 5) {
            cal_target(argv[2], argv[3], argv[4]);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
	} else if (parameter == "--group") {
		if (argc == 4) {
            cal_group(argv[2], argv[3]);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
	} else if (parameter == "--analy") {
        if (argc == 4) {
            analy_case(argv[2], argv[3], false);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
	} else if (parameter == "--analy-quiet") {
		if (argc == 4) {
            analy_case(argv[2], argv[3], true);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
	} else if (parameter == "--analy-group") {
		if (argc == 5) {
            analy_group(argv[2], argv[3], argv[4], false);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
    } else if (parameter == "--analy-group-all") {
		if (argc == 5) {
            analy_group(argv[2], argv[3], argv[4], true);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
	} else if (parameter == "--all") {
		if (argc == 3) {
            find_all(argv[2]);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
	} else if (parameter == "--all-code") {
		if (argc == 3) {
            find_all_code(argv[2]);
		} else {
            cout << "parameter error" << endl;
            cout << endl;
            return 0;
		}
	} else if (parameter == "--help") {
		show_help();
	} else {
		cout << "unknow parameter" << endl;
        cout << "You can try to use \"--help\" for more information" << endl;
        cout << endl;
    }
	return 0;
}
