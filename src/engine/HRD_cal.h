#ifndef HRD_cal_H
#define HRD_cal_H

#include <vector>
#include <string>
using namespace std;

class HRD_cal {
    public:
        ~HRD_cal();
        unsigned long long Change_int(char str[10]);
        string Change_str(unsigned long long dat);
        bool Check_Code(unsigned long long code);
        vector <unsigned long long> Calculate(unsigned long long code);
        vector <unsigned long long> Calculate_All(unsigned long long code);
        vector <unsigned long long> Calculate(unsigned long long code, unsigned long long target);

    private:
        struct Case_cal {
            bool freeze[4][5]; // true -> no move ; false -> can move
            unsigned char status[4][5]; // 0xFF -> undefined ; 0xFE -> space
            unsigned char type[15]; // 0 -> 2 * 2 ; 1 -> 2 * 1 ; 2 -> 1 * 2 ; 3 -> 1 * 1
            unsigned long long code;
        };
        vector <Case_cal *> List; // 主队列 储存每个节点的信息
        vector <Case_cal *> *List_hash = new vector <Case_cal *> [0x10000]; // 哈希表
        vector <unsigned int> List_source; // 储存上一步编号 用于溯源
        unsigned int now_move; // 当前正在计算的块的编号
        unsigned int result; // 得到的目标编号
        unsigned long long target_code;
        unsigned char mode; // 0 -> Calculate_All / 1 -> Calculate_Solution / 2 -> Calculate_Target
        bool flag; // 判断是否已找到目标

        bool Parse_Code(Case_cal &dat, unsigned long long code);
        void Get_Code(Case_cal &dat);
        void Find_Sub_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
        void Build_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
        void Find_Next_Case(Case_cal &dat_raw);
        void Add_Case(Case_cal *dat);
        void cal(unsigned long long code);
        void init_data();
        vector <unsigned long long> Get_Path(unsigned int result_num);
};

#endif
