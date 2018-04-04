

#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstdio>
#include<map>
#include<climits>
#include <vector>

using namespace std;

#include "optab.cpp"
// include the opcode table in the assembler to check the valid opcode

typedef string hexa;

hexa extendTo(int digit,hexa a)
{
    hexa temp="";
    for(int i=0;i<digit-a.length();++i)
        temp+='0';
    return temp+a;
}

int toDecDigit(char a)
{
    if(a=='0') return 0;
    if(a=='1') return 1;
    if(a=='2') return 2;
    if(a=='3') return 3;
    if(a=='4') return 4;
    if(a=='5') return 5;
    if(a=='6') return 6;
    if(a=='7') return 7;
    if(a=='8') return 8;
    if(a=='9') return 9;
    if(a=='A') return 10;
    if(a=='B') return 11;
    if(a=='C') return 12;
    if(a=='D') return 13;
    if(a=='E') return 14;
    if(a=='F') return 15;
}


char toHexDigit(int a)
{
    if(a==0) return '0';
    if(a==1) return '1';
    if(a==2) return '2';
    if(a==3) return '3';
    if(a==4) return '4';
    if(a==5) return '5';
    if(a==6) return '6';
    if(a==7) return '7';
    if(a==8) return '8';
    if(a==9) return '9';
    if(a==10) return 'A';
    if(a==11) return 'B';
    if(a==12) return 'C';
    if(a==13) return 'D';
    if(a==14) return 'E';
    if(a==15) return 'F';
}

string toHex(int a)
{
    string res="";
    while(a>0)
    {
        res=toHexDigit(a%16)+res;
        a/=16;
    }
    return res;
}


int toDec(string val)
{
    int l=val.length()-1,curr=1,res=0;
    while(l>=0)
    {
        res+=(toDecDigit(val[l])*curr);
        curr*=16;
        --l;
    }
    return res;
}



void fetch(string a,string word[],int& count)
{
    int i;
    for(i=0;i<5;++i) word[i]="";
    count=0;
    i=0;
    while((a[i]==' '||a[i]=='\t')&&i<a.length()) {++i;continue;}
    if(i==a.length()||a[i]=='.')   return;
    for(;i<a.length();)
    {
        while((a[i]==' '||a[i]=='\t')&&i<a.length()) {++i;continue;}
        if(i==a.length()) break;
        for(;(!(a[i]==' '||a[i]=='\t'))&&i<a.length();++i) word[count]+=a[i];
        ++count;
    }
}

int block_num,line;
//to store the block number of the block and the current line number in the instrucation
hexa pc;
//to store the program counter
string curr_block;
//to store the current block name from the find_block function in optab.cpp
bool error_flag=0;

ifstream fin;
//input file stream to take the input from the input.txt
ofstream fout,error;
//output file stream to give the output to the intermediate.txt and error.txt
void execute(string[],int);
//declare the execute function

void pass1()
{
    char ch;
    string str,word[5];
    int count=0;
    create();
    // call the create function for generating the opcode table
    fin.open("input.txt");
    fout.open("intermediate.txt");
    error.open("error.txt");

    line=5;
    //line number of the first instruction
    getline(fin,str);
    //get 
    fetch(str,word,count);
    //store the current string line in word array
    while(count==0)
    {
        fout<<line<<endl;
        fout<<"$";
        fout<<str<<endl;
        fout<<""<<endl;
        fout<<""<<endl;
        line+=5;
        cout<<str<<endl;
    }
    
    curr_block="DEFAULT";
    block_num=1;
    line=5;
    pc="0";
    BLOCK["DEFAULT"].num=0;
    BLOCK["DEFAULT"].address=pc;
    BLOCK["DEFAULT"].length="0";
    BLOCK["DEFAULT"].exist='y';

    if(word[1]=="START")
    {
        pc=word[2];
        fout<<line<<endl;
        fout<<word[0]<<endl;
        fout<<"START"<<endl;
        fout<<pc<<endl;
        fout<<pc<<endl;
        fout<<""<<endl;
    }
    
    else if(word[0]=="START")
    {
        pc=word[1];
        fout<<line<<endl;
        fout<<""<<endl;
        fout<<"START"<<endl;
        fout<<pc<<endl;
        fout<<pc<<endl;
        fout<<""<<endl;
    
    }
    else
        execute(word,count);
    while(true)
    {
        getline(fin,str);
        fetch(str,word,count);
        if(word[0]=="LTORG")
            line+=3;
        // increase the line number by 3 in case of LTORG
        //it is used for literals in case of pc relative addressing mode
        else if(word[1].find('=')==0)
            line+=2;
        else
        line+=5;
        cout<<str<<endl;
        if(word[1].find('=')!=0)
            fout<<line<<endl;
        else
            fout<<endl;
        if(count==0)
        {
            cout<<"Comment detected!"<<endl;
            fout<<"$"<<endl;
            fout<<str<<endl;
            fout<<""<<endl;
            fout<<""<<endl;
            fout<<""<<endl;
            continue;
        }
        if(word[0]=="END")
        {
            BLOCK[curr_block].length=pc;
            fout<<""<<endl;
            fout<<word[0]<<endl;
            fout<<word[1]<<endl;
            fout<<pc<<endl;
            fout<<""<<endl;
            break;
        }
        execute(word,count);
       }
     hexa addr,len;
     string temp=eval_block(0);
     addr=BLOCK[temp].address;
     len=BLOCK[temp].length;
     

     for(int i=1;i<block_num;++i)
     {
            temp=eval_block(i);
            BLOCK[temp].address=toHex(toDec(addr)+toDec(len));
            addr=BLOCK[temp].address;
            len=BLOCK[temp].length;

     }
}

void execute(string word[],int count)
{
    cout<<"word[0]: "<<endl;
    cout<<"pc:  "<<pc<<endl;
    if(word[0]=="USE")
    {
        cout<<"USE detected!"<<endl;
        BLOCK[curr_block].length=pc;
        if(BLOCK[word[1]].exist=='y')
        {
            curr_block=word[1];
            pc=BLOCK[word[1]].length;
        }
        else if(word[1]=="")
        {
            curr_block="DEFAULT";
            pc=BLOCK["DEFAULT"].length;
        }
        else
        {
            BLOCK[word[1]].num=block_num;
            BLOCK[word[1]].exist='y';
            BLOCK[word[1]].length="0";
            curr_block=word[1];
            pc="0";
            ++block_num;
        }
        fout<<""<<endl;
        fout<<word[0]<<endl;
        fout<<word[1]<<endl;
        fout<<pc<<endl;
        fout<<""<<endl;
        return;
    }
    if(word[0][0]=='+')
    {
        cout<<"Format 4"<<endl;
        fout<<""<<endl;
        fout<<word[0]<<endl;
        fout<<word[1]<<endl;
        fout<<pc<<endl;
        pc=toHex(toDec(pc)+4);
        fout<<pc<<endl;
        return;
    }
    if(OPTAB[word[0]].exist=='y')
    {
        cout<<"0 is opcode!"<<endl;
        fout<<""<<endl;
        fout<<word[0]<<endl;
        fout<<word[1]<<endl;
        fout<<pc<<endl;
        pc=toHex(toDec(pc)+OPTAB[word[0]].format);
        fout<<pc<<endl;
        return;
    }
    if(OPTAB[word[0]].exist=='n')
    {
        if(SYMTAB[word[0]].exist=='y')
        {
            error<<"Line "<<line<<": Duplicate Symbol"<<endl;
            error_flag=1;
        }
        else if(word[0]=="*")
        {
            SYMTAB[word[1]].address=pc;
            SYMTAB[word[1]].block=curr_block;
            SYMTAB[word[1]].exist='y';
            fout<<word[0]<<endl;
            fout<<word[1]<<endl;
            fout<<word[2]<<endl;
            fout<<pc<<endl;
            if(word[1][0]=='+')
                pc=toHex(toDec(pc)+4);
            else if(OPTAB[word[1]].exist=='y')
                pc=toHex(toDec(pc)+OPTAB[word[1]].format);
            else if(word[1]=="WORD")      pc=toHex(toDec(pc)+3);
            else if(word[1]=="RESW")      pc=toHex(toDec(pc)+(atoi(word[2].c_str())*3));
            else if(word[1]=="RESB")      pc=toHex(toDec(pc)+atoi(word[2].c_str()));
            else if(word[1]=="BYTE")
            {
                 int len=word[1].length()-3;
                 if(word[1][0]=='X') len/=2;
                 pc=toHex(toDec(pc)+len);
            }
            else if(word[0]=="LTORG")
            {
                
            }   
            else if(word[1].find('=')==0)
            {
                  pc=toHex(toDec(pc)+3);      
            } 
            else if(word[1].find('*')==0)
            {
            SYMTAB[word[0]].address=pc;
            SYMTAB[word[0]].block=curr_block;
            SYMTAB[word[0]].exist='y';
            fout<<word[0]<<endl;
            fout<<word[1]<<endl;
            fout<<word[2]<<endl;
            fout<<pc<<endl;
            if(word[1][0]=='+')
                pc=toHex(toDec(pc)+4);
            else if(OPTAB[word[1]].exist=='y')
                pc=toHex(toDec(pc)+OPTAB[word[1]].format);
            else if(word[1]=="WORD")      
                pc=toHex(toDec(pc)+3);
            else if(word[1]=="RESW")      
                pc=toHex(toDec(pc)+(atoi(word[2].c_str())*3));
            else if(word[1]=="RESB")      
                pc=toHex(toDec(pc)+atoi(word[2].c_str()));
            else if(word[1]=="BYTE")
            {
                 int len=word[1].length()-3;
                 if(word[1][0]=='X') len/=2;
                 pc=toHex(toDec(pc)+len);
            }
            else if(word[0]=="LTORG")
            {
                
            }   
            else if(word[1].find('=')==0)
            {
                  pc=toHex(toDec(pc)+3);      
            } 
            else
            {
                error<<"Line "<<line<<": Opcode not found"<<endl;
                error_flag=1;
            }
           if(word[0]!="LTORG")
            fout<<pc<<endl;
            else
                fout<<endl;
            }
            else
            {
                error<<"Line "<<line<<": Opcode not found"<<endl;
                error_flag=1;
            }
           if(word[0]!="LTORG")
            fout<<pc<<endl;
            else
                fout<<endl;
        }
        else
        {
            SYMTAB[word[0]].address=pc;
            SYMTAB[word[0]].block=curr_block;
            SYMTAB[word[0]].exist='y';
            fout<<word[0]<<endl;
            fout<<word[1]<<endl;
            fout<<word[2]<<endl;
            fout<<pc<<endl;
            if(word[1][0]=='+')
                pc=toHex(toDec(pc)+4);
            else if(OPTAB[word[1]].exist=='y')
                pc=toHex(toDec(pc)+OPTAB[word[1]].format);
            else if(word[1]=="WORD")      pc=toHex(toDec(pc)+3);
            else if(word[1]=="RESW")      pc=toHex(toDec(pc)+(atoi(word[2].c_str())*3));
            else if(word[1]=="RESB")      pc=toHex(toDec(pc)+atoi(word[2].c_str()));
            else if(word[1]=="BYTE")
            {
                 int len=word[1].length()-3;
                 if(word[1][0]=='X') len/=2;
                 pc=toHex(toDec(pc)+len);
            }
            else if(word[0]=="LTORG")
            {
                
            }   
            else if(word[1].find('=')==0)
            {
                  pc=toHex(toDec(pc)+3);      
            } 
            else
            {
                error<<"Line "<<line<<": Opcode not found"<<endl;
                error_flag=1;
            }
           if(word[0]!="LTORG")
            fout<<pc<<endl;
            else
                fout<<endl;
        }
    }
}


//Pass2 Assembler

ofstream obj,lst;
ifstream intm;
int curr_block_num;
vector<string> modVector;
bool imm,ind;

void input(string a[])
{
    int i;
    for(i=1;i<7;++i)
    getline(intm,a[i-1]);
    cout<<a[0]<<endl;
    for(i=1;i<6;++i)
    cout<<a[i]<<endl;
}

void assemblepass2(string[]);
string generatecode(string[]);

string texts="",texte="";
int textlength=0,base=INT_MAX;

int main()
{
    pass1();
    string a[6];
    char ch;
    hexa start;
    if(error_flag)
    {
        cout<<"Errors encountered! Listing file not prepared!"<<endl;
        
        exit(1);
    }
    intm.open("intermediate.txt");
    obj.open("object.txt");
    lst.open("list.txt");
    lst<<"Line\t\tLoc  Block\t   Source Statement\t\t\t\tObjectCode"<<endl;
    lst<<"========================================================================================="<<endl<<endl;
    input(a);
    curr_block="DEFAULT";
    curr_block_num=0;
    while(a[1]=="$")
    {
        lst<<a[0]<<"\t\t "<<a[2]<<endl;
        input(a);
    }
    if(a[2]!="START")
    {
        error_flag=1;
        error<<"Intermediate File has no start statement!"<<endl;
    }
    else
    {
        lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"  "<<curr_block_num<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
        obj<<"H^";
        int i;
        for(i=0;i<a[1].length();++i)
            obj<<a[1][i];
        for(;i<6;++i)
            obj<<" ";
        string temp=eval_block(block_num-1);
        cout<<block_num<<endl;
        cout<<temp<<endl;
        hexa len;
        len=toHex(toDec(BLOCK[temp].address)+toDec(BLOCK[temp].length));
        obj<<"^"<<extendTo(6,a[3])<<"^"<<extendTo(6,len)<<endl;
        start=a[3];

    }
    while(true)
    {
        input(a);
        if(a[1]=="$")
        {
            lst<<a[0]<<"\t\t "<<a[2]<<endl;
            continue;
        }
        if(a[2]=="END")
        {
            lst<<a[0]<<"\t\t\t\t\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
            if(textlength>0)
            {
                obj<<texts<<"^"<<extendTo(2,toHex(textlength/2))<<texte<<endl;
                cout<<"!!"<<endl;
            }
            textlength=0;
            texts="";
            texte="";
            for (unsigned i=0; i<modVector.size(); i++)
            //entering the modification record from the modVector in the object.txt
                obj<< modVector.at(i)<<endl;

            obj<<"E^"<<extendTo(6,start)<<endl;
            break;
        }
       // cout<<a[2]<<endl;
        assemblepass2(a);
        cout<<"opcode: "<<a[2]<<":::";
       // cin>>ch;
    }
    if(error_flag)
    {
        cout<<"Errors found. Listing file not prepared!"<<endl;
        
    }
    else
    {
        cout<<"The Input File Is Assembled Successfully!"<<endl;
    }
}

void assemblepass2(string a[])
{
    string object_code;hexa loc_ctr;    
    int format;
    if(a[2]=="USE")
    {
        curr_block=a[3];
        curr_block_num=BLOCK[curr_block].num;
        lst<<a[0]<<"\t\t0000  "<<curr_block_num<<"\t\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
        if(textlength>0) 
            obj<<texts<<"^"<<extendTo(2,toHex(textlength/2))<<texte<<endl;
        texts="";
        texte="";
        textlength=0;
        return;
    }
    if(a[2]=="RESB"||a[2]=="RESW")
    {
        lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"  "<<curr_block_num<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
        if(textlength>0) 
            obj<<texts<<"^"<<extendTo(2,toHex(textlength/2))<<texte<<endl;
        texts="";
        texte="";
        textlength=0;
        return;
    }
    imm=ind=false;
    object_code=generatecode(a);
    cout<<"a[2]: "<<a[2]<<":::"<<object_code<<endl;
    if(a[2]=="BYTE"||a[2]=="WORD")
        lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"  "<<curr_block_num<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<"\t\t"<<object_code<<endl;
    else
    {
        if(imm)      
            lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"  "<<curr_block_num<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t#"<<a[3]<<"\t\t"<<object_code<<endl;
        else if(ind)
            lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"  "<<curr_block_num<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t@"<<a[3]<<"\t\t"<<object_code<<endl;
        else
            lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"  "<<curr_block_num<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<"\t\t"<<object_code<<endl;
    }
    if(texts=="")
    {
        loc_ctr=toHex(toDec(BLOCK[curr_block].address)+toDec(a[4]));
        texts="T^"+extendTo(6,loc_ctr);
        texte="^"+object_code;
        textlength=object_code.length();
    }
    else if(textlength+object_code.length()>60)
    {
        obj<<texts<<"^"<<extendTo(2,toHex(textlength/2))<<texte<<endl;
        loc_ctr=toHex(toDec(BLOCK[curr_block].address)+toDec(a[4]));
        texts="T^"+extendTo(6,loc_ctr);
        texte="^"+object_code;
        textlength=object_code.length();
    }
    else
    {
        texte+="^"+object_code;
        textlength+=object_code.length();
    }
    if(a[2]=="LDB")
    {
        base=toDec(SYMTAB[a[3]].address)+toDec(BLOCK[SYMTAB[a[3]].block].address);
    }
}

string generatecode(string a[])
{
    string ob1,ob2,ob3;
    hexa operand_addr,prgm_ctr;
    int format;
    if(a[2].find("=")==0)
    {
        int i;
        ob1="";
        if(a[2][1]=='X')
            for(i=3;i<a[2].length()-1;++i) ob1+=a[2][i];
        else //a[3][0]=='C'
            for(i=3;i<a[2].length()-1;++i)
                ob1+=toHex((int)a[2][i]);
        return ob1;
    }

    if(a[2]=="BYTE")
    {
        int i;
        ob1="";
        if(a[3][0]=='X')
            for(i=2;i<a[3].length()-1;++i) ob1+=a[3][i];
        else 
            for(i=2;i<a[3].length()-1;++i)
                ob1+=toHex((int)a[3][i]);
        return ob1;
    }
    if(a[2]=="RSUB")
    {
        ob1="4F0000";
        return ob1;
    }
    if(a[2]=="+RSUB")
    {
        ob1="4F000000";
        return ob1;
    }
    if(a[2]=="WORD")
    {
        ob1=toHex(atoi(a[3].c_str()));
        return ob1;
    }
    
    if(a[2][0]=='+')
    {
        format=4;
        a[2]=a[2].substr(1);
    }
    else
        format=OPTAB[a[2]].format;
    if(format==1)
    {
        cout<<"Format 1"<<endl;
        ob1=OPTAB[a[2]].opcode;
        return ob1;
    }
    if(format==2)
    {
        cout<<"Format 2"<<endl;
        ob1=OPTAB[a[2]].opcode;
        if(a[3].length()==3)
        {
            ob2=toHex(register_no(a[3][0]));
            if(isdigit(a[3][2]))  ob2=ob2+toHexDigit(a[3][2]-1);
            else
            {
                ob2=ob2+toHexDigit(register_no(a[3][2]));
            }
        }
        else  //a[3].length==1
        {
            if(isdigit(a[3][0]))
            {
                ob2=toHex(atoi(a[3].c_str()))+"0";
                cout<<"isdigit! ob2: "<<ob2<<endl;
            }
            else
            {
                cout<<toHex(register_no(a[3][0]))<<endl;
                ob2=toHex(register_no(a[3][0]))+"0";
                cout<<"Not Isdigit! ob2: "<<ob2<<endl;
            }
        }
        cout<<"a[2]: "<<a[2]<<" ob1:"<<ob1<<"ob2:"<<ob2<<endl;
        return (ob1+extendTo(2,ob2));
    }
    if(format==3)
    {
        cout<<"Format 3"<<endl;
        cout<<a[2]<<endl;
        ob1=OPTAB[a[2]].opcode;
        if(a[3][0]=='#')
        {
            imm=true;
            cout<<"Immediate"<<endl;
            ob1=toHex(toDec(ob1)+1);
            a[3]=a[3].substr(1);
            if(isdigit(a[3][0]))
            {
                ob2="0";
                ob3=toHex(atoi(a[3].c_str()));
                return extendTo(2,ob1)+ob2+extendTo(3,ob3);
            }
            //cout<<"ob1: "<<ob1<<endl;
        }
        else if(a[3][0]=='@')
        {
            ind=true;
            cout<<"Indirect!"<<endl;
            ob1=toHex(toDec(ob1)+2);
            a[3]=a[3].substr(1);
        }
        else
            ob1=toHex(toDec(ob1)+3);
        ob2="0";
        bool x=false;
        if(a[3][a[3].length()-2]==',')
        {
            x=true;
            ob2=toHex(toDec(ob2)+8);
            a[3]=a[3].substr(0,a[3].length()-2);
        }
        //cout<<"ob1:"<<ob1<<"ob2:"<<ob2<<endl;
        prgm_ctr=toHex(toDec(BLOCK[curr_block].address)+toDec(a[5]));
        operand_addr=toHex(toDec(SYMTAB[a[3]].address)+toDec(BLOCK[SYMTAB[a[3]].block].address));
        cout<<"prgm_ctr: "<<prgm_ctr<<" operand_addr: "<<operand_addr<<endl;
        if(x) a[3]+=",X";
        int disp=toDec(operand_addr)-toDec(prgm_ctr);
        cout<<"disp: "<<disp<<endl;
        if(disp>=-2048 && disp<2048)
        {
            ob2=toHex(toDec(ob2)+2);
            if(disp<0) disp+=4096;
            ob3=toHex(disp);
            return extendTo(2,ob1)+extendTo(1,ob2)+extendTo(3,ob3);
        }
        disp=toDec(operand_addr)-base;
        if(disp>=-2048 && disp<2048)
        {
            ob2=toHex(toDec(ob2)+4);
            if(disp<0) disp+=4096;
            ob3=toHex(disp);
            return extendTo(2,ob1)+extendTo(1,ob2)+extendTo(3,ob3);
        }
        //If still here, means overflow
        error_flag=1;
        error<<"Line "<<a[0]<<": Overflow "<<endl;
    }
    if(format==4)
    {
        ob1=OPTAB[a[2]].opcode;
        if(a[3][0]=='#')
        {
            imm=true;
            ob1=toHex(toDec(ob1)+1);
            a[3]=a[3].substr(1);
            if(isdigit(a[3][0]))
            {
                ob2="0";
                ob3=toHex(atoi(a[3].c_str()));
                a[2]="+"+a[2];
                return ob1+ob2+extendTo(5,ob3);
            }
        }
        else if(a[3][0]=='@')
        {
            ind=true;
            ob1=toHex(toDec(ob1)+2);
            a[3]=a[3].substr(1);
        }
        else
            ob1=toHex(toDec(ob1)+3);
        bool x=false;
        ob2="1";
        if(a[3][a[3].length()-2]==',')
        {
            x=true;
            ob2=toHex(toDec(ob2)+8);
            a[3]=a[3].substr(0,a[3].length()-2);
        }
        operand_addr=toHex(toDec(SYMTAB[a[3]].address)+toDec(BLOCK[SYMTAB[a[3]].block].address));
        if(x) a[3]+=",X";
        ob3=operand_addr;
        a[2]="+"+a[2];
        hexa loc_ctr=toHex(toDec(BLOCK[curr_block].address)+toDec(a[4])+1);
        modVector.push_back("M^"+extendTo(6,loc_ctr)+"^05");
        return extendTo(2,ob1)+extendTo(1,ob2)+extendTo(5,ob3);
    }
}

