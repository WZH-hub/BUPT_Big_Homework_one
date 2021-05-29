# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"

void print(int id) //输出状态的函数 
{
	int i,address;
	if(id==1)	address=0;
	else		address=256;
	WaitForSingleObject(plock,INFINITE);
	printf("id = %d\n",id);
	printf("ip = %hd\n",ip[id]+address);//这个与核心1和2有关 
	printf("flag = %hd\n",flag[id]);
	printf("ir = %hd\n",ir[id]);
	for(i=1;i<9;i++)
	{
		printf("ax%d = %hd",i,normal_register[id][i]);
		if(i%4==0)	printf("\n");
		else 		printf(" "); 
	}
	printf("\n");
	ReleaseMutex(plock); 
}
