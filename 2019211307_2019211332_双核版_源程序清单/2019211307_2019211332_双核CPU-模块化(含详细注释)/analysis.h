#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__


#define deta 256 
#define address1 0   //第一个核心的起始地址 
#define address2 256 //第二个核心的起始地址 
typedef unsigned char BYTE;
extern HANDLE mlock,plock;

extern short ip[3],flag[3];//程序计数器和标志寄存器
extern short normal_register[3][9],ir[3];//通用寄存器，1234，5678 和指令寄存器 
BYTE memory[32768];//这个是内存 ,遇见大于127的数，先-256 

void stop(int i,int id);//停机指令，0
void trans(int i,int id);//数据传送指令，1 
void calcul(int i,int id);//算数运算指令，2345，+-*/  
void logic(int i,int id);//逻辑运算指令，678，逻辑与，逻辑或，逻辑非
void compare(int i,int id);//比较指令，9，
void jump(int i,int id);//跳转指令，10，
void in_out(int i,int id);//输入输出指令 ，11，12
void ask_for_lock(int i,int id);//请求互斥对象，13 
void release(int i,int id);//释放互斥对象，14
void sleep(int i,int id);//休眠，15

short change(int i);//内存中连着两个的数（十进制表示的）转化为十进制的数 
void ten_to_two(short m,short num);//把num变为二进制，转变为两个十进制，并保存在memory[m]和[m+1] 
short four_situ(int num,short m,short n);//加减乘除 
short three_logic(int num,short m,short n);//与或非，
void comp_num(short m,short n,int id);//比较大小，修改flag 


#endif
