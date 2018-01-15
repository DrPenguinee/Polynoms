#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

struct monom
{
	int deg, coef;
	monom *next = nullptr, *prev = nullptr;
};
struct base
{
	int num;
	monom *polynom;
	base *prev = nullptr, *next = nullptr;
};

base *L = nullptr;
int sizeofbase = 0;
FILE *f = fopen("f.txt", "r");

int nod(int a, int b)
{
	if (a % b == 0)
		return b;
	return nod(b, a % b);
}
void inputConsole(monom *&L);
int inputFile(monom *&L, FILE *f);
void basefromfile(FILE *f);
void output(monom *L);
void outputbase();
void sortpolynom(monom *&L);
int derivativecoef(int deg, int n);
monom *derivative(monom *L, int n);
monom *sum(monom *L1, monom *L2);
monom *multiplication(monom *L1, monom *L2);
monom *division(monom *L1, monom *L2);
int count(monom *L, int x);
void integerroots(monom *L);
void deletemonom(monom *&L);
void commandlist();
void command();
void baseorconsole(monom *&a);
void addtobase(monom *&a);
void deletefrombase(int n); 
monom *copy(monom *a);


int inputFile(monom *&L, FILE *f)
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
				while (!(isspace(a) && a != ' ' || a == EOF))
					a = fgetc(f);
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
				while (!(isspace(a) && a != ' ' || a == EOF))
					a = fgetc(f);
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
				while (!(isspace(a) && a != ' ' || a == EOF))
					a = fgetc(f);
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
				while (!(isspace(a) && a != ' ' || a == EOF))
					a = fgetc(f);
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
				while (!(isspace(a) && a != ' ' || a == EOF))
					a = fgetc(f);
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
				while (!(isspace(a) && a != ' ' || a == EOF))
					a = fgetc(f);
				return 1;
			}
		}
	}
}
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
	//1 - '-', 2 - '+', 3 - digitcoef, 4 - digitdeg, 5 - 'x', 6 - '^'
	a = cin.get();
	if (a == '-')
	{
		flag = 1;
		minus = 1;
	}
	else if (a == 'x')
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
		delete L;
		L = nullptr;
		printf("Некорректный ввод. Попробуйте еще:\n");
		while (!(isspace(a) && a != ' '))
			a = cin.get();
		inputConsole(L);
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
				printf("Некорректный ввод. Попробуйте еще:\n");
				while (!(isspace(a) && a != ' '))
					a = cin.get();
				inputConsole(L);
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
				printf("Некорректный ввод. Попробуйте еще:\n");
				while (!(isspace(a) && a != ' '))
					a = cin.get();
				inputConsole(L);
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
				sortpolynom(L);
				return;
			}
			else
			{
				deletemonom(L);
				printf("Некорректный ввод. Попробуйте еще:\n");
				while (!(isspace(a) && a != ' '))
					a = cin.get();
				inputConsole(L);
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
				sortpolynom(L);
				return;
			}
			else
			{
				deletemonom(L);
				printf("Некорректный ввод. Попробуйте еще:\n");
				while (!(isspace(a) && a != ' '))
					a = cin.get();
				inputConsole(L);
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
				sortpolynom(L);
				return;
			}
			else
			{
				deletemonom(L);
				printf("Некорректный ввод. Попробуйте еще:\n");
				while (!(isspace(a) && a != ' '))
					a = cin.get();
				inputConsole(L);
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
				printf("Некорректный ввод. Попробуйте еще:\n");
				while (!(isspace(a) && a != ' '))
					a = cin.get();
				inputConsole(L);
				return;
			}
		}
	}
}
void basefromfile(FILE *f)
{
	if (feof(f))
	{
		printf("Файл пуст.\n");
		return;
	}
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
		p->next->prev = p;
		p->next->next = nullptr;
		p = p->next;
	}
	for (int i = 0; !feof(f); i++)
	{
		sizeofbase = i + k;
		p->num = i + k;
		p->polynom = new monom;
		if (inputFile(p->polynom, f))
		{
			printf("Ошибка в %d строке файла.\nЧтобы добавить следующие многочлены повторите команду.\n\n", i + 1);
			p->prev->next = nullptr;
			delete p;
			sizeofbase--;
			return;
		}
		sortpolynom(p->polynom);
		if (!feof(f))
		{
			p->next = new base;
			p->next->prev = p;
			p->next->next = nullptr;
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
void outputbase()
{
	if (L == nullptr)
		printf("База пуста.\n");
	base *p;
	monom *q;
	p = L;
	if (p != nullptr)
	{
		printf("База:\n");
		printf("%d: ", p->num);
		q = p->polynom;
		output(q);
		p = p->next;
	}
	while (p != nullptr)
	{
		printf("%d: ", p->num);
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
	if (L->deg < n)
		return nullptr;
	monom *L1, *p;
	L1 = new monom;
	L1->prev = nullptr;
	p = L1;
	while (L != nullptr && L->deg >= n)
	{
		p->deg = L->deg - n;
		p->coef = L->coef * derivativecoef(L->deg, n);
		if (L->next != nullptr && L->next->deg >= n)
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
monom *sum(monom *L1, monom *L2){
	if (L1 == nullptr && L2 == nullptr)
		return nullptr;
	monom *L, *p;
	L = new monom;
	L->prev = nullptr;
	p = L;
	p->deg = -1;
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
	if (p->deg == -1)
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
monom *division(monom *L1, monom *L2)
{
	if (L1 == nullptr || L1->deg < L2->deg)
		return nullptr;
	monom *minus = new monom;
	minus->deg = 0;
	minus->coef = -1;
	monom *coefficient = new monom;
	coefficient->coef = 1;
	coefficient->deg = 0;
	monom *L3 = new monom;
	monom *NL1, *p, *q;
	NL1 = multiplication(L1, coefficient);
	q = L3;
	while (true)
	{
		if (NL1->coef % L2->coef != 0)
		{
			coefficient->coef *= abs(L2->coef);
			deletemonom(NL1);
			NL1 = multiplication(L1, coefficient);
			deletemonom(L3);
			L3 = new monom;
			q = L3;
			continue;
		}
		q->coef = NL1->coef / L2->coef;
		q->deg = NL1->deg - L2->deg;
		minus->coef = -1 * q->coef;
		minus->deg = q->deg;
		p = NL1;
		NL1 = sum(NL1, multiplication(L2, minus));
		deletemonom(p);
		if (NL1 != nullptr && NL1->deg >= L2->deg)
		{
			q->next = new monom;
			q = q->next;
		}
		else break;
	}
	q = L3;
	if (q->next != nullptr)
	{
		int k = nod(abs(q->coef), abs(q->next->coef));
		q = q->next;
		while (q->next != 0)
		{
			k = nod(k, abs(q->next->coef));
			q = q->next;
		}
		q = L3;
		while (q != nullptr)
		{
			q->coef /= k;
			q = q->next;
		}
	}
	else q->coef /= abs(q->coef);
	return L3;
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
		printf("Дан нулевой многочлен. Множество корней.\n");
		return;
	}
	int k = 0;
	monom *p;
	p = L;
	while (p->next != nullptr)
		p = p->next;
	if (p->deg != 0)
	{
		printf("Целые корни: 0");
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
					printf("Целые корни: %d", i);
					k = 1;
				}
				else
					printf(", %d", i);
			}
			if (!count(L, -1 * i))
			{
				if (k == 0)
				{
					printf("Целые корни: %d", -1 * i);
					k = 1;
				}
				else
					printf(", %d", -1 * i);
			}
		}
	}
	if (k == 0)
		printf("Целых корней нет.\n");
	else
		printf(".\n");
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
void commandlist()
{
	printf("Введите номер команды:\n");
	printf(" 0. Пополнить базу из файла\n");
	printf(" 1. Добавить многочлен в базу\n");
	printf(" 2. Вывести базу\n");
	printf(" 3. Удалить многочлен из базы\n");
	printf(" 4. Сложить два многочлена\n");
	printf(" 5. Перемножить два многочлена\n");
	printf(" 6. Поделить многочлен на многочлен\n");
	printf(" 7. Найти производную\n");
	printf(" 8. Найти значение в точке\n");
	printf(" 9. Найти целые корни\n");
	printf("10. Завершение работы\n");
	printf("\n");
}
void command()
{
	char a, b, c;
	a = cin.get();
	while (isspace(a))
		a = cin.get();
	b = cin.get();
	if (!(isspace(b) && b != ' '))
		c = cin.get();
	printf("\n");
	if (a == '0' && isspace(b) && b != ' ')
	{
		system("cls");
		basefromfile(f);
		commandlist();
		command();
	}
	else if (a == '1' && isspace(b) && b != ' ')
	{
		system("cls");
		printf("Переменная - x. Пробелы и знак умножения не использовать, знак степени только после переменной!\n");
		printf("Примеры: -x, 6x^2-9x+24, 87\n");
		printf("Введите многочлен:\n");
		monom *a = nullptr;
		inputConsole(a);
		addtobase(a);
		system("cls");
		commandlist();
		command();
	}
	else if (a == '2' && isspace(b) && b != ' ')
	{
		system("cls");
		outputbase();
		printf("\n");
		commandlist();
		command();
	}
	else if (a == '3' && isspace(b) && b != ' ')
	{
		system("cls");
		outputbase();
		printf("\n");
		if (L != nullptr)
		{
			printf("Введите номер многочлена:\n");
			int n;
			while (true)
			{
				a = cin.get();
				while (true)
				{
					n = 0;
					while (isdigit(a))
					{
						n = 10 * n + a - '0';
						a = cin.get();
					}
					if (isspace(a) && a != ' ')
						break;
					else
						printf("Некорректный ввод. Пожалуйста, введите число:\n");
				}
				if (n > sizeofbase)
				{
					printf("Введённый номер превышает размер базы. Повторите попытку:\n");
					continue;
				}
				else
					deletefrombase(n);
				system("cls");
				break;
			}
		}
		commandlist();
		command();
	}
	else if (a == '4' && isspace(b) && b != ' ')
	{
		system("cls");
		monom *m = nullptr, *p = nullptr, *n = nullptr;
		baseorconsole(m);
		system("cls");
		printf("Первый многочлен: ");
		output(m);
		baseorconsole(p);
		n = sum(m, p);
		system("cls");
		printf("Первый многочлен: ");
		output(m);
		printf("Второй многочлен: ");
		output(p);
		printf("Их сумма: ");
		output(n);
		printf("\nЕсли желаете добавить многочлен в базу введите 1.\nЕсли нет, любую другую команду.\n");
		a = cin.get();
		if (a == '1')
			addtobase(n);
		else
			while (!(isspace(a) && a != ' '))
				a = cin.get();
		system("cls");
		commandlist();
		command();
	}
	else if (a == '5' && isspace(b) && b != ' ')
	{
		system("cls");
		monom *m = nullptr, *p = nullptr, *n = nullptr;
		baseorconsole(m);
		system("cls");
		printf("Первый многочлен: ");
		output(m);
		baseorconsole(p);
		n = multiplication(m, p);
		system("cls");
		printf("Первый многочлен: ");
		output(m);
		printf("Второй многочлен: ");
		output(p);
		printf("Их произведение: ");
		output(n);
		printf("\nЕсли желаете добавить многочлен в базу введите 1.\nЕсли нет, любую другую команду.\n");
		a = cin.get();
		if (a == '1')
			addtobase(n);
		else
			while (!(isspace(a) && a != ' '))
				a = cin.get();
		system("cls");
		commandlist();
		command();
	}
	else if (a == '6' && isspace(b) && b != ' ')
	{
		system("cls");
		monom *m = nullptr, *p = nullptr, *n = nullptr;
		baseorconsole(m);
		system("cls");
		printf("Делимое: ");
		output(m);
		baseorconsole(p);
		system("cls");
		printf("Внимание! Частное вычисляется с точностью до коэффициента.\n");
		printf("Делимое: ");
		output(m);
		printf("Делитель: ");
		output(p);
		if (p != nullptr)
		{
			n = division(m, p);
			printf("Частное: ");
			output(n);
			printf("Если желаете добавить многочлен в базу введите 1.\nЕсли нет, любую другую команду.\n");
			a = cin.get();
			if (a == '1')
				addtobase(n);
			else
				while (!(isspace(a) && a != ' '))
					a = cin.get();
		}
		else
		{
			printf("Ошибка! Делитель не может быть равен нулю.\n");
			system("pause");
		}
		system("cls");
		commandlist();
		command();
	}
	else if (a == '7' && isspace(b) && b != ' ')
	{
		system("cls");
		monom *m = nullptr;
		baseorconsole(m);
		system("cls");
		output(m);
		printf("\nВведите степень производной: ");
		monom *b = nullptr;
		int n;
		a = cin.get();
		while (true)
		{
			n = 0;
			while (isdigit(a))
			{
				n = 10 * n + a - '0';
				a = cin.get();
			}
			if (isspace(a) && a != ' ')
				break;
			else
				printf("Некорректный ввод. Пожалуйста, введите число:\n");
		}
		b = derivative(m, n);
		printf("Производная: ");
		output(b);
		printf("\nЕсли желаете добавить многочлен в базу введите 1.\nЕсли нет, любую другую команду.\n");
		a = cin.get();
		if (a == '1')
			addtobase(b);
		else
			while (!(isspace(a) && a != ' '))
				a = cin.get();
		system("cls");
		commandlist();
		command();
	}
	else if (a == '8' && isspace(b) && b != ' ')
	{
		system("cls");
		monom *m = nullptr;
		baseorconsole(m);
		system("cls");
		output(m);
		printf("Введите целое число: ");
		int n, k;
		int minus;
		while (true)
		{
			a = cin.get();
			minus = 0;
			if (a == '-')
			{
				a = cin.get();
				minus = 1;
			}
			n = 0;
			while (isdigit(a))
			{
				n = 10 * n + a - '0';
				a = cin.get();
			}
			if (isspace(a) && a != ' ')
				break;
			else
			{
				printf("Некорректный ввод. Пожалуйста, введите число: ");
				while (!(isspace(a) && a != ' '))
					a = cin.get();
			}
		}
		if (minus)
		{
			k = count(m, -1 * n);
			printf("Значение многочлена в точке -%d равно %d.\n", n, k);
		}
		else
		{
			k = count(m, n);
			printf("Значение многочлена в точке %d равно %d.\n", n, k);
		}
		system("pause");
		system("cls");
		commandlist();
		command();
	}
	else if (a == '9' && isspace(b) && b != ' ')
	{
		system("cls");
		monom *m = nullptr;
		baseorconsole(m);
		system("cls");
		integerroots(m);
		system("pause");
		system("cls");
		commandlist();
		command();
	}
	else if (a == '1' && b == '0' && isspace(c) && c != ' ')
	{
		return;
	}
	else
	{
		while (!(isspace(c) && c != ' '))
			c = cin.get();
		printf("Введена некорректная команда. Повторите ещё раз\n");
		command();
	}
}
void baseorconsole(monom *&a)
{
	if (L == nullptr)
	{
		printf("Переменная - x. Пробелы и знак умножения не использовать, знак степени только после переменной!\n");
		printf("Примеры: -x, 6x^2-9x+24, 87\n");
		printf("Введите многочлен:\n");
		inputConsole(a);
	}
	else
	{
		printf("1. Ввести многочлен с консоли\n");
		printf("2. Добавить из базы\n");
		char c, d;
		while (true)
		{
			c = cin.get();
			d = cin.get();
			if (c == '1' && isspace(d))
			{
				system("cls");
				printf("Переменная - x. Пробелы и знак умножения не использовать, знак степени только после переменной!\n");
				printf("Примеры: -x, 6x^2-9x+24, 87\n");
				printf("Введите многочлен:\n");
				inputConsole(a);
				break;
			}
			else if (c == '2' && isspace(d))
			{
				system("cls");
				outputbase();
				printf("Введите номер многочлена:\n");
				int k;
				while (true)
				{
					k = 0;
					c = cin.get();
					do
					{
						k = k * 10 + c - '0';
						c = cin.get();
					} while (!isspace(c));
					if (k > 0 && k <= sizeofbase)
						break;
					else
					{
						printf("Неверный номер. Попробуйте еще:\n");
						continue;
					}
				}
				base *p = L;
				while (p->num != k)
					p = p->next;
				a = p->polynom;
				break;
			}
			else
			{
				printf("Введена некорректная команда. Повторите ещё раз:\n");
			}
		}

	}
}
void addtobase(monom *&a)
{
	if (L == nullptr)
	{
		L = new base;
		L->prev = L->next = nullptr;
		L->num = 1;
		L->polynom = a;
		sizeofbase = 1;
	}
	else
	{
		base *p;
		p = L;
		while (p->next != nullptr)
			p = p->next;
		p->next = new base;
		p->next->prev = p;
		p->next->next = nullptr;
		p->next->num = p->num + 1;
		p->next->polynom = a;
		sizeofbase++;
	}
}
void deletefrombase(int n)
{

	base *p;
	p = L;
	for (int i = 1; i < n; ++i)
		p = p->next;
	if (sizeofbase == 1)
	{
		sizeofbase = 0;
		L = nullptr;
		deletemonom(p->polynom);
		delete p;
	}
	else
	{
		if (p->prev != nullptr)
			p->prev->next = p->next;
		else
			L = p->next;
		if (p->next != nullptr)
			p->next->prev = p->prev;
		sizeofbase--;
		deletemonom(p->polynom);
		delete p;
		p = L;
		for (int i = 1; p != nullptr; i++, p = p->next)
			p->num = i;
	}
}
monom *copy(monom *a)
{
	if (a == nullptr)
		return nullptr;
	monom *L = new monom, *p = L;
	while (a != nullptr)
	{
		*p = *a;
		a = a->next;
		if (a != nullptr)
		{
			p->next = new monom;
			p = p->next;
		}
	}
	return L;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	printf("Здравствуйте!\n");
	commandlist();
	command();
}