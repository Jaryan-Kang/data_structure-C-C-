#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define NumOfASC 128   //ASCII 码个数（为了保证每一个字符都可以被记录到
#define MAXSIZE 2000   // 最大字符数
#define STACKSIZE 200  //栈大小
#define INPUTNUM 3      //输入参数个数
#define TOCODELEN 19    //最小待编码字符长度
#define ONECODE   1    //防止单字符重复
#pragma warning(disable: 4996)//我也不知道strcpy咋就有问题了。我就直接给他屏蔽了

typedef struct HuffTNode
{
	char ch;//字符
	int weight;//权值
	int parent, lchild, rchild;//结点的父母，左右孩子
	int degree;//节点的度
	int floor;//节点的层数
}HuffTNode, * HuffmanTree;

typedef char** HuffCodeStr;  //霍夫曼编码和译码字符串

typedef struct
{
	HuffTNode* base;  //栈底
	HuffTNode* top;   //栈顶
	int stacksize;     //栈大小
}HuffStack;//霍夫曼树的栈

//霍夫曼树栈及基本操作
int InitStack(HuffStack& hStack)   // 初始化栈
{
	hStack.base = (HuffTNode*)malloc(STACKSIZE * sizeof(HuffTNode));
	if (!hStack.base)
		return -1;
	hStack.top = hStack.base;
	hStack.stacksize = STACKSIZE;
	return 0;
}
// 节点压栈
int PushNode(HuffStack& htStack, HuffTNode htNode)
{
	if (htStack.top - htStack.base == STACKSIZE)
		return -1;
	*(htStack.top++) = htNode;
	return 0;
}
// 节点出栈
int PopNode(HuffStack& hStack, HuffTNode& htNode)
{
	if (hStack.base == hStack.top)
		return -1;
	htNode = *(--hStack.top);
	return 0;
}
// 判断栈是否为空
int StackIsEmpty(HuffStack& hStack)
{
	if (hStack.base == hStack.top)
		return 1;
	else return 0;
}
//霍夫曼树栈及基本操作

//从huffTree[1]到huffTree[n]中选择parent为0且weight最小的两个节点，把其序号赋给num1，num2
int SelectNode(HuffmanTree& huffTree, int n, int& num1, int& num2)
{
	int i = 0;
	num1 = 0;//初始化num1，num2
	num2 = num1 + 1;
	for (i = 1; i <= n; i++)
	{
		if (huffTree[i].parent == 0 && huffTree[i].weight != 0)
		{
			num1 = i;
			break;
		}
	}//找到第一个符合要求的结点，记录该节点序号为num1
	for (i = num1 + 1; i <= n; i++)
	{
		if (huffTree[i].parent == 0 && huffTree[i].weight != 0)
		{
			num2 = i;
			break;
		}
	}//再找到一个符合要求的结点，记录该节点序号为num2
	if (huffTree[num1].weight > huffTree[num2].weight)
	{
		int temp = num1;
		num1 = num2;
		num2 = temp;
	}//保证num1的权值小于num2的权值
	//没有专门写权值相等，保证num1的字符更小的代码，因为按照统计不相同字符的代码的思路，只要不交换num1/num2，就能保证按照ASC码从小到大的顺序排列
	if (num1 > num2)
	{
		i = num1;
	}
	else
	{
		i = num2;
	}
	// 从数值较大的那一位开始向后比较，因为刚刚可能涉及了num1/num2的交换
	i++;
	for (; i <= n; i++)//从数值较大的那一位开始向后比较，因为可能涉及了num1/num2的交换
	{
		if (huffTree[i].weight != 0 && huffTree[i].parent == 0)//选择weight不为0且parent为0的结点
		{
			if (huffTree[i].weight < huffTree[num1].weight)  //若该结点weight小于num1结点weight，则该结点最小，
			{                                               //修改num1为该位置，修改num2为原num1
				num2 = num1;
				num1 = i;
			}
			else if (huffTree[i].weight < huffTree[num2].weight)  //若该结点weight小于num2结点weight，则该结点次小，
			{                                                     //修改num2为该位置
				num2 = i;
			}
		}
	}
	return 0;
}

//霍夫曼编码实现函数
int HuffmanCode(HuffmanTree& huffTree, HuffCodeStr& huffStr, int* weight, int n, char* chBuff)
{                      //weight为n个字符权值的数组，chBuff为存放n个字符的字符串
	int m = 2 * n - 1;//共需要的单元数量
	huffTree = (HuffmanTree)malloc((m + 1) * sizeof(HuffTNode));
	if (!huffTree)
	{
		printf("ERROR_02");
		return -1;
	}//入参检查，如果内存分配失败，说明编码失败，返回ERROR_02
	int i = 1;
	for (i = 1; i <= n; i++)
	{
		huffTree[i].ch = chBuff[i - 1];
		huffTree[i].weight = weight[i - 1];
		huffTree[i].parent = 0;
		huffTree[i].lchild = 0;
		huffTree[i].rchild = 0;
		huffTree[i].degree = 0;
		huffTree[i].floor = 1;
	}//初始化叶子结点
	for (; i <= m; ++i)
	{
		huffTree[i].ch = '\0';
		huffTree[i].weight = 0;
		huffTree[i].parent = 0;
		huffTree[i].lchild = 0;
		huffTree[i].rchild = 0;
		huffTree[i].degree = 2;
		huffTree[i].floor = 1;
	}//初始化其他结点
	int num1 = 0, num2 = 0;
	for (i = n + 1; i <= m; ++i)
	{
		SelectNode(huffTree, i - 1, num1, num2);
		huffTree[num1].parent = i;
		huffTree[num2].parent = i;
		huffTree[i].lchild = num1;
		huffTree[i].rchild = num2;
		huffTree[i].weight = huffTree[num1].weight + huffTree[num2].weight;
	}
	//从叶子到根逆向求每个字符的赫夫曼编码
	huffStr = (HuffCodeStr)malloc((n + 1) * sizeof(char*));//分配n个字符编码的头指针向量
	if (!huffStr)
	{
		printf("EEROR_02");
		return -1;
	}//入参检查，如果内存申请失败，说明编码失败
	char* chCode = (char*)malloc(n * sizeof(char));//分配求编码的工作空间
	if (!chCode)
	{
		printf("EEROR_02");
		return -1;
	}//入参检查，如果内存申请失败，编码失败
	for (i = 0; i <= n - 2; i++)
	{
		chCode[i] = 0;//初始化工作空间
	}
	chCode[n - 1] = '\0';//编码结束符
	int start = 0;//start为编码结束符位置，初始化为0
	int f = 1; int c = 1;
	for (i = 1; i <= n; ++i)//逐个字符求霍夫曼编码
	{
		start = n - 1;//编码结束符位置先设为最后
		for (c = i, f = huffTree[i].parent; f != 0; c = f, f = huffTree[f].parent)
		{
			if (huffTree[f].lchild == c)
			{
				chCode[--start] = '0';
			}
			else
			{
				chCode[--start] = '1';
			}
		}
		huffStr[i] = (char*)malloc((n - start) * sizeof(char));//为第i个字符编码分配空间
		if (!huffStr[i])
		{
			printf("EEROR_02");
			return -1;
		}//内存申请失败，编码失败
		strcpy(huffStr[i], &chCode[start]);//从工作空间cd复制编码到HC
	}
	free(chCode);//malloc和free对应
	return 0;
}

//利用先序遍历霍夫曼树栈
int TraverseInOrder(HuffmanTree& huffTree, int n)
{
	HuffTNode ptr = huffTree[2 * n - 1];
	ptr.floor = 1;//设置一个标记结点，先指向根节点
	HuffStack stack;
	InitStack(stack);
	PushNode(stack, ptr);//由于是先序遍历，所以先将根节点压栈
	while (!StackIsEmpty(stack))//若栈不空
	{
		PopNode(stack, ptr);
		printf("结点字符：%c 权值：%d 度：%d 层数：%d\n", ptr.ch, ptr.weight, ptr.degree, ptr.floor);
		if (ptr.rchild != 0)
		{
			huffTree[ptr.rchild].floor = ptr.floor + 1;
			PushNode(stack, huffTree[ptr.rchild]);
		}
		if (ptr.lchild != 0)
		{
			huffTree[ptr.lchild].floor = ptr.floor + 1;
			PushNode(stack, huffTree[ptr.lchild]);
		}
	}
	free(stack.base);//malloc和free对应
	return 0;
}

//霍夫曼译码实现函数
int HuffmanDecode(HuffmanTree& huffTree, HuffCodeStr& huffStr, char* chTC, char* chTD, int n)
{                        //chTC为输入的待编码字符串，chTD为输入的待译码字符串
	int pos = 2 * n - 1;//标记根节点位置
	int i = 0;
	int j = 1;
	char codeRes[100][20];
	char decodeStr[100];//定义两个数组储存编码和译码的结果。防止编码成功但译码失败时，输出编码结果和ERROR_03，与题目要求不符
	int codePos = 0;//编码数组的下标
	int decodePos = 0;//译码数组的下标
	for (i = 0; chTC[i] != '\0'; i++)
	{
		for (j = 1; j <= 2 * n - 1; j++)
		{
			if (huffTree[j].ch == chTC[i])
			{
				strcpy(codeRes[codePos], huffStr[j]);
				codePos++;
				break;
			}
		}
	}//得到该字符串编码结果
	while (*chTD != '\0')
	{
		if (*chTD == '0')//若为0,指向左孩子
			pos = huffTree[pos].lchild;
		else if (*chTD == '1')//若为1，指向右孩子
			pos = huffTree[pos].rchild;
		else//若为非0,1的数，译码失败
		{
			printf("ERROR_03");
			return -1;
		}
		chTD++;//依次读取字符串b中字符
		if (!huffTree[pos].lchild && !huffTree[pos].rchild)//若为叶子结点,存储结点字符，并使标记返回根节点
		{
			decodeStr[decodePos] = huffTree[pos].ch;
			decodePos++;
			pos = 2 * n - 1;
		}
	}
	if (pos != 2 * n - 1 && huffTree[pos].ch == '\0')//若字符串b已结束，而标记结点的字符为'\0'，说明还未到叶子结点，编码失败
	{
		printf("ERROR_03");
		return -1;
	}
	for (i = 0; i < codePos; i++)
	{
		printf("%s", codeRes[i]);
	}
	printf(" ");
	for (i = 0; i < decodePos; i++)
	{
		printf("%c", decodeStr[i]);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc != INPUTNUM)
	{
		printf("EEROR_01");
		return -1;
	}
	int chAsc[NumOfASC] = { 0 };//大小为128的数组，用于记录出现的字符，按照asc码的顺序储存
	char strCode[MAXSIZE] = { '\0' };  //待编码字符串
	char strDecode[MAXSIZE] = { '\0' }; //待译码字符串
	strcpy(strCode, argv[1]);
	strcpy(strDecode, argv[2]);
	int i = 0;
	for (i = 0; strCode[i] != '\0'; i++)
	{
		chAsc[(int)strCode[i]]++;//记录权值
	}
	if (i < TOCODELEN)  //如果待编码字符串的长度小于20，提示编码失败
	{
		printf("ERROR_02");
		return -1;
	}
	int weight[NumOfASC] = { 0 };//weight为存放字符权值的数组
	char chBuff[NumOfASC] = { 0 };//chBuff为存放字符的字符串
	int n = 0;//统计不相同的字符个数
	for (i = 0; i < NumOfASC; i++)
	{
		if (chAsc[i] != 0)
		{
			chBuff[n] = (char)i;//chBuff数组中，下标从小到大，ASCII码也从小到大，所以SelectNode函数中，num1，num2(num1<num2)指向的结点的字符满足按字母顺序排列
			weight[n] = chAsc[i];
			n++;//修改位置向后移动
		}
	}
	if (n == ONECODE)//如果待编码字符中只有一种字符，提示编码失败
	{
		printf("ERROR_02");
		return -1;
	}
	HuffmanTree huffTree;  //霍夫曼树
	HuffCodeStr huffStr;   //霍夫曼编码
	HuffmanCode(huffTree, huffStr, weight, n, chBuff);  //编码
	TraverseInOrder(huffTree, n);//遍历并打印各个节点的信息
	HuffmanDecode(huffTree, huffStr, strCode, strDecode, n); //译码
	free(huffTree);//释放huffTree的空间
	free(huffStr);//释放huffStr的空间
	return 0;
}