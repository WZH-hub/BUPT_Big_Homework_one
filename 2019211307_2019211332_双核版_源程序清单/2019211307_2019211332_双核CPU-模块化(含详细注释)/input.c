# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"

int loadByte(FILE* fin)//读8位，转化为十进制 ;
{
	int i;
	char temp;
	BYTE ans=0;
	int val=0;
	for(i=0;i<8&&!feof(fin);i++)
	{
		temp=fgetc(fin);//读取一位 
		if(temp=='\n')//如果是换行符，i-1 
			i--;
		else if(temp==EOF)
			return 0;
		else
			ans=ans*2+(temp-'0');//二进制转化为十进制 
	}
	return ans;
} 

int read_instructions(char* name,int id)//载入指令到内存中，返回使用的字节数top; 
{
	FILE *ftp;
	ftp=fopen(name,"r");
	int i,top=0;
	if(id==1)	i=address1;//核心1 
	else		i=address2;//核心2 
	while(!feof(ftp))
	{
		memory[i++]=loadByte(ftp);//内存的此位被赋值，同时移动到下一位 
		top++;//字节数+1 
	}
	fclose(ftp);
	return top-1;
} 
