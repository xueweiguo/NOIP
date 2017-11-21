#include<stdlib.h>
#include<cstdio>
#include<stdio.h>
//#include<utility>
#include<string>
#include<iostream>

#define NODE_COUNT (300500 * 2)
int nodeStack[NODE_COUNT];
int usedNode;

inline void initNodeStack() {
	for (int i = 0; i < NODE_COUNT; i++) {
		nodeStack[i] = i  + 1;
	}
	usedNode = 0;
}

inline int getNode()
{
	if (usedNode < NODE_COUNT) {
		return nodeStack[usedNode++];
	}
	else {
		return 0;
	}
}

inline void releaseNode(int ni) {
	nodeStack[--usedNode] = ni;
}

typedef long long KeyType;

	KeyType key[NODE_COUNT];
	long long number[NODE_COUNT];
	int length[NODE_COUNT];
	int size[NODE_COUNT];
	int priority[NODE_COUNT];
	int left[NODE_COUNT];
	int right[NODE_COUNT];

inline void init(int ni, KeyType _key, int len, long long num ){
	key[ni] = _key;
	number[ni] = num;
	length[ni] = len;
	size[ni] = len;
	priority[ni] = rand();
	left[ni] = 0;
	right[ni] = 0;
}

inline void calcSize(int ni) {
	size[ni] = length[ni];
	if(left[ni] != 0) size[ni] += size[left[ni]];
	if(right[ni]!= 0) size[ni] += size[right[ni]];
}

inline void setLeft(int ni, int _left) {
	left[ni] = _left;
	calcSize(ni);
}
inline void setRight(int ni, int _right) {
	right[ni] = _right;
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

inline NodePair split(int root, KeyType _key) {
	if (root == 0) return NodePair(0, 0);
	if (key[root] < _key) {
		NodePair rs = split(right[root], _key);
		setRight(root, rs.first);
		return NodePair(root, rs.second);
	}
	NodePair ls = split(left[root], _key);
	setLeft(root, ls.second);
	return NodePair(ls.first, root);
}

inline int insert(int root, int ni) {
	if (root == 0) return ni;
	if (priority[root] < priority[ni]) {
		NodePair splitted = split(root, key[ni]);
		setLeft(ni, splitted.first);
		setRight(ni, splitted.second);
		return ni;
	}
	else if (key[ni] < key[root]) {
		setLeft(root, insert(left[root], ni));
	}
	else {
		setRight(root, insert(right[root], ni));
	}
	return root;
}

inline int merge(int a, int b) {
	if (a == 0) return b;
	if (b == 0) return a;
	if (priority[a] < priority[b]) {
		setLeft(b, merge(a, left[b]));
		return b;
	}
	setRight(a, merge(right[a], b));
	return a;
}

inline int erase(int parent, KeyType _key) {
	if (parent == 0) return parent;
	if (key[parent] == _key) {
		int ret = merge(left[parent], right[parent]);
		releaseNode(parent);
		return ret;
	}
	if (_key < key[parent]) {
		setLeft(parent, erase(left[parent], _key));
	}
	else {
		setRight(parent, erase(right[parent], _key));
	}
	return parent;
}

void output(int ni, int layer) {
	std::string prefix("                                ", layer);
	if (left[ni]) {
		output(left[ni], layer + 1);
	}
	std::cout << prefix << key[ni] << "," << length[ni] << ",size:" << size[ni] << std::endl;
	if (right[ni]) {
		output(right[ni], layer + 1);
	}
}

void output(int ni) {
	if (left[ni]) {
		output(left[ni]);
	}
	if (number[ni] == 0) {
		//cout << "[" << ni << ":" << node[ni].key << "," << node[ni].length << "],";
		std::cout << "[" << key[ni] << "," << length[ni] << "],";
	}
	else {
		std::cout << "[" << number[ni] <<  "],";
	}
	if (right[ni]) {
		output(right[ni]);
	}
}

inline int find(int parent, int col, int& from_col) {
	if (col >= from_col + size[parent]) return 0;
	
	if (left[parent] != 0) {
		if (col < (from_col + size[left[parent]])) {
			return find(left[parent], col, from_col);
		}
		from_col += size[left[parent]];
	}

	if (col < (from_col + length[parent])) {
		return parent;
	}

	from_col += length[parent];

	if (right[parent] != 0) {
		if (col < (from_col + size[right[parent]])) {
			return find(right[parent], col, from_col);
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

inline long long leaveFromRow(int row, int col) {
	int from_col = 1;
	int found = find(root[row], col, from_col);
	if (found != 0) {
		if (number[found] != 0) {
			long long num = number[found];
			root[row] = erase(root[row], key[found]);
			return num;
		}

		int offset = col - from_col;
		long long number = key[found] + offset;
		int len = length[found] - 1;
		if (len == 0) {
			root[row] = erase(root[row], key[found]);
			return number;
		}
		
		NodePair pair(0,0);
	    if (offset == 0) {
			pair.first = getNode();
			init(pair.first, key[found] + 1, len, 0);
		}
		else if(offset == length[found] - 1){
			pair.first = getNode();
			init(pair.first, key[found], len, 0);
		}
		else {
			pair.first = getNode();
			init(pair.first, key[found], offset, 0);
			pair.second = getNode();
			init(pair.second, key[found]+ offset + 1, len - offset, 0);
		}
		root[row] = erase(root[row], key[found]);
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

inline long long leaveFromMatrix(int row, int col) {
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
		std::cout << lastCol[r] << std::endl;
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
