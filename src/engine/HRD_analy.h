#ifndef HRD_analy_H
#define HRD_analy_H

#include <vector>
#include <string>
using namespace std;

class HRD_analy {
public:
    ~HRD_analy();
    struct Case_cal {
        bool freeze[4][5]; // true -> no move ; false -> can move
        unsigned char status[4][5]; // 0xFF -> undefined ; 0xFE -> space
        unsigned char type[15]; // 0 -> 2 * 2 ; 1 -> 2 * 1 ; 2 -> 1 * 2 ; 3 -> 1 * 1
        unsigned long long code;
        unsigned int layer_num;
        unsigned int layer_index;
        vector <Case_cal *> *source_case;
        vector <Case_cal *> *next_case;
    };
    vector < vector <Case_cal *> > Layer; // 储存全部层数据的节点
    Case_cal Parse_dat;
    bool quiet = false; // true -> 静默模式  false -> 输出运算情况
    // 布局的基本参数
    int stop_point_num; // 端点的个数
    int min_solution_step; // 最少的步数
    int min_solution_num; // 最少步解的个数
    vector <unsigned long long> min_solution_case; // 所有最少步解
    vector <unsigned int> solution_step; // 所有解对应的步数
    int solution_num; // 解的个数
    vector <unsigned long long> solution_case; // 所有解
    int farthest_step; // 最远布局的步数
    int farthest_num; // 最远布局的个数
    vector <unsigned long long> farthest_case; // 所有最远的布局

    unsigned long long Change_int(char str[10]);
    string Change_str(unsigned long long dat);
    bool Check_Code(unsigned long long code);
    bool Parse_Code(unsigned long long code);
    bool Is_Mirror(unsigned long long code);
    void Analyse_Case(unsigned long long code);
    void Output_Detail(string File_name);
    void Free_Data();
    void Output_All_Path(string File_name);
    bool Output_Path(vector <unsigned long long> target, string File_name);
    void Output_Graph(unsigned long long code, unsigned int square_width, unsigned int square_gap, char str[2]);

private:
    Case_cal *now_move_case;
    vector <Case_cal *> *Layer_hash = new vector <Case_cal *> [0x10000]; // 哈希表
    unsigned int now_move_num, now_move_index; // 当前扫描节点的层编号 / 当前扫描节点的层中编号
    bool Parse_Code(Case_cal &dat, unsigned long long code);
    void Get_Code(Case_cal &dat);
    void Find_Sub_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
    void Build_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
    void Find_Next_Case(Case_cal &dat_raw);
    void Add_Case(Case_cal *dat);
    void Calculate(unsigned long long code);
    bool Search_Case(unsigned long long code, Case_cal* &dat);
};

#endif
