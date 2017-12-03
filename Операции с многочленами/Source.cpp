#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <clocale>

using namespace std;

FILE *f = fopen("f.txt", "r");

struct monom
{
	int deg, coef;
	monom *next, *prev;
};
struct base
{
	int num;
	monom *polynom;
	base *prev, *next;
};

void inputConsole(monom *&L);
int inputFile(monom *&L);
void basefromfile(base *&L);
void output(monom *L);
void outputbase(base *&L);
void sortpolynom(monom *&L);
int derivativecoef(int deg, int n);
monom *derivative(monom *L, int n);
monom *sum(monom *L1, monom *L2);
monom *multiplication(monom *L1, monom *L2);
int count(monom *L, int x);
void integerroots(monom *L);
void deletemonom(monom *&L);

void inputConsole(monom *&L)
{
	if (L == nullptr)
		L = new monom;
	L->prev = nullptr;
	L->next = nullptr;
	monom *p;
	p = L;
	char a;
	int flag;
	int begin = 0, minus = 0, k = 0;
	//1 - '-', 2 - '+', 3 - digitcoef, 4 - digitdeg, 5 - 'x', 6 - '^', 7 - Enter
	a = cin.get();
	if (a == '-')
	{
		flag = 1;
		minus = 1;
	}
	else if (a == 'x')
	{
		flag = 5;
	}
	else if (isdigit(a))
	{
		p->coef = a - '0';
		flag = 3;
	}
	else
	{
		delete L;
		L = nullptr;
		printf("Error");
		return;
	}
	while (true)
	{
		a = cin.get();
		if (flag == 1)
		{
			if (a == 'x')
			{
				minus = 0;
				p->coef = -1;
				flag = 5;
			}
			else if (isdigit(a))
			{
				p->coef = a - '0';
				flag = 3;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return;
			}
		}
		else if (flag == 2)
		{
			if (a == 'x')
			{
				p->coef = 1;
				flag = 5;
			}
			else if (isdigit(a))
			{
				p->coef = a - '0';
				flag = 3;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return;
			}
		}
		else if (flag == 3)
		{
			if (a == 'x')
			{
				if (minus)
				{
					p->coef *= -1;
					minus = 0;
				}
				flag = 5;
			}
			else if (a == '+')
			{
				if (minus)
				{
					p->coef *= -1;
					minus = 0;
				}
				p->deg = 0;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 2;
			}
			else if (a == '-')
			{
				if (minus)
				{
					p->coef *= -1;
				}
				minus = 1;
				p->deg = 0;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 1;
			}
			else if (isdigit(a))
			{
				p->coef = p->coef * 10 + a - '0';
			}
			else if (isspace(a) && a != ' ')
			{
				if (minus)
					p->coef *= -1;
				p->deg = 0;
				p->next = nullptr;
				return;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return;
			}
		}
		else if (flag == 4)
		{
			if (a == '+')
			{
				if (minus)
				{
					p->coef *= -1;
					minus = 0;
				}
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 2;
			}
			else if (a == '-')
			{
				if (minus)
					p->coef *= -1;
				minus = 1;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 1;
			}
			else if (isdigit(a))
			{
				p->deg = p->deg * 10 + a - '0';
			}
			else if (isspace(a) && a != ' ')
			{
				if (minus)
					p->coef *= -1;
				p->next = nullptr;
				return;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return;
			}

		}
		else if (flag == 5)
		{
			if (a == '+')
			{
				if (minus)
				{
					p->coef *= -1;
					minus = 0;
				}
				p->deg = 1;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 2;
			}
			else if (a == '-')
			{
				if (minus)
				{
					p->coef *= -1;
				}
				minus = 1;
				p->deg = 1;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 1;
			}
			else if (a == '^')
			{
				flag = 6;
			}
			else if (isspace(a) && a != ' ')
			{
				if (minus)
					p->coef *= -1;
				p->deg = 1;
				p->next = nullptr;
				return;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return;
			}
		}
		else if (flag == 6)
		{
			if (isdigit(a))
			{
				p->deg = a - '0';
				flag = 4;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return;
			}
		}
	}
}
int inputFile(monom *&L)
{
	if (L == nullptr)
		L = new monom;
	L->prev = nullptr;
	L->next = nullptr;
	monom *p;
	p = L;
	char a;
	int flag;
	int begin = 0, minus = 0, k = 0;
	//1 - '-', 2 - '+', 3 - digitcoef, 4 - digitdeg, 5 - 'x', 6 - '^'
	a = fgetc(f);
	if (a == '-')
	{
		flag = 1;
		minus = 1;
	}
	else if (a == 'x')
	{
		flag = 5;
	}
	else if (isdigit(a))
	{
		p->coef = a - '0';
		flag = 3;
	}
	else
	{
		delete L;
		L = nullptr;
		printf("Error");
		return 1;
	}
	while (true)
	{
		a = fgetc(f);
		if (flag == 1)
		{
			if (a == 'x')
			{
				minus = 0;
				p->coef = -1;
				flag = 5;
			}
			else if (isdigit(a))
			{
				p->coef = a - '0';
				flag = 3;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return 1;
			}
		}
		else if (flag == 2)
		{
			if (a == 'x')
			{
				p->coef = 1;
				flag = 5;
			}
			else if (isdigit(a))
			{
				p->coef = a - '0';
				flag = 3;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return 1;
			}
		}
		else if (flag == 3)
		{
			if (a == 'x')
			{
				if (minus)
				{
					p->coef *= -1;
					minus = 0;
				}
				flag = 5;
			}
			else if (a == '+')
			{
				if (minus)
				{
					p->coef *= -1;
					minus = 0;
				}
				p->deg = 0;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 2;
			}
			else if (a == '-')
			{
				if (minus)
				{
					p->coef *= -1;
				}
				minus = 1;
				p->deg = 0;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 1;
			}
			else if (isdigit(a))
			{
				p->coef = p->coef * 10 + a - '0';
			}
			else if (isspace(a) && a != ' ' || a == EOF)
			{
				if (minus)
					p->coef *= -1;
				p->deg = 0;
				p->next = nullptr;
				return 0;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return 1;
			}
		}
		else if (flag == 4)
		{
			if (a == '+')
			{
				if (minus)
				{
					p->coef *= -1;
					minus = 0;
				}
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 2;
			}
			else if (a == '-')
			{
				if (minus)
					p->coef *= -1;
				minus = 1;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 1;
			}
			else if (isdigit(a))
			{
				p->deg = p->deg * 10 + a - '0';
			}
			else if (isspace(a) && a != ' ' || a == EOF)
			{
				if (minus)
					p->coef *= -1;
				p->next = nullptr;
				return 0;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return 1;
			}

		}
		else if (flag == 5)
		{
			if (a == '+')
			{
				if (minus)
				{
					p->coef *= -1;
					minus = 0;
				}
				p->deg = 1;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 2;
			}
			else if (a == '-')
			{
				if (minus)
				{
					p->coef *= -1;
				}
				minus = 1;
				p->deg = 1;
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				flag = 1;
			}
			else if (a == '^')
			{
				flag = 6;
			}
			else if (isspace(a) && a != ' ' || a == EOF)
			{
				if (minus)
					p->coef *= -1;
				p->deg = 1;
				p->next = nullptr;
				return 0;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return 1;
			}
		}
		else if (flag == 6)
		{
			if (isdigit(a))
			{
				p->deg = a - '0';
				flag = 4;
			}
			else
			{
				deletemonom(L);
				printf("Error");
				return 1;
			}
		}
	}
}
void basefromfile(base *&L)
{
	base *p;
	int k;
	if (L == nullptr) 
	{
		L = new base;
		L->prev = nullptr;
		p = L;
		k = 1;
	}
	else
	{
		p = L;
		while (p->next != nullptr)
			p = p->next;
		k = p->num + 1;
		p->next = new base;
	}
	for (int i = k; !feof(f); i++)
	{
		p->num = i;
		p->polynom = new monom;
		if (inputFile(p->polynom))
		{
			printf("Error in polynom %d", i);
			p->prev->next = nullptr;
			delete p;
			return;
		}
		sortpolynom(p->polynom);
		if (!feof(f))
		{
			p->next = new base;
			p->next->prev = p;
			p = p->next;
		}
		else
			p->next = nullptr;
	}
}
void output(monom *L)
{ 
	if (L == nullptr)
	{
		printf("0\n");
		return;
	}
	else if (L != nullptr && L->deg != 0)
	{
		if (L->coef == 1)
		{
			if (L->deg == 1)
				printf("x");
			else
				printf("x^%d", L->deg);
		}
		else if (L->coef == -1)
		{
			if (L->deg == 1)
				printf("-x");
			else
				printf("-x^%d", L->deg);
		}
		else
		{
			if (L->deg == 1)
				printf("%dx", L->coef);
			else
				printf("%dx^%d", L->coef, L->deg);
		}
		L = L->next;
	}
	else
	{
		printf("%d\n", L->coef);
		return;
	}
	while (L != nullptr)
	{
		if (L->coef > 0)
		{
			if (L->coef == 1)
			{
				if (L->deg == 1)
					printf("+x");
				else if (L->deg == 0)
					printf("+1");
				else
					printf("+x^%d", L->deg);
			}
			else
			{
				if (L->deg == 1)
					printf("+%dx", L->coef);
				else if (L->deg == 0)
					printf("+%d", L->coef);
				else
					printf("+%dx^%d", L->coef, L->deg);
			}
		}
		else if (L->coef < 0)
		{
			if (L->coef == -1)
			{
				if (L->deg == 1)
					printf("-x");
				else if (L->deg == 0)
					printf("-1");
				else
					printf("-x^%d", L->deg);
			}
			else
			{
				if (L->deg == 1)
					printf("%dx", L->coef);
				else if (L->deg == 0)
					printf("%d", L->coef);
				else
					printf("%dx^%d", L->coef, L->deg);
			}
		}
		else
		{
			if (L->coef > 0)
				printf("+%d", L->coef);
			if (L->coef < 0)
				printf("%d", L->coef);
		}
		L = L->next;
	}
	printf("\n");
}
void outputbase(base *&L)
{
	base *p;
	monom *q;
	p = L;
	while (p != nullptr)
	{
		printf("%d ", p->num);
		q = p->polynom;
		output(q);
		p = p->next;
	}
}
void sortpolynom(monom *&L)
{
	monom *p, *q;
	int i = 0;
	do
	{
		i = 0;
		p = L;
		while (p->next != nullptr)
		{
			if (p->next->deg > p->deg)
			{
				int k = p->deg;
				p->deg = p->next->deg;
				p->next->deg = k;
				k = p->coef;
				p->coef = p->next->coef;
				p->next->coef = k;
				i++;
			}
			p = p->next;
		}
	} while (i != 0);

	p = L;
	while (p->next != nullptr)
	{
		if (p->deg == p->next->deg)
		{
			q = p->next;
			p->coef += q->coef;
			p->next = q->next;
			if (q->next != nullptr)
				q->next->prev = p;
			delete q;
		}
		else
			p = p->next;
	}

	p = L;
	while (p != nullptr)
	{
		if (p->coef == 0)
		{
			q = p;
			if (p->prev != nullptr && p->next != nullptr)
			{
				p->prev->next = p->next;
				p->next->prev = p->prev;
				p = p->next;
				delete q;
			}
			else if (p->prev == nullptr && p->next != nullptr)
			{
				p = p->next;
				L = p;
				p->prev = nullptr;
				delete q;
			}
			else if (p->prev != nullptr && p->next == nullptr)
			{
				p->prev->next = nullptr;
				p = nullptr;
				delete q;
			}
			else
			{
				L = nullptr;
				p = L;
				delete q;
			}
			continue;
		}
		p = p->next;
	}
}
int derivativecoef(int deg, int n)
{
	if (n == 1)
		return deg;
	else
		return deg * derivativecoef(deg - 1, n - 1);
}
monom *derivative(monom *L, int n)
{
	if (L == nullptr)
		return nullptr;
	monom *L1, *p;
	L1 = new monom;
	L1->prev = nullptr;
	p = L1;
	while (L != nullptr && L->deg >= n)
	{
		p->deg = L->deg - n;
		p->coef = L->coef * derivativecoef(L->deg, n);
		if (L->next != nullptr && L->next->deg != 0)
		{
			p->next = new monom;
			p->next->prev = p;
		}
		else
			p->next = nullptr;
		p = p->next;
		L = L->next;
	}
	return L1;
}
monom *sum(monom *L1, monom *L2) //Рассмотреть случай, когда многочлены противоположны 
{
	if (L1 == nullptr && L2 == nullptr)
		return nullptr;
	monom *L, *p;
	L = new monom;
	L->prev = nullptr;
	p = L;
	while (L1 != nullptr || L2 != nullptr)
	{
		if (L1 != nullptr && L2 == nullptr)
		{
			p->deg = L1->deg;
			p->coef = L1->coef;
			L1 = L1->next;
			if (L1 != nullptr)
			{
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
			}
			else
				p->next = nullptr;
		}
		else if (L2 != nullptr && L1 == nullptr)
		{
			p->deg = L2->deg;
			p->coef = L2->coef;
			L2 = L2->next;
			if (L2 != nullptr)
			{
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
			}
			else
				p->next = nullptr;
		}
		else if (L1->deg == L2->deg)
		{
			if (L1->coef == -1 * L2->coef)
			{
				L1 = L1->next;
				L2 = L2->next;
			}
			else
			{
				p->deg = L1->deg;
				p->coef = L1->coef + L2->coef;
				L1 = L1->next;
				L2 = L2->next;
				if (L1 != nullptr || L2 != nullptr)
				{
					p->next = new monom;
					p->next->prev = p;
					p = p->next;
				}
				else
					p->next = nullptr;
			}
		}
		else if (L1->deg > L2->deg)
		{
			p->deg = L1->deg;
			p->coef = L1->coef;
			L1 = L1->next;
			p->next = new monom;
			p->next->prev = p;
			p = p->next;
		}
		else if (L1->deg < L2->deg)
		{
			p->deg = L2->deg;
			p->coef = L2->coef;
			L2 = L2->next;
			p->next = new monom;
			p->next->prev = p;
			p = p->next;
		}
	}
	if (p == L)
		return nullptr;
	return L;
}
monom *multiplication(monom *L1, monom *L2)
{
	if (L1 == nullptr || L2 == nullptr)
		return nullptr;
	monom *L, *p, *q, *r;
	L = new monom;
	L->prev = nullptr;
	p = L;
	q = L1;
	r = L2;
	while (q != nullptr)
	{
		while (r != nullptr)
		{
			p->deg = q->deg + r->deg;
			p->coef = q->coef * r->coef;
			if (q->next != nullptr || r->next != nullptr)
			{
				p->next = new monom;
				p->next->prev = p;
				p = p->next;
			}
			else
				p->next = nullptr;
			r = r->next;
		}
		r = L2;
		q = q->next;
	}
	sortpolynom(L);
	return L;
}
int count(monom *L, int x)
{
	if (L == nullptr)
		return 0;
	int y = L->coef;
	int n = L->deg;
	while (L->next != nullptr)
	{
		while (n != L->next->deg)
		{
			y *= x;
			n--;
		}
		y += L->next->coef;
		L = L->next;
	}
	while (n != 0)
	{
		y *= x;
		n--;
	}
	return y;
}
void integerroots(monom *L)
{
	if (L == nullptr)
	{
		printf("Zero pointer\n");
		return;
	}
	int k = 0;
	monom *p;
	p = L;
	while (p->next != nullptr)
		p = p->next;
	if (p->deg != 0)
	{
		printf("Integer roots: 0");
		k = 1;
	}
	for (int i = 1; i <= abs(p->coef); i++)
	{
		if (!(p->coef % i))
		{
			if (!count(L, i))
			{
				if (k == 0)
				{
					printf("Integer roots: %d", i);
					k = 1;
				}
				else
					printf(", %d", i);
			}
			if (!count(L, -1 * i))
			{
				if (k == 0)
				{
					printf("Integer roots: %d", -1 * i);
					k = 1;
				}
				else
					printf(", %d", -1 * i);
			}
		}
	}
	if (k == 0)
		printf("No integer roots\n");
	else
		printf("\n");
}
void deletemonom(monom *&L)
{
	if (L == nullptr)
		return;
	monom *p;
	while (L != nullptr)
	{
		p = L;
		L = L->next;
		delete p;
	}
}
void interface()
{
	cout << "Здравствуйте!\n";
}


int main()
{
	setlocale(0, "");
	interface();
	base *L, *p, *q;
	L = nullptr;
	basefromfile(L);
	outputbase(L);
	fclose(f);
	monom *a;
	a = new monom;
	inputConsole(a);
	sortpolynom(a);
	output(a);
	system("pause");
}
