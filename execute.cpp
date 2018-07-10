/*#include"Header.h"
int **inst;

struct symbolTable
{
	char **name;
	unsigned long *address;
	unsigned long *size;
	int *isReadOnly;
	int count;
};

struct instructionTable
{
	unsigned long *instNo;
	int *opCode;
	unsigned long **param;
	int *paramCount;
	int count;
};

struct labelTable
{
	char **name;
	int *instNo;
	int count;
};


int loadCode()
{
	inst = (int**)malloc(sizeof(int*)* 100);
	for (int i = 0; i < 100; i++)
		inst[i] = (int*)malloc(sizeof(int)* 6);
	FILE *fp = fopen("instTable.txt", "r");
	if (fp == NULL)
	{
		cout << "File doesn't exist\n";
		return 0;
	}
	else
	{
		int line = 0, flag = 1;
		char buf[100];
		while (!feof(fp))
		{
			fgets(buf, 100, fp);
			char **tokens = tokenizer(buf, ' ');
			for (int i = 0; tokens[i] != '\0'; i++)
			{
				int l = strlen(tokens[i]);
				if (tokens[i][l - 1] == '\n')tokens[i][l - 1] = '\0';
				inst[line][i] = toNum(tokens[i]);
			}
			line += 1;
		}
		return line;
	}
	
}

void mov1(int arg1, int arg2)
{
	mem[arg1] = reg[arg2];
}

void mov2(int arg1, int arg2)
{
	reg[arg1] = mem[arg2];
}

void add(int arg1, int arg2, int arg3)
{
	reg[arg1] = reg[arg2] + reg[arg3];
}

void sub(int arg1, int arg2, int arg3)
{
	reg[arg1] = reg[arg2] - reg[arg3];
}

void mul(int arg1, int arg2, int arg3)
{
	reg[arg1] = reg[arg2] * reg[arg3];
}

int jmp(int arg)
{
	return arg;
}

int ltt(int arg1, int arg2)
{
	if (reg[arg1] < reg[arg2])
		return 1;
	return 0;
}

int eq(int arg1, int arg2)
{
	return reg[arg1] == reg[arg2];
}

int gt(int arg1, int arg2)
{
	return reg[arg1] > reg[arg2];
}

int lteq(int arg1, int arg2)
{
	return reg[arg1] <= reg[arg2];
}

int gteq(int arg1, int arg2)
{
	return reg[arg1] >= reg[arg2];
}

int iff(int line,int arg1, int op1, int arg2, int arg3)
{
	int f=0;

	switch (op1)
	{
	case 9: f =	ltt(arg1, arg2);
		break;
	case 8:f = eq(arg1, arg2);
		break;
	case 10:f = gt(arg1, arg2);
		break;
	case 11:f = lteq(arg1, arg2);
		break;
	case 12:f = gteq(arg1, arg2);
		break;
	}
	if (f!=0)return line;
	else return arg3;
}

void print(int arg1)
{
	printf("\nprint %d  , %d\n", arg1,reg[arg1]);
}

void read(int arg1)
{
	cin >> reg[arg1];
}

int performOp(int line)
{
	switch (inst[line][1])
	{
	case 1:mov1(inst[line][2], inst[line][3]);
		break;
	case 2:mov2(inst[line][2], inst[line][3]);
		break;
	case 3:add(inst[line][2], inst[line][3], inst[line][4]);
		break;
	case 4:sub(inst[line][2], inst[line][3], inst[line][4]);
		break;
	case 5:mul(inst[line][2], inst[line][3], inst[line][4]);
		break;
	case 6:line = jmp(inst[line][2]);
		break;
	case 7:line = iff(line,inst[line][2], inst[line][3], inst[line][4], inst[line][5]);
		break;
	case 13:print(inst[line][2]);
		break;
	case 14:read(inst[line][2]);
		break;
	}
	return line+1;
}


void exec(int n)
{
	int i;
	for (i = 1; i < n; i++)
	{
		i=performOp(i-1);
		cout << "\ni "<<i;
	}
}

void loadSymbolTable()
{
	FILE *fp = fopen("symboltable.txt", "r");
	if (fp == NULL)
	{
		cout << "File doesn't exist\n";
		return ;
	}
	else
	{
		int line = 0, flag = 1;
		char buf[100];
		while (!feof(fp))
		{
			fgets(buf, 100, fp);
			char **tokens = tokenizer(buf, ' ');
			if (!strcmp(tokens[2], "0"))
			{
				int m = toNum(tokens[1]);
				int l = strlen(tokens[3]);
				if (tokens[3][l - 1] == '\n')tokens[3][l - 1] = '\0';
				mem[m] = toNum(tokens[3]);
			}
		}
	}
}*/

/*
int main()
{
	for (int i = 0; i < 8; i++)reg[i] = i;
	int n = loadCode();
	loadSymbolTable();
	exec(n);
	system("PAUSE");
}*/