#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#define OK					1
#define ERROR				0   //未输入
#define ERROR_02			2	//表达式存在格式错误
#define ERROR_03			-5	//表达式在计算过程中出错
#define OVERFLOW			-2  //栈分配空间失败
#define OperandType			double
#define SElemType			double
#define STACK_INIT_SIZE		100//栈的初始大小
#define STACKINCREMENT		10//栈扩容单次大小
#define row					10//行
#define	col					10//列
typedef int Status;
typedef struct stack
{
	int stacksize;
	SElemType* base;
	SElemType* top;
}NumStack;

NumStack OPTR;//运算符栈
NumStack OPND;//运算数栈

Status InitStack(NumStack& S)//InitStack 构造栈S
{
	S.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!S.base)
		exit(OVERFLOW);
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}
Status Push(NumStack& S, SElemType e)//Push 插入新的栈顶元素e, 如果栈已满，则需要扩容
{
	if (S.top - S.base >= S.stacksize)
	{
		double* q;
		q = (SElemType*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (!q) exit(OVERFLOW);
		S.base = q;
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return OK;
}
Status Pop(NumStack& S, SElemType& e)//Pop S栈非空，则删除栈顶元素，用e返回栈顶元素值，并返回OK；否则返回ERROR
{
	if (S.top == S.base)
		return ERROR;
	else
	{
		e = *--S.top;
		return OK;
	}
}
double GetTop(NumStack S, SElemType& e)//Gettop 获取栈顶
{
	if (S.top == S.base)
		return ERROR;

	e = *(S.top - 1);
	return e;
}
int StackLength(NumStack S)//StrackLength 获取栈元素个数
{
	return S.top - S.base;
}
Status DestoryStack(NumStack& S)//DestroyStack 销毁栈
{
	free(S.base);
	S.base = NULL;
	S.top = NULL;
	S.stacksize = 0;
	return OK;
}

Status Judge(char c)//Judge 判断运算符
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']' || c == '^' || c == '#')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int change(char theta)//定义运算符为数字，方便查优先级矩阵
{
	switch (theta)
	{
	case '+':return 0; break;
	case '-':return 1; break;
	case '*':return 2; break;
	case '/':return 3; break;
	case '(':return 4; break;
	case ')':return 5; break;
	case '^':return 6; break;
	case '[':return 7; break;
	case ']':return 8; break;
	case '#':return 9; break;
	}
}
int Precede(double theta1, double theta2)//Precede 比较运算符的优先级，若比较失败返回ERROR_02
{
	int result;
	int i = 0, j = 0;
	int pre[row][col] =  //定义优先级矩阵，按照优先级排序
		//1为>，-1为<，0为=，2为语法错误 , 在计算时，首先放入一个#以便于后续比较，作为行向量的#优先级最低，作为列向量的#优先级最高
	 //优先级矩阵
	{//		+	-	*	/	(	)	^	[	]	#
		{	1,	1,	-1,	-1,	-1,	1,	-1,	-1,	1,	1},//	+
		{	1,	1,	-1,	-1,	-1,	1,	-1,	-1,	1,	1},//	-
		{	1,	1,	1,	1,	-1,	1,	-1,	-1,	1,	1},//	*
		{	1,	1,	1,	1,	-1,	1,	-1,	-1	,1,	1},//	/
		{	-1,	-1,	-1,	-1,	2,	0,	-1,	2,	2,	2},//	(
		{	1,	1,	1,	1,	0,	2,	1,	1,	1,	1},//	)
		{	1,	1,	1,	1,	-1,	1,	1,	-1,	1,	1},//	^
		{	-1,	-1,	-1,-1,	-1,	2,	-1,	2,	0,	2},//	[
		{	1,	1,	1,	1,	1,	2,	1,	1,	2,	1},//	]
		{	-1,	-1,	-1,-1,	-1,	2,	-1,	-1,	2,	0} //	#
	};
	i = change((char)theta1);
	j = change((char)theta2);
	result = pre[i][j];
	return result;
}

double Operate(double n1, double theta, double n2, int expjudge)//Operate 定义运算
{
	double ans = 1;
	if ((char)theta == '+')
		ans = n1 + n2;
	else if ((char)theta == '-')
		ans = n1 - n2;
	else if ((char)theta == '*')
		ans = n1 * n2;
	else if ((char)theta == '/')
	{
		if (n2 == 0)
		{
			ans = ERROR_03;
			expjudge = ERROR_03;//0在分母上
		}
		else
			ans = n1 / n2;
	}
	else if ((char)theta == '^')
	{
		ans = pow(n1, n2);
	}
	return ans;
}

OperandType CalculateExpression(char* s, int length, int expjudge)//CalculateExpression 求值表达式
{
	char c = '\0';//c用来遍历表达式
	double o = 10.0;
	double optrcmp;//OPTR中的运算符取出放入optrcmp中，用来进行优先级比较
	double theta;//theta为运算符
	double n1, n2;//Operate函数参数，即参与运算的两个数
	double e = 0;//OPND入栈元素
	InitStack(OPTR);
	Push(OPTR, '#');//为OPTR栈预存入'#'用以进行优先级判断
	InitStack(OPND);
	int j = 0;//j用来分割每一个运算数及判断整数和小数部分
	for (int i = 0; i < length - 1; i++)
	{
		if (Judge(s[i]) && Judge(s[i + 1]))//相邻两个都是操作符
		{
			if (s[i] == ')' || s[i] == ']')
				break;
			else if (s[i + 1] == '(' || s[i + 1] == '[')
				break;//剔除两种由括号产生的正确情况
			else
			{
				expjudge = ERROR_02;
				return expjudge;
			}//表达式格式错误：例：++
		}
	}
	if (expjudge != ERROR_02 && expjudge != ERROR_03)//不存在表达式格式错误和运算逻辑错误
	{
		for (int i = 0; i < length; i++)
		{
			c = s[i];
			if (!Judge(c))//判定属于操作数 
			{
				if ((c >= '0' && c <= '9') || c == '.')
				{
					if (c == '.')
					{
						j = -1;
					}
					else if (j == 0)
					{
						e = (double)c - 48;
						Push(OPND, e);
						j = 1;
					}
					else if (j == 1)
					{
						Pop(OPND, e);
						e = e * 10.0 + ((double)c - 48);
						Push(OPND, e);
					}
					else if (j == -1)
					{
						Pop(OPND, e);
						e = e + ((double)c - 48) / o;
						Push(OPND, e);
						o = o * 10;//对于小数点后的正确操作
					}
				}//if
				else
					expjudge = ERROR_02;//例如输入的不是数字也不是运算符（比如文字？），返回ERROR_02
			}//将字符存储的表达式中的运算数转换成浮点数类型
			else//判定属于操作符
			{
				j = 0;
				switch (Precede(GetTop(OPTR, optrcmp), (double)c))//OPTR中的运算符取出放入optrcmp中，进行优先级比较
				{
				case -1:
					Push(OPTR, (double)c);
					break;
				case 0:
					Pop(OPTR, optrcmp);
					break;
				case 1:
					Pop(OPTR, theta);
					Pop(OPND, n2);
					Pop(OPND, n1);
					Push(OPND, Operate(n1, theta, n2, expjudge));
					i--;//运算完成后继续进行比较
					break;
				case ERROR_02:
					expjudge = ERROR_02;
					break;
				}
			}
		}
	}
	if (expjudge == ERROR_02)
		return ERROR_02;
	else if (expjudge == ERROR_03)
		return ERROR_03;
	else
	{
		Pop(OPTR, theta);
		Pop(OPND, n2); Pop(OPND, n1);
		Push(OPND, Operate(n1, theta, n2, expjudge));
		return GetTop(OPND, e);//运算表达式
	}
}//对表达式分析计算
int main(int argc, char* argv[])
{
	int expjudge = OK;
	int length = 0;
	length = strlen(argv[1]);//argv[1]的长度
	char* s, * p;//复制argv
	s = (char*)malloc((length) * sizeof(char));
	if (!s)
		return ERROR;
	p = argv[1];
	int i = 0;
	while (*p) {
		s[i] = *p;
		p++;
		i++;
	}//s中放进argv
	if (length < 3)
		printf("ERROR_02");//输入数量小于3个字符了
	else
	{
		double answer = CalculateExpression(s, length, expjudge);
		if (answer == ERROR_02)
			printf("ERROR_02");//表达式格式错误
		else if (answer == ERROR_03)
			printf("ERROR_03");//表达式逻辑错误
		else
		{
			if (answer == (int)answer)
				printf("%.0lf", answer);//输出整数
			else
				printf("%g", answer);//小数，有几位输出几位
		}
	}
	DestoryStack(OPTR);
	DestoryStack(OPND);
	free(s);
	return 0;
}