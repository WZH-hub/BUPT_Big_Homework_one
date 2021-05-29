# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"

void print_code()//打印代码段 
{
	int i,ans,m=0;
	printf("codeSegment :\n");
	for(i=0;i<8*16*4;i+=4)
	{
		if(memory[i]>127)	ans=(memory[i]-256)*16777216+memory[i+1]*65536+memory[i+2]*256+memory[i+3];//把补码变为十进制数字 
		else				ans=memory[i]*16777216+memory[i+1]*65536+memory[i+2]*256+memory[i+3];//正常的正数变为十进制 
		printf("%d",ans);
		m+=1;
		if(m%8==0)		printf("\n");//每八个打印一个换行符 
		else 			printf(" "); 
	}
	printf("\n");
}

void print_data()//打印数据段 
{
	int i,m=0;
	short ans;
	printf("dataSegment :\n");
	for(i=16384;m<16*16;i+=2)
	{
		ans=change(i-2);
		printf("%hd",ans);
		m+=1;
		if(m%16==0)		printf("\n");//每八个打印一个换行符 
		else 			printf(" "); 
	}
}

