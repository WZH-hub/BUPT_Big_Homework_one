# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"


#define deta 256 
#define address1 0   //��һ�����ĵ���ʼ��ַ 
#define address2 256 //�ڶ������ĵ���ʼ��ַ 
typedef unsigned char BYTE;
HANDLE mlock,plock;

short ip[3]={0},flag[3]={0};//����������ͱ�־�Ĵ���
short normal_register[3][9]={0},ir[3]={0};//ͨ�üĴ�����1234��5678 ��ָ��Ĵ��� 
BYTE memory[32768]={0};//������ڴ� ,��������127��������-256 

unsigned __stdcall run(void* pArguments);//���г���

int main(void)//������ 
{
	ten_to_two(16384,100);//��ʼ�� 
	HANDLE hThread1,hThread2;//��� 
	unsigned ThreadID1,ThreadID2;    
	int id1=1,id2=2;//ID�Ŵ����ĸ����� 
	
	mlock=CreateMutex(NULL,FALSE,NULL);//��ס16384�Ļ������ 
	plock=CreateMutex(NULL,FALSE,NULL);//��ס���ʱ��Ļ������
	
	hThread1=(HANDLE)_beginthreadex(NULL,0,run,(void*)&id1,0,&ThreadID1);//��������� 
	hThread2=(HANDLE)_beginthreadex(NULL,0,run,(void*)&id2,0,&ThreadID2);//��������� 
	
	WaitForSingleObject(hThread1, INFINITE);//�ȴ� 
	CloseHandle (hThread1);//���� 
 	WaitForSingleObject(hThread2, INFINITE);//�ȴ� 
	CloseHandle (hThread2);//���� 
	
	print_code();//��ӡ����� 
	print_data();//��ӡ���ݶ� 
	return 0;
} 

unsigned __stdcall run(void* pArguments) //���г��� 
{
	int top,i,id,address=0;//address����ʼ��ַ 
	int	*p ;
	p = (int *)  pArguments ;
	id=*p;//��ú������ 
	if(id==1)//����1 
	{
		i=address1;
		address=address1;
		top=read_instructions("dict1.dic",id);//��ȡָ������ֽ��� 
	}
	else if(id==2)//����2 
	{
		i=address2;
		address=address2;
		top=read_instructions("dict2.dic",id);//��ȡָ������ֽ��� 
	}
	for(;i<address+top;i=address+ip[id])//Ϊ����ת 
	{
		switch(memory[i])//����ָ��ִ�в�ͬ���� 
		{
			case  0:	stop(i,id);break;
			case  1:  	trans(i,id);break;
			case  2:  case 3:case 4:case 5:
						calcul(i,id);break;
			case  6:  case 7:case 8:
						logic(i,id);break;
			case  9:  	compare(i,id);break;
			case 10: 	jump(i,id);break;
			case 11: case 12:
						in_out(i,id);break;
			case 13:	ask_for_lock(i,id);break;
			case 14:	release(i,id);break;
			case 15:	sleep(i,id);break;
		}
	}
	_endthreadex(0);//�����߳�
	return 0; 
} 
