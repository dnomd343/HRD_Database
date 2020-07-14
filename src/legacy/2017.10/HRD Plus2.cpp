#include<iostream>
#include<vector>
#include<list>
#include<fstream>
using namespace std;
ofstream File_Output;
vector<unsigned int> Data_All;
unsigned char Data_Num[21],Data_Blocks[12],Data_Direction[6];
unsigned char Space_Num[9],Space_Style[9];
unsigned int u,Address,Result;
bool Found;
void Add(unsigned int Data);
void Output_Data(unsigned int Data){
    unsigned char i;
    for(i=0;i<8;i++){
        if((0xF&Data>>(7-i)*4)==0x0){File_Output<<"0";}
        if((0xF&Data>>(7-i)*4)==0x1){File_Output<<"1";}
        if((0xF&Data>>(7-i)*4)==0x2){File_Output<<"2";}
        if((0xF&Data>>(7-i)*4)==0x3){File_Output<<"3";}
        if((0xF&Data>>(7-i)*4)==0x4){File_Output<<"4";}
        if((0xF&Data>>(7-i)*4)==0x5){File_Output<<"5";}
        if((0xF&Data>>(7-i)*4)==0x6){File_Output<<"6";}
        if((0xF&Data>>(7-i)*4)==0x7){File_Output<<"7";}
        if((0xF&Data>>(7-i)*4)==0x8){File_Output<<"8";}
        if((0xF&Data>>(7-i)*4)==0x9){File_Output<<"9";}
        if((0xF&Data>>(7-i)*4)==0xA){File_Output<<"A";}
        if((0xF&Data>>(7-i)*4)==0xB){File_Output<<"B";}
        if((0xF&Data>>(7-i)*4)==0xC){File_Output<<"C";}
        if((0xF&Data>>(7-i)*4)==0xD){File_Output<<"D";}
        if((0xF&Data>>(7-i)*4)==0xE){File_Output<<"E";}
        if((0xF&Data>>(7-i)*4)==0xF){File_Output<<"F";}
    }
}
void Data_cout(unsigned int Data){
    unsigned char i;
    for(i=0;i<8;i++){
        if((0xF&Data>>(7-i)*4)==0x0){cout<<"0";}
        if((0xF&Data>>(7-i)*4)==0x1){cout<<"1";}
        if((0xF&Data>>(7-i)*4)==0x2){cout<<"2";}
        if((0xF&Data>>(7-i)*4)==0x3){cout<<"3";}
        if((0xF&Data>>(7-i)*4)==0x4){cout<<"4";}
        if((0xF&Data>>(7-i)*4)==0x5){cout<<"5";}
        if((0xF&Data>>(7-i)*4)==0x6){cout<<"6";}
        if((0xF&Data>>(7-i)*4)==0x7){cout<<"7";}
        if((0xF&Data>>(7-i)*4)==0x8){cout<<"8";}
        if((0xF&Data>>(7-i)*4)==0x9){cout<<"9";}
        if((0xF&Data>>(7-i)*4)==0xA){cout<<"A";}
        if((0xF&Data>>(7-i)*4)==0xB){cout<<"B";}
        if((0xF&Data>>(7-i)*4)==0xC){cout<<"C";}
        if((0xF&Data>>(7-i)*4)==0xD){cout<<"D";}
        if((0xF&Data>>(7-i)*4)==0xE){cout<<"E";}
        if((0xF&Data>>(7-i)*4)==0xF){cout<<"F";}
    }
}
unsigned char Get_Style(unsigned char Num){
    if(Num==0){return 0;}
    if(Num>=1&&Num<=5){
        if(Data_Direction[Num]==0){return 1;}else{return 2;}
    }
    if(Num>=6&&Num<=9){return 3;}
    return 4;
}
void Search_Space(){
    unsigned char Num,i=0;
    Space_Again:;
    if(Data_Blocks[i+10]>4){
        Num=Data_Num[Data_Blocks[i+10]-4];
        Space_Num[i*4+1]=Num;
        Space_Style[i*4+1]=Get_Style(Num);
    }
    else{
        Space_Num[i*4+1]=0;
        Space_Style[i*4+1]=4;
    }
    if(Data_Blocks[i+10]<17){
        Num=Data_Num[Data_Blocks[i+10]+4];
        Space_Num[i*4+2]=Num;
        Space_Style[i*4+2]=Get_Style(Num);
    }
    else{
        Space_Num[i*4+2]=0;
        Space_Style[i*4+2]=4;
    }
    if((Data_Blocks[i+10]%4)!=1){
        Num=Data_Num[Data_Blocks[i+10]-1];
        Space_Num[i*4+3]=Num;
        Space_Style[i*4+3]=Get_Style(Num);
    }
    else{
        Space_Num[i*4+3]=0;
        Space_Style[i*4+3]=4;
    }
    if((Data_Blocks[i+10]%4)!=0){
        Num=Data_Num[Data_Blocks[i+10]+1];
        Space_Num[i*4+4]=Num;
        Space_Style[i*4+4]=Get_Style(Num);
    }
    else{
        Space_Num[i*4+4]=0;
        Space_Style[i*4+4]=4;
    }
    if(i==0){i++;goto Space_Again;}
}
void Analyse(unsigned int Data){
    unsigned char Num,Style,i,c,b=1,j=1,k=1;
    for(i=1;i<=20;i++){
        Data_Num[i]=12;
    }
    c=Data&0xF;
    i=1;
    for(Num=1;Num<=20;Num++){
        if(Data_Num[Num]==12){
            if(c==i){
                Data_Num[Num]=0;
                Data_Num[Num+1]=0;
                Data_Num[Num+4]=0;
                Data_Num[Num+5]=0;
                Data_Blocks[0]=Num;
                c=0;
                goto Again;
            }
            Style=(Data>>(32-i*2))&3;
            i++;
            if(Style==0){
                Data_Num[Num]=10;
                Data_Blocks[k+9]=Num;
                k++;
            }
            if(Style==1){
                Data_Num[Num]=j;
                Data_Num[Num+1]=j;
                Data_Direction[j]=0;
                Data_Blocks[j]=Num;
                j++;
            }
            if(Style==2){
                Data_Num[Num]=j;
                Data_Num[Num+4]=j;
                Data_Direction[j]=1;
                Data_Blocks[j]=Num;
                j++;
            }
            if(Style==3){
                Data_Num[Num]=b+5;
                Data_Blocks[b+5]=Num;
                b++;
            }
            Again:;
        }
    }
    Search_Space();
}
unsigned char Direction_Back(unsigned char Way){
    if(Way==1){return 2;}
    if(Way==2){return 1;}
    if(Way==3){return 4;}
    if(Way==4){return 3;}
}
void Move_Block(unsigned char Num,unsigned char Direction){
    if(Num==0){
        Data_Num[Data_Blocks[Num]]=10;
        Data_Num[Data_Blocks[Num]+1]=10;
        Data_Num[Data_Blocks[Num]+4]=10;
        Data_Num[Data_Blocks[Num]+5]=10;
    }
    if(Num>=1&&Num<=5){
        if(Data_Direction[Num]==0){
            Data_Num[Data_Blocks[Num]]=10;
            Data_Num[Data_Blocks[Num]+1]=10;
        }
        else{
            Data_Num[Data_Blocks[Num]]=10;
            Data_Num[Data_Blocks[Num]+4]=10;
        }
    }
    if(Num>=6&&Num<=9){
        Data_Num[Data_Blocks[Num]]=10;
    }
    if(Direction==1)(Data_Blocks[Num]=Data_Blocks[Num]-4);
    if(Direction==2)(Data_Blocks[Num]=Data_Blocks[Num]+4);
    if(Direction==3)(Data_Blocks[Num]=Data_Blocks[Num]-1);
    if(Direction==4)(Data_Blocks[Num]=Data_Blocks[Num]+1);
    if(Num==0){
        Data_Num[Data_Blocks[Num]]=0;
        Data_Num[Data_Blocks[Num]+1]=0;
        Data_Num[Data_Blocks[Num]+4]=0;
        Data_Num[Data_Blocks[Num]+5]=0;
    }
    if(Num>=1&&Num<=5){
        if(Data_Direction[Num]==0){
            Data_Num[Data_Blocks[Num]]=Num;
            Data_Num[Data_Blocks[Num]+1]=Num;
        }
        else{
            Data_Num[Data_Blocks[Num]]=Num;
            Data_Num[Data_Blocks[Num]+4]=Num;
        }
    }
    if(Num>=6&&Num<=9){
        Data_Num[Data_Blocks[Num]]=Num;
    }
}
void Create(unsigned char Num,unsigned char Direction_1,unsigned char Direction_2){
    unsigned char i,j,n;
    unsigned int Data_Output,Space_Address[2];
    Move_Block(Num,Direction_1);
    Move_Block(Num,Direction_2);
    n=0;
    for(i=1;i<=20;i++){
        if(Data_Num[i]==10){Space_Address[n]=i;n++;}
    }
    j=0;
    Data_Output=0;
    for(n=1;n<=20;n++){
        for(i=1;i<10;i++){
            if(Data_Blocks[i]==n){
                if(i<=5){Data_Output=Data_Output|((Data_Direction[i]+1)<<(30-j*2));}
                if(i>=6){Data_Output=Data_Output|(3<<(30-j*2));}
                j++;
            }
        }
        if(Space_Address[0]==n){Data_Output=Data_Output|(0<<(30-j*2));j++;}
        if(Space_Address[1]==n){Data_Output=Data_Output|(0<<(30-j*2));j++;}
        if(Data_Blocks[0]==n){Data_Output=Data_Output|(j+1);}
    }
    Add(Data_Output);
    if(Data_Blocks[0]==14){
        Result=Data_Output;
        Found=true;
    }
    Move_Block(Num,Direction_Back(Direction_2));
    Move_Block(Num,Direction_Back(Direction_1));
}
void Search_Way(unsigned int Data){
    Analyse(Data);
    if((Data_Blocks[11]-Data_Blocks[10])==1){
        if((Space_Style[1]==0)&&(Space_Style[5]==0)){Create(Space_Num[1],2,0);}
        if((Space_Style[1]==1)&&(Space_Style[5]==1)){Create(Space_Num[1],2,0);}
        if(Space_Style[1]==2){Create(Space_Num[1],2,0);}
        if(Space_Style[5]==2){Create(Space_Num[5],2,0);}
        if(Space_Style[1]==3){Create(Space_Num[1],2,0);Create(Space_Num[1],2,4);}
        if(Space_Style[5]==3){Create(Space_Num[5],2,0);Create(Space_Num[5],2,3);}
        if((Space_Style[2]==0)&&(Space_Style[6]==0)){Create(Space_Num[2],1,0);}
        if((Space_Style[2]==1)&&(Space_Style[6]==1)){Create(Space_Num[2],1,0);}
        if(Space_Style[2]==2){Create(Space_Num[2],1,0);}
        if(Space_Style[6]==2){Create(Space_Num[6],1,0);}
        if(Space_Style[2]==3){Create(Space_Num[2],1,0);Create(Space_Num[2],1,4);}
        if(Space_Style[6]==3){Create(Space_Num[6],1,0);Create(Space_Num[6],1,3);}
        if(Space_Style[3]==1){Create(Space_Num[3],4,0);Create(Space_Num[3],4,4);}
        if(Space_Style[3]==3){Create(Space_Num[3],4,0);Create(Space_Num[3],4,4);}
        if(Space_Style[8]==1){Create(Space_Num[8],3,0);Create(Space_Num[8],3,3);}
        if(Space_Style[8]==3){Create(Space_Num[8],3,0);Create(Space_Num[8],3,3);}
    }
    else if((Data_Blocks[11]-Data_Blocks[10])==4){
        if(Space_Style[1]==2){Create(Space_Num[1],2,0);Create(Space_Num[1],2,2);}
        if(Space_Style[1]==3){Create(Space_Num[1],2,0);Create(Space_Num[1],2,2);}
        if(Space_Style[6]==2){Create(Space_Num[6],1,0);Create(Space_Num[6],1,1);}
        if(Space_Style[6]==3){Create(Space_Num[6],1,0);Create(Space_Num[6],1,1);}
        if((Space_Style[3]==0)&&(Space_Style[7]==0)){Create(Space_Num[3],4,0);}
        if((Space_Style[3]==2)&&(Space_Style[7]==2)){Create(Space_Num[3],4,0);}
        if(Space_Style[3]==1){Create(Space_Num[3],4,0);}
        if(Space_Style[7]==1){Create(Space_Num[7],4,0);}
        if(Space_Style[3]==3){Create(Space_Num[3],4,0);Create(Space_Num[3],4,2);}
        if(Space_Style[7]==3){Create(Space_Num[7],4,0);Create(Space_Num[7],4,1);}
        if((Space_Style[4]==0)&&(Space_Style[8]==0)){Create(Space_Num[4],3,0);}
        if((Space_Style[4]==2)&&(Space_Style[8]==2)){Create(Space_Num[4],3,0);}
        if(Space_Style[4]==1){Create(Space_Num[4],3,0);}
        if(Space_Style[8]==1){Create(Space_Num[8],3,0);}
        if(Space_Style[4]==3){Create(Space_Num[4],3,0);Create(Space_Num[4],3,2);}
        if(Space_Style[8]==3){Create(Space_Num[8],3,0);Create(Space_Num[8],3,1);}
    }
    else{
        if(Space_Style[1]==2){Create(Space_Num[1],2,0);}
        if(Space_Style[1]==3){Create(Space_Num[1],2,0);}
        if(Space_Style[2]==2){Create(Space_Num[2],1,0);}
        if(Space_Style[2]==3){Create(Space_Num[2],1,0);}
        if(Space_Style[3]==1){Create(Space_Num[3],4,0);}
        if(Space_Style[3]==3){Create(Space_Num[3],4,0);}
        if(Space_Style[4]==1){Create(Space_Num[4],3,0);}
        if(Space_Style[4]==3){Create(Space_Num[4],3,0);}
        if(Space_Style[5]==2){Create(Space_Num[5],2,0);}
        if(Space_Style[5]==3){Create(Space_Num[5],2,0);}
        if(Space_Style[6]==2){Create(Space_Num[6],1,0);}
        if(Space_Style[6]==3){Create(Space_Num[6],1,0);}
        if(Space_Style[7]==1){Create(Space_Num[7],4,0);}
        if(Space_Style[7]==3){Create(Space_Num[7],4,0);}
        if(Space_Style[8]==1){Create(Space_Num[8],3,0);}
        if(Space_Style[8]==3){Create(Space_Num[8],3,0);}
    }
}
void Add(unsigned int Data){
    for(u=0;u<Data_All.size();u++){
        if(Data_All[u]==Data){goto out;}
    }
    Data_All.push_back(Data);
    out:;
}
void Calculate(unsigned int Data){
    unsigned int i;
    Data_All.push_back(Data);
    File_Output.open("Data.txt");
    Address=0;
    Found=false;
    while(1){
        Search_Way(Data_All[Address]);
        Address++;
        if(Found==true){break;}
    }
    for(i=0;i<Data_All.size();i++){
        Output_Data(Data_All[i]);
        File_Output<<endl;
    }
    File_Output.close();
}
int main(){
    unsigned int Start_Data=0xA9BF0C02;
    Calculate(Start_Data);
    return 0;
}
