/*------------------------------------------------------------------------------
	* file:AMLGrpah.c
	* date:10-12-2014
	* author:doodlesomething@163.com
	* version:1.0
	* description:邻接多重表实现无向图/网的基本操作
	* more:在实现感觉和十字表有相似之处
--------------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include "amlgraph.h"


/*
* @description:创建图(无向图/网)
*/
Status CreateGraph(AMLGraph *G) {
	int i,j,k,w;
	VertexType va,vb;
	EBox *p;
	
	//确定图的种类
	printf("please enter the kind of graph:");
	scanf("%d",&(*G).kind);

	//确定图的顶点和边数
	printf("please enter vexnum and arcnum:");
	scanf("%d,%d",&(*G).vexnum,&(*G).arcnum);


	//确定图的各个顶点
	printf("please enter each value of graph:");
	for(i = 0; i < (*G).vexnum ; i++)  {
		scanf("%d,",&(*G).adjmulist[i].data);
		(*G).adjmulist[i].firstarc = NULL;
	}
	
	//确定各个顶点间关系，即建立边
	if((*G).kind == UDG)
		printf("please enter heads and tails:\n");
	else 
		printf("please enter heads,weights and tails\n");

	for(k = 0; k < (*G).arcnum ;k++) {
		if((*G).kind == UDG)
			scanf("%d,%d",&va,&vb);
		else 
			scanf("%d,%d",&va,&vb,&w);
		//放回两顶点在顶点向量数组中位置
		i = LocateVex(*G,va);
		j = LocateVex(*G,vb);
		

		if(i < 0 || j < 0)
			return ERROR;


		p = (EBox *) malloc(sizeof(struct EBox));
		if(!p)
			exit(OVERFLOW);

		///建立新节点
		p->ivex = i;
		p->jvex = j;
		p->mark = unvisited;
		//注意每次插入新节点都是在链表的表头进行
		p->ilink = (*G).adjmulist[i].firstarc;
		p->jlink = (*G).adjmulist[j].firstarc;
		(*G).adjmulist[i].firstarc = p;
		(*G).adjmulist[j].firstarc = p;
		if((*G).kind == UDN)
			p->weight = w;
	}

	return OK;
}



/*
* @description:定位一个顶点值在图中的位置,无则返回-1
*/

int LocateVex(AMLGraph G,VertexType v) {
	int i;

	for(i = 0; i < G.vexnum ; i++)
		if(G.adjmulist[i].data == v)
			return i;

	return -1;
}


/*
* @description:返回顶点v的值
*/
VertexType GetVex(AMLGraph G,int v) {
	if(v > G.vexnum || v < 0)
		exit(ERROR);
	
	return G.adjmulist[v].data;
}


/*
* @description:对顶点值为v的顶点赋值为value
*/
Status PutVex(AMLGraph *G,VertexType v,VertexType value) {
	int i;

	i = LocateVex(*G,v);

	if(i < 0)
		return ERROR;

	(*G).adjmulist[i].data = value;

	return OK;
}


/*
* @description:返回顶点值为v的下一个邻接点的序号，否则返回-1
*/
int FirstAdjVex(AMLGraph G,VertexType v) {
	int i;
	EBox *p;

	i = LocateVex(G,v);

	if(i < 0)
		return -1;
	
	p = G.adjmulist[i].firstarc;

	if(p)
		if(p->ivex == i)
			return p->jvex;
		//要注意这种情况的理解
		else
			return p->ivex;

	else 
		return -1;

}


/*
* @description:返回顶点值为v相对于顶点值为w的下一个邻接顶点的序号
*/
int NextAdjVex(AMLGraph G,VertexType v,VertexType w) {
	int i,j;
	EBox *p;

	i = LocateVex(G,v);
	j = LocateVex(G,w);

	p = G.adjmulist[i].firstarc;	//不管怎样先找到入口
	
	while(p)
		if(p->ivex == i && p->jvex != j ) //情况1
			p = p->ilink; 
		else if(p->jvex == i && p->ivex != j) //情况2
			p = p->jlink;
		else
			break;
	
	//情况1
	if(p && p->ivex == i && p->jvex == j) {
		p = p->ilink;

		if(p && p->ivex == i)
			return p->jvex;
		else if(p && p->jvex == i)
			return p->ivex;
	}

	//情况2
	if(p && p->ivex == j && p->jvex == i) {
		p = p->jlink;

		if(p && p->ivex == i)
			return p->jvex;
		else if(p && p->jvex == i)
			return p->ivex;
	}


	return -1;
}


/*
* @description:插入一个顶点
*/
Status InsertVex(AMLGraph *G,VertexType v) {
	int i;

	i = (*G).vexnum;

	(*G).adjmulist[i].data = v;
	(*G).adjmulist[i].firstarc = NULL;
	(*G).vexnum++;

	return OK;
}


/*
* @description:插入一段弧
*/
Status InsertArc(AMLGraph *G,VertexType v,VertexType w) {
	int i,j;
	EBox *p;
	//放回两顶点在顶点向量数组中位置
	i = LocateVex(*G,v);
	j = LocateVex(*G,w);


	if(i < 0 || j < 0)
		return ERROR;

	p = (EBox *) malloc(sizeof(struct EBox));

	if(!p)
		exit(OVERFLOW);

	///建立新节点
	p->ivex = i;
	p->jvex = j;
	p->mark = unvisited;
	//注意每次插入新节点都是在链表的表头进行
	p->ilink = (*G).adjmulist[i].firstarc;
	p->jlink = (*G).adjmulist[j].firstarc;
	(*G).adjmulist[i].firstarc = p;
	(*G).adjmulist[j].firstarc = p;

	if((*G).kind == UDN) {
		printf("please enter the weight of the arc:");
		scanf("%d",&p->weight);
	}


	return OK;

}



/*
 * @description:深度优先遍历图
 */
Status DFSTraverse(AMLGraph G) {
	int i;

	for(i = 0;i < G.vexnum ; i++) 
		visited[i] = FALSE;
	//循环是为保证每个顶点都能被访问到
	for(i = 0; i < G.vexnum; i++) 
		if(!visited[i])
			DFS(G,i);

}


/*
 * @description:递归实现深度优先遍历
 */
void DFS(AMLGraph G,int i) {
	int j;
	EBox *p;

	printf("%d",G.adjmulist[i].data);
	visited[i] = TRUE;

	p = G.adjmulist[i].firstarc;

	while(p) {
		j = p->ivex == i ? p->jvex : p->ivex;
		if(!visited[j])
			DFS(G,j);

		p = p->ivex == i ? p->ilink : p->jlink ;
	}

}





