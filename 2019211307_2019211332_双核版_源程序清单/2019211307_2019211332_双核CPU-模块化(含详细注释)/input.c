# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"

int loadByte(FILE* fin)//��8λ��ת��Ϊʮ���� ;
{
	int i;
	char temp;
	BYTE ans=0;
	int val=0;
	for(i=0;i<8&&!feof(fin);i++)
	{
		temp=fgetc(fin);//��ȡһλ 
		if(temp=='\n')//����ǻ��з���i-1 
			i--;
		else if(temp==EOF)
			return 0;
		else
			ans=ans*2+(temp-'0');//������ת��Ϊʮ���� 
	}
	return ans;
} 

int read_instructions(char* name,int id)//����ָ��ڴ��У�����ʹ�õ��ֽ���top; 
{
	FILE *ftp;
	ftp=fopen(name,"r");
	int i,top=0;
	if(id==1)	i=address1;//����1 
	else		i=address2;//����2 
	while(!feof(ftp))
	{
		memory[i++]=loadByte(ftp);//�ڴ�Ĵ�λ����ֵ��ͬʱ�ƶ�����һλ 
		top++;//�ֽ���+1 
	}
	fclose(ftp);
	return top-1;
} 
