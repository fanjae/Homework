#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool double_cal = false; // 실수형 연산 여부 
bool prev_token_is_number = false;
char prev_token;
int number[105]={0}; // 자릿수 저장 배열 
int	double_cal_count = 1; // n번째 혼합연산 횟수
float num;

enum {null,NUMBER,PLUS,STAR,LPAREN,RPAREN,DOUBLE,END} token; // NULL이 예약어이므로, null로 변경

double add();
double double_add();
double expression();
double term();
double factor();
void get_token();
void error(int i);

void get_token()
{
	char next_token;
	char number_temp[105];
	int i = 0;
	if(prev_token_is_number == false)
		next_token = getchar();
	else
		next_token = prev_token;

	if(next_token >= '0' && next_token <= '9')
	{
		do
		{
			number_temp[i++] = next_token;
			next_token = getchar();
		} while (isdigit(next_token));
		number_temp[i] = 0;
		num = atof(number_temp);
		token = NUMBER;
		prev_token_is_number = true;
		prev_token = next_token;
	}
	else if(next_token == '+')
	{
		token = PLUS;
		prev_token_is_number = false;
		return ;
	}
	else if(next_token == '*')
	{
		token = STAR;
		prev_token_is_number = false;
		return ;
	}
	else if(next_token == '(')
	{
		token = LPAREN;
		prev_token_is_number = false;
		return ;
	}
	else if(next_token == ')')
	{
		token = RPAREN;
		prev_token_is_number = false;
		return ;
	}
	else if(next_token == '.')
	{
		token = DOUBLE;
		prev_token_is_number = false;
		return ;
	}
	else if(next_token == EOF)
	{
		token = END;
		prev_token_is_number = false;
		return ;
	}
	else
	{
		token = null;
		prev_token_is_number = false;
	}
}
double expression()
{
	double result;
	result = term();
	while(token == PLUS)
	{
		get_token();
		result = result + term();
	}
	return result;
}
double term()
{
	double result;
	result = factor();
	while (token == STAR)
	{
		get_token();
		result = result * factor();
	}
	return result;
}
double factor()
{
	double result;
	if(token == NUMBER)
	{
		result = num; // token이 number인 경우 result에 숫자값을 넣음.
		get_token();
	}
	else if(token == LPAREN)
	{
		get_token();
		result = expression();
		if (token == RPAREN)
			get_token();
		else
			error(2);
	}
	else
		error(1);
	return result;
}
int main()
{
	double result;
	get_token();
	result=expression();
	if(token!=END)
		error(3);
	else
		if(double_cal == true)
			printf("\n결과 : %lf",result);
		else
			printf("\n결과 : %d",(int)result);
}
void error(int i)
{
	switch (i)
	{
		case 1: printf("error: number of '(' expected\n"); break;
		case 2: printf("error: ')' expected\n"); break;
		case 3: printf("error: EOF expected\n"); break;
	}
	exit(1);
}

