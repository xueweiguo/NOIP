#include<stdlib.h>
#include<cstdio>
#include<stdio.h>
#include<string>
#include<iostream>

using namespace std;

#define NODE_COUNT (300500 * 2)
int nodeStack[NODE_COUNT];
int usedNode;

int randnum = 0;

int random[50] = { 13 , 16 , 65 , 761 , 5 , 774 , 1120 , 9975 , 10 , 93 ,
				   661 , 105 , 2 , 51 , 31 , 68 , 71 , 250 , 38 , 22 ,
				   313 , 50 , 105 , 3 , 19 , 23 , 1106 , 319 , 914 , 1027 ,
				   1969 , 335 , 1970 , 220 , 989 , 16 , 24 , 781 , 2222 , 6961 ,
				   233 , 1117 , 1025 , 793 , 18 , 1002 , 656 , 131 , 64 , 89 };

void initNodeStack() {
	for (int i = 0; i < NODE_COUNT; i++) {
		nodeStack[i] = i  + 1;
	}
	usedNode = 0;
}

int getNode()
{
	if (usedNode < NODE_COUNT) {
		return nodeStack[usedNode++];
	}
	else {
		return 0;
	}
}

void releaseNode(int ni) {
	nodeStack[--usedNode] = ni;
}

typedef long long KeyType;

struct Node {
	KeyType key;
	long long number;
	int length;
	int size;
	int priority;
	int left, right;
};

Node node[NODE_COUNT];

void init(int ni, KeyType _key, int len, long long number ){
	node[ni].key = _key;
	node[ni].number = number;
	node[ni].length = len;
	node[ni].size = len;
	node[ni].priority = rand();
	node[ni].left = 0;
	node[ni].right = 0;
}

void calcSize(int ni) {
	node[ni].size = node[ni].length;
	if(left != 0) node[ni].size += node[node[ni].left].size;
	if(right != 0) node[ni].size += node[node[ni].right].size;
}

void setLeft(int ni, int left) {
	node[ni].left = left;
	calcSize(ni);
}
void setRight(int ni, int right) {
	node[ni].right = right;
	calcSize(ni);
}

struct NodePair
{
	NodePair(int _first, int _second)
		:first(_first), second(_second){
	}
	int first;
	int second;
};

NodePair split(int root, KeyType key) {
	if (root == 0) return NodePair(0, 0);
	if (node[root].key < key) {
		NodePair rs = split(node[root].right, key);
		setRight(root, rs.first);
		return NodePair(root, rs.second);
	}
	NodePair ls = split(node[root].left, key);
	setLeft(root, ls.second);
	return NodePair(ls.first, root);
}

int insert(int root, int ni) {
	if (root == 0) return ni;
	if (node[root].priority < node[ni].priority) {
		NodePair splitted = split(root, node[ni].key);
		setLeft(ni, splitted.first);
		setRight(ni, splitted.second);
		return ni;
	}
	else if (node[ni].key < node[root].key) {
		setLeft(root, insert(node[root].left, ni));
	}
	else {
		setRight(root, insert(node[root].right, ni));
	}
	return root;
}

int merge(int a, int b) {
	if (a == 0) return b;
	if (b == 0) return a;
	if (node[a].priority < node[b].priority) {
		setLeft(b, merge(a, node[b].left));
		return b;
	}
	setRight(a, merge(node[a].right, b));
	return a;
}

int erase(int parent, KeyType key) {
	if (parent == 0) return parent;
	if (node[parent].key == key) {
		int ret = merge(node[parent].left, node[parent].right);
		releaseNode(parent);
		return ret;
	}
	if (key < node[parent].key) {
		setLeft(parent, erase(node[parent].left, key));
	}
	else {
		setRight(parent, erase(node[parent].right, key));
	}
	return parent;
}

void output(int ni, int layer) {
	string prefix("                                ", layer);
	if (node[ni].left) {
		output(node[ni].left, layer + 1);
	}
	cout << prefix << node[ni].key << "," << node[ni].length << ",size:" << node[ni].size << endl;
	if (node[ni].right) {
		output(node[ni].right, layer + 1);
	}
}

void output(int ni) {
	if (node[ni].left) {
		output(node[ni].left);
	}
	if (node[ni].number == 0) {
		//cout << "[" << ni << ":" << node[ni].key << "," << node[ni].length << "],";
		cout << "[" << node[ni].key << "," << node[ni].length << "],";
	}
	else {
		cout << "[" << node[ni].number <<  "],";
	}
	if (node[ni].right) {
		output(node[ni].right);
	}
}

int find(int parent, int col, int& from_col) {
	if (col >= from_col + node[parent].size) return 0;
	
	if (node[parent].left != 0) {
		if (col < (from_col + node[node[parent].left].size)) {
			return find(node[parent].left, col, from_col);
		}
		from_col += node[node[parent].left].size;
	}

	if (col < (from_col + node[parent].length)) {
		return parent;
	}

	from_col += node[parent].length;

	if (node[parent].right != 0) {
		if (col < (from_col + node[node[parent].right].size)) {
			return find(node[parent].right, col, from_col);
		}
	}
	return 0;
}

//#define TEST

int root[NODE_COUNT];
long long lastCol[NODE_COUNT];
int pushCounter[NODE_COUNT];
int rows = 0;
int cols = 0;
int events = 0;
long long pushKeyBase = 0;

long long leaveFromRow(int row, int col) {
	int from_col = 1;
	int found = find(root[row], col, from_col);
	if (found != 0) {
		if (node[found].number != 0) {
			long long number = node[found].number;
			root[row] = erase(root[row], node[found].key);
			return number;
		}

		int offset = col - from_col;
		long long number = node[found].key + offset;
		int length = node[found].length - 1;
		if (length == 0) {
			root[row] = erase(root[row], node[found].key);
			return number;
		}
		
		NodePair pair(0,0);
	    if (offset == 0) {
			pair.first = getNode();
			init(pair.first, node[found].key + 1, length, 0);
		}
		else if(offset == node[found].length - 1){
			pair.first = getNode();
			init(pair.first, node[found].key, length, 0);
		}
		else {
			pair.first = getNode();
			init(pair.first, node[found].key, offset, 0);
			pair.second = getNode();
			init(pair.second, node[found].key + offset + 1, length - offset, 0);
		}
		root[row] = erase(root[row], node[found].key);
		if (pair.first != 0) {
			root[row] = insert(root[row], pair.first);
		}
		if (pair.second != 0) {
			root[row] = insert(root[row], pair.second);
		}
		return number;
	}
	return 0;
}

long long leaveFromMatrix(int row, int col) {
#ifdef TEST
	cout << "row=" << row << ",col=" << col << ",number=";
#endif
	long long number = 0;
	if (col < cols) {
		number = leaveFromRow(row, col);
		int new_node = getNode();
		init(new_node, pushKeyBase + pushCounter[row], 1, lastCol[row]);
		pushCounter[row]++;
		root[row] = insert(root[row], new_node);
		for (int r = row; r <= rows - 1; ++r) {
			lastCol[r] = lastCol[r + 1];
		}
		lastCol[rows] = number;
	}
	else if(col == cols){
		number = lastCol[row];
		for (int r = row; r <= rows - 1; ++r) {
			lastCol[r] = lastCol[r + 1];
		}
		lastCol[rows] = number;
	}
	return number;
}


void outputData()
{
	for (int r = 1; r <= rows; r++) {
		output(root[r]);
		//cout << lastCol[r] << ",size:" << node[root[r]].size + 1 << endl;
		cout << lastCol[r] << endl;
	}
}

int main() {
	initNodeStack();

	srand(50);
#ifdef TEST
	rows = 5;
	cols = 5;
#else
	freopen("phalanx15.in", "r", stdin);
	freopen("phalanx.out", "w", stdout);
	scanf("%d%d%d", &rows, &cols, &events);
#endif

	pushKeyBase = (long long)rows * cols + 1;

	for (int r = 1; r <= rows; ++r) {
		root[r] = getNode();
		init(root[r], (long long)(r - 1) * cols + 1, cols - 1, 0);
		lastCol[r] = (long long)r * cols;
		pushCounter[r] = 0;
	}

#ifdef TEST
	outputData();

	cout << leaveFromMatrix(1, 1) << endl;
	outputData();

	cout << leaveFromMatrix(1, 2) << endl;
	outputData();

	cout << leaveFromMatrix(1, 1) << endl;
	outputData();
	
	cout << leaveFromMatrix(1, 4) << endl;
	outputData();

	cout << leaveFromMatrix(2, 1) << endl;
	outputData();

	cout << leaveFromMatrix(2, 1) << endl;
	outputData();

	cout << leaveFromMatrix(2, 1) << endl;
	outputData();

	cout << leaveFromMatrix(2, 1) << endl;
	outputData();

	cout << leaveFromMatrix(2, 1) << endl;
	outputData();

	cout << leaveFromMatrix(2, 5) << endl;
	outputData();

	cout << leaveFromMatrix(3, 4) << endl;
	outputData();

	cout << leaveFromMatrix(3, 4) << endl;
	outputData();

	cout << leaveFromMatrix(3, 4) << endl;
	outputData();

	cout << leaveFromMatrix(3, 4) << endl;
	outputData();
#else
	int r, c;
	for (int i = 1; i <= events; ++i)
	{
		scanf("%d%d", &r, &c);
		printf("%lld\n", leaveFromMatrix(r, c));
	}
	fclose(stdin);
	fclose(stdout);
#endif
 	return 0;
}
