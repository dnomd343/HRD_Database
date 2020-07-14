#include<iostream>
#include<list>
#include<string>
#include<fstream>
#include "HRD_Calculate.h"
using namespace std;

ifstream File_Input;
ofstream File_Output;

int main(){
	cout<<"This is a test of HRD_Calculate"<<endl;
	HRD_Calculate HRD;
	HRD.Calculate(0x1A9BF0C);
    list<unsigned int>::iterator poi;
    File_Output.open("Data_Output.txt");
    for(unsigned int i=0;i<HRD.List.size();i++){
        File_Output<<i<<" -> "<<HRD.Change_str(HRD.List[i])<<" ("<<HRD.Layer_Num[i]<<")";
        poi=HRD.Source[i].begin();
        while(poi!=HRD.Source[i].end()){File_Output<<"  ["<<*poi<<"]";++poi;}
        File_Output<<endl;
    }
    File_Output.close();
	return 0;
}
