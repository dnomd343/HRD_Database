#include <iostream>
#include <string>
#include <fstream>

using namespace std;
ifstream File_input;
ofstream File_output;

int main() {
    string str;
    int i, j, num, times;
    
    // farthest
    File_input.open("farthest_raw.csv");
    File_output.open("farthest.csv");
    num = 0;
    while (!File_input.eof()) {
        File_input >> str;
        times = 0;
        for (i = 0; i < str.size(); i++) {
            if (str[i] == ',') {times++;}
            if (times == 3) {break;}
        }
        File_output << str.substr(0, i) << "\n";
        num++;
        if (num % 100000 == 0) {cout << num << "/29334498" << endl;}
        File_input.get();
        if (File_input.peek() == '\n') {break;}
    }
    File_input.close();
    File_output.close();

    // solution
    File_input.open("solution_raw.csv");
    File_output.open("solution.csv");
    num = 0;
    while (!File_input.eof()) {
        File_input >> str;
        times = 0;
        for (i = 0; i < str.size(); i++) {
            if (str[i] == ',') {
                times++;
                if (times == 3) {j = i;}
            }
            if (times == 4) {break;}
        }
        File_output << str.substr(0, j) << str.substr(i) << "\n";
        num++;
        if (num % 100000 == 0) {cout << num << "/29334498" << endl;}
        File_input.get();
        if (File_input.peek() == '\n') {break;}
    }
    File_input.close();
    File_output.close();

    return 0;
}