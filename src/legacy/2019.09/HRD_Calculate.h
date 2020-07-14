#ifndef HRD_H
#define HRD_H

#include<iostream>
#include<vector>
#include<list>
#include<string>
using namespace std;

class HRD_Calculate{
	public:
    struct Block_struct{
        unsigned char Address;
        unsigned char Style;
    };
    unsigned char Table[20];//num:Block  0xA:Space  0xFF:Empty
    unsigned char Space[2];
    struct Block_struct Block[10];//0:2*2  1~5:1*2,2*1  6~9:1*1
    const unsigned char Output_Width=4;
    const string Square_str="[]";
    const unsigned char Up=1,Down=2,Left=3,Right=4;
    vector <unsigned int> List;
    vector <unsigned int> Layer_Num;
    list <unsigned int> Hash[0xFFFF];
    vector< list <unsigned int> > Source;
    list <unsigned int> Empty_uint_List;
    unsigned int Now_Move;

    void Calculate(unsigned int Start_Code);
    void Add_Case(unsigned int Code);
    void Move_Block(unsigned char Block_Num,unsigned char Dir_1,unsigned char Dir_2);
    void Find_Next();
    unsigned int Get_Code();
    void Analyse_Code(unsigned int Code);
    bool Check(unsigned int Code);
    void Output_Graph(unsigned int Code);
    unsigned int Change_int(char Data_str[7]);
	string Change_str(unsigned int Data_int);
};

#endif
