#include <stdio.h>
#include <stdbool.h>
#include <string.h>

unsigned short int table[9][9];

bool solid(unsigned int i)
{
	return (i != 0)&&(((i)&(i - 1)) == 0);
};

unsigned int order(unsigned int i)
{
	unsigned int j = 0;
	while (i != (1 << j)) j++;
	return j;
};

void init_table(void)
{
	for (unsigned int i = 0; i < 9; i++)
	{
		char s[10];
		scanf("%9s\n", s);
		for (unsigned int j = 0; j < 9; j++)
		{
			unsigned int num = 0;
			if (s[j] >= '1' && s[j] <= '9') table[i][j] = (1 << (s[j] - '1'));
			else table[i][j] = 0x1ff;
		}
	}
};

void print_table(void)
{
	unsigned int i = 0;

	for (unsigned int i = 0; i < 9; i++)
	{
		for (unsigned int j = 0; j < 9; j++)
		{
			unsigned int num = table[i][j];
			if (solid(num)) printf("%u", order(num) + 1);
			else printf("?");
		}
		printf("\n");
	}
};

bool except_rule(unsigned short int *group)
{
	unsigned int solidnum = 0;
	bool locchanged = false;
	for (unsigned int i = 0; i < 9; i++)
	{
		if (solid(group[i]))
		{
			solidnum = group[i];
			for (unsigned int j = 0; j < 9; j++)
			{
				if (i != j && ((group[j] & solidnum) != 0))
				{
					group[j] &= ~solidnum;
					locchanged = true;
				}
			}
		}
	}
	return locchanged;
};

bool unique_rule(unsigned short int *group)
{
	struct Unique
	{
		unsigned char placemet;
		unsigned char count;
	} unique[9];
	memset(unique, 0, 9 * sizeof(struct Unique));
	
	bool locchanged = false;
	for (unsigned int i = 0; i < 9; i++)
	{
		unsigned int somenum = group[i];
		if (!solid(somenum))
		{
			for (unsigned int numi = 0; numi < 9; numi++)
			{
				if ((somenum & (1 << numi)) != 0)
				{
					if (unique[numi].count == 0)
					{
						unique[numi].placemet = i;
					}
					unique[numi].count++;
				}
			}
		}
	}

	for (unsigned int numi = 0; numi < 9; numi++)
	{
		if (unique[numi].count == 1)
		{
			group[unique[numi].placemet] = (1 << numi);
			locchanged = true;
		}
	}

	return locchanged;
};

bool line_rule(unsigned int line, bool except)
{
	unsigned short int group[9];
	bool locchange = false;

	for (unsigned int i = 0; i < 9; i++)
	{
		group[i] = table[line][i];
	}

	if (except) locchange = except_rule(group);
	else locchange = unique_rule(group);

	if (locchange)
	{
		for (unsigned int i = 0; i < 9; i++)
		{
			table[line][i] = group[i];
		}
	}
	return locchange;
};

bool column_rule(unsigned int column, bool except)
{
	unsigned short int group[9];
	bool locchange = false;

	for (unsigned int i = 0; i < 9; i++)
	{
		group[i] = table[i][column];
	}

	if (except) locchange = except_rule(group);
	else locchange = unique_rule(group);

	if (locchange)
	{
		for (unsigned int i = 0; i < 9; i++)
		{
			table[i][column] = group[i];
		}
	}
	return locchange;
};

bool sector_rule(unsigned int line, unsigned int column, bool except)
{
	unsigned short int group[9];
	bool locchange = false;

	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			group[3*i+j] = table[3*line+i][3*column+j];
		}
	}

	if (except) locchange = except_rule(group);
	else locchange = unique_rule(group);

	if (locchange)
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				table[3*line+i][3*column+j] = group[3*i+j];
			}
		}
	}

	return locchange;
};

bool line_column_sector_rule(bool except)
{
	bool locchanged = false;

	for (unsigned int i = 0; i < 9; i++)
	{
		if (line_rule(i, except)) locchanged = true;
		if (column_rule(i, except)) locchanged = true;
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			if (sector_rule(i, j, except)) { locchanged = true; };
		}
	}
	return locchanged;
};

int main()
{
	init_table();
	while (line_column_sector_rule(true) || line_column_sector_rule(false)) {}
	print_table();
	return 0;
};

