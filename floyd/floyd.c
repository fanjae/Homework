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
                    p[i][j] = 0; // p[i][j]�� 0���� �ʱ�ȭ
                    d[i][j] = w[i][j]; // d �迭�� �ʱⰪ�� w�� �Ȱ��� �����.
            }
    }

    for(int k = 1; k <= n; k++) {
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                if(d[i][k] + d[k][j] < d[i][j]) { // minimum(D[i][j], D[i][k] + D[k][j])
                   p[i][j] = k; // �ִ� ����� ��� ���
                   d[i][j] = d[i][k] + d[k][j];

                }
            }
        }
    }
}
int main()
{
    printf("Floyd Algorithm Program\n");
    printf("Vertex ������ �Է��ϼ���.\n");
    printf("�Է� : ");
    scanf("%d",&n);

    printf("Floyd Algorithm Program\n");
    printf("adjacent matrix�� �Է����ּ���. \n");
    printf("�� ���� �����̽��� �����ϸ�, �� ���� ���ͷ� �����մϴ�.\n");
    printf("-10000������ ���� �Է��� ��� �ش� ��δ� INF(�������� �ʴ´ٰ�)�� �Ǵ��մϴ�.\n");
    printf("=== �Է� ���� ===\n");
    printf("1 2 3 4 5\n");
    printf("-10000 4 5 6 3 \n");
    printf("5 -10000 5 3 2 \n");
    printf("3 1 2 6 4\n");
    printf("3 9 4 2 5\n");
    printf("==== �Է� ====\n");

    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            scanf("%d", &w[i][j]);
            if(w[i][j] <= -10000) {
                w[i][j] = INF;
            }
        }
    }
    floyd(w,d,p);

    printf("===�ִܰ�� ���� D ���===\n");
    for(int i = 1; i <= n ; i++) {
            for(int j = 1; j <= n; j++) {
                    printf("%d ",d[i][j]);
            }
            printf("\n");
    }
    printf("===�ִܰ�� ��Ʈ P ���===\n");
    for(int i = 1; i <= n ; i++) {
            for(int j = 1; j <= n; j++) {
                    printf("%d ",p[i][j]);
            }
            printf("\n");
    }
    //path(5,3);
}
