#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<iostream>
#include<stdio.h>
#include<cstring>
#include<malloc.h>

int **inst;
int *mem = (int*)malloc(1000);
int reg[8];
int stack[10], top = -1;
int *symbolTable = (int*)malloc(sizeof(int)* 130);
int offset = 0;

char** tokenizer(char *s, char delim);
int toNum(char *s);
void mov1(int arg1, int arg2);
void mov2(int arg1, int arg2);
void add(int arg1, int arg2, int arg3);
void sub(int arg1, int arg2, int arg3);
void mul(int arg1, int arg2, int arg3);
int jmp(int arg);
int ltt(int arg1, int arg2);
int eq(int arg1, int arg2);
int gt(int arg1, int arg2);
int lteq(int arg1, int arg2);
int gteq(int arg1, int arg2);
int iff(int line, int arg1, int op1, int arg2, int arg3);
void print(int arg1);
void read(int arg1);
int getOpCode(char *s);

using namespace std;

struct symbolTable
{
	char **name;
	unsigned long *address;
	unsigned long *size;
	 int *isReadOnly;
	int count;
}st;

struct instructionTable
{
	unsigned long *instNo;
	int *opCode;
	unsigned long **param;
	int *paramCount;
	int count;
}it;

struct labelTable
{
	char **name;
	int *instNo;
	int count;
}lt;

void createLabelTable()
{
	lt.name = (char**)malloc(sizeof(char*)* 100);
	for (int i = 0; i < 100; i++)
		lt.name[i] = (char*)malloc(sizeof(char)* 30);
	lt.instNo = (int*)malloc(sizeof(int)* 100);
	lt.count = 0;
}

int isRegister(char *name)
{
	int l = strlen(name);
	if (strlen(name) == 2)
	{
		if (name[0] >= 'A'&&name[0] <= 'H'&&name[1] == 'X')
			return 1;
	}
	return 0;
}

void createInstructionTable()
{
	it.instNo = (unsigned long*)malloc(sizeof(unsigned long)* 400);
	it.opCode = (int*)malloc(sizeof(int)* 400);
	it.paramCount = (int*)malloc(sizeof(int)* 400);
	it.param = (unsigned long**)malloc(sizeof(unsigned long*)* 400);
	for (int i = 0; i < 400;i++)
		it.param[i] = (unsigned long*)malloc(sizeof(unsigned long)* 7);
	it.count = 0;
}

char** tokenizer(char *s, char delim)
{
	int i = 0, c = 0, j;
	char **tokens = (char**)malloc(sizeof(char*)* 50);
	for (i = 0; i<50; i++)tokens[i] = (char*)malloc(sizeof(char)* 50);
	i = 0;
	int l = strlen(s);
	if (s[l - 1] == '\n')s[l - 1] = '\0';
	while (s[i])
	{
		j = 0;
		while (s[i] && s[i] == delim)i++;
		while (s[i] && s[i] != delim)
		{
			tokens[c][j++] = s[i++];
		}
		tokens[c][j] = '\0';
		if (j != 0)c++;
	}
	tokens[c] = '\0';
	return tokens;
}

int toNum(char *s)
{
	int num = 0,i=0;
	while (s[i])
	{
		num = num * 10 + s[i] - '0';
		i++;
	}
	return num;
}

void createSymbolTable()
{
	st.name = (char**)malloc(sizeof(char*)* 100);
	for (int i = 0; i < 100; i++)
		st.name[i] = (char*)malloc(sizeof(char)* 100);
	st.address = (unsigned long*)malloc(sizeof(unsigned long)* 100);
	st.size = (unsigned long*)malloc(sizeof(unsigned long)* 100);
	st.isReadOnly = (int*)malloc(sizeof(int)* 100);
	st.count = 0;
}

unsigned long hashAdd(char *str)
{
	unsigned long hash = 5381;
	int c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

int isLabel(char *name)
{
	int l = strlen(name);
	//cout << lt.count;
	for (int i = 0; i < lt.count; i++)
	{
		//cout << "\nlabel  " << name << " " << lt.name[i];
		if (strcmp(lt.name[i],name)==0)
		{
			//cout << "inst no "<<lt.instNo[i];
			return lt.instNo[i];
		}
	}
	//cout << "here";
	return -1;
}

char* trim(char *s)
{
	char s1[50];
	int i = 0, c = 0,j=0;
	while (s[j++] == ' ');
	for (i = j; s[i]!='\0'; i++)
	{
		s1[c++] = s[i];
	}
	s1[c] = '\0';
	return s1;
}

void addInstruction(int instNo, int opCode, char *param)
{
	int i;
	it.instNo[it.count] = instNo;
	it.opCode[it.count] = opCode;
	char **tokens;// = (char**)malloc(sizeof(char*)* 7);
	//param = trim(param);
	//for (i = 0; i<7; i++)tokens[i] = (char*)malloc(sizeof(char)* 30);
	tokens = tokenizer(param, ',');
	int lbl;
	for (i = 0; tokens[i] != '\0'; i++)
	{
		//cout <<" tok "<< tokens[i] << " ";
		int j = 0;
		//strcpy(tokens[i], trim(tokens[i]));
		int l = strlen(tokens[i]);
		if (tokens[i][l - 1] == '\n')
			tokens[i][l - 1] = '\0';
		if (getOpCode(tokens[i])>0)
			it.param[it.count][i] = getOpCode(tokens[i]);
		else if (isRegister(tokens[i]))
			it.param[it.count][i] = tokens[i][0] - 'A';
		else if ((lbl = isLabel(tokens[i]))!=-1)
			it.param[it.count][i] = lbl;
		else
		{
			int indx = 0;
			char **subTokens = tokenizer(tokens[i], '[');
			if (subTokens[1])
			{
				char **sbt = tokenizer(subTokens[1], ']');
				indx = toNum(sbt[0]);
			}
			it.param[it.count][i] = symbolTable[hashAdd(subTokens[0]) % 127]+indx;
		}
	}
	if (opCode == 7)
		stack[++top] = instNo;
	it.paramCount[it.count] = i;
	//if (opCode == 7)cout << "\nif param count : "[it.count]
	it.count += 1;
}

void addSymbol(char *name,unsigned long size,int readable)
{
	strcpy(st.name[st.count], name);
	int index = hashAdd(name)%127;
	symbolTable[index] = offset;
	st.address[st.count] = index;
	st.size[st.count] = size;
	st.isReadOnly[st.count] = readable;
	st.count += 1;
	offset += size ? size : 1;
}

void addToSymbolTable(char *s)
{
	char **tokens = tokenizer(s,' ');
	unsigned long space = 1;
	if (!strcmp(tokens[0], "DATA"))
	{
		char **subtokens = tokenizer(tokens[1], '[');
		if (subtokens[1])
		{
			char **size = tokenizer(subtokens[1], ']');
			space = toNum(size[0]);
		}
		int l = strlen(subtokens[0]);
		if (subtokens[0][l - 1] =='\n')
		subtokens[0][l - 1] = '\0';
		addSymbol(subtokens[0], space, 0);
	}
	else if (!strcmp(tokens[0], "CONST"))
	{
		int l = strlen(tokens[3]);
		if (tokens[3][l - 1] == '\n')
		tokens[3][l - 1] = '\0';
		space = toNum(tokens[3]);
		addSymbol(tokens[1], 0, space);
	}
	
}


int getOpCode(char *s)
{
	char inst[15][10] = { "MOV", "MOV2", "ADD", "SUB", "MUL", "JUMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ" };
	for (int i = 0; i<14; i++)
	{
		if (!strcmp(inst[i], s))
			return i + 1;
	}
	return -1;
}


void createLabel(char *name, int instNo)
{
	int l = strlen(name);
	l--;
	name[l ] = '\0';
	strcpy(lt.name[lt.count], name);
	lt.instNo[lt.count] = instNo;
	//cout << lt.name[lt.count] << " " << lt.instNo[lt.count] << "\n";
	lt.count += 1;
}

void convert(char *s, int instNo)
{
	char **tokens = tokenizer(s, ' ');
	int opCode = getOpCode(tokens[0]);
	if (opCode == 1)
	{
		char **subtokens = tokenizer(tokens[1], ',');
		if (isRegister(subtokens[0]))
			opCode = 2;
	}
	if (opCode == 7)
	{
		int i;
		for ( i =2 ; tokens[i + 1] != '\0'; i++)
		{
			strcat(tokens[1], ",");
			strcat(tokens[1], tokens[i]);
		}
	}
	int l = strlen(tokens[0]);
	if (tokens[0][l - 1] == '\n')tokens[0][l - 1] = '\0';
	if (opCode>0)
	addInstruction(instNo,opCode, tokens[1]);
	else if (!strcmp(tokens[0], "ELSE"))
	{
		int indx = stack[top--];
		//indx -= 1;
		it.param[indx][it.paramCount[indx]] = instNo + 1;
			it.paramCount[indx] += 1;
			stack[++top] = instNo;
			//cout << "\nelse " << stack[top];
			addInstruction(instNo, 6, "");
	}
	else if (!strcmp(tokens[0], "ENDIF"))
	{
		int indx = stack[top--];
		//indx -= 1;		
		it.param[indx][it.paramCount[indx]] = instNo;
		//cout << "\NENDIF " << instNo<<"	indx "<<indx;
		//cout <<"INDX "<<indx<< "PARAM ARG : " << it.param[indx][it.paramCount[it.paramCount[indx]]] << " cnt " << it.paramCount[indx] << " ;";
		it.paramCount[indx] += 1;
		//stack[top++] = instNo;
		//addInstruction(instNo, 6, "");
	}
	else
	{
		int l = strlen(tokens[0]);
		if (tokens[0][l - 1] == '\n')
			tokens[0][l - 1] = '\0';
		l--;
		if (tokens[0][l] == ':')
		{
			createLabel(tokens[0], instNo);
		}
	}
}
void readFile(char *file)
{
	FILE *fp = fopen(file, "r");
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
			if (flag)
			{
				if (!strcmp(buf, "START:\n")||!(strcmp(buf,"START:")))
					flag = 0;
				else
					addToSymbolTable(buf);
			}
			else
			{
				convert(buf, line );
				int l = strlen(buf);
				if (buf[l - 1] == '\n')
				{
					buf[l - 1] = '\0';
					l--;
				}
				if (!strcmp(buf, "ENDIF") || !strcmp(buf, "ENDIF\n"));
				else if (buf[l - 1] == ':');
				else
				line += 1;
			}
		}
	}
}

void symbols()
{
	for (int i = 0; i < st.count; i++)
	{
		if (st.size[i] == 0)
			mem[symbolTable[st.address[i]]] = st.isReadOnly[i];
	}
}


int performOperation(int i)
{
	switch (it.opCode[i])
	{
	case 1:mov1(it.param[i][0], it.param[i][1]);
		break;
	case 2:mov2(it.param[i][0], it.param[i][1]);
		break;
	case 3:add(it.param[i][0], it.param[i][1], it.param[i][2]);
		break;
	case 4:sub(it.param[i][0], it.param[i][1], it.param[i][2]);
		break;
	case 5:mul(it.param[i][0], it.param[i][1], it.param[i][2]);
		break;
	case 6:i = jmp(it.param[i][0]);
		break;
	case 7:i = iff(i, it.param[i][0], it.param[i][1], it.param[i][2], it.param[i][3]);
		break;
	case 13:print(it.param[i][0]);
		break;
	case 14:read(it.param[i][0]);
		break;
	}
	return i;
}


void execIns()
{
	int i;
	for (i = 0; i < it.count; i++)
	{
		i = performOperation(i);
		//if (i==it.count-1)
			//i = performOperation(i);
	}
}

void writeToFile(char *file)
{
	FILE *fp = fopen(file, "w");
	for (int i = 0; i < st.count; i++)
	{
		fprintf(fp, "%s %u %u %d\n", st.name[i], symbolTable[st.address[i]], st.size[i], st.isReadOnly[i]);
	}
	fclose(fp);
	fp = fopen("instTable.txt","w");
	for (int i = 0; i < it.count; i++)
	{
		fprintf(fp, "%d %d ", it.instNo[i], it.opCode[i]);
		//printf( " %d %d ", it.instNo[i], it.opCode[i]);
		if (it.paramCount[i]==0)
			fprintf(fp, "%d ", it.param[i][0]);
		for (int j = 0; j < it.paramCount[i]; j++)
		{
			fprintf(fp, "%d ", it.param[i][j]);
			//printf("%d ", it.param[i][j]);
			//if (j == it.paramCount[i] - 1)fprintf(fp, "%s", "\n");
		}
		//printf("\n");
		fprintf(fp, "\n");
	}
	fclose(fp);
}




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
	//cout << mem[arg1];
	mem[arg1] = reg[arg2];
}

void mov2(int arg1, int arg2)
{
	//cout << mem[arg2];
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
	return arg-1;
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

int iff(int line, int arg1, int op1, int arg2, int arg3)
{
	int f = 0;

	switch (op1)
	{
	case 9: f = ltt(arg1, arg2);
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
	if (f != 0)return line;
	else return arg3-1;
}

void print(int arg1)
{
	printf("\n%d\n",reg[arg1]);
}

void read(int arg1)
{
	cin >> reg[arg1];
	//cout << "Input value " << reg[arg1];
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
	case 7:line = iff(line, inst[line][2], inst[line][3], inst[line][4], inst[line][5]);
		break;
	case 13:print(inst[line][2]);
		break;
	case 14:read(inst[line][2]);
		break;
	}
	return line + 1;
}


void exec(int n)
{
	int i;
	for (i = 1; i < n; i++)
	{
		i = performOp(i - 1);
		if (i == n )i = performOp(i-1);
		//cout << "\ni " << i;
	}
}

void loadSymbolTable()
{
	FILE *fp = fopen("symboltable.txt", "r");
	if (fp == NULL)
	{
		cout << "File doesn't exist\n";
		return;
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
}

/*
int main()
{
for (int i = 0; i < 8; i++)reg[i] = i;
int n = loadCode();
loadSymbolTable();
exec(n);
system("PAUSE");
}*/

int main()
{
	char file[50] = "notepad.txt";
	createSymbolTable();
	createInstructionTable();
	createLabelTable();
	readFile(file);
	writeToFile("symboltable.txt");
	symbols();
	execIns();
	system("PAUSE");
}
