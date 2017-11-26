#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>

using namespace std;

struct node
{
	int dad;
	int fc = -1;
	int prev;
	int next2 = -1;
};
node tree[15];

void MTree(int a)
{
	if (tree[tree[a].dad].fc == -1)
	{
		tree[tree[a].dad].fc = a;
	}
	int nexxt = tree[tree[a].dad].fc;
	while (nexxt != -1)
	{
		nexxt = tree[nexxt].next2;
	}
	tree[nexxt].next2 = a;
	tree[a].prev = nexxt;
}

int main2()
{
	tree[1].dad = 0;
	tree[2].dad = 1;
	tree[3].dad = 1;
	tree[4].dad = 1;
	tree[5].dad = 2;
	tree[6].dad = 2;
	tree[7].dad = 2;
	tree[1].prev = tree[1].next2 = 0;
	for (int i = 2; i <= 7; ++i)
	{
		MTree(i);
	}
	int n = 1;
	while (tree[n].next2 != -1)
	{
		cout << n << endl;
		n = tree[n].next2;
	}
	system("pause");
	return 0;
}