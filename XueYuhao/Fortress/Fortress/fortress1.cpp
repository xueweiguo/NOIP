#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<algorithm>
using namespace std;

struct tree
{
	int x;
	int y;
	int r;
	int icircle[101];
	int ocircle = -1;
	int fc = -1;
	int next2 = -1;
	int prev = -1;
	int mostdown = 0;
	int rootroot = 0;
};
tree wall[101];
int datas;
int walls;
double dis;
int pos_f = 0;

void DC(int d)
{
	if (wall[wall[d].ocircle].fc == d)
	{
		wall[wall[d].ocircle].fc = wall[d].next2;
	}
	wall[wall[d].prev].next2 = wall[d].next2;
	wall[wall[d].next2].prev = wall[d].prev;
	wall[d].ocircle = -1;
	wall[d].prev = -1;
	wall[d].next2 = -1;
}

void AC(int father, int c)
{
	if (wall[father].fc == -1)
	{
		wall[c].ocircle = father;
		wall[father].fc = c;
		return;
	}
	int fc2 = wall[father].fc;
	wall[fc2].prev = c;
	wall[c].next2 = fc2;
	wall[father].fc = c;
}


void AD(int d, int a)
{
	if (d == 1)
	{
		AC(1, a);
	}
	else
	{
		DC(wall[d].fc);
		AC(d, a);
		AC(a, wall[d].fc);
	}
}

void root_root(int w)
{
	/*if (wall[w].in == 0)
	{
		wall[wall[w].ocircle].mostdown = wall[w].mostdown + 1;
	}
	for (int i = 1; i <= wall[w].in; ++i)
	{
		root_root(wall[i].icircle[i]);
	}
	int most = -2 , mpos;
	for (int i = 1; i <= wall[w].in; ++i)
	{
		if (most < wall[wall[w].icircle[i]].mostdown)
		{
			most = wall[wall[w].icircle[i]].mostdown;
			mpos = i;
		}
	}
	wall[w].rootroot += most;
	most = 0;
	for (int i = 1; i <= wall[w].in; ++i)
	{
		if (i == mpos)
		{
			continue;
		}
		if (most < wall[wall[w].icircle[i]].mostdown)
		{
			most = wall[wall[w].icircle[i]].mostdown;
			mpos = i;
		}
	}*/
	
}

int inside(int a , int b)
{
	dis = sqrt(pow((double)(wall[a].x - wall[b].x), 2) +
		       pow((double)(wall[a].y - wall[b].y), 2));
	if (wall[a].r > wall[b].r)
	{
		if (dis + (double)wall[b].r < (double)wall[a].r)
		{
			return 1;
		}
	}
	if (dis > (double)wall[b].r + (double)wall[a].r)
	{
		return -1;
	}
	return 0;
}

void find_pos(int a, int b)//a is in b or not
{
	if (inside(b, a) == -1)
	{
		return;
	}
	if (inside(b, a) == 0)
	{
		pos_f = wall[b].ocircle;
		return;
	}
	if (inside(b, a) == 1)
	{
		int nexxt;
		if (wall[b].fc == -1)
		{
			pos_f = b;
			return;
		}
		if (wall[b].fc != -1)
		{
			nexxt = wall[b].fc;
			while (nexxt != -1)
			{
				find_pos(a, nexxt);
			}
		}
	}
}

void ptree(int a)
{
	printf("Dad:%d\n", a);
	int nexxt = wall[a].fc;
	if (nexxt == -1)
	{
		cout << "NULL" << endl;
	}
	if (nexxt != -1)
	{
		while (1)
		{
			printf("%d ", nexxt);
			nexxt = wall[nexxt].next2;
			if (nexxt == -1)
			{
				break;
			}
		}
		printf("\n");
		nexxt = wall[a].fc;
		while (1)
		{
			ptree(nexxt);
			nexxt = wall[nexxt].next2;
			if (nexxt == -1)
			{
				break;
			}
		}
	}
}

int main()
{
	freopen("f.in", "r", stdin);
	//freopen("f.out", "w", stdout);
	scanf("%d", &datas);
	for (int d = 1; d <= datas; ++d)
	{
		scanf("%d", &walls); 
		//scanf x , y , r
		for (int i = 1; i <= walls; ++i)
		{
			scanf("%d%d%d", &wall[i].x, &wall[i].y, &wall[i].r);
		}
		for (int i = 2; i <= walls; ++i)
		{
			find_pos(i, 1);
			AD(pos_f, i);
		}
		//root_root(1);
		//printf("%d", wall[1].rootroot);
		ptree(1);
	}
	fclose(stdin);
	//fclose(stdout);
	system("pause");
	return 0;
}