#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#define OK					1
#define ERROR				0   //δ����
#define ERROR_02			2	//���ʽ���ڸ�ʽ����
#define ERROR_03			-5	//���ʽ�ڼ�������г���
#define OVERFLOW			-2  //ջ����ռ�ʧ��
#define OperandType			double
#define SElemType			double
#define STACK_INIT_SIZE		100//ջ�ĳ�ʼ��С
#define STACKINCREMENT		10//ջ���ݵ��δ�С
#define row					10//��
#define	col					10//��
typedef int Status;
typedef struct stack
{
	int stacksize;
	SElemType* base;
	SElemType* top;
}NumStack;

NumStack OPTR;//�����ջ
NumStack OPND;//������ջ

Status InitStack(NumStack& S)//InitStack ����ջS
{
	S.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!S.base)
		exit(OVERFLOW);
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}
Status Push(NumStack& S, SElemType e)//Push �����µ�ջ��Ԫ��e, ���ջ����������Ҫ����
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
Status Pop(NumStack& S, SElemType& e)//Pop Sջ�ǿգ���ɾ��ջ��Ԫ�أ���e����ջ��Ԫ��ֵ��������OK�����򷵻�ERROR
{
	if (S.top == S.base)
		return ERROR;
	else
	{
		e = *--S.top;
		return OK;
	}
}
double GetTop(NumStack S, SElemType& e)//Gettop ��ȡջ��
{
	if (S.top == S.base)
		return ERROR;

	e = *(S.top - 1);
	return e;
}
int StackLength(NumStack S)//StrackLength ��ȡջԪ�ظ���
{
	return S.top - S.base;
}
Status DestoryStack(NumStack& S)//DestroyStack ����ջ
{
	free(S.base);
	S.base = NULL;
	S.top = NULL;
	S.stacksize = 0;
	return OK;
}

Status Judge(char c)//Judge �ж������
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

int change(char theta)//���������Ϊ���֣���������ȼ�����
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
int Precede(double theta1, double theta2)//Precede �Ƚ�����������ȼ������Ƚ�ʧ�ܷ���ERROR_02
{
	int result;
	int i = 0, j = 0;
	int pre[row][col] =  //�������ȼ����󣬰������ȼ�����
		//1Ϊ>��-1Ϊ<��0Ϊ=��2Ϊ�﷨���� , �ڼ���ʱ�����ȷ���һ��#�Ա��ں����Ƚϣ���Ϊ��������#���ȼ���ͣ���Ϊ��������#���ȼ����
	 //���ȼ�����
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

double Operate(double n1, double theta, double n2, int expjudge)//Operate ��������
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
			expjudge = ERROR_03;//0�ڷ�ĸ��
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

OperandType CalculateExpression(char* s, int length, int expjudge)//CalculateExpression ��ֵ���ʽ
{
	char c = '\0';//c�����������ʽ
	double o = 10.0;
	double optrcmp;//OPTR�е������ȡ������optrcmp�У������������ȼ��Ƚ�
	double theta;//thetaΪ�����
	double n1, n2;//Operate���������������������������
	double e = 0;//OPND��ջԪ��
	InitStack(OPTR);
	Push(OPTR, '#');//ΪOPTRջԤ����'#'���Խ������ȼ��ж�
	InitStack(OPND);
	int j = 0;//j�����ָ�ÿһ�����������ж�������С������
	for (int i = 0; i < length - 1; i++)
	{
		if (Judge(s[i]) && Judge(s[i + 1]))//�����������ǲ�����
		{
			if (s[i] == ')' || s[i] == ']')
				break;
			else if (s[i + 1] == '(' || s[i + 1] == '[')
				break;//�޳����������Ų�������ȷ���
			else
			{
				expjudge = ERROR_02;
				return expjudge;
			}//���ʽ��ʽ��������++
		}
	}
	if (expjudge != ERROR_02 && expjudge != ERROR_03)//�����ڱ��ʽ��ʽ����������߼�����
	{
		for (int i = 0; i < length; i++)
		{
			c = s[i];
			if (!Judge(c))//�ж����ڲ����� 
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
						o = o * 10;//����С��������ȷ����
					}
				}//if
				else
					expjudge = ERROR_02;//��������Ĳ�������Ҳ������������������֣���������ERROR_02
			}//���ַ��洢�ı��ʽ�е�������ת���ɸ���������
			else//�ж����ڲ�����
			{
				j = 0;
				switch (Precede(GetTop(OPTR, optrcmp), (double)c))//OPTR�е������ȡ������optrcmp�У��������ȼ��Ƚ�
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
					i--;//������ɺ�������бȽ�
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
		return GetTop(OPND, e);//������ʽ
	}
}//�Ա��ʽ��������
int main(int argc, char* argv[])
{
	int expjudge = OK;
	int length = 0;
	length = strlen(argv[1]);//argv[1]�ĳ���
	char* s, * p;//����argv
	s = (char*)malloc((length) * sizeof(char));
	if (!s)
		return ERROR;
	p = argv[1];
	int i = 0;
	while (*p) {
		s[i] = *p;
		p++;
		i++;
	}//s�зŽ�argv
	if (length < 3)
		printf("ERROR_02");//��������С��3���ַ���
	else
	{
		double answer = CalculateExpression(s, length, expjudge);
		if (answer == ERROR_02)
			printf("ERROR_02");//���ʽ��ʽ����
		else if (answer == ERROR_03)
			printf("ERROR_03");//���ʽ�߼�����
		else
		{
			if (answer == (int)answer)
				printf("%.0lf", answer);//�������
			else
				printf("%g", answer);//С�����м�λ�����λ
		}
	}
	DestoryStack(OPTR);
	DestoryStack(OPND);
	free(s);
	return 0;
}