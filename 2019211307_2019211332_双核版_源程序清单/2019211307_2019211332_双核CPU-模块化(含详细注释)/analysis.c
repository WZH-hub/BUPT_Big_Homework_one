# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"
/********************
˵��ǰ�棺	
			short change(int i)//�ڴ�����������������ʮ���Ʊ�ʾ�ģ�ת��Ϊʮ���Ƶ���   
			void ten_to_two(short m,short num)//��num��Ϊ�����ƣ���ת��Ϊ����ʮ���ƣ���������memory[m]��[m+1] 
�������õıȽ�Ƶ���������ǹ��̻���ĺ���
*******************/ 
void stop(int i,int id)//ͣ��ָ�0
{
	ir[id]=0; 
	ip[id]+=4;
	print(id);
} 

void trans(int i,int id)//���ݴ���ָ�1 
{
	int m,n;
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)//���������� 
	{
		if(m==1||m==2||m==3||m==4)
			normal_register[id][m]=change(i);//ֱ�Ӵ��͵��Ĵ���1234 
		else
			normal_register[id][m]=change(i);//�����Ĵ���5678��Ҳ���Ǹ���һ�����ִ����ַ 
	}
	else
	{
		if(m<=4&&n<=4)		normal_register[id][m]=normal_register[id][n];//ֱ�Ӹ�ֵ 
		else if(m<=4&&n>4)  normal_register[id][m]=change(normal_register[id][n]-2);//��ָ����ڴ浥Ԫ��ֵ��ȥ 
		else if(m>4&&n<=4)  ten_to_two(normal_register[id][m],normal_register[id][n]);//������Ū����ַָ����ڴ浥Ԫ 
		else if(m>4&&n>4)
		{
			memory[normal_register[id][m]]=memory[normal_register[id][n]];//ֱ�Ӱѵ�ַ���ָ���ȥ 
			memory[normal_register[id][m]+1]=memory[normal_register[id][n]+1];//ֱ�Ӱѵ�ַ���ָ���ȥ 
		}
	} 
	ip[id]+=4; 
	print(id);
}

void calcul(int i,int id)//��������ָ�2345��+-*/  
{
	int m,n,num;
	num=memory[i];
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)//�������в��� 
	{
		if(m==1||m==2||m==3||m==4)
			normal_register[id][m]=four_situ(num,normal_register[id][m],change(i));//�������Ӽ��˳� 
		else
			ten_to_two(normal_register[id][m],four_situ(num,change(normal_register[id][m]-2),change(i)));//�������Ժ����±������ʮ���� 
	} 
	else
	{
		if(m<=4&&n<=4)	normal_register[id][m]=four_situ(num,normal_register[id][m],normal_register[id][n]);//ֱ���������� 
		if(m<=4&&n>4)	normal_register[id][m]=four_situ(num,normal_register[id][m],change(normal_register[id][n]-2));//changeΪ10���ƺ����� 
		if(m>4&&n<=4)	ten_to_two(normal_register[id][m],four_situ(num,change(normal_register[id][m]-2),normal_register[id][n]));//changeΪ10���ƺ����㣬��תΪ����ʮ���� 
		if(m>4&&n>4)	ten_to_two(normal_register[id][m],four_situ(num,change(normal_register[id][m]-2),change(normal_register[id][n]-2)));//changeΪ10���ƺ����㣬��תΪ����ʮ����
	}
	ip[id]+=4;
	print(id);
}

void logic(int i,int id)//�߼�����ָ�678���߼��룬�߼����߼���
{
	int m,n,num;
	num=memory[i];
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)//�������в���
	{
		if(m<4)		normal_register[id][m]=three_logic(num,normal_register[id][m],change(i));//ֱ������ 
		else  		ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),change(i)));//����ת�������� 
	}
	else if(m==0)//���������� 
	{
		if(n<4)		normal_register[id][n]=three_logic(num,normal_register[id][n],change(i));//ֱ������
		else  		ten_to_two(normal_register[id][n],three_logic(num,change(normal_register[id][n]-2),change(i)));//����ת�������� 
	}
	else
	{
		if(m<=4&&n<=4)	normal_register[id][m]=three_logic(num,normal_register[id][m],normal_register[id][n]);//ֱ������ 
		if(m<=4&&n>4)	normal_register[id][m]=three_logic(num,normal_register[id][m],change(normal_register[id][n]-2));//����ת�������� 
		if(m>4&&n<=4)	ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),normal_register[id][n]));//����ת�������� 
		if(m>4&&n>4)	ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),change(normal_register[id][n]-2)));//����ת��������
	}
	ip[id]+=4;
	print(id);
} 

void compare(int i,int id)//�Ƚ�ָ�9��
{
	int m,n,num;
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)
	{
		if(m<=4)	comp_num(normal_register[id][m],change(i),id);//ֱ�ӱȽ� 
		if(m>4)		comp_num(change(normal_register[id][m]-2),change(i),id);//��ת��һ���ٱȽ� 
	}
	else
	{
		if(m<=4&&n<=4)	comp_num(normal_register[id][m],normal_register[id][n],id);//ֱ�ӱȽ� 
		if(m<=4&&n>4)	comp_num(normal_register[id][m],change(normal_register[id][n]-2),id);//ת��һ���ٱȽ� 
		if(m>4&&n<=4)	comp_num(change(normal_register[id][m]-2),normal_register[id][n],id);//ת��һ���ٱȽ� 
		if(m>4&&n>4)	comp_num(change(normal_register[id][m]-2),change(normal_register[id][n]-2),id);//ת��һ���ٱȽ� 
	} 
	ip[id]+=4;
	print(id);
} 

void jump(int i,int id)//��תָ�10��
{
	int m,n,num;
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)	ip[id]+=change(i);
	if(n==1)
	{
		if(flag[id]==0)		ip[id]+=change(i);
		else 				ip[id]+=4;
	}
	if(n==2)
	{
		if(flag[id]==1)		ip[id]+=change(i);
		else 				ip[id]+=4;
	}
	if(n==3)
	{
		if(flag[id]==-1)	ip[id]+=change(i);
		else 				ip[id]+=4;
	}
	print(id);
}

void in_out(int i,int id)//�������ָ�� ��11��12
{
	int m,n,num;
	short code;
	num=memory[i];
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(num==11)//���� 
	{
		printf("in:\n");
		scanf("%hd",&code);
		if(m<=4)	normal_register[id][m]=code;//������1234 
		if(m>4)		ten_to_two(normal_register[id][m],code);//������5678ָ����ڴ浥Ԫ 
	}
	if(num==12)//��� 
	{
		if(m<=4)	code=normal_register[id][m];
		if(m>4)		code=change(normal_register[id][m]-2);
		printf("id = %d    out: %hd\n",id,code);
		printf("\n");
	}
	ip[id]+=4;
	print(id);
}

void ask_for_lock(int i,int id)//���󻥳����13 
{
	ir[id]=memory[i]*deta+memory[i+1];
	WaitForSingleObject(mlock,INFINITE);//��Ϊʵ��ֻ����һ���ڴ� 
	ip[id]+=4;
	print(id);
}

void release(int i,int id)//�ͷŻ������14
{
	ir[id]=memory[i]*deta+memory[i+1];
	ReleaseMutex(mlock);
	ip[id]+=4;
	print(id);
} 

void sleep(int i,int id)//���ߣ�15
{
	ir[id]=memory[i]*deta+memory[i+1];
	Sleep(change(i));
	ip[id]+=4;
	print(id);
} 

/*������һЩ��������*/ 
short change(int i)//�ڴ�����������������ʮ���Ʊ�ʾ�ģ�ת��Ϊʮ���Ƶ�����ע���ǵ�3��4λ 
{
	int n;
	if(memory[i+2]>127)	n=(memory[i+2]-256)*256+memory[i+3];
	else				n=memory[i+2]*256+memory[i+3];
	return n;
}

void ten_to_two(short m,short num)//��num��Ϊ�����ƣ���ת��Ϊ����ʮ���ƣ���������memory[m]��[m+1] 
{
	int b[20]={0},i=1,j;
	if(num<0)
	{
		num=num*-1;
		b[0]=1;
	}
	for(i=1,j=15;num!=0;i=i*10,j--)
	{
		b[j]=num%2;
		num=num/2;
	}
	if(b[0]==1)//����Ǹ�������01���� 
	{
		for(i=1;i<16;i++)//1��0���� 
		{
			if(b[i]==1) 	    b[i]=0;
			else if(b[i]==0)	b[i]=1;
		}
		b[15]+=1;
		for(i=15;i>0;i--)
		{
			if(b[i]==2)
			{
				b[i-1]+=1;
				b[i]=0;
			}
		}
	}//���������Ժ�ֱ𱣴� 
	for(i=0;i<8;i++)
		memory[m]=memory[m]*2+b[i];//ǰ8λ 
	for(i=8;i<16;i++)
		memory[m+1]=memory[m+1]*2+b[i];//��8λ 
}

short four_situ(int num,short m,short n)//�Ӽ��˳� 
{
	if(num==2)	return (m+n);
	if(num==3)	return (m-n);
	if(num==4)	return (m*n);
	if(num==5)	return (m/n);
}

short three_logic(int num,short m,short n)//���ǣ�
{
	if(num==6)	return (m&&n);
	if(num==7)	return (m||n);
	if(num==8)  return (!m);
} 

void comp_num(short m,short n,int id)//�Ƚϴ�С���޸�flag 
{
	if(m==n)	flag[id]=0;
	if(m<n)		flag[id]=-1;
	if(m>n)		flag[id]=1;
}

