# include <stdio.h>
# include <windows.h>
# include <process.h> 
# include "input.h"
# include "analysis.h"
# include "output_end.h"
# include "output_now.h"
/********************
说在前面：	
			short change(int i)//内存中连着两个的数（十进制表示的）转化为十进制的数   
			void ten_to_two(short m,short num)//把num变为二进制，再转变为两个十进制，并保存在memory[m]和[m+1] 
这两个用的比较频繁，而且是过程互逆的函数
*******************/ 
void stop(int i,int id)//停机指令，0
{
	ir[id]=0; 
	ip[id]+=4;
	print(id);
} 

void trans(int i,int id)//数据传送指令，1 
{
	int m,n;
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(n==0)//立即数传递 
	{
		if(m==1||m==2||m==3||m==4)
			normal_register[id][m]=change(i);//直接传送到寄存器1234 
		else
			normal_register[id][m]=change(i);//传到寄存器5678，也就是赋给一个数字代表地址 
	}
	else
	{
		if(m<=4&&n<=4)		normal_register[id][m]=normal_register[id][n];//直接赋值 
		else if(m<=4&&n>4)  normal_register[id][m]=change(normal_register[id][n]-2);//把指向的内存单元赋值过去 
		else if(m>4&&n<=4)  ten_to_two(normal_register[id][m],normal_register[id][n]);//把数据弄到地址指向的内存单元 
		else if(m>4&&n>4)
		{
			memory[normal_register[id][m]]=memory[normal_register[id][n]];//直接把地址数字赋过去 
			memory[normal_register[id][m]+1]=memory[normal_register[id][n]+1];//直接把地址数字赋过去 
		}
	} 
	ip[id]+=4; 
	print(id);
}

void calcul(int i,int id)//算数运算指令，2345，+-*/  
{
	int m,n,num;
	num=memory[i];
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(n==0)//立即数有操作 
	{
		if(m==1||m==2||m==3||m==4)
			normal_register[id][m]=four_situ(num,normal_register[id][m],change(i));//立即数加减乘除 
		else
			ten_to_two(normal_register[id][m],four_situ(num,change(normal_register[id][m]-2),change(i)));//运算完以后，重新变成两个十进制 
	} 
	else
	{
		if(m<=4&&n<=4)	normal_register[id][m]=four_situ(num,normal_register[id][m],normal_register[id][n]);//直接数字运算 
		if(m<=4&&n>4)	normal_register[id][m]=four_situ(num,normal_register[id][m],change(normal_register[id][n]-2));//change为10进制后运算 
		if(m>4&&n<=4)	ten_to_two(normal_register[id][m],four_situ(num,change(normal_register[id][m]-2),normal_register[id][n]));//change为10进制后运算，再转为两个十进制 
		if(m>4&&n>4)	ten_to_two(normal_register[id][m],four_situ(num,change(normal_register[id][m]-2),change(normal_register[id][n]-2)));//change为10进制后运算，再转为两个十进制
	}
	ip[id]+=4;
	print(id);
}

void logic(int i,int id)//逻辑运算指令，678，逻辑与，逻辑或，逻辑非
{
	int m,n,num;
	num=memory[i];
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(n==0)//立即数有操作
	{
		if(m<4)		normal_register[id][m]=three_logic(num,normal_register[id][m],change(i));//直接运算 
		else  		ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),change(i)));//经过转化后运算 
	}
	else if(m==0)//立即数操作 
	{
		if(n<4)		normal_register[id][n]=three_logic(num,normal_register[id][n],change(i));//直接运算
		else  		ten_to_two(normal_register[id][n],three_logic(num,change(normal_register[id][n]-2),change(i)));//经过转化后运算 
	}
	else
	{
		if(m<=4&&n<=4)	normal_register[id][m]=three_logic(num,normal_register[id][m],normal_register[id][n]);//直接运算 
		if(m<=4&&n>4)	normal_register[id][m]=three_logic(num,normal_register[id][m],change(normal_register[id][n]-2));//经过转化后运算 
		if(m>4&&n<=4)	ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),normal_register[id][n]));//经过转化后运算 
		if(m>4&&n>4)	ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),change(normal_register[id][n]-2)));//经过转化后运算
	}
	ip[id]+=4;
	print(id);
} 

void compare(int i,int id)//比较指令，9，
{
	int m,n,num;
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(n==0)
	{
		if(m<=4)	comp_num(normal_register[id][m],change(i),id);//直接比较 
		if(m>4)		comp_num(change(normal_register[id][m]-2),change(i),id);//都转换一下再比较 
	}
	else
	{
		if(m<=4&&n<=4)	comp_num(normal_register[id][m],normal_register[id][n],id);//直接比较 
		if(m<=4&&n>4)	comp_num(normal_register[id][m],change(normal_register[id][n]-2),id);//转换一下再比较 
		if(m>4&&n<=4)	comp_num(change(normal_register[id][m]-2),normal_register[id][n],id);//转换一下再比较 
		if(m>4&&n>4)	comp_num(change(normal_register[id][m]-2),change(normal_register[id][n]-2),id);//转换一下再比较 
	} 
	ip[id]+=4;
	print(id);
} 

void jump(int i,int id)//跳转指令，10，
{
	int m,n,num;
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
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

void in_out(int i,int id)//输入输出指令 ，11，12
{
	int m,n,num;
	short code;
	num=memory[i];
	ir[id]=memory[i]*deta+memory[i+1];
	m=memory[i+1]/16;//前一部分 
	n=memory[i+1]%16;//后一部分
	if(num==11)//输入 
	{
		printf("in:\n");
		scanf("%hd",&code);
		if(m<=4)	normal_register[id][m]=code;//保存在1234 
		if(m>4)		ten_to_two(normal_register[id][m],code);//保存在5678指向的内存单元 
	}
	if(num==12)//输出 
	{
		if(m<=4)	code=normal_register[id][m];
		if(m>4)		code=change(normal_register[id][m]-2);
		printf("id = %d    out: %hd\n",id,code);
		printf("\n");
	}
	ip[id]+=4;
	print(id);
}

void ask_for_lock(int i,int id)//请求互斥对象，13 
{
	ir[id]=memory[i]*deta+memory[i+1];
	WaitForSingleObject(mlock,INFINITE);//因为实际只锁了一个内存 
	ip[id]+=4;
	print(id);
}

void release(int i,int id)//释放互斥对象，14
{
	ir[id]=memory[i]*deta+memory[i+1];
	ReleaseMutex(mlock);
	ip[id]+=4;
	print(id);
} 

void sleep(int i,int id)//休眠，15
{
	ir[id]=memory[i]*deta+memory[i+1];
	Sleep(change(i));
	ip[id]+=4;
	print(id);
} 

/*下面是一些辅助函数*/ 
short change(int i)//内存中连着两个的数（十进制表示的）转化为十进制的数，注意是第3、4位 
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
		memory[m]=memory[m]*2+b[i];//前8位 
	for(i=8;i<16;i++)
		memory[m+1]=memory[m+1]*2+b[i];//后8位 
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

void comp_num(short m,short n,int id)//比较大小，修改flag 
{
	if(m==n)	flag[id]=0;
	if(m<n)		flag[id]=-1;
	if(m>n)		flag[id]=1;
}

