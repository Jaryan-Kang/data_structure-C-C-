#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define VEXNUM    13        //建筑物数，就是图的顶点数
#define INF        100000   //用一个比较大的值表示两点之间不通
#define ARGNUM     3        //输入参数个数
#define NAMELEN    20       //建筑物名字符长度
#define HALFSIZE    2       // 边的数目时，由于是无向图，取总边数的一半值
#define ERROR_01    -1      //输入参数个数错误
#define ERROR_02    -1      //建筑物输入错误
#define INVALID_VEX -1      //无效建筑物顶点
#define VALID_VEX 0         //有效建筑物顶点
#define SHORTPATH_FIND 1    //搜索到最短路径

typedef struct // 邻接矩阵,来代替无向图
{
    int vexNum;           // 顶点数
    int edgNum;           // 边数
    int matrix[VEXNUM][VEXNUM]; // 邻接矩阵
}Graph, * PGraph;

Graph* CreatGraph()    //根据建筑物之间的路径关系创建图
{
    int matrix[VEXNUM][VEXNUM] = {
        //           传送门1  传送门4  宪梓堂  北门  饮水思源  腾飞塔  图书馆  教学主楼  活动中心  南门  传送门3  传送门2  西迁馆
        /*传送门1*/	{INF,     INF,     INF,   INF,   27 ,      INF,     4,     INF,      INF,     INF,   INF,    INF,     INF},
        /*传送门4*/	{INF,     INF,     4,     INF,   INF,      32,     INF,    INF,      INF,     INF,   INF,    INF,     INF},
        /*宪梓堂*/	{INF,     4,       INF,   INF,   INF,      INF,    INF,    8,        INF,     14,    INF,    INF,     INF},
        /*北门*/	{INF,     INF,     INF,   INF,   18,       INF,    INF,    INF,      INF,     INF,   22,     INF,     INF},
        /*饮水思源*/{27,      INF,     INF,   18,    INF,      19,     INF,    INF,      INF,     INF,   INF,    INF,     INF},
        /*腾飞塔*/	{INF,     32,      INF,   INF,   19,       INF,     23,    INF,      INF,     INF,    4,     INF,     INF},
        /*图书馆*/	{4,       INF,     INF,   INF,   INF,      23,     INF,    15,       INF,     INF,   INF,     4,      INF},
        /*教学主楼*/{INF,     INF,     8,     INF,   INF,      INF,     15,    INF,      21,      INF,   INF,    INF,     INF},
        /*活动中心*/{INF,     INF,     INF,   INF,   INF,      INF,    INF,    21,       INF,      30,   INF,    INF,     INF},
        /*南门*/	{INF,     INF,     14,    INF,   INF,      INF,    INF,    INF,      30,      INF,   INF,     21,     20 },
        /*传送门3*/	{INF,     INF,     INF,   22,    INF,      4,      INF,    INF,      INF,     INF,   INF,    INF,     INF},
        /*传送门2*/	{INF,     INF,     INF,   INF,   INF,      INF,    4,      INF,      INF,      21,   INF,    INF,     43 },
        /*西迁馆*/	{INF,     INF,     INF,   INF,   INF,      INF,    INF,    INF,      INF,      20,   INF,     43,     INF},
    };
    int i, j;//遍历系数
    Graph* pGraph;//定义一个邻接矩阵

    // 输入"顶点数"和"边数"
    if ((pGraph = (Graph*)malloc(sizeof(Graph))) == NULL)
        return NULL;
    memset(pGraph, 0, sizeof(Graph));//赋值为零，初始化

    // 初始化"顶点数"
    pGraph->vexNum = VEXNUM;//VEXNUM是13，也就是我们要用到的顶点数
    // 初始化"边"
    for (i = 0; i < pGraph->vexNum; i++)
        for (j = 0; j < pGraph->vexNum; j++)
            pGraph->matrix[i][j] = matrix[i][j];  //图邻接矩阵赋值，将上述矩阵赋给pGraph

    // 统计边的数目
    for (i = 0; i < pGraph->vexNum; i++)
        for (j = 0; j < pGraph->vexNum; j++)
            if (i != j && pGraph->matrix[i][j] != INF)  //两点之间有路径
                pGraph->edgNum++;
    pGraph->edgNum /= HALFSIZE; //由于是无向图，取总边数的一半值
    return pGraph;
}

//算法核心部分来喽
void dijkstra(Graph graph, int vexStart, int vexEnd, int prev[], int dist[])
{
    char strPlaceName[VEXNUM][NAMELEN] = { "传送门1","传送门4","宪梓堂","北门","饮水思源","腾飞塔","图书馆","教学主楼",
                                      "活动中心","南门","传送门3","传送门2", "西迁馆" };
    int i, j, k;
    int min;
    int tmp;
    int flag[VEXNUM];      // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取，为0则表示还未获取，判断是否完成的标志位


    for (i = 0; i < graph.vexNum; i++)
    {
        flag[i] = 0;              // 顶点i的最短路径还没获取到0。为标志位
        prev[i] = 0;              // 顶点i的前驱顶点为0。
        dist[i] = graph.matrix[vexStart][i];// 顶点i的最短路径为"顶点vs"到"顶点i"的权。
    }

    // 对"顶点vs"自身进行初始化
    flag[vexStart] = SHORTPATH_FIND;//首顶点权值已确定
    dist[vexStart] = 0;//自己到自己的距离为零

    /*如下的for循环是整个djkstra算法的核心，djkstra算法即为：从起始点开始，将已有路径找出并且取已有路径的最小值，将最小值标志为确定值并从
    最小值点引出其他边与顶点进行更新，更新过后的点参与计算*/
    for (i = 1; i < graph.vexNum; i++)//计数循环
    {
        // 寻找当前最小的路径；
        // 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
        min = INF;
        for (j = 0; j < graph.vexNum; j++)//找点，只要和vs点有路径的点统统找到，对最小路径点进行标注
        {
            if (flag[j] == 0 && dist[j] < min)//不是确定距离点且dist存在
            {
                min = dist[j];
                k = j;
            }
        }
        // 标记"顶点k"为已经获取到最短路径
        flag[k] = SHORTPATH_FIND;
        // 修正当前最短路径和前驱顶点
        // 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
        for (j = 0; j < graph.vexNum; j++)//对最小路径点引出的边进行计算并部分更新
        {
            tmp = (graph.matrix[k][j] == INF ? INF : (min + graph.matrix[k][j])); // 有距离吗？有的话，更新
            if (flag[j] == 0 && (tmp < dist[j]))//首先你得距离不确定且比你原来的距离短，我才给你更新
            {
                dist[j] = tmp;
                prev[j] = k;
            }
        }
    }

  //万恶的要求，还要输出路径！！！！不过还好有prev哈哈哈

    int path[VEXNUM]; // 用于记录路径的顶点序号
    int pathLength = 0; // 记录最短路径的顶点数量

    int tmpVex = vexEnd; // 从终点开始向起点回溯
    while (tmpVex != vexStart && prev[tmpVex] != 0) // 回溯到起点或无前驱节点为止
    {
        path[pathLength++] = tmpVex; // 将当前顶点加入路径中
        tmpVex = prev[tmpVex]; // 更新为前驱节点
    }
    path[pathLength++] = tmpVex;  //这个是一个小bug，因为第一个点直接和vs相连且每次遍历都要经过，与其在那里加一个flag函数，在这里单独输入更方便
    path[pathLength++] = vexStart; // 加入起始顶点

    // 输出最短路径
    if (dist[vexEnd] < INF) // 如果有最短路径
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
    // 打印dijkstra最短路径的结果
    printf("%d", dist[vexEnd]);
}

int main(int argc, char* argv[])
{
    if (argc != ARGNUM)   //输入参数个数错误
    {
        printf("ERROR_01");   //输入参数个数错误
        return ERROR_01;
    }

    int start = INVALID_VEX;  //无效顶点 
    int end = INVALID_VEX;    //无效顶点
    char strPlaceName[VEXNUM][NAMELEN] = { "传送门1","传送门4","宪梓堂","北门","饮水思源","腾飞塔","图书馆","教学主楼",
                                          "活动中心","南门","传送门3","传送门2", "西迁馆" };

    for (int i = 0; i < VEXNUM; i++)//识别输入信号
    {
        if (strcmp(argv[1], strPlaceName[i]) == VALID_VEX)
        {
            start = i;  //找到起点的序号
        }
        if (strcmp(argv[2], strPlaceName[i]) == VALID_VEX)
        {
            end = i;  //找到终点的序号
        }
    }
    if (start == INVALID_VEX || end == INVALID_VEX)  //有无效顶点 
    {
        printf("ERROR_02");   //无效顶点错误
        return ERROR_02;
    }
    int prev[VEXNUM] = { 0 };  //前驱顶点数组
    int dist[VEXNUM] = { 0 };  //路径长度数组
    Graph* pGraph;
    //创建建筑物的连接"图"
    pGraph = CreatGraph();

    // dijkstra算法获取"start"到其它各个顶点的最短距离,打印"start"到"end"之间的最短长度	
    dijkstra(*pGraph, start, end, prev, dist);
    free(pGraph);
    return 0;
}
