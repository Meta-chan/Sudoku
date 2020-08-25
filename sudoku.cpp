#include <iostream>

struct uniqueelem
{
	unsigned char placemet = 0;
	unsigned char count = 0;
};

unsigned short int table[9][9];

bool solid(unsigned int i)
{
	return (i != 0)&&(((i)&(i - 1)) == 0);
}

unsigned int order(unsigned int i)
{
	unsigned int j = 0;
	while (i != (1 << j))
	{
		j++;
	}
	return j;
}

void inittable()
{
	for (unsigned int i = 0; i < 9; i++)
	{
		char s[10];
		std::cin >> s;
		for (unsigned int j = 0; j < 9; j++)
		{
			unsigned int num = 0;
			if (s[j] >= '1' && s[j] <= '9') table[i][j] = (1 << (s[j] - '1'));
			else table[i][j] = 0x1ff;
		}
	}
}

void giveanswer()
{
	unsigned int i = 0, j, num;

	std::cout << '\n';
	while (i < 9)
	{
		j = 0;
		while (j < 9) {
			num = table[i][j];
			if (solid(num))
			{
				std::cout << order(num)+1;
			}
			else
			{
				std::cout << '?';
			}
			j++;
		}
		std::cout << '\n';
		i++;
	}
}

bool exceptrule(unsigned short int *group)
{
	unsigned int solidnum = 0;
	bool locchanged = false;
	for (unsigned int i = 0; i < 9; i++)
	{
		if (solid(*(group + i)))
		{
			solidnum = *(group + i);
			for (unsigned int j = 0; j < 9; j++)
			{
				if ((i != j) && (((*(group + j)) & solidnum) != 0))
				{
					*(group + j) = (*(group + j)) & (~solidnum);
					locchanged = true;
				}
			}
		}
	}
	return locchanged;
}

bool uniquerule(unsigned short int *group)
{
	uniqueelem unique[9];
	bool locchanged = false;
	unsigned int somenum;
	for (unsigned int i = 0; i < 9; i++)
	{
		somenum = *(group + i);
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
			*(group + unique[numi].placemet) = (1 << numi);
			locchanged = true;
		}
	}

	return locchanged;
}

bool linerule(unsigned int line, bool except)
{
	unsigned short int group[9];
	bool locchange=false;

	for (unsigned int i = 0; i < 9; i++)
	{
		group[i] = table[line][i];
	}

	if (except) locchange = exceptrule(&(group[0]));
	else locchange = uniquerule(&(group[0]));

	if (locchange)
	{
		for (unsigned int i = 0; i < 9; i++)
		{
			table[line][i] = group[i];
		}
	}
	return locchange;
}

bool columnrule(unsigned int column, bool except)
{
	unsigned short int group[9];
	bool locchange=false;

	for (unsigned int i = 0; i < 9; i++)
	{
		group[i] = table[i][column];
	}

	if (except) locchange = exceptrule(&(group[0]));
	else locchange = uniquerule(&(group[0]));

	if (locchange)
	{
		for (unsigned int i = 0; i < 9; i++)
		{
			table[i][column] = group[i];
		}
	}
	return locchange;
}

bool sectorrule(unsigned int line, unsigned int column, bool except)
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

	if (except) locchange = exceptrule(&(group[0]));
	else locchange = uniquerule(&(group[0]));

	if (locchange)
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				table[3 * line + i][3 * column + j] = group[3 * i + j];
			}
		}
	}

	return locchange;
}

bool tablerule(bool except)
{
	bool locchanged = false;

	for (unsigned int i = 0; i < 9; i++)
	{
		if (linerule(i, except)) locchanged = true;
		if (columnrule(i, except)) locchanged = true;
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			if (sectorrule(i, j, except)) { locchanged = true; };
		}
	}
	return locchanged;
};

unsigned int main()
{
	inittable();
	while (tablerule(true)||tablerule(false)) {}
	giveanswer();
	char c;
	std::cin >> c;
	return 0;
}

