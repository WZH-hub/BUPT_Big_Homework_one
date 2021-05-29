#include<stdio.h>
#define deta 256 
typedef unsigned char BYTE;
short ip=0,flag=0;//����������ͱ�־�Ĵ���
short normal_register[9]={0},ir=0;//ͨ�üĴ�����1234��5678 ��ָ��Ĵ��� 
BYTE memory[32768]={0};//������ڴ� ,��������127��������-256 

int loadByte(FILE* fin);//��8λ��ת��Ϊʮ���� ;
int read_instructions(char* name);//����ָ��ڴ��У�����ʹ�õ��ֽ���i; 
void run(); //���г��� 
void stop(int i);//ͣ��ָ�0
void trans(int i);//���ݴ���ָ�1 
void calcul(int i);//��������ָ�2345��+-*/  
void logic(int i);//�߼�����ָ�678���߼��룬�߼����߼���
void compare(int i);//�Ƚ�ָ�9��
void jump(int i);//��תָ�10��
void in_out(int i);//�������ָ�� ��11��12
void print(); //���״̬�ĺ��� 
short change(int i);//�ڴ������������Ķ����ƣ�ʮ���Ʊ�ʾ�ģ�ת��Ϊʮ���Ƶ��� 
void ten_to_two(short m,short num);//��num��Ϊ�����ƣ�ת��Ϊ����ʮ���ƣ���������memory[m]��[m+1] 
short four_situ(int num,short m,short n);//�Ӽ��˳� 
short three_logic(int num,short m,short n);//���ǣ�
void comp_num(short m,short n);//�Ƚϴ�С���޸�flag 
void print_code();
void print_data();

int main(void)//������ 
{
	run();
	return 0;
} 

int loadByte(FILE* fin)//��8λ��ת��Ϊʮ���� ;
{
	int i;
	char temp;
	BYTE ans=0;
	int val=0;
	for(i=0;i<8&&!feof(fin);i++)
	{
		temp=fgetc(fin);
		if(temp=='\n')
			i--;
		else if(temp==EOF)
			return 0;
		else
			ans=ans*2+(temp-'0');
	}
	return ans;
} 

int read_instructions(char* name)//����ָ��ڴ��У�����ʹ�õ��ֽ���i; 
{
	FILE *ftp;
	ftp=fopen(name,"r");
	int i=0,top=0;
	while(!feof(ftp))
		memory[i++]=loadByte(ftp);
	fclose(ftp);
	return i-1;
} 
 
void run() //���г��� 
{
	int top,i;
	top=read_instructions("dict.dic");
	for(i=0;i<top;i=ip)//Ϊ����ת 
	{
		switch(memory[i])
		{
			case 0:		stop(i);break;
			case 1:  	trans(i);break;
			case 2:  case 3:case 4:case 5:
						calcul(i);break;
			case 6:  case 7:case 8:
						logic(i);break;
			case 9:  	compare(i);break;
			case 10: 	jump(i);break;
			case 11: case 12:
						in_out(i);break;
		}
	}
	printf("\n");
	print_code();
	print_data();
}

void stop(int i)//ͣ��ָ�0
{
	ir=0; 
	ip+=4;
	print();
} 

void trans(int i)//���ݴ���ָ�1 
{
	int m,n;
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)//���������� 
	{
		if(m==1||m==2||m==3||m==4)
			normal_register[m]=change(i);//ֱ�Ӵ��͵��Ĵ���1234 
		else
		{ 
			normal_register[m]=change(i);//�����Ĵ���5678��Ҳ���Ǹ���һ�����ִ����ַ 
			memory[normal_register[m]]=memory[i+2];
			memory[normal_register[m]+1]=memory[i+3];//ֱ�Ӱ���������������Ū��ȥ 
		} 
	}
	else
	{
		if(m<=4&&n<=4)		normal_register[m]=normal_register[n];//ֱ�Ӹ�ֵ 
		else if(m<=4&&n>4)  normal_register[m]=change(normal_register[n]-2);//��ָ����ڴ浥Ԫ��ֵ��ȥ 
		else if(m>4&&n<=4)  ten_to_two(normal_register[m],normal_register[n]);//������Ū����ַָ����ڴ浥Ԫ 
		else if(m>4&&n>4)
		{
			memory[normal_register[m]]=memory[normal_register[n]];
			memory[normal_register[m]+1]=memory[normal_register[n]+1];//ֱ�Ӱѵ�ַ���ָ���ȥ 
		}
	} 
	ip+=4; 
	print();
}

void calcul(int i)//��������ָ�2345��+-*/  
{
	int m,n,num;
	num=memory[i];
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)//�������в��� 
	{
		if(m==1||m==2||m==3||m==4)
			normal_register[m]=four_situ(num,normal_register[m],change(i));//�������Ӽ��˳� 
		else
			ten_to_two(normal_register[m],four_situ(num,change(normal_register[m]-2),change(i)));//�������Ժ����±������ʮ���� 
	} 
	else
	{
		if(m<=4&&n<=4)	normal_register[m]=four_situ(num,normal_register[m],normal_register[n]);//ֱ���������� 
		if(m<=4&&n>4)	normal_register[m]=four_situ(num,normal_register[m],change(normal_register[n]-2));//changeΪ10���ƺ����� 
		if(m>4&&n<=4)	ten_to_two(normal_register[m],four_situ(num,change(normal_register[m]-2),normal_register[n]));//changeΪ10���ƺ����㣬��תΪ����ʮ���� 
		if(m>4&&n>4)	ten_to_two(normal_register[m],four_situ(num,change(normal_register[m]-2),change(normal_register[n]-2)));//changeΪ10���ƺ����㣬��תΪ����ʮ����
	}
	ip+=4;
	print();
}

void logic(int i)//�߼�����ָ�678���߼��룬�߼����߼���
{
	int m,n,num;
	num=memory[i];
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)//�������в���
	{
		if(m<4)		normal_register[m]=three_logic(num,normal_register[m],change(i));//ֱ�Ӽ��� 
		else  		ten_to_two(normal_register[m],three_logic(num,change(normal_register[m]-2),change(i)));//����ת������� 
	}
	else if(m==0)//���������� 
	{
		if(n<4)		normal_register[n]=three_logic(num,normal_register[n],change(i));//ֱ�Ӽ���
		else  		ten_to_two(normal_register[n],three_logic(num,change(normal_register[n]-2),change(i)));//����ת������� 
	}
	else
	{
		if(m<=4&&n<=4)	normal_register[m]=three_logic(num,normal_register[m],normal_register[n]);//ֱ�Ӽ��� 
		if(m<=4&&n>4)	normal_register[m]=three_logic(num,normal_register[m],change(normal_register[n]-2));
		if(m>4&&n<=4)	ten_to_two(normal_register[m],three_logic(num,change(normal_register[m]-2),normal_register[n]));
		if(m>4&&n>4)	ten_to_two(normal_register[m],three_logic(num,change(normal_register[m]-2),change(normal_register[n]-2)));
	}
	ip+=4;
	print();
} 

void compare(int i)//�Ƚ�ָ�9��
{
	int m,n,num;
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)
	{
		if(m<=4)	comp_num(normal_register[m],change(i));//ֱ�ӱȽ� 
		if(m>4)		comp_num(change(normal_register[m]-2),change(i));//��ת��һ���ٱȽ� 
	}
	else
	{
		if(m<=4&&n<=4)	comp_num(normal_register[m],normal_register[n]);
		if(m<=4&&n>4)	comp_num(normal_register[m],change(normal_register[n]-2));
		if(m>4&&n<4)	comp_num(change(normal_register[m]-2),normal_register[n]);
		if(m>4&&n>4)	comp_num(change(normal_register[m]-2),change(normal_register[n]-2));
	} 
	ip+=4;
	print();
} 

void jump(int i)//��תָ�10��
{
	int m,n,num;
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(n==0)	ip+=change(i);
	if(n==1)
	{
		if(flag==0)		ip+=change(i);
		else 			ip+=4;
	}
	if(n==2)
	{
		if(flag==1)		ip+=change(i);
		else 			ip+=4;
	}
	if(n==3)
	{
		if(flag==-1)	ip+=change(i);
		else 			ip+=4;
	}
	print();
}

void in_out(int i)//�������ָ�� ��11��12
{
	int m,n,num;
	short code;
	num=memory[i];
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//ǰһ���� 
	n=memory[i+1]%16;//��һ����
	if(num==11)
	{
		printf("in:\n");
		scanf("%hd",&code);
		if(m<=4)	normal_register[m]=code;//������1234 
		if(m>4)		ten_to_two(normal_register[m],code);//������5678ָ����ڴ浥Ԫ 
	}
	if(num==12)
	{
		if(m<=4)	code=normal_register[m];
		if(m>4)		code=change(normal_register[m]-2);
		printf("out: %hd\n",code);
	}
	ip+=4;
	print();
} 

void print() //���״̬�ĺ��� 
{
	int i;
	printf("ip = %hd\n",ip);
	printf("flag = %hd\n",flag);
	printf("ir = %hd\n",ir);
	for(i=1;i<9;i++)
	{
		printf("ax%d = %hd",i,normal_register[i]);
		if(i%4==0)	printf("\n");
		else 		printf(" "); 
	}
}

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


/*������һЩ��������*/ 
short change(int i)//�ڴ������������Ķ����ƣ�ʮ���Ʊ�ʾ�ģ�ת��Ϊʮ���Ƶ��� 
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
		memory[m]=memory[m]*2+b[i];
	for(i=8;i<16;i++)
		memory[m+1]=memory[m+1]*2+b[i];
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

void comp_num(short m,short n)//�Ƚϴ�С���޸�flag 
{
	if(m==n)	flag=0;
	if(m<n)		flag=-1;
	if(m>n)		flag=1;
}
