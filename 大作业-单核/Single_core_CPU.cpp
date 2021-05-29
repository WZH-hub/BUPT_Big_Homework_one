#include<stdio.h>
#define deta 256 
typedef unsigned char BYTE;
short ip=0,flag=0;//程序计数器和标志寄存器
short normal_register[9]={0},ir=0;//通用寄存器，1234，5678 和指令寄存器 
BYTE memory[32768]={0};//这个是内存 ,遇见大于127的数，先-256 

int loadByte(FILE* fin);//读8位，转化为十进制 ;
int read_instructions(char* name);//载入指令到内存中，返回使用的字节数i; 
void run(); //运行程序 
void stop(int i);//停机指令，0
void trans(int i);//数据传送指令，1 
void calcul(int i);//算数运算指令，2345，+-*/  
void logic(int i);//逻辑运算指令，678，逻辑与，逻辑或，逻辑非
void compare(int i);//比较指令，9，
void jump(int i);//跳转指令，10，
void in_out(int i);//输入输出指令 ，11，12
void print(); //输出状态的函数 
short change(int i);//内存中连着两个的二进制（十进制表示的）转化为十进制的数 
void ten_to_two(short m,short num);//把num变为二进制，转变为两个十进制，并保存在memory[m]和[m+1] 
short four_situ(int num,short m,short n);//加减乘除 
short three_logic(int num,short m,short n);//与或非，
void comp_num(short m,short n);//比较大小，修改flag 
void print_code();
void print_data();

int main(void)//主函数 
{
	run();
	return 0;
} 

int loadByte(FILE* fin)//读8位，转化为十进制 ;
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

int read_instructions(char* name)//载入指令到内存中，返回使用的字节数i; 
{
	FILE *ftp;
	ftp=fopen(name,"r");
	int i=0,top=0;
	while(!feof(ftp))
		memory[i++]=loadByte(ftp);
	fclose(ftp);
	return i-1;
} 
 
void run() //运行程序 
{
	int top,i;
	top=read_instructions("dict.dic");
	for(i=0;i<top;i=ip)//为了跳转 
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

void stop(int i)//停机指令，0
{
	ir=0; 
	ip+=4;
	print();
} 

void trans(int i)//数据传送指令，1 
{
	int m,n;
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(n==0)//立即数传递 
	{
		if(m==1||m==2||m==3||m==4)
			normal_register[m]=change(i);//直接传送到寄存器1234 
		else
		{ 
			normal_register[m]=change(i);//传到寄存器5678，也就是赋给一个数字代表地址 
			memory[normal_register[m]]=memory[i+2];
			memory[normal_register[m]+1]=memory[i+3];//直接把立即数的两部分弄过去 
		} 
	}
	else
	{
		if(m<=4&&n<=4)		normal_register[m]=normal_register[n];//直接赋值 
		else if(m<=4&&n>4)  normal_register[m]=change(normal_register[n]-2);//把指向的内存单元赋值过去 
		else if(m>4&&n<=4)  ten_to_two(normal_register[m],normal_register[n]);//把数据弄到地址指向的内存单元 
		else if(m>4&&n>4)
		{
			memory[normal_register[m]]=memory[normal_register[n]];
			memory[normal_register[m]+1]=memory[normal_register[n]+1];//直接把地址数字赋过去 
		}
	} 
	ip+=4; 
	print();
}

void calcul(int i)//算数运算指令，2345，+-*/  
{
	int m,n,num;
	num=memory[i];
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(n==0)//立即数有操作 
	{
		if(m==1||m==2||m==3||m==4)
			normal_register[m]=four_situ(num,normal_register[m],change(i));//立即数加减乘除 
		else
			ten_to_two(normal_register[m],four_situ(num,change(normal_register[m]-2),change(i)));//运算完以后，重新变成两个十进制 
	} 
	else
	{
		if(m<=4&&n<=4)	normal_register[m]=four_situ(num,normal_register[m],normal_register[n]);//直接数字运算 
		if(m<=4&&n>4)	normal_register[m]=four_situ(num,normal_register[m],change(normal_register[n]-2));//change为10进制后运算 
		if(m>4&&n<=4)	ten_to_two(normal_register[m],four_situ(num,change(normal_register[m]-2),normal_register[n]));//change为10进制后运算，再转为两个十进制 
		if(m>4&&n>4)	ten_to_two(normal_register[m],four_situ(num,change(normal_register[m]-2),change(normal_register[n]-2)));//change为10进制后运算，再转为两个十进制
	}
	ip+=4;
	print();
}

void logic(int i)//逻辑运算指令，678，逻辑与，逻辑或，逻辑非
{
	int m,n,num;
	num=memory[i];
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(n==0)//立即数有操作
	{
		if(m<4)		normal_register[m]=three_logic(num,normal_register[m],change(i));//直接计算 
		else  		ten_to_two(normal_register[m],three_logic(num,change(normal_register[m]-2),change(i)));//经过转化后计算 
	}
	else if(m==0)//立即数操作 
	{
		if(n<4)		normal_register[n]=three_logic(num,normal_register[n],change(i));//直接计算
		else  		ten_to_two(normal_register[n],three_logic(num,change(normal_register[n]-2),change(i)));//经过转化后计算 
	}
	else
	{
		if(m<=4&&n<=4)	normal_register[m]=three_logic(num,normal_register[m],normal_register[n]);//直接计算 
		if(m<=4&&n>4)	normal_register[m]=three_logic(num,normal_register[m],change(normal_register[n]-2));
		if(m>4&&n<=4)	ten_to_two(normal_register[m],three_logic(num,change(normal_register[m]-2),normal_register[n]));
		if(m>4&&n>4)	ten_to_two(normal_register[m],three_logic(num,change(normal_register[m]-2),change(normal_register[n]-2)));
	}
	ip+=4;
	print();
} 

void compare(int i)//比较指令，9，
{
	int m,n,num;
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(n==0)
	{
		if(m<=4)	comp_num(normal_register[m],change(i));//直接比较 
		if(m>4)		comp_num(change(normal_register[m]-2),change(i));//都转换一下再比较 
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

void jump(int i)//跳转指令，10，
{
	int m,n,num;
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
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

void in_out(int i)//输入输出指令 ，11，12
{
	int m,n,num;
	short code;
	num=memory[i];
	ir=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(num==11)
	{
		printf("in:\n");
		scanf("%hd",&code);
		if(m<=4)	normal_register[m]=code;//保存在1234 
		if(m>4)		ten_to_two(normal_register[m],code);//保存在5678指向的内存单元 
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

void print() //输出状态的函数 
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


/*下面是一些辅助函数*/ 
short change(int i)//内存中连着两个的二进制（十进制表示的）转化为十进制的数 
{
	int n;
	if(memory[i+2]>127)	n=(memory[i+2]-256)*256+memory[i+3];
	else				n=memory[i+2]*256+memory[i+3];
	return n;
}

void ten_to_two(short m,short num)//把num变为二进制，再转变为两个十进制，并保存在memory[m]和[m+1] 
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
	if(b[0]==1)//如果是负数，就01互换 
	{
		for(i=1;i<16;i++)//1和0互换 
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
	}//互换完了以后分别保存 
	for(i=0;i<8;i++)
		memory[m]=memory[m]*2+b[i];
	for(i=8;i<16;i++)
		memory[m+1]=memory[m+1]*2+b[i];
}

short four_situ(int num,short m,short n)//加减乘除 
{
	if(num==2)	return (m+n);
	if(num==3)	return (m-n);
	if(num==4)	return (m*n);
	if(num==5)	return (m/n);
}

short three_logic(int num,short m,short n)//与或非，
{
	if(num==6)	return (m&&n);
	if(num==7)	return (m||n);
	if(num==8)  return (!m);
} 

void comp_num(short m,short n)//比较大小，修改flag 
{
	if(m==n)	flag=0;
	if(m<n)		flag=-1;
	if(m>n)		flag=1;
}
