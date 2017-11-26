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
	int mostdown = 0;
	int nodenode = 0;

	//must have in tree
	int ocircle = -1;
	int fc = -1;
	int next2 = -1;
	int prev = -1;
	
};
tree wall[101];
int datas;
int walls;
double dis;
int pos_f = 0;
int mostnode = -99;

void max2(int fs[], int most)
{
	if (most < fs[1]) return;
	if (most > fs[0]) {
		fs[1] = fs[0];
		fs[0] = most;
		return;
	}
	fs[1] = most;
}

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


void ptree(int a)
{
	printf("Dad:%d", a);
	printf("(Mostdown:%d Nodenode:%d)\n", wall[a].mostdown, wall[a].nodenode);
	int nexxt = wall[a].fc;
	if (nexxt == -1)
	{
		cout << "NULL" << endl;
	}
	if (nexxt != -1)
	{
		while (1)
		{
			printf("%d", nexxt);
			printf("(Mostdown:%d Nodenode:%d) ", wall[nexxt].mostdown, wall[nexxt].nodenode);
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

#define RET_1_IN_2	0
#define RET_2_IN_1  1
#define RET_NONE   -1

int inside(int a, int b)
{
	dis = sqrt(pow((double)(wall[a].x - wall[b].x), 2) +
		pow((double)(wall[a].y - wall[b].y), 2));
	if (wall[a].r > wall[b].r)
	{
		if (dis + (double)wall[b].r < (double)wall[a].r)
		{
			return RET_2_IN_1;
		}
	}
	if (dis >(double)wall[b].r + (double)wall[a].r)
	{
		return RET_NONE;
	}
	return RET_1_IN_2;
}


bool addNode(int parent, int ni) {
	int in = inside(parent, ni);
	if(in == RET_2_IN_1) {
		int child = wall[parent].fc;
		while (child != -1) {
			if (addNode(child, ni)){
				return true;
			}
			child = wall[child].next2;
		}
		AC(parent, ni);
		return true;
	}
	else if(in == RET_1_IN_2){
		if (wall[parent].ocircle != -1) {
			DC(parent);
			AC(ni, parent);
			AC(wall[parent].ocircle, ni);
			return true;
		}
		return false;
	}
	else{//none
		return false;
	}
}

void node_node(int w)
{
	//if NoChild
	if (wall[w].fc == -1)
	{
		wall[w].mostdown = 0;
		return;
	}

	//Search ChildNode
	int nexxt = wall[w].fc;
	int fs[2];
	fs[0] = fs[1] = -99;
	while (nexxt != -1)
	{
		node_node(nexxt);
		max2(fs, wall[nexxt].mostdown);
		nexxt = wall[nexxt].next2;
	}

	wall[w].mostdown = fs[0] + 1;
	wall[w].nodenode = fs[0] + 1;
	if (fs[1] >= 0)
	{
		wall[w].nodenode += fs[1] + 1;
	}
	mostnode = max(wall[w].nodenode, mostnode);
}

int main()
{
	freopen("f.in", "r", stdin);
	freopen("f.out", "w", stdout);
	scanf("%d", &datas);
	for (int d = 1; d <= datas; ++d)
	{
		mostnode = -99;
		scanf("%d", &walls); 
		//scanf x , y , r
		for (int i = 1; i <= walls; ++i)
		{
			scanf("%d%d%d", &wall[i].x, &wall[i].y, &wall[i].r);
		}
		for (int i = 2; i <= walls; ++i)
		{
			addNode(1, i);
		}
		node_node(1);
		ptree(1);
		printf("%d", mostnode);
	}
	fclose(stdin);
	fclose(stdout);
	return 0;
}