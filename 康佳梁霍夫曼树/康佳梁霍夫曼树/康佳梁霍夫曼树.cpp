#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define NumOfASC 128   //ASCII �������Ϊ�˱�֤ÿһ���ַ������Ա���¼��
#define MAXSIZE 2000   // ����ַ���
#define STACKSIZE 200  //ջ��С
#define INPUTNUM 3      //�����������
#define TOCODELEN 19    //��С�������ַ�����
#define ONECODE   1    //��ֹ���ַ��ظ�
#pragma warning(disable: 4996)//��Ҳ��֪��strcpyզ���������ˡ��Ҿ�ֱ�Ӹ���������

typedef struct HuffTNode
{
	char ch;//�ַ�
	int weight;//Ȩֵ
	int parent, lchild, rchild;//���ĸ�ĸ�����Һ���
	int degree;//�ڵ�Ķ�
	int floor;//�ڵ�Ĳ���
}HuffTNode, * HuffmanTree;

typedef char** HuffCodeStr;  //����������������ַ���

typedef struct
{
	HuffTNode* base;  //ջ��
	HuffTNode* top;   //ջ��
	int stacksize;     //ջ��С
}HuffStack;//����������ջ

//��������ջ����������
int InitStack(HuffStack& hStack)   // ��ʼ��ջ
{
	hStack.base = (HuffTNode*)malloc(STACKSIZE * sizeof(HuffTNode));
	if (!hStack.base)
		return -1;
	hStack.top = hStack.base;
	hStack.stacksize = STACKSIZE;
	return 0;
}
// �ڵ�ѹջ
int PushNode(HuffStack& htStack, HuffTNode htNode)
{
	if (htStack.top - htStack.base == STACKSIZE)
		return -1;
	*(htStack.top++) = htNode;
	return 0;
}
// �ڵ��ջ
int PopNode(HuffStack& hStack, HuffTNode& htNode)
{
	if (hStack.base == hStack.top)
		return -1;
	htNode = *(--hStack.top);
	return 0;
}
// �ж�ջ�Ƿ�Ϊ��
int StackIsEmpty(HuffStack& hStack)
{
	if (hStack.base == hStack.top)
		return 1;
	else return 0;
}
//��������ջ����������

//��huffTree[1]��huffTree[n]��ѡ��parentΪ0��weight��С�������ڵ㣬������Ÿ���num1��num2
int SelectNode(HuffmanTree& huffTree, int n, int& num1, int& num2)
{
	int i = 0;
	num1 = 0;//��ʼ��num1��num2
	num2 = num1 + 1;
	for (i = 1; i <= n; i++)
	{
		if (huffTree[i].parent == 0 && huffTree[i].weight != 0)
		{
			num1 = i;
			break;
		}
	}//�ҵ���һ������Ҫ��Ľ�㣬��¼�ýڵ����Ϊnum1
	for (i = num1 + 1; i <= n; i++)
	{
		if (huffTree[i].parent == 0 && huffTree[i].weight != 0)
		{
			num2 = i;
			break;
		}
	}//���ҵ�һ������Ҫ��Ľ�㣬��¼�ýڵ����Ϊnum2
	if (huffTree[num1].weight > huffTree[num2].weight)
	{
		int temp = num1;
		num1 = num2;
		num2 = temp;
	}//��֤num1��ȨֵС��num2��Ȩֵ
	//û��ר��дȨֵ��ȣ���֤num1���ַ���С�Ĵ��룬��Ϊ����ͳ�Ʋ���ͬ�ַ��Ĵ����˼·��ֻҪ������num1/num2�����ܱ�֤����ASC���С�����˳������
	if (num1 > num2)
	{
		i = num1;
	}
	else
	{
		i = num2;
	}
	// ����ֵ�ϴ����һλ��ʼ���Ƚϣ���Ϊ�ոտ����漰��num1/num2�Ľ���
	i++;
	for (; i <= n; i++)//����ֵ�ϴ����һλ��ʼ���Ƚϣ���Ϊ�����漰��num1/num2�Ľ���
	{
		if (huffTree[i].weight != 0 && huffTree[i].parent == 0)//ѡ��weight��Ϊ0��parentΪ0�Ľ��
		{
			if (huffTree[i].weight < huffTree[num1].weight)  //���ý��weightС��num1���weight����ý����С��
			{                                               //�޸�num1Ϊ��λ�ã��޸�num2Ϊԭnum1
				num2 = num1;
				num1 = i;
			}
			else if (huffTree[i].weight < huffTree[num2].weight)  //���ý��weightС��num2���weight����ý���С��
			{                                                     //�޸�num2Ϊ��λ��
				num2 = i;
			}
		}
	}
	return 0;
}

//����������ʵ�ֺ���
int HuffmanCode(HuffmanTree& huffTree, HuffCodeStr& huffStr, int* weight, int n, char* chBuff)
{                      //weightΪn���ַ�Ȩֵ�����飬chBuffΪ���n���ַ����ַ���
	int m = 2 * n - 1;//����Ҫ�ĵ�Ԫ����
	huffTree = (HuffmanTree)malloc((m + 1) * sizeof(HuffTNode));
	if (!huffTree)
	{
		printf("ERROR_02");
		return -1;
	}//��μ�飬����ڴ����ʧ�ܣ�˵������ʧ�ܣ�����ERROR_02
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
	}//��ʼ��Ҷ�ӽ��
	for (; i <= m; ++i)
	{
		huffTree[i].ch = '\0';
		huffTree[i].weight = 0;
		huffTree[i].parent = 0;
		huffTree[i].lchild = 0;
		huffTree[i].rchild = 0;
		huffTree[i].degree = 2;
		huffTree[i].floor = 1;
	}//��ʼ���������
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
	//��Ҷ�ӵ���������ÿ���ַ��ĺշ�������
	huffStr = (HuffCodeStr)malloc((n + 1) * sizeof(char*));//����n���ַ������ͷָ������
	if (!huffStr)
	{
		printf("EEROR_02");
		return -1;
	}//��μ�飬����ڴ�����ʧ�ܣ�˵������ʧ��
	char* chCode = (char*)malloc(n * sizeof(char));//���������Ĺ����ռ�
	if (!chCode)
	{
		printf("EEROR_02");
		return -1;
	}//��μ�飬����ڴ�����ʧ�ܣ�����ʧ��
	for (i = 0; i <= n - 2; i++)
	{
		chCode[i] = 0;//��ʼ�������ռ�
	}
	chCode[n - 1] = '\0';//���������
	int start = 0;//startΪ���������λ�ã���ʼ��Ϊ0
	int f = 1; int c = 1;
	for (i = 1; i <= n; ++i)//����ַ������������
	{
		start = n - 1;//���������λ������Ϊ���
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
		huffStr[i] = (char*)malloc((n - start) * sizeof(char));//Ϊ��i���ַ��������ռ�
		if (!huffStr[i])
		{
			printf("EEROR_02");
			return -1;
		}//�ڴ�����ʧ�ܣ�����ʧ��
		strcpy(huffStr[i], &chCode[start]);//�ӹ����ռ�cd���Ʊ��뵽HC
	}
	free(chCode);//malloc��free��Ӧ
	return 0;
}

//�������������������ջ
int TraverseInOrder(HuffmanTree& huffTree, int n)
{
	HuffTNode ptr = huffTree[2 * n - 1];
	ptr.floor = 1;//����һ����ǽ�㣬��ָ����ڵ�
	HuffStack stack;
	InitStack(stack);
	PushNode(stack, ptr);//��������������������Ƚ����ڵ�ѹջ
	while (!StackIsEmpty(stack))//��ջ����
	{
		PopNode(stack, ptr);
		printf("����ַ���%c Ȩֵ��%d �ȣ�%d ������%d\n", ptr.ch, ptr.weight, ptr.degree, ptr.floor);
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
	free(stack.base);//malloc��free��Ӧ
	return 0;
}

//����������ʵ�ֺ���
int HuffmanDecode(HuffmanTree& huffTree, HuffCodeStr& huffStr, char* chTC, char* chTD, int n)
{                        //chTCΪ����Ĵ������ַ�����chTDΪ����Ĵ������ַ���
	int pos = 2 * n - 1;//��Ǹ��ڵ�λ��
	int i = 0;
	int j = 1;
	char codeRes[100][20];
	char decodeStr[100];//�����������鴢����������Ľ������ֹ����ɹ�������ʧ��ʱ�������������ERROR_03������ĿҪ�󲻷�
	int codePos = 0;//����������±�
	int decodePos = 0;//����������±�
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
	}//�õ����ַ���������
	while (*chTD != '\0')
	{
		if (*chTD == '0')//��Ϊ0,ָ������
			pos = huffTree[pos].lchild;
		else if (*chTD == '1')//��Ϊ1��ָ���Һ���
			pos = huffTree[pos].rchild;
		else//��Ϊ��0,1����������ʧ��
		{
			printf("ERROR_03");
			return -1;
		}
		chTD++;//���ζ�ȡ�ַ���b���ַ�
		if (!huffTree[pos].lchild && !huffTree[pos].rchild)//��ΪҶ�ӽ��,�洢����ַ�����ʹ��Ƿ��ظ��ڵ�
		{
			decodeStr[decodePos] = huffTree[pos].ch;
			decodePos++;
			pos = 2 * n - 1;
		}
	}
	if (pos != 2 * n - 1 && huffTree[pos].ch == '\0')//���ַ���b�ѽ���������ǽ����ַ�Ϊ'\0'��˵����δ��Ҷ�ӽ�㣬����ʧ��
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
	int chAsc[NumOfASC] = { 0 };//��СΪ128�����飬���ڼ�¼���ֵ��ַ�������asc���˳�򴢴�
	char strCode[MAXSIZE] = { '\0' };  //�������ַ���
	char strDecode[MAXSIZE] = { '\0' }; //�������ַ���
	strcpy(strCode, argv[1]);
	strcpy(strDecode, argv[2]);
	int i = 0;
	for (i = 0; strCode[i] != '\0'; i++)
	{
		chAsc[(int)strCode[i]]++;//��¼Ȩֵ
	}
	if (i < TOCODELEN)  //����������ַ����ĳ���С��20����ʾ����ʧ��
	{
		printf("ERROR_02");
		return -1;
	}
	int weight[NumOfASC] = { 0 };//weightΪ����ַ�Ȩֵ������
	char chBuff[NumOfASC] = { 0 };//chBuffΪ����ַ����ַ���
	int n = 0;//ͳ�Ʋ���ͬ���ַ�����
	for (i = 0; i < NumOfASC; i++)
	{
		if (chAsc[i] != 0)
		{
			chBuff[n] = (char)i;//chBuff�����У��±��С����ASCII��Ҳ��С��������SelectNode�����У�num1��num2(num1<num2)ָ��Ľ����ַ����㰴��ĸ˳������
			weight[n] = chAsc[i];
			n++;//�޸�λ������ƶ�
		}
	}
	if (n == ONECODE)//����������ַ���ֻ��һ���ַ�����ʾ����ʧ��
	{
		printf("ERROR_02");
		return -1;
	}
	HuffmanTree huffTree;  //��������
	HuffCodeStr huffStr;   //����������
	HuffmanCode(huffTree, huffStr, weight, n, chBuff);  //����
	TraverseInOrder(huffTree, n);//��������ӡ�����ڵ����Ϣ
	HuffmanDecode(huffTree, huffStr, strCode, strDecode, n); //����
	free(huffTree);//�ͷ�huffTree�Ŀռ�
	free(huffStr);//�ͷ�huffStr�Ŀռ�
	return 0;
}