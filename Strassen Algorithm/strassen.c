#include <stdio.h>스
int threshold = 1;
int N;
// 행렬 덧셈
void matrix_add(int n, int (*a)[N], int (*b)[N], int (*c)[N])
{
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

// 행렬 뺄셈
void matrix_sub(int n, int (*a)[N], int (*b)[N], int (*c)[N])
{

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
}

// 행렬 곱셈
void matrix_multiple(int n, int (*a)[N], int (*b)[N], int (*c)[N])
{
    int i,j,k;
	for(int i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			c[i][j] = 0;
			for(k = 0; k < n; k++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	}
}

// 스트라센 알고리즘
void strassen(int n, int (*a)[N], int (*b)[N], int (*c)[N])
{
    int a11[n][n],a12[n][n],a21[n][n],a22[n][n];
    int b11[n][n],b12[n][n],b21[n][n],b22[n][n];
    int c11[n][n],c12[n][n],c21[n][n],c22[n][n];
    int m1[n][n],m2[n][n],m3[n][n],m4[n][n],m5[n][n],m6[n][n],m7[n][n];
    int aa[n][n],bb[n][n];

	if(n <= threshold) // n이 임계값 이하인 경우
	{
		matrix_multiple(threshold,a,b,c);
	}
	else {
        for(int i = 0; i < n/2; i++) { // 스트라센 알고리즘 과정을 위해 배열을 나눔. (partition)
            for(int j=0; j< n/2; j++) {
                a11[i][j] = a[i][j];
                a12[i][j] = a[i][j+n/2];
                a21[i][j] = a[i+n/2][j];
                a22[i][j] = a[i+n/2][j+n/2];

                b11[i][j] = b[i][j];
                b12[i][j] = b[i][j+n/2];
                b21[i][j] = b[i+n/2][j];
                b22[i][j] = b[i+n/2][j+n/2];

            }

        }

        matrix_add(n/2, a11, a22, aa);
        matrix_add(n/2, b11, b22, bb);
        strassen(n/2, aa, bb, m1); // m1 = (a11+a22)*(b11+b22)

        matrix_add(n/2, a21, a22, aa);
        strassen(n/2, aa, b11, m2); // m2 = (a21+a22)*b11

        matrix_sub(n/2, b12, b22, bb);
        strassen(n/2, a11, bb, m3); // m3 = a11*(b12-b22)

        matrix_sub(n/2, b21, b11, bb);
        strassen(n/2, a22, bb, m4); // m4 = a22*(b21-b11)

        matrix_add(n/2, a11, a12, aa);
        strassen(n/2, aa, b22, m5); // m5 = (a11+a12)*b22

        matrix_sub(n/2, a21, a11, aa);
        matrix_add(n/2, b11, b12, bb);
        strassen(n/2, aa, bb, m6); // m6 = (a21-a11)*(b11+b12)

        matrix_sub(n/2, a12, a22, aa);
        matrix_add(n/2, b21, b22, bb);
        strassen(n/2, aa, bb, m7); // m7 = (a12-a22)*(b21+b22)

        matrix_add(n/2, m1, m4, aa);
        matrix_sub(n/2, m7, m5, bb);
        matrix_add(n/2, aa, bb, c11); // c11 = m1+m4-m5+m7

        matrix_add(n/2, m3, m5, c12); // c12 = m3+m5
        matrix_add(n/2, m2, m4, c21); // c21 = m2+m4

        matrix_sub(n/2, m1, m2, aa);
        matrix_add(n/2, m3, m6, bb);
        matrix_add(n/2, aa, bb, c22); // m1+m3-m2+m6

        for(int i=0; i<n/2; i++) { // 나눠져 있는 C값을 다시 합쳐줌.
           for(int j=0; j<n/2; j++) {
              c[i][j] = c11[i][j];
              c[i][j+n/2] = c12[i][j];
              c[i+n/2][j] = c21[i][j];
              c[i+n/2][j+n/2] = c22[i][j];
           }
        }
    }
}
int main(void)
{
    printf("n*n 행렬에서 n의 크기를 입력하세요.\n");
    printf("입력 : ");
    scanf("%d",&N);

    int a[N][N];
    int b[N][N];
    int c[N][N];

    printf("행렬 a를 입력하세요. 입력은 아래와 같이 입력합니다.\n");
    printf("값 사이 구분은 스페이로 되어있으며 행 구분은 엔터로 구분합니다.\n");
    printf("<입력 예시>\n");
    printf("1 2 3 4\n");
    printf("5 6 7 8\n");
    printf("9 10 11 12\n");
    printf("13 14 15 16\n");
    printf("<행렬a 입력>\n");
    for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                    scanf("%d",&a[i][j]);
            }
    }
    printf("행렬 b를 입력하세요. 입력은 아래와 같이 입력합니다.\n");
    printf("값 사이 구분은 스페이스로 되어있으며 행 구분은 엔터로 구분합니다.\n");
    printf("<입력 예시>\n");
    printf("1 2 3 4\n");
    printf("5 6 7 8\n");
    printf("9 10 11 12\n");
    printf("13 14 15 16\n");
    printf("<행렬b 입력>\n");
    for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                    scanf("%d",&b[i][j]);
            }
    }
	strassen(N,a,b,c);
    printf("===<최종 결과값>===\n");
	for(int i = 0 ; i < N; i++) {
		for(int j = 0; j < N; j++) {
			printf("%d ",c[i][j]);

		}
		printf("\n");
	}
}


