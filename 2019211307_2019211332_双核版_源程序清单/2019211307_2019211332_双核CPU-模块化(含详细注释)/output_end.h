#ifndef __OUTPUT_END_H__
#define __OUTPUT_END_H__


#define deta 256 
#define address1 0   //��һ�����ĵ���ʼ��ַ 
#define address2 256 //�ڶ������ĵ���ʼ��ַ 
typedef unsigned char BYTE;
extern HANDLE mlock,plock;

extern short ip[3],flag[3];//����������ͱ�־�Ĵ���
extern short normal_register[3][9],ir[3];//ͨ�üĴ�����1234��5678 ��ָ��Ĵ��� 
BYTE memory[32768];//������ڴ� ,��������127��������-256 

void print_code();
void print_data();

#endif
