#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#define MAXIMUM 100
#define OVERFLOW -1
#define OK 1
#define INPUTNUM 3   //输入参数的个数
#define STARTPOS 1   //匹配起始位置
#define NOTMATCH -1  // 没有匹配

//定义串
typedef struct {
	char* ch;   //串的内容
	int length; //串的长度
}HeapString;

void StrAssign(HeapString* heapStr, char* chAss)//建立一个串，并且进行赋值操作 
{
	if (heapStr->ch != NULL)//不等于空串 
	{
		free(heapStr->ch);  //先释放空间
		heapStr->ch = NULL; //清空
	}
	int length = strlen(chAss);//得到字符串chAss的长度
	if (!length)//判断chAss是否为空
	{
		heapStr->length = 0;
	}
	else
	{
		heapStr->ch = (char*)malloc((length + 1) * sizeof(char)); //分配空间
		if (heapStr->ch == NULL)//检测是否分配成功 
		{
			exit(OVERFLOW);
		}
		int i = 0;
		for (i = 0; i < length; i++)
		{
			heapStr->ch[i + 1] = chAss[i];   //赋值
			heapStr->length = length;   //设置串长度
		}
	}
}//要赋到i+1，是因为比较的时候要避免冲突，模式串我们的next数组是从0开始的

int StrCompare(HeapString heapStr1, HeapString heapStr2)//比较两个串 
{
	for (int i = 0; i < heapStr1.length && i < heapStr2.length; ++i)
	{
		if (heapStr1.ch[i] != heapStr2.ch[i])
		{
			return (heapStr1.ch[i] - heapStr2.ch[i]);
		}
	}
	return (heapStr1.length - heapStr2.length);
}//只有为零才是相等 

void ClearString(HeapString heapStr)//清空串 
{
	if (heapStr.ch != NULL)//先判断 
	{
		free(heapStr.ch);
		heapStr.ch = NULL;
		heapStr.length = 0;
	}
}


void GetPrefix(HeapString strPattern, int nextVal[])//next数组制备 ,算法核心是对模式串进行分析 
{
	int i = 1;//i不回溯，只有j回溯！ 
	int j = 0;
	nextVal[0] = 0;
	for (i = 1; i < strPattern.length; i++)                    // 注意i从1开始，这样才能和j比较
	{                                                          //处理前后缀不相同，回溯是个连续的过程，所以是while不是if，
		while (j > 0 && strPattern.ch[i] != strPattern.ch[j])  //又因为j起始位置是0，不能再回溯，所以是j>0
		{
			j = nextVal[j - 1];  // 向前回溯，回溯前一位的nextVal中的位置 
		}
		if (strPattern.ch[i] == strPattern.ch[j])              // 找到相同的前后缀
		{
			j++;
		}                                                       //最长相等前后缀长度加1 
		nextVal[i] = j;                                           // 将j（前缀的长度）赋给nextVal[i]不管前后缀是否相同，都要存放
	}
}//abxaewabxab
 //abxaewabxab

//kmp算法实现模式匹配
int KMPMatch(HeapString strOriginal, HeapString strPattern, int pos)
{
	int* nextVal = (int*)malloc((strPattern.length + 1) * sizeof(int));//动态分配模式串对应的前缀prefix数组nextVal的大小
	if (!nextVal)
	{
		exit(OVERFLOW);
	}
	GetPrefix(strPattern, nextVal);////得到前缀nextVal数组
	int i = pos;
	int j = 1;
	while (i < strOriginal.length && j <= strPattern.length)
	{
		if (j == 0 || strOriginal.ch[i] == strPattern.ch[j])
		{        //继续比较
			j++;
			i++;//abababkcedabke   abkc
		}
		else //串向右滑动
			j = nextVal[j];
	}
	free(nextVal);
	if (j > strPattern.length)
	{
		return i - strPattern.length;//目标串在模式串中的匹配起始位置
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
		printf("ERROR_01");  //输入参数个数!= 3，返回ERROR_01
	}
	else
	{
		HeapString strOriginal = { NULL, 0 };      // 要匹配的原串, 一定要初始化为NULL
		HeapString strPattern = { NULL, 0 };      // 模式串, 一定要初始化为NULL
		StrAssign(&strOriginal, argv[1]);       //创建要匹配的原串
		StrAssign(&strPattern, argv[2]);        //创建模式串
		int res = KMPMatch(strOriginal, strPattern, STARTPOS);  //用KMP算法匹配
		printf("%d", res);
		ClearString(strOriginal);//释放原串分配的空间
		ClearString(strPattern);//释放模式串分配的空间
	}
	return 0;
}
