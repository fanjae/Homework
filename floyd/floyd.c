#include <stdio.h>
#include <stdlib.h>
#define INF 9999999
int n;
int w[5005][5005]={0};
int d[5005][5005]={0};
int p[5005][5005]={0};
void path(int x, int y)
{
    if(p[x][y] != 0) {
       path(x, p[x][y]);
       printf("<v : %d>",p[x][y]);
       path(p[x][y],y);
    }
}
void floyd(const int (*w)[5005], int (*d)[5005], int (*p)[5005])
{
    for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                    p[i][j] = 0; // p[i][j]를 0으로 초기화
                    d[i][j] = w[i][j]; // d 배열의 초기값을 w와 똑같이 만든다.
            }
    }

    for(int k = 1; k <= n; k++) {
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                if(d[i][k] + d[k][j] < d[i][j]) { // minimum(D[i][j], D[i][k] + D[k][j])
                   p[i][j] = k; // 최단 경로인 경우 출력
                   d[i][j] = d[i][k] + d[k][j];

                }
            }
        }
    }
}
int main()
{
    printf("Floyd Algorithm Program\n");
    printf("Vertex 개수를 입력하세요.\n");
    printf("입력 : ");
    scanf("%d",&n);

    printf("Floyd Algorithm Program\n");
    printf("adjacent matrix를 입력해주세요. \n");
    printf("각 열은 스페이스로 구분하며, 각 행은 엔터로 구분합니다.\n");
    printf("-10000이하의 값을 입력한 경우 해당 경로는 INF(존재하지 않는다고)로 판단합니다.\n");
    printf("=== 입력 예시 ===\n");
    printf("1 2 3 4 5\n");
    printf("-10000 4 5 6 3 \n");
    printf("5 -10000 5 3 2 \n");
    printf("3 1 2 6 4\n");
    printf("3 9 4 2 5\n");
    printf("==== 입력 ====\n");

    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            scanf("%d", &w[i][j]);
            if(w[i][j] <= -10000) {
                w[i][j] = INF;
            }
        }
    }
    floyd(w,d,p);

    printf("===최단경로 길이 D 출력===\n");
    for(int i = 1; i <= n ; i++) {
            for(int j = 1; j <= n; j++) {
                    printf("%d ",d[i][j]);
            }
            printf("\n");
    }
    printf("===최단경로 루트 P 출력===\n");
    for(int i = 1; i <= n ; i++) {
            for(int j = 1; j <= n; j++) {
                    printf("%d ",p[i][j]);
            }
            printf("\n");
    }
    //path(5,3);
}
