#ifndef __OUTPUT_END_H__
#define __OUTPUT_END_H__


#define deta 256 
#define address1 0   //第一个核心的起始地址 
#define address2 256 //第二个核心的起始地址 
typedef unsigned char BYTE;
extern HANDLE mlock,plock;

extern short ip[3],flag[3];//程序计数器和标志寄存器
extern short normal_register[3][9],ir[3];//通用寄存器，1234，5678 和指令寄存器 
BYTE memory[32768];//这个是内存 ,遇见大于127的数，先-256 

void print_code();
void print_data();

#endif
