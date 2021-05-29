# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"

void print_code()//��ӡ����� 
{
	int i,ans,m=0;
	printf("codeSegment :\n");
	for(i=0;i<8*16*4;i+=4)
	{
		if(memory[i]>127)	ans=(memory[i]-256)*16777216+memory[i+1]*65536+memory[i+2]*256+memory[i+3];//�Ѳ����Ϊʮ�������� 
		else				ans=memory[i]*16777216+memory[i+1]*65536+memory[i+2]*256+memory[i+3];//������������Ϊʮ���� 
		printf("%d",ans);
		m+=1;
		if(m%8==0)		printf("\n");//ÿ�˸���ӡһ�����з� 
		else 			printf(" "); 
	}
	printf("\n");
}

void print_data()//��ӡ���ݶ� 
{
	int i,m=0;
	short ans;
	printf("dataSegment :\n");
	for(i=16384;m<16*16;i+=2)
	{
		ans=change(i-2);
		printf("%hd",ans);
		m+=1;
		if(m%16==0)		printf("\n");//ÿ�˸���ӡһ�����з� 
		else 			printf(" "); 
	}
}

