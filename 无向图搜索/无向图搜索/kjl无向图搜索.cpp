#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define VEXNUM    13        //��������������ͼ�Ķ�����
#define INF        100000   //��һ���Ƚϴ��ֵ��ʾ����֮�䲻ͨ
#define ARGNUM     3        //�����������
#define NAMELEN    20       //���������ַ�����
#define HALFSIZE    2       // �ߵ���Ŀʱ������������ͼ��ȡ�ܱ�����һ��ֵ
#define ERROR_01    -1      //���������������
#define ERROR_02    -1      //�������������
#define INVALID_VEX -1      //��Ч�����ﶥ��
#define VALID_VEX 0         //��Ч�����ﶥ��
#define SHORTPATH_FIND 1    //���������·��

typedef struct // �ڽӾ���,����������ͼ
{
    int vexNum;           // ������
    int edgNum;           // ����
    int matrix[VEXNUM][VEXNUM]; // �ڽӾ���
}Graph, * PGraph;

Graph* CreatGraph()    //���ݽ�����֮���·����ϵ����ͼ
{
    int matrix[VEXNUM][VEXNUM] = {
        //           ������1  ������4  ������  ����  ��ˮ˼Դ  �ڷ���  ͼ���  ��ѧ��¥  �����  ����  ������3  ������2  ��Ǩ��
        /*������1*/	{INF,     INF,     INF,   INF,   27 ,      INF,     4,     INF,      INF,     INF,   INF,    INF,     INF},
        /*������4*/	{INF,     INF,     4,     INF,   INF,      32,     INF,    INF,      INF,     INF,   INF,    INF,     INF},
        /*������*/	{INF,     4,       INF,   INF,   INF,      INF,    INF,    8,        INF,     14,    INF,    INF,     INF},
        /*����*/	{INF,     INF,     INF,   INF,   18,       INF,    INF,    INF,      INF,     INF,   22,     INF,     INF},
        /*��ˮ˼Դ*/{27,      INF,     INF,   18,    INF,      19,     INF,    INF,      INF,     INF,   INF,    INF,     INF},
        /*�ڷ���*/	{INF,     32,      INF,   INF,   19,       INF,     23,    INF,      INF,     INF,    4,     INF,     INF},
        /*ͼ���*/	{4,       INF,     INF,   INF,   INF,      23,     INF,    15,       INF,     INF,   INF,     4,      INF},
        /*��ѧ��¥*/{INF,     INF,     8,     INF,   INF,      INF,     15,    INF,      21,      INF,   INF,    INF,     INF},
        /*�����*/{INF,     INF,     INF,   INF,   INF,      INF,    INF,    21,       INF,      30,   INF,    INF,     INF},
        /*����*/	{INF,     INF,     14,    INF,   INF,      INF,    INF,    INF,      30,      INF,   INF,     21,     20 },
        /*������3*/	{INF,     INF,     INF,   22,    INF,      4,      INF,    INF,      INF,     INF,   INF,    INF,     INF},
        /*������2*/	{INF,     INF,     INF,   INF,   INF,      INF,    4,      INF,      INF,      21,   INF,    INF,     43 },
        /*��Ǩ��*/	{INF,     INF,     INF,   INF,   INF,      INF,    INF,    INF,      INF,      20,   INF,     43,     INF},
    };
    int i, j;//����ϵ��
    Graph* pGraph;//����һ���ڽӾ���

    // ����"������"��"����"
    if ((pGraph = (Graph*)malloc(sizeof(Graph))) == NULL)
        return NULL;
    memset(pGraph, 0, sizeof(Graph));//��ֵΪ�㣬��ʼ��

    // ��ʼ��"������"
    pGraph->vexNum = VEXNUM;//VEXNUM��13��Ҳ��������Ҫ�õ��Ķ�����
    // ��ʼ��"��"
    for (i = 0; i < pGraph->vexNum; i++)
        for (j = 0; j < pGraph->vexNum; j++)
            pGraph->matrix[i][j] = matrix[i][j];  //ͼ�ڽӾ���ֵ�����������󸳸�pGraph

    // ͳ�Ʊߵ���Ŀ
    for (i = 0; i < pGraph->vexNum; i++)
        for (j = 0; j < pGraph->vexNum; j++)
            if (i != j && pGraph->matrix[i][j] != INF)  //����֮����·��
                pGraph->edgNum++;
    pGraph->edgNum /= HALFSIZE; //����������ͼ��ȡ�ܱ�����һ��ֵ
    return pGraph;
}

//�㷨���Ĳ������
void dijkstra(Graph graph, int vexStart, int vexEnd, int prev[], int dist[])
{
    char strPlaceName[VEXNUM][NAMELEN] = { "������1","������4","������","����","��ˮ˼Դ","�ڷ���","ͼ���","��ѧ��¥",
                                      "�����","����","������3","������2", "��Ǩ��" };
    int i, j, k;
    int min;
    int tmp;
    int flag[VEXNUM];      // flag[i]=1��ʾ"����vs"��"����i"�����·���ѳɹ���ȡ��Ϊ0���ʾ��δ��ȡ���ж��Ƿ���ɵı�־λ


    for (i = 0; i < graph.vexNum; i++)
    {
        flag[i] = 0;              // ����i�����·����û��ȡ��0��Ϊ��־λ
        prev[i] = 0;              // ����i��ǰ������Ϊ0��
        dist[i] = graph.matrix[vexStart][i];// ����i�����·��Ϊ"����vs"��"����i"��Ȩ��
    }

    // ��"����vs"������г�ʼ��
    flag[vexStart] = SHORTPATH_FIND;//�׶���Ȩֵ��ȷ��
    dist[vexStart] = 0;//�Լ����Լ��ľ���Ϊ��

    /*���µ�forѭ��������djkstra�㷨�ĺ��ģ�djkstra�㷨��Ϊ������ʼ�㿪ʼ��������·���ҳ�����ȡ����·������Сֵ������Сֵ��־Ϊȷ��ֵ����
    ��Сֵ�������������붥����и��£����¹���ĵ�������*/
    for (i = 1; i < graph.vexNum; i++)//����ѭ��
    {
        // Ѱ�ҵ�ǰ��С��·����
        // ������δ��ȡ���·���Ķ����У��ҵ���vs����Ķ���(k)��
        min = INF;
        for (j = 0; j < graph.vexNum; j++)//�ҵ㣬ֻҪ��vs����·���ĵ�ͳͳ�ҵ�������С·������б�ע
        {
            if (flag[j] == 0 && dist[j] < min)//����ȷ���������dist����
            {
                min = dist[j];
                k = j;
            }
        }
        // ���"����k"Ϊ�Ѿ���ȡ�����·��
        flag[k] = SHORTPATH_FIND;
        // ������ǰ���·����ǰ������
        // �������Ѿ�"����k�����·��"֮�󣬸���"δ��ȡ���·���Ķ�������·����ǰ������"��
        for (j = 0; j < graph.vexNum; j++)//����С·���������ı߽��м��㲢���ָ���
        {
            tmp = (graph.matrix[k][j] == INF ? INF : (min + graph.matrix[k][j])); // �о������еĻ�������
            if (flag[j] == 0 && (tmp < dist[j]))//������þ��벻ȷ���ұ���ԭ���ľ���̣��ҲŸ������
            {
                dist[j] = tmp;
                prev[j] = k;
            }
        }
    }

  //����Ҫ�󣬻�Ҫ���·��������������������prev������

    int path[VEXNUM]; // ���ڼ�¼·���Ķ������
    int pathLength = 0; // ��¼���·���Ķ�������

    int tmpVex = vexEnd; // ���յ㿪ʼ��������
    while (tmpVex != vexStart && prev[tmpVex] != 0) // ���ݵ�������ǰ���ڵ�Ϊֹ
    {
        path[pathLength++] = tmpVex; // ����ǰ�������·����
        tmpVex = prev[tmpVex]; // ����Ϊǰ���ڵ�
    }
    path[pathLength++] = tmpVex;  //�����һ��Сbug����Ϊ��һ����ֱ�Ӻ�vs������ÿ�α�����Ҫ�����������������һ��flag�����������ﵥ�����������
    path[pathLength++] = vexStart; // ������ʼ����

    // ������·��
    if (dist[vexEnd] < INF) // ��������·��
    {
        for (int i = pathLength - 1; i >= 0; i--)
        {
            if (i > 0)
            printf("%s->", strPlaceName[path[i]]);
            else
            printf("%s", strPlaceName[path[i]]);
        }
        printf("\n");
    }
    // ��ӡdijkstra���·���Ľ��
    printf("%d", dist[vexEnd]);
}

int main(int argc, char* argv[])
{
    if (argc != ARGNUM)   //���������������
    {
        printf("ERROR_01");   //���������������
        return ERROR_01;
    }

    int start = INVALID_VEX;  //��Ч���� 
    int end = INVALID_VEX;    //��Ч����
    char strPlaceName[VEXNUM][NAMELEN] = { "������1","������4","������","����","��ˮ˼Դ","�ڷ���","ͼ���","��ѧ��¥",
                                          "�����","����","������3","������2", "��Ǩ��" };

    for (int i = 0; i < VEXNUM; i++)//ʶ�������ź�
    {
        if (strcmp(argv[1], strPlaceName[i]) == VALID_VEX)
        {
            start = i;  //�ҵ��������
        }
        if (strcmp(argv[2], strPlaceName[i]) == VALID_VEX)
        {
            end = i;  //�ҵ��յ�����
        }
    }
    if (start == INVALID_VEX || end == INVALID_VEX)  //����Ч���� 
    {
        printf("ERROR_02");   //��Ч�������
        return ERROR_02;
    }
    int prev[VEXNUM] = { 0 };  //ǰ����������
    int dist[VEXNUM] = { 0 };  //·����������
    Graph* pGraph;
    //���������������"ͼ"
    pGraph = CreatGraph();

    // dijkstra�㷨��ȡ"start"�����������������̾���,��ӡ"start"��"end"֮�����̳���	
    dijkstra(*pGraph, start, end, prev, dist);
    free(pGraph);
    return 0;
}
