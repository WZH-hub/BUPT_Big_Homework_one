#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__


#define deta 256 
#define address1 0   //��һ�����ĵ���ʼ��ַ 
#define address2 256 //�ڶ������ĵ���ʼ��ַ 
typedef unsigned char BYTE;
extern HANDLE mlock,plock;

extern short ip[3],flag[3];//����������ͱ�־�Ĵ���
extern short normal_register[3][9],ir[3];//ͨ�üĴ�����1234��5678 ��ָ��Ĵ��� 
BYTE memory[32768];//������ڴ� ,��������127��������-256 

void stop(int i,int id);//ͣ��ָ�0
void trans(int i,int id);//���ݴ���ָ�1 
void calcul(int i,int id);//��������ָ�2345��+-*/  
void logic(int i,int id);//�߼�����ָ�678���߼��룬�߼����߼���
void compare(int i,int id);//�Ƚ�ָ�9��
void jump(int i,int id);//��תָ�10��
void in_out(int i,int id);//�������ָ�� ��11��12
void ask_for_lock(int i,int id);//���󻥳����13 
void release(int i,int id);//�ͷŻ������14
void sleep(int i,int id);//���ߣ�15

short change(int i);//�ڴ�����������������ʮ���Ʊ�ʾ�ģ�ת��Ϊʮ���Ƶ��� 
void ten_to_two(short m,short num);//��num��Ϊ�����ƣ�ת��Ϊ����ʮ���ƣ���������memory[m]��[m+1] 
short four_situ(int num,short m,short n);//�Ӽ��˳� 
short three_logic(int num,short m,short n);//���ǣ�
void comp_num(short m,short n,int id);//�Ƚϴ�С���޸�flag 


#endif
