# include <stdio.h>
# include <windows.h>
# include <process.h> 

#define deta 256 
#define address1 0   //��һ�����ĵ���ʼ��ַ 
#define address2 256 //�ڶ������ĵ���ʼ��ַ 
typedef unsigned char BYTE;
HANDLE mlock,plock;

short ip[3]={0},flag[3]={0};//����������ͱ�־�Ĵ���
short normal_register[3][9]={0},ir[3]={0};//ͨ�üĴ�����1234��5678 ��ָ��Ĵ��� 
BYTE memory[32768]={0};//������ڴ� ,��������127��������-256 

int loadByte(FILE* fin);//��8λ��ת��Ϊʮ���� ;
int read_instructions(char* name,int id);//����ָ��ڴ��У�����ʹ�õ��ֽ���i; 
unsigned __stdcall run(void* pArguments);//���г���
void stop(int i,int id);//ͣ��ָ�0
void trans(int i,int id);//���ݴ���ָ�1 
void calcul(int i,int id);//��������ָ�2345��+-*/  
void logic(int i,int id);//�߼�����ָ�678���߼��룬�߼����߼���
void compare(int i,int id);//�Ƚ�ָ�9��
void jump(int i,int id);//��תָ�10��
void in_out(int i,int id);//�������ָ�� ��11��12
void ask_for_lock(int i,int id);//���󻥳����13 
void release(int i,int id);//�ͷŻ������14
void sleep(int i,int id);//���ߣ�15

void print(int id); //���״̬�ĺ��� 
short change(int i);//�ڴ������������Ķ����ƣ�ʮ���Ʊ�ʾ�ģ�ת��Ϊʮ���Ƶ��� 
void ten_to_two(short m,short num);//��num��Ϊ�����ƣ�ת��Ϊ����ʮ���ƣ���������memory[m]��[m+1] 
short four_situ(int num,short m,short n);//�Ӽ��˳� 
short three_logic(int num,short m,short n);//���ǣ�
void comp_num(short m,short n,int id);//�Ƚϴ�С���޸�flag 
void print_code();
void print_data();

int main(void)//������ 
{
	ten_to_two(16384,100);//��ʼ�� 
	HANDLE hThread1,hThread2;//��� 
	unsigned ThreadID1,ThreadID2;    
	int id1=1,id2=2;//ID�Ŵ����ĸ����� 
	
	mlock=CreateMutex(NULL,FALSE,NULL);//��ס16384�Ļ������ 
	plock=CreateMutex(NULL,FALSE,NULL);//��ס���ʱ��Ļ������
	
	hThread1=(HANDLE)_beginthreadex(NULL,0,run,(void*)&id1,0,&ThreadID1);
	hThread2=(HANDLE)_beginthreadex(NULL,0,run,(void*)&id2,0,&ThreadID2);
	
	WaitForSingleObject(hThread1, INFINITE); 
	CloseHandle (hThread1);  
 	WaitForSingleObject(hThread2, INFINITE); 
	CloseHandle (hThread2);
	
	print_code();
	print_data();
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

int read_instructions(char* name,int id)//����ָ��ڴ��У�����ʹ�õ��ֽ���top; 
{
	FILE *ftp;
	ftp=fopen(name,"r");
	int i,top=0;
	if(id==1)	i=address1;//����1 
	else		i=address2;//����2 
	while(!feof(ftp))
	{
		memory[i++]=loadByte(ftp);
		top++;
	}
	fclose(ftp);
	return top-1;
} 
 
unsigned __stdcall run(void* pArguments) //���г��� 
{
	int top,i,id,address=0;
	int	*p ;
	p = (int *)  pArguments ;
	id=*p;
	if(id==1)//����1 
	{
		i=address1;
		address=address1;
		top=read_instructions("dict1.dic",id);
	}
	else if(id==2)//����2 
	{
		i=address2;
		address=address2;
		top=read_instructions("dict2.dic",id);
	}
	for(;i<address+top;i=address+ip[id])//Ϊ����ת 
	{
		switch(memory[i])
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
	_endthreadex(0);//�����߳�
	return 0; 
}

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
			memory[normal_register[id][m]]=memory[normal_register[id][n]];
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
		if(m<4)		normal_register[id][m]=three_logic(num,normal_register[id][m],change(i));//ֱ�Ӽ��� 
		else  		ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),change(i)));//����ת������� 
	}
	else if(m==0)//���������� 
	{
		if(n<4)		normal_register[id][n]=three_logic(num,normal_register[id][n],change(i));//ֱ�Ӽ���
		else  		ten_to_two(normal_register[id][n],three_logic(num,change(normal_register[id][n]-2),change(i)));//����ת������� 
	}
	else
	{
		if(m<=4&&n<=4)	normal_register[id][m]=three_logic(num,normal_register[id][m],normal_register[id][n]);//ֱ�Ӽ��� 
		if(m<=4&&n>4)	normal_register[id][m]=three_logic(num,normal_register[id][m],change(normal_register[id][n]-2));
		if(m>4&&n<=4)	ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),normal_register[id][n]));
		if(m>4&&n>4)	ten_to_two(normal_register[id][m],three_logic(num,change(normal_register[id][m]-2),change(normal_register[id][n]-2)));
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
		if(m<=4&&n<=4)	comp_num(normal_register[id][m],normal_register[id][n],id);
		if(m<=4&&n>4)	comp_num(normal_register[id][m],change(normal_register[id][n]-2),id);
		if(m>4&&n<=4)	comp_num(change(normal_register[id][m]-2),normal_register[id][n],id);
		if(m>4&&n>4)	comp_num(change(normal_register[id][m]-2),change(normal_register[id][n]-2),id);
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
	if(num==11)
	{
		printf("in:\n");
		scanf("%hd",&code);
		if(m<=4)	normal_register[id][m]=code;//������1234 
		if(m>4)		ten_to_two(normal_register[id][m],code);//������5678ָ����ڴ浥Ԫ 
	}
	if(num==12)
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
	WaitForSingleObject(mlock,INFINITE);
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

void print(int id) //���״̬�ĺ��� 
{
	int i,address;
	if(id==1)	address=0;
	else		address=256;
	WaitForSingleObject(plock,INFINITE);
	printf("id = %d\n",id);
	printf("ip = %hd\n",ip[id]+address);
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

void comp_num(short m,short n,int id)//�Ƚϴ�С���޸�flag 
{
	if(m==n)	flag[id]=0;
	if(m<n)		flag[id]=-1;
	if(m>n)		flag[id]=1;
}
