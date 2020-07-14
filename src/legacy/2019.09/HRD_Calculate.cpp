#include "HRD_Calculate.h"
using namespace std;
void HRD_Calculate::Calculate(unsigned int Start_Code){
	unsigned int i;
	for(i=0;i<0xFFFF;i++){Hash[i].clear();}
	List.clear();
	Layer_Num.clear();
	Hash[(Start_Code>>8)&0xFFFF].push_back(0);
	List.push_back(Start_Code);
	Layer_Num.push_back(0);
	Source.push_back(Empty_uint_List);
	Now_Move=0;
	while(1){
		if(Now_Move==List.size()){break;}
		Analyse_Code(List[Now_Move]);
		Find_Next();
		Now_Move++;
	}
}
void HRD_Calculate::Add_Case(unsigned int Code){
    list<unsigned int>::iterator poi;
	poi=Hash[(Code>>8)&0xFFFF].begin();
	while(poi!=Hash[(Code>>8)&0xFFFF].end()){
		if(Code==List[*poi]){
            if((Layer_Num[*poi]-Layer_Num[Now_Move])==1){Source[*poi].push_back(Now_Move);}
            return;
        }
		++poi;
	}
	Hash[(Code>>8)&0xFFFF].push_back(List.size());
	List.push_back(Code);
	Layer_Num.push_back(Layer_Num[Now_Move]+1);
	Source.push_back(Empty_uint_List);
	Source[Source.size()-1].push_back(Now_Move);
}
void HRD_Calculate::Move_Block(unsigned char Block_Num,unsigned char Dir_1,unsigned char Dir_2){
    unsigned char Table_bak[21];
	unsigned char i,addr;
	if(Dir_1==Up){Block[Block_Num].Address-=4;}
	else if(Dir_1==Down){Block[Block_Num].Address+=4;}
	else if(Dir_1==Left){Block[Block_Num].Address--;}
	else if(Dir_1==Right){Block[Block_Num].Address++;}
	if(Dir_2==Up){Block[Block_Num].Address-=4;}
	else if(Dir_2==Down){Block[Block_Num].Address+=4;}
	else if(Dir_2==Left){Block[Block_Num].Address--;}
	else if(Dir_2==Right){Block[Block_Num].Address++;}
	for(i=0;i<20;i++){Table_bak[i]=Table[i];Table[i]=0xA;}
	for(i=0;i<10;i++){
		addr=Block[i].Address;
		if(Block[i].Style==0){Table[addr]=i;Table[addr+1]=i;Table[addr+4]=i;Table[addr+5]=i;}
		if(Block[i].Style==1){Table[addr]=i;Table[addr+1]=i;}
		if(Block[i].Style==2){Table[addr]=i;Table[addr+4]=i;}
		if(Block[i].Style==3){Table[addr]=i;}
	}
    Add_Case(Get_Code());
	for(i=0;i<20;i++){Table[i]=Table_bak[i];}
	if(Dir_2==Up){Block[Block_Num].Address+=4;}
	else if(Dir_2==Down){Block[Block_Num].Address-=4;}
	else if(Dir_2==Left){Block[Block_Num].Address++;}
	else if(Dir_2==Right){Block[Block_Num].Address--;}
	if(Dir_1==Up){Block[Block_Num].Address+=4;}
	else if(Dir_1==Down){Block[Block_Num].Address-=4;}
	else if(Dir_1==Left){Block[Block_Num].Address++;}
	else if(Dir_1==Right){Block[Block_Num].Address--;}
}
void HRD_Calculate::Find_Next(){
    bool Can_Move[10];
    unsigned char i,addr;
	for(i=0;i<10;i++){Can_Move[i]=false;}
	for(i=0;i<=1;i++){
		addr=Space[i];
		if(addr>3){if(Table[addr-4]!=0xA){Can_Move[Table[addr-4]]=true;}}
		if(addr<16){if(Table[addr+4]!=0xA){Can_Move[Table[addr+4]]=true;}}
		if((addr%4)!=0){if(Table[addr-1]!=0xA){Can_Move[Table[addr-1]]=true;}}
		if((addr%4)!=3){if(Table[addr+1]!=0xA){Can_Move[Table[addr+1]]=true;}}
	}
    for(i=0;i<10;i++){
        if(Can_Move[i]==true){
			addr=Block[i].Address;
			if(Block[i].Style==0){
				if((addr>3)&&(Table[addr-4]==0xA)&&(Table[addr-3]==0xA)){Move_Block(i,Up,0);}
				if((addr<12)&&(Table[addr+8]==0xA)&&(Table[addr+9]==0xA)){Move_Block(i,Down,0);}
				if(((addr%4)!=0)&&(Table[addr-1]==0xA)&&(Table[addr+3]==0xA)){Move_Block(i,Left,0);}
				if(((addr%4)!=2)&&(Table[addr+2]==0xA)&&(Table[addr+6]==0xA)){Move_Block(i,Right,0);}
			}
			else if(Block[i].Style==1){
				if((addr>3)&&(Table[addr-4]==0xA)&&(Table[addr-3]==0xA)){Move_Block(i,Up,0);}
				if((addr<16)&&(Table[addr+4]==0xA)&&(Table[addr+5]==0xA)){Move_Block(i,Down,0);}
				if(((addr%4)!=0)&&(Table[addr-1]==0xA)){
					Move_Block(i,Left,0);
					if(((addr%4)!=1)&&(Table[addr-2]==0xA)){Move_Block(i,Left,Left);}
				}
				if(((addr%4)!=2)&&(Table[addr+2]==0xA)){
					Move_Block(i,Right,0);
					if(((addr%4)!=1)&&(Table[addr+3]==0xA)){Move_Block(i,Right,Right);}
				}
			}
			else if(Block[i].Style==2){
				if((addr>3)&&(Table[addr-4]==0xA)){
					Move_Block(i,Up,0);
					if((addr>7)&&(Table[addr-8]==0xA)){Move_Block(i,Up,Up);}
				}
				if((addr<12)&&(Table[addr+8]==0xA)){
					Move_Block(i,Down,0);
					if((addr<8)&&(Table[addr+12]==0xA)){Move_Block(i,Down,Down);}
				}
				if(((addr%4)!=0)&&(Table[addr-1]==0xA)&&(Table[addr+3]==0xA)){Move_Block(i,Left,0);}
				if(((addr%4)!=3)&&(Table[addr+1]==0xA)&&(Table[addr+5]==0xA)){Move_Block(i,Right,0);}
			}
			else if(Block[i].Style==3){
				if((addr>3)&&(Table[addr-4]==0xA)){
					Move_Block(i,Up,0);
					if((addr>7)&&(Table[addr-8]==0xA)){Move_Block(i,Up,Up);}
					if(((addr%4)!=0)&&(Table[addr-5]==0xA)){Move_Block(i,Up,Left);}
					if(((addr%4)!=3)&&(Table[addr-3]==0xA)){Move_Block(i,Up,Right);}
				}
				if((addr<16)&&(Table[addr+4]==0xA)){
					Move_Block(i,Down,0);
					if((addr<12)&&(Table[addr+8]==0xA)){Move_Block(i,Down,Down);}
					if(((addr%4)!=0)&&(Table[addr+3]==0xA)){Move_Block(i,Down,Left);}
					if(((addr%4)!=3)&&(Table[addr+5]==0xA)){Move_Block(i,Down,Right);}
				}
				if(((addr%4)!=0)&&(Table[addr-1]==0xA)){
					Move_Block(i,Left,0);
					if(((addr%4)!=1)&&(Table[addr-2]==0xA)){Move_Block(i,Left,Left);}
					if((addr>3)&&(Table[addr-5]==0xA)){Move_Block(i,Left,Up);}
					if((addr<16)&&(Table[addr+3]==0xA)){Move_Block(i,Left,Down);}
				}
				if(((addr%4)!=3)&&(Table[addr+1]==0xA)){
					Move_Block(i,Right,0);
					if(((addr%4)!=2)&&(Table[addr+2]==0xA)){Move_Block(i,Right,Right);}
					if((addr>3)&&(Table[addr-3]==0xA)){Move_Block(i,Right,Up);}
					if((addr<16)&&(Table[addr+5]==0xA)){Move_Block(i,Right,Down);}
				}
			}
        }
    }
}
unsigned int HRD_Calculate::Get_Code(){
	bool temp[20];
	unsigned int Code=0;
	unsigned char i,addr,style;
	for(i=0;i<20;i++){temp[i]=false;}
	temp[Block[0].Address]=temp[Block[0].Address+1]=temp[Block[0].Address+4]=temp[Block[0].Address+5]=true;
	Code=Code|(Block[0].Address<<24);
	addr=0;
	for(i=1;i<=11;i++){
		while(temp[addr]==true){if(addr<19){addr++;}else{return 0;}}
		if(Table[addr]==0xA){temp[addr]=true;}
		else{
			style=Block[Table[addr]].Style;
			if(style==1){
				temp[addr]=temp[addr+1]=true;
				Code=Code|(1<<(24-i*2));
			}
			else if(style==2){
				temp[addr]=temp[addr+4]=true;
				Code=Code|(2<<(24-i*2));
			}
			else if(style==3){
				temp[addr]=true;
				Code=Code|(3<<(24-i*2));
			}
		}
	}
	return Code;
}
void HRD_Calculate::Analyse_Code(unsigned int Code){
	unsigned char i,addr,style;
	unsigned char Type_1=0,Type_2=0,Type_3=5;
	for(i=0;i<20;i++){Table[i]=0xFF;}
	Block[0].Address=(Code>>24)&0xF;
	if(Block[0].Address>14){goto err;}
	Block[0].Style=0;
	Table[(Code>>24)&0xF]=0;
	Table[((Code>>24)&0xF)+1]=0;
	Table[((Code>>24)&0xF)+4]=0;
	Table[((Code>>24)&0xF)+5]=0;
	addr=0;
	for(i=0;i<11;i++){
		while(Table[addr]!=0xFF){if(addr<19){addr++;}else{break;}}
		style=(Code>>(22-i*2))&0x3;
		if(style==0){
			Table[addr]=0xA;
			Space[Type_1]=addr;
			if(Type_1==0){Type_1=1;}
		}
		if(style==1){
			if(Type_2<5){Type_2++;}
			if(addr>18){goto err;}
			Table[addr]=Type_2;
			Table[addr+1]=Type_2;
			Block[Type_2].Address=addr;
			Block[Type_2].Style=1;
		}
		if(style==2){
			if(Type_2<5){Type_2++;}
			if(addr>15){goto err;}
			Table[addr]=Type_2;
			Table[addr+4]=Type_2;
			Block[Type_2].Address=addr;
			Block[Type_2].Style=2;
		}
		if(style==3){
			if(Type_3<9){Type_3++;}
			Table[addr]=Type_3;
			Block[Type_3].Address=addr;
			Block[Type_3].Style=3;
		}
	}
	err:;
}
bool HRD_Calculate::Check(unsigned int Code){
	bool temp[20];
	unsigned char addr,i;
	unsigned char Style_0,Style_1,Style_2,Style_3;
	Style_0=Style_1=Style_2=Style_3=0;
	Analyse_Code(Code);
	for(i=0;i<20;i++){temp[i]=false;}
	for(i=0;i<20;i++){if(Table[i]>10){return false;}}
	if(Space[0]>19){return false;}else{if(temp[Space[0]]==true){return false;}else{temp[Space[0]]=true;}}
	if(Space[1]>19){return false;}else{if(temp[Space[1]]==true){return false;}else{temp[Space[1]]=true;}}
	for(i=0;i<=9;i++){
		addr=Block[i].Address;
		if(Block[i].Style==0){
			Style_0++;
			if(addr>14){return false;}
			if((addr%4)==3){return false;}
			if(temp[addr]==true||temp[addr+1]==true||temp[addr+4]==true||temp[addr+5]==true){return false;}
			temp[addr]=true;
			temp[addr+1]=true;
			temp[addr+4]=true;
			temp[addr+5]=true;
		}
		if(Block[i].Style==1){
			Style_1++;
			if(addr>18){return false;}
			if((addr%4)==3){return false;}
			if(temp[addr]==true||temp[addr+1]==true){return false;}
			temp[addr]=true;
			temp[addr+1]=true;
		}
		if(Block[i].Style==2){
			Style_2++;
			if(addr>15){return false;}
			if(temp[addr]==true||temp[addr+4]==true){return false;}
			temp[addr]=true;
			temp[addr+4]=true;
		}
		if(Block[i].Style==3){
			Style_3++;
			if(addr>19){return false;}
			if(temp[addr]==true){return false;}
			temp[addr]=true;
		}
	}
	if(Style_0!=1){return false;}
	if((Style_1+Style_2)!=5){return false;}
	if(Style_3!=4){return false;}
	if(Block[0].Style!=0){return false;}
	if(Table[Block[0].Address]!=0){return false;}
	if(Table[Block[0].Address+1]!=0){return false;}
	if(Table[Block[0].Address+4]!=0){return false;}
	if(Table[Block[0].Address+5]!=0){return false;}
	for(i=1;i<=5;i++){
		if(Block[i].Style==1){
			if(Table[Block[i].Address]!=i){return false;}
			if(Table[Block[i].Address+1]!=i){return false;}
		}
		else if(Block[i].Style==2){
			if(Table[Block[i].Address]!=i){return false;}
			if(Table[Block[i].Address+4]!=i){return false;}
		}
		else{return false;}
	}
	for(i=6;i<=9;i++){
		if(Table[Block[i].Address]!=i){return false;}
		if(Block[i].Style!=3){return false;}
	}
	if(Table[Space[0]]!=0xA){return false;}
	if(Table[Space[1]]!=0xA){return false;}
	return true;
}
void HRD_Calculate::Output_Graph(unsigned int Code){
	unsigned int i,k,m;
	unsigned int Start_x,Start_y,End_x,End_y;
	bool Graph[(Output_Width+1)*4+1][(Output_Width+1)*5+1];
	if(Check(Code)==false){cout<<"The Code is Wrong!"<<endl;return;}
	Analyse_Code(Code);
	for(k=0;k<=(Output_Width+1)*5;k++){
		for(i=0;i<=(Output_Width+1)*4;i++){Graph[i][k]=false;}
	}
	for(m=0;m<=9;m++){
		Start_x=(Block[m].Address%4)*(Output_Width+1)+1;
		Start_y=Block[m].Address/4;
		Start_y=Start_y*(Output_Width+1)+1;
		if((Block[m].Style==1)||(Block[m].Style==0)){End_x=Start_x+Output_Width*2;}
		else{End_x=Start_x+Output_Width-1;}
		if((Block[m].Style==2)||(Block[m].Style==0)){End_y=Start_y+Output_Width*2;}
		else{End_y=Start_y+Output_Width-1;}
		for(i=Start_x;i<=End_x;i++){
			for(k=Start_y;k<=End_y;k++){Graph[i][k]=true;}
		}
	}
	for(i=1;i<=(Output_Width+1)*4+3;i++){cout<<Square_str;}
	cout<<endl<<Square_str;
	for(k=0;k<=(Output_Width+1)*5;k++){
		for(i=0;i<=(Output_Width+1)*4;i++){
			if(Graph[i][k]==false){cout<<"  ";}else{cout<<Square_str;}
		}
		cout<<Square_str<<endl<<Square_str;
	}
	for(i=1;i<=(Output_Width+1)*4+2;i++){cout<<Square_str;}
	cout<<endl;
}
unsigned int HRD_Calculate::Change_int(char Data_str[7]){
	unsigned int i,Dat=0;
	for(i=0;i<7;i++){
		if(Data_str[i]=='0'){Dat=Dat|(0x0<<(24-i*4));}
		if(Data_str[i]=='1'){Dat=Dat|(0x1<<(24-i*4));}
		if(Data_str[i]=='2'){Dat=Dat|(0x2<<(24-i*4));}
		if(Data_str[i]=='3'){Dat=Dat|(0x3<<(24-i*4));}
		if(Data_str[i]=='4'){Dat=Dat|(0x4<<(24-i*4));}
		if(Data_str[i]=='5'){Dat=Dat|(0x5<<(24-i*4));}
		if(Data_str[i]=='6'){Dat=Dat|(0x6<<(24-i*4));}
		if(Data_str[i]=='7'){Dat=Dat|(0x7<<(24-i*4));}
		if(Data_str[i]=='8'){Dat=Dat|(0x8<<(24-i*4));}
		if(Data_str[i]=='9'){Dat=Dat|(0x9<<(24-i*4));}
		if(Data_str[i]=='A'||Data_str[i]=='a'){Dat=Dat|(0xA<<(24-i*4));}
		if(Data_str[i]=='B'||Data_str[i]=='b'){Dat=Dat|(0xB<<(24-i*4));}
		if(Data_str[i]=='C'||Data_str[i]=='c'){Dat=Dat|(0xC<<(24-i*4));}
		if(Data_str[i]=='D'||Data_str[i]=='d'){Dat=Dat|(0xD<<(24-i*4));}
		if(Data_str[i]=='E'||Data_str[i]=='e'){Dat=Dat|(0xE<<(24-i*4));}
		if(Data_str[i]=='F'||Data_str[i]=='f'){Dat=Dat|(0xF<<(24-i*4));}
	}
	return Dat;
}
string HRD_Calculate::Change_str(unsigned int Data_int){
	unsigned char i,bit;
	string Dat="";
	for(i=0;i<7;i++){
		bit=(0xF&Data_int>>(6-i)*4);
		if(bit==0x0){Dat+='0';}
		if(bit==0x1){Dat+='1';}
		if(bit==0x2){Dat+='2';}
		if(bit==0x3){Dat+='3';}
		if(bit==0x4){Dat+='4';}
		if(bit==0x5){Dat+='5';}
		if(bit==0x6){Dat+='6';}
		if(bit==0x7){Dat+='7';}
		if(bit==0x8){Dat+='8';}
		if(bit==0x9){Dat+='9';}
		if(bit==0xA){Dat+='A';}
		if(bit==0xB){Dat+='B';}
		if(bit==0xC){Dat+='C';}
		if(bit==0xD){Dat+='D';}
		if(bit==0xE){Dat+='E';}
		if(bit==0xF){Dat+='F';}
	}
	return Dat;
}
