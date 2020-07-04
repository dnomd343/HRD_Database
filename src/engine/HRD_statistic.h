#ifndef HRD_statistic_H
#define HRD_statistic_H

#include <vector>
#include <string>
using namespace std;

class HRD_statistic {
    public:
        void Find_All_Case(string File_name);
        void All_Statistic();

    private:
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
        vector <Case *> All_Case; // 记录所有布局的信息

        void Get_seed();
        void Find_All_Case();
        void Sort_All_Case();
        vector <Case_group *> Split_Group(vector <unsigned long long> dat);
        void Output_All_Case(string File_name);
        void Output_main_table(string File_name);
};

#endif
