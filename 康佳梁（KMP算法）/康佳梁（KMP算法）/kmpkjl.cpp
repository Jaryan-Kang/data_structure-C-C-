#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#define MAXIMUM 100
#define OVERFLOW -1
#define OK 1
#define INPUTNUM 3   //��������ĸ���
#define STARTPOS 1   //ƥ����ʼλ��
#define NOTMATCH -1  // û��ƥ��

//���崮
typedef struct {
	char* ch;   //��������
	int length; //���ĳ���
}HeapString;

void StrAssign(HeapString* heapStr, char* chAss)//����һ���������ҽ��и�ֵ���� 
{
	if (heapStr->ch != NULL)//�����ڿմ� 
	{
		free(heapStr->ch);  //���ͷſռ�
		heapStr->ch = NULL; //���
	}
	int length = strlen(chAss);//�õ��ַ���chAss�ĳ���
	if (!length)//�ж�chAss�Ƿ�Ϊ��
	{
		heapStr->length = 0;
	}
	else
	{
		heapStr->ch = (char*)malloc((length + 1) * sizeof(char)); //����ռ�
		if (heapStr->ch == NULL)//����Ƿ����ɹ� 
		{
			exit(OVERFLOW);
		}
		int i = 0;
		for (i = 0; i < length; i++)
		{
			heapStr->ch[i + 1] = chAss[i];   //��ֵ
			heapStr->length = length;   //���ô�����
		}
	}
}//Ҫ����i+1������Ϊ�Ƚϵ�ʱ��Ҫ�����ͻ��ģʽ�����ǵ�next�����Ǵ�0��ʼ��

int StrCompare(HeapString heapStr1, HeapString heapStr2)//�Ƚ������� 
{
	for (int i = 0; i < heapStr1.length && i < heapStr2.length; ++i)
	{
		if (heapStr1.ch[i] != heapStr2.ch[i])
		{
			return (heapStr1.ch[i] - heapStr2.ch[i]);
		}
	}
	return (heapStr1.length - heapStr2.length);
}//ֻ��Ϊ�������� 

void ClearString(HeapString heapStr)//��մ� 
{
	if (heapStr.ch != NULL)//���ж� 
	{
		free(heapStr.ch);
		heapStr.ch = NULL;
		heapStr.length = 0;
	}
}


void GetPrefix(HeapString strPattern, int nextVal[])//next�����Ʊ� ,�㷨�����Ƕ�ģʽ�����з��� 
{
	int i = 1;//i�����ݣ�ֻ��j���ݣ� 
	int j = 0;
	nextVal[0] = 0;
	for (i = 1; i < strPattern.length; i++)                    // ע��i��1��ʼ���������ܺ�j�Ƚ�
	{                                                          //����ǰ��׺����ͬ�������Ǹ������Ĺ��̣�������while����if��
		while (j > 0 && strPattern.ch[i] != strPattern.ch[j])  //����Ϊj��ʼλ����0�������ٻ��ݣ�������j>0
		{
			j = nextVal[j - 1];  // ��ǰ���ݣ�����ǰһλ��nextVal�е�λ�� 
		}
		if (strPattern.ch[i] == strPattern.ch[j])              // �ҵ���ͬ��ǰ��׺
		{
			j++;
		}                                                       //����ǰ��׺���ȼ�1 
		nextVal[i] = j;                                           // ��j��ǰ׺�ĳ��ȣ�����nextVal[i]����ǰ��׺�Ƿ���ͬ����Ҫ���
	}
}//abxaewabxab
 //abxaewabxab

//kmp�㷨ʵ��ģʽƥ��
int KMPMatch(HeapString strOriginal, HeapString strPattern, int pos)
{
	int* nextVal = (int*)malloc((strPattern.length + 1) * sizeof(int));//��̬����ģʽ����Ӧ��ǰ׺prefix����nextVal�Ĵ�С
	if (!nextVal)
	{
		exit(OVERFLOW);
	}
	GetPrefix(strPattern, nextVal);////�õ�ǰ׺nextVal����
	int i = pos;
	int j = 1;
	while (i < strOriginal.length && j <= strPattern.length)
	{
		if (j == 0 || strOriginal.ch[i] == strPattern.ch[j])
		{        //�����Ƚ�
			j++;
			i++;//abababkcedabke   abkc
		}
		else //�����һ���
			j = nextVal[j];
	}
	free(nextVal);
	if (j > strPattern.length)
	{
		return i - strPattern.length;//Ŀ�괮��ģʽ���е�ƥ����ʼλ��
	}
	else
	{
		return NOTMATCH;
	}
}

int main(int argc, char* argv[])
{
	if (argc != INPUTNUM)
	{
		printf("ERROR_01");  //�����������!= 3������ERROR_01
	}
	else
	{
		HeapString strOriginal = { NULL, 0 };      // Ҫƥ���ԭ��, һ��Ҫ��ʼ��ΪNULL
		HeapString strPattern = { NULL, 0 };      // ģʽ��, һ��Ҫ��ʼ��ΪNULL
		StrAssign(&strOriginal, argv[1]);       //����Ҫƥ���ԭ��
		StrAssign(&strPattern, argv[2]);        //����ģʽ��
		int res = KMPMatch(strOriginal, strPattern, STARTPOS);  //��KMP�㷨ƥ��
		printf("%d", res);
		ClearString(strOriginal);//�ͷ�ԭ������Ŀռ�
		ClearString(strPattern);//�ͷ�ģʽ������Ŀռ�
	}
	return 0;
}
