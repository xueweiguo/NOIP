#include<stdlib.h>
#include<cstdio>
#include<stdio.h>
#include<string>
#include<iostream>

//#define LOG_ON
//#define TEST

using namespace std;

#define NODE_COUNT (300500 * 5)
int nodeStack[NODE_COUNT];
int usedNode;

int randnum = 0;

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
	//nodeStack[--usedNode] = ni;
}

typedef long long KeyType;

struct Node {
	KeyType key;
	long long number;
	int length;
	int size;
	int priority;
	int parent;
	int left, right;
};

Node node[NODE_COUNT];

void init(int ni, KeyType _key, int len, long long number){
#ifdef LOG_ON
	std::cout << "init(" << ni << "," << _key << ")" << std::endl;
#endif
	node[ni].key = _key;
	node[ni].number = number;
	node[ni].length = len;
	node[ni].size = len;
	node[ni].priority = rand();
	node[ni].parent = 0;
	node[ni].left = 0;
	node[ni].right = 0;
}

void calcSize(int ni) {
#ifdef LOG_ON
	std::cout << "calcSize(" << ni << ")" << std::endl;
#endif
	node[ni].size = node[ni].length;
	if(left != 0) node[ni].size += node[node[ni].left].size;
	if(right != 0) node[ni].size += node[node[ni].right].size;
}

void reportSize(int ni) {
	int parent = ni;
	while (parent != 0) {
		calcSize(parent);
		parent = node[parent].parent;
	}
}

void setLeft(int ni, int left) {
#ifdef LOG_ON
	std::cout << "setLeft(" << ni << "," << left << ")" << std::endl;
#endif
	node[ni].left = left;
	node[left].parent = ni;
	calcSize(ni);
}
void setRight(int ni, int right) {
#ifdef LOG_ON
	std::cout << "setRight(" << ni << "," << right << ")" << std::endl;
#endif
	node[ni].right = right;
	node[right].parent = ni;
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
#ifdef LOG_ON
	std::cout << "split(" << root << "," << key << ")" << std::endl;
#endif
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
#ifdef LOG_ON
	std::cout << "insert(" << root << "," << ni << ")" << std::endl;
#endif
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
#ifdef LOG_ON
	std::cout << "merge(" << a << "," << b << ")" << std::endl;
#endif	
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
#ifdef LOG_ON
	std::cout << "erase(" << parent << "," << key << ")" << std::endl;
#endif	
	if (parent == 0) return parent;
	if (node[parent].key == key) {
		int ret = merge(node[parent].left, node[parent].right);
		node[parent].left = 0;
		node[parent].right = 0;
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
	//cout << "[" << node[ni].key << ":" << node[ni].number << "," << node[ni].number + node[ni].length - 1 << "],";
	if(node[ni].length != 1)
		cout << "[" << node[ni].number << "," << node[ni].number + node[ni].length - 1 << "],";
	else
		cout << node[ni].number << ",";

	if (node[ni].right) {
		output(node[ni].right);
	}
}

int find(int parent, int col, int& from_col) {
#ifdef LOG_ON
	std::cout << "find(" << parent<< "," << col << "," << from_col << ")" << std::endl;
#endif
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

int root[NODE_COUNT];
int rows = 0;
int cols = 0;
int events = 0;
long long pushKeyBase = 0;

long long leaveFromTree(int row, int col) {
#ifdef LOG_ON
	std::cout << "leaveFromRow(" << row << "," << col << ")" << std::endl;
#endif
	int from_col = 1;
	int found = find(root[row], col, from_col);
	if (found != 0) {
		int offset = col - from_col;
		long long number = node[found].number + offset;
		int length = node[found].length - 1;
		if (length == 0) {
			root[row] = erase(root[row], node[found].key);
			return number;
		}
		
	    if (offset == 0) {
			++node[found].key;
			++node[found].number;
			--node[found].length;
			reportSize(found);
		}
		else if(offset == node[found].length - 1){
			--node[found].length;
			reportSize(found);
		}
		else {
			node[found].length = offset;
			reportSize(found);

			int second = getNode();
			init(second, node[found].key + offset + 1, length - offset, node[found].key + offset + 1);
			root[row] = insert(root[row], second);
		}
		return number;
	}
	return 0;
}

long long leaveFromMatrix(int row, int col) {
#ifdef LOG_ON
	std::cout << "leaveFromMatrix(" << row << "," << col << ")" << std::endl;
#endif
	long long number = 0;
	if (col < cols) {
		number = leaveFromTree(row, col);
		int from = 1;
		int push_node = find(root[0], row, from);
		root[0] = erase(root[0], node[push_node].key);
		root[row] = insert(root[row], push_node);
		
		int end_node = getNode();
		init(end_node, pushKeyBase + end_node, 1, number);
		root[0] = insert(root[0], end_node);
	}
	else if(col == cols){
		int from = 1;
		int push_node = find(root[0], row, from);
		number = node[push_node].number;
		root[0] = erase(root[0], node[push_node].key);
		
		int end_node = getNode();
		init(end_node, pushKeyBase + end_node, 1, number);
		root[0] = insert(root[0], end_node);
	}
	return number;
}


void outputData()
{
	for (int r = 1; r <= rows; r++) {
		output(root[r]);
		//cout << lastCol[r] << ",size:" << node[root[r]].size + 1 << endl;
		int find_from = 1;
		int found = find(root[0], r, find_from);
		std::cout << node[found].number << std::endl;
	}
}

int main() {
	initNodeStack();

	srand(50);
#ifdef TEST
	rows = 5;
	cols = 5;
#else
	//freopen("phalanx20.in", "r", stdin);
	//freopen("phalanx.out", "w", stdout);
	scanf("%d%d%d", &rows, &cols, &events);
#endif

	pushKeyBase = (long long)rows * cols + 1;

	for (int r = 1; r <= rows; ++r) {
		root[r] = getNode();
		init(root[r], (long long)(r - 1) * cols + 1, cols - 1, (long long)(r - 1) * cols + 1);
	}
	root[0] = 0;
	for (int r = 1; r <= rows; ++r) {
		int n = getNode();
		init(n, pushKeyBase + n, 1, (long long)r * cols);
		root[0] = insert(root[0], n);
	}


#ifdef TEST
	outputData();

	cout << leaveFromMatrix(1, 5) << endl;
	outputData();

	cout << leaveFromMatrix(1, 1) << endl;
	outputData();

	cout << leaveFromMatrix(1, 2) << endl;
	outputData();

	cout << leaveFromMatrix(1, 1) << endl;
	outputData();
	
	cout << leaveFromMatrix(1, 4) << endl;
	outputData();

	cout << leaveFromMatrix(1, 3) << endl;
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
	//fclose(stdin);
	//fclose(stdout);
#endif
 	return 0;
}
