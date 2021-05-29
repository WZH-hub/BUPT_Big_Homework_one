# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"


#define deta 256 
#define address1 0   //第一个核心的起始地址 
#define address2 256 //第二个核心的起始地址 
typedef unsigned char BYTE;
HANDLE mlock,plock;

short ip[3]={0},flag[3]={0};//程序计数器和标志寄存器
short normal_register[3][9]={0},ir[3]={0};//通用寄存器，1234，5678 和指令寄存器 
BYTE memory[32768]={0};//这个是内存 ,遇见大于127的数，先-256 

unsigned __stdcall run(void* pArguments);//运行程序

int main(void)//主函数 
{
	ten_to_two(16384,100);//初始化 
	HANDLE hThread1,hThread2;//句柄 
	unsigned ThreadID1,ThreadID2;    
	int id1=1,id2=2;//ID号代表哪个核心 
	
	mlock=CreateMutex(NULL,FALSE,NULL);//锁住16384的互斥对象 
	plock=CreateMutex(NULL,FALSE,NULL);//锁住输出时候的互斥对象
	
	hThread1=(HANDLE)_beginthreadex(NULL,0,run,(void*)&id1,0,&ThreadID1);//互斥对象句柄 
	hThread2=(HANDLE)_beginthreadex(NULL,0,run,(void*)&id2,0,&ThreadID2);//互斥对象句柄 
	
	WaitForSingleObject(hThread1, INFINITE);//等待 
	CloseHandle (hThread1);//结束 
 	WaitForSingleObject(hThread2, INFINITE);//等待 
	CloseHandle (hThread2);//结束 
	
	print_code();//打印代码段 
	print_data();//打印数据段 
	return 0;
} 

unsigned __stdcall run(void* pArguments) //运行程序 
{
	int top,i,id,address=0;//address是起始地址 
	int	*p ;
	p = (int *)  pArguments ;
	id=*p;//获得核心序号 
	if(id==1)//核心1 
	{
		i=address1;
		address=address1;
		top=read_instructions("dict1.dic",id);//读取指令并返回字节数 
	}
	else if(id==2)//核心2 
	{
		i=address2;
		address=address2;
		top=read_instructions("dict2.dic",id);//读取指令并返回字节数 
	}
	for(;i<address+top;i=address+ip[id])//为了跳转 
	{
		switch(memory[i])//根据指令执行不同操作 
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
	_endthreadex(0);//结束线程
	return 0; 
} 
