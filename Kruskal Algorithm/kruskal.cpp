#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <stdbool.h>

using namespace std;

typedef struct edges // edge 구조체(이음선 v1,v2에 대한 정보와 가중치)
{
	int v1;
	int v2;
	int weight;
}edges;

typedef struct nodetype
{
	int parent;
	int depth;
}universe;

universe u[1005];
int f_index = 1;

void makeset(int i);
int find(int i);
void merge(int p, int q);
bool equal(int p, int q);
void initial(int n);
bool compare(const edges& e1, const edges& e2);
void kruskal(int n, int m, edges e[], edges *f);

bool compare(const edges& e1, const edges& e2)
{
	return e1.weight < e2.weight;
}

bool equal(int p, int q) // p와 q가 같은가?
{
	if(p == q)
		return true;
	else
		return false;
}
void makeset(int i) // 부모는 기본값 i, depth = 1로 설정.
{
	u[i].parent = i;
	u[i].depth = 0;
}
int find (int i)
{
	int j;
	j = i;
	while(u[j].parent != j) {
		j = u[j].parent;
	}
	return j;
}
void merge(int p, int q)
{
	if(u[p].depth == u[q].depth) { // p와 q의 depth가 동일하면
		u[p].depth++; // p의 death 증가
		u[q].parent = p; // q의 부모는 p가 되게 한다.(같은 집합으로 처리)
	}
	else if(u[p].depth < u[q].depth) {
		u[p].parent = q;
	}
	else {
		u[q].parent = p;
	}
}
void initial(int n) // 초기화 함수
{
	for(int i = 1; i <= n; i++) 
	{
		makeset(i); // 초기화 작업 진행
	}
}

void kruskal(int n, int m, edges E[], edges *F)
{
	int i,j;
	int k = 1;
	int p,q;
	edges e;

	sort(E+1,E+1+m,compare); // 정렬 알고리즘
	initial(n);	// n만큼의 크기 초기화(nodetype U)

	while(k <= m) { // k = 1부터 n까지 반복한다.
		e =	E[k]; // k번째 엣지 값을 넣는다.
		i = E[k].v1; // edge의 v1값
		j = E[k].v2; // edge의 v2값
		p = find(i); // i의 부모값 찾아옴
		q = find(j); // j의 부모값 찾아옴
		if(!equal(p,q)) { // 집합이 같지 않은 경우
			merge(p,q); // 합친다.
			F[f_index] = e; // F_list에 e를 추가함.
			f_index++; // F 엣지 개수 값 증가
		}
		k++;
	}
}

int main(void)
{	
	int n,m;
	edges E[1005];
	edges F[1005];
	printf("Kruskal's Algorithm");
	printf("number of nodes : ");
	scanf("%d",&n);
	printf("number of egdes : ");
	scanf("%d",&m);
	

	memset((universe *)u,0,sizeof(universe)); // 전역변수 nodetype universe 초기화
	printf("Input Edge v1, v2, weight\n");
	for(int i = 1; i <= m; i++)
	{
		printf("Edge %d : ",i);
		scanf("%d %d %d",&E[i].v1, &E[i].v2,&E[i].weight);
	}

	kruskal(n,m,E,F); // kruskal 알고리즘 진행
	
	printf("<F list>\n");
	for(int i = 1; i <= n - 1; i++) // 최종 f값 출력
	{
		printf("v%d,v%d weight : %d\n",F[i].v1,F[i].v2,F[i].weight);
	}
}
