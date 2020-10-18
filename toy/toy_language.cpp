#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct token_stack
{
	char data[20];
}token_stack;
void error_exist(int err_no)
{
	if(err_no == 1)
	{
		printf("Error :  \"(\" Position Miss.\n");
	}
	else if(err_no == 2)
	{
		printf("Error : \")\" Position Miss.\n");
	}
	else if(err_no == 3)
	{
		printf("Error : \"MINUS\" Position Miss.\n");
	}
	else if(err_no == 4)
	{
		printf("Error : \"IF\" Position Miss. \n");
	}
	else if(err_no == 5)
	{
		printf("Error : \"-\" Position Miss.\n");
	}
	else if(err_no == 6)
	{
		printf("Error : Number Position Miss.\n");
	}
	else if(err_no == 7)
	{
		printf("Can't input Float. \n");
	}
	else if(err_no == 8)
	{
		printf("Error : Undefined Function.\n");
	}
	else if(err_no == 9)
	{
		printf("Error : It's not valid. \n");
	}
	return ;
}

int type_check(char data) // 문장에서 나올 수 있는 상황을 'type'으로 체크
{
    if(data == '(')
    {
        return 1;
    }
    else if(data == ')')
    {
        return 2;
    }
    else if(data == '-')
    {
        return 5;
    }
    else if(data >= '0' && data <= '9')
    {
        return 6;
    }
    else if(data == '.')
    {
        return 7;
    }
    else if((data >= 'a' && data <='z')	|| (data >= 'A' && data <='Z'))
    {
        return 8;
    }
    else if((data >= 33 && data <= 126))
    {
        return 9;
    }
}
void append(char *dst, char c) // 문자열에 문자 추가 하는 함수 제타위키 참고 하였습니다. -> 참고 사이트 :  https://zetawiki.com/wiki/C%EC%96%B8%EC%96%B4_%EB%AC%B8%EC%9E%90%EC%97%B4_%EB%81%9D%EC%97%90_%EB%AC%B8%EC%9E%90_%EC%B6%94%EA%B0%80%ED%95%98%EA%B8%B0
{
	char *p = dst;
	while(*p != '\0') p++;
	*p = c;
	*(p+1) = '\0';
}
void file_load_toy() // 파일 읽어오는 함수
{
	FILE *fp = NULL;
	while(fp == NULL)
	{
		char filename[50];
		char sentence[105][100];
		int line_count = 0;
		printf("File Name input : ");
		scanf("%s",filename);
		if((fp = fopen(filename,"r")) == NULL) // 파일 없음
		{
			printf("File not Exist.\n");
			printf("Correct File name Please \n");
		}
		else // File Exist
		{
			printf("File list \n");
			printf("---------------------------------\n");

			while(fgets(sentence[line_count],100,fp) != NULL) // Read txt in ile
			{
				printf("%s",sentence[line_count]);
				line_count++;
			}

			printf("\n---------------------------------\n");
			printf("End\n");
			for(int i=0; i<line_count; i++)
			{
				token_stack keyword[105][105]={0}; // '(' 하나에 나오는 keyword
				int token_count[105]={0};
				int sentence_len = strlen(sentence[i]);
				int open_top=-1,close_top=-1;
				int save = 0,value_top = -1;
				int value[105] = {0};
				int error_num = 0;


				for(int j=0; j<sentence_len; j++)
				{
					if(type_check(sentence[i][j]) == 1)
					{
						if(save == 1 || close_top > 0) // ')'가 이미 있거나 숫자 저장 중인 경우
						{
							error_num = 1;
							break;
						}
						else
						{
							open_top++;
							strcpy(keyword[open_top][token_count[open_top]].data,"(");
							token_count[open_top]++;
						}
					}
					else if(save == 1 || type_check(sentence[i][j]) == 2) // 닫힌 괄호가 등장한 경우
					{
						if(open_top < 0)
						{
							error_num = 2;
							break;
						}
						else
						{
							for(int k=token_count[open_top]; k>=0; k--)
							{

								if(strcmp(keyword[open_top][token_count[open_top]-k].data,"MINUS") == 0) // MINUS 존재 여부 확인
								{
									value[value_top-1] = value[value_top-1] - value[value_top];
									value[value_top] = 0;
									value_top--;
								}
								else if(strcmp(keyword[open_top][token_count[open_top]-k].data,"IF") == 0) // IF 존재 여부 확인
								{
									if(value[value_top-1] > 0)
									{
										value[value_top-1] = value[value_top];
										value[value_top] = 0;
										value_top--;
									}
									else if(value[value_top-1] == 0 || value[value_top-1] < 0)
									{
										value[value_top-1] = 0;
										value[value_top] = 0;
										value_top--;
									}
								}
							}
						}
						for(int i=0; i<token_count[open_top]; i++)
						{
							keyword[open_top][i].data[0] = 0;
						}
						token_count[open_top] = 0;
						open_top--;
					}
					else if(sentence[i][j] == 'M' && sentence[i][j+1] == 'I' && sentence[i][j+2] == 'N' && sentence[i][j+3] == 'U' && sentence[i][j+4] == 'S') // MINUS 인가?
					{
						if(strcmp( keyword[open_top][token_count[open_top]-1].data,"(") != 0)
						{
							error_num = 3;
							break;
						}
						strcpy( keyword[open_top][token_count[open_top]].data,"MINUS");
						token_count[open_top]++;
						j+=5;
					}
					else if(sentence[i][j] == 'I' && sentence[i][j+1] == 'F') // IF 함수 처리
					{
						if(strcmp( keyword[open_top][token_count[open_top]-1].data,"(") != 0) // 괄호가 이전에 나오지 않은 경우 문제가 있음
						{
							error_num = 4;
							break;
						}
						strcpy( keyword[open_top][token_count[open_top]].data,"IF");
						token_count[open_top]++;
						j+=2;
					}
					else if(type_check(sentence[i][j]) == 5)
					{
						if( strcmp( keyword[open_top][token_count[open_top]-1].data,"-") == 0) // 기존에 이미 음수가 나온 경우
						{
							error_num = 5;
							break;
						}
						strcpy( keyword[open_top][token_count[open_top]].data,"-");
						token_count[open_top]++;
					}
					else if(type_check(sentence[i][j]) == 6) // 숫자 처리
					{
						if(sentence[i][j+1] == ' ' || sentence[i][j+1] == ')') // 공백(띄어쓰기)가 있거나, 닫힌 괄호가 나오는 경우 ')'
						{
							append( keyword[open_top][token_count[open_top]].data,sentence[i][j]);
							save = 0;
							value_top++;
							value[value_top] = atoi( keyword[open_top][token_count[open_top]].data);
							if(strcmp( keyword[open_top][token_count[open_top]-1].data,"-") == 0)
							{
								value[value_top] *= -1;
							}
							token_count[open_top]++;
						}
						else
						{
							append( keyword[open_top][token_count[open_top]].data,sentence[i][j]);
						}
					}
					else if(type_check(sentence[i][j]) == 7) // 실수(real_number)를 허용하지 않음.
					{
						error_num = 7;
						break;
					}
					else if(type_check(sentence[i][j]) == 8) // MINUS,IF외의 다른 키워드가 없음
					{
						error_num = 8;
						break;
					}
					else if(type_check(sentence[i][j]) == 9)
					{
						error_num = 9;
						break;
					}
					if(j == sentence_len-1)
					{
						if(open_top > -1)
						{
							error_num = 1;
							break;
						}
						if(value_top >= 1) // 계산 값이 1개이상 남아 있는 상황은 존재하지 않음.
						{
							error_num = 6;
							break;
						}
						else
						{
							printf("Value : %d\n",value[value_top]);
						}
					}
				}
                if(error_num >= 1 && error_num <= 9)
				{
					error_exist(error_num);
				}
			}
		}
	}
}

void sentence_toy()
{
	char sentence[105];
	token_stack keyword[105][105]={0};
    int token_count[105]={0};
	int open_top=-1,close_top=-1;
	int save = 0,value_top = -1;
	int value[105] = {0};
	int error_num = 0;

    printf("Input Sentence \n");
	printf("---------------------------------\n");
	getchar();
    gets(sentence);
	printf("---------------------------------\n");
    int sentence_len = strlen(sentence);

	for(int j=0; j<sentence_len; j++)
	{
		if(type_check(sentence[j]) == 1)
		{
			if(save == 1 || close_top > 0)
			{
				error_num = 1;
				break;
			}
			else
			{
				open_top++;
				strcpy( keyword[open_top][token_count[open_top]].data,"(");
				token_count[open_top]++;
			}
		}
		else if(save == 1 || type_check(sentence[j]) == 2)
		{
			if(open_top < 0)
			{
				error_num = 2;
				break;
			}
			else
			{
				for(int k=token_count[open_top]; k>=0; k--)
				{
					if(strcmp( keyword[open_top][token_count[open_top]-k].data,"MINUS") == 0)
					{
						value[value_top-1] = value[value_top-1] - value[value_top];
						value[value_top] = 0;
						value_top--;
					}
				if(strcmp( keyword[open_top][token_count[open_top]-k].data,"IF") == 0)
					{
						if(value[value_top-1] > 0)
						{
							value[value_top-1] = value[value_top];
							value[value_top] = 0;
							value_top--;
						}
						else if(value[value_top-1] == 0 || value[value_top-1] < 0)
						{
							value[value_top-1] = 0;
							value[value_top] = 0;
							value_top--;
						}
					}
				}
				for(int i=0; i<token_count[open_top]; i++)
				{
					keyword[open_top][i].data[0] = 0;
				}
				token_count[open_top] = 0;
				open_top--;
			}
		}
		else if( sentence[j] == 'M' && sentence[j+1] == 'I' && sentence[j+2] == 'N' && sentence[j+3] == 'U' && sentence[j+4] == 'S' )
		{
			if(strcmp( keyword[open_top][token_count[open_top]-1].data,"(") != 0)
			{
				error_num = 3;
				break;
			}
			strcpy( keyword[open_top][token_count[open_top]].data,"MINUS");
			token_count[open_top]++;
			j+=5;
		}
		else if(sentence[j] == 'I' && sentence[j+1] == 'F')
		{
			if(strcmp( keyword[open_top][token_count[open_top]-1].data,"(") != 0)
			{
				error_num = 4;
				break;
			}
			strcpy( keyword[open_top][token_count[open_top]].data,"IF");
			token_count[open_top]++;
			j+=2;
		}
		else if(type_check(sentence[j]) == 5)
		{
			if(strcmp( keyword[open_top][token_count[open_top]-1].data,"-") == 0)
			{
				error_num = 5;
				break;
			}
			strcpy( keyword[open_top][token_count[open_top]].data,"-");
			token_count[open_top]++;
		}
		else if(type_check(sentence[j]) == 6)
		{
			if(sentence[j+1] == ' ' || sentence[j+1] == ')')
			{
				append( keyword[open_top][token_count[open_top]].data,sentence[j]);
				save = 0;
				value_top++;
				value[value_top] = atoi( keyword[open_top][token_count[open_top]].data);
				if(strcmp( keyword[open_top][token_count[open_top]-1].data,"-") == 0)
				{
					value[value_top] *= -1;
				}
				token_count[open_top]++;
			}
			else
			{
				append( keyword[open_top][token_count[open_top]].data,sentence[j]);
			}
		}
		else if(type_check(sentence[j]) == 7)
		{
			error_num = 7;
			break;
		}
		else if(type_check(sentence[j]) == 8)
		{
			error_num = 8;
			break;
		}
		else if(type_check(sentence[j]) == 9)
		{
			error_num = 9;
			break;
		}
        if(j == sentence_len-1)
		{
			if(open_top > -1)
			{
				error_num = 1;
				break;
			}
			if(value_top != 0)
			{
				error_num = 6;
                break;
            }
			else
			{
			printf("Value : %d\n",value[value_top]);
			}
		}
	}
    if(error_num >= 1 && error_num <= 9)
	{
		error_exist(error_num);
    }
}
int main(void)
{
	while(1)
	{
		int menu;
		printf("========================\n");
		printf("1. File Load\n");
		printf("2. Interactive Mode\n");
		printf("3. Exit\n");
		printf("========================\n");
		printf("Input Menu >> ");
		scanf(" %d",&menu);
		if(menu == 1) // 파일 불러오기 모드
		{
			file_load_toy();
		}
		if(menu == 2) // 문장 입력 모드
		{
            sentence_toy();
		}
		if(menu == 3) // 프로그램 종료
		{
			printf("End Program.\n");
			return 0;
		}
	}
}
