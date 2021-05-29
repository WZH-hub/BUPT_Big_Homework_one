#ifndef __INPUT_H__
#define __INPUT_H__


#define deta 256 
#define address1 0   //第一个核心的起始地址 
#define address2 256 //第二个核心的起始地址 
typedef unsigned char BYTE;
extern HANDLE mlock,plock;

extern short ip[3],flag[3];//程序计数器和标志寄存器
extern short normal_register[3][9],ir[3];//通用寄存器，1234，5678 和指令寄存器 
BYTE memory[32768];//这个是内存 ,遇见大于127的数，先-256 

int loadByte(FILE* fin);//读8位，转化为十进制 ;
int read_instructions(char* name,int id);//载入指令到内存中，返回使用的字节数i; 

#endif
