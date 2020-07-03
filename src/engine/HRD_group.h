#ifndef HRD_group_H
#define HRD_group_H

#include <vector>
#include <string>
using namespace std;

class HRD_group {
    public:
        unsigned long long Change_int(char str[10]);
        string Change_str(unsigned long long dat);
        void Batch_Analyse(unsigned long long seed, string name_farthest, string name_solution, bool is_output_solution);
        bool Multi_Analyse(string seed_File_name, string name_farthest, string name_solution, bool is_output_solution);

    private:
        struct Case_cal {
            unsigned long long code;
            unsigned char status[4][5]; // 0xFF -> undefined ; 0xFE -> space
            unsigned char type[15]; // 0 -> 2 * 2 ; 1 -> 2 * 1 ; 2 -> 1 * 2 ; 3 -> 1 * 1
        };
        struct Case {
            unsigned long long Code;
            int Layer_num;
            bool Flag;
            unsigned int addr_2x2;
            vector <Case *> Next;
        };
        struct Case_detail {
            unsigned long long code;
            int farthest_step;
            int farthest_num;
            vector <unsigned long long> farthest_case;

            int min_solution_step;
            int min_solution_num;
            vector <unsigned long long> min_solution_case;

            int solution_num;
            vector <unsigned long long> solution_case;
            vector <unsigned long long> solution_step;
        };
        vector <unsigned long long> Next_Case_dat; // 储存Find_Next_Case找到的结果
        bool Output_solution_case; // 是否输出全部solution_case

        bool Parse_Code(Case_cal &dat, unsigned long long Code);
        void Get_Code(Case_cal &dat);
        void Find_Sub_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
        void Build_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
        vector <unsigned long long> Find_Next_Case(unsigned long long Code);
        void Case_detail_init(Case_detail &dat);
        Case_detail* Analyse_Case(Case *start);
        void Analyse_Group(vector <unsigned long long> dat);
        void Output_detail(Case_detail *dat);
};

#endif
