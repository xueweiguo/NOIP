#include<stdlib.h>
#include<cstdio>
#include<stdio.h>
#include<string>
#include<iostream>

//#define LOG_ON
//#define TEST

struct NodeAllocator
{
	int* stack;
	int size;
	int used;
	NodeAllocator(int _node_id[], int _size)
		:stack(_node_id), size(_size)
	{
		for (int i = 0; i < _size; i++) stack[i] = i + 1;
		used = 0;
	}
	int getNode() {
		if (used < size) return stack[used++];
		return 0;
	}
	void releaseNode(int ni) {
		//stack[--used] = ni;
	}
};

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

struct NodePair
{
	NodePair(int _first, int _second)
		:first(_first), second(_second) {
	}
	int first;
	int second;
};

struct Treap {
	Node* node;
	NodeAllocator allocator;
	Treap(Node _node[], int node_id[], int _max)
		:node(_node), allocator(node_id, _max)
	{
	}

	void init(int ni, KeyType _key, int len, long long number) {
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
		node[ni].size = node[ni].length;
		if (node[ni].left != 0) node[ni].size += node[node[ni].left].size;
		if (node[ni].right != 0) node[ni].size += node[node[ni].right].size;
	}

	void reportSize(int ni) {
		int parent = ni;
		while (parent != 0) {
			calcSize(parent);
			parent = node[parent].parent;
		}
	}

	void setLeft(int ni, int left) {
		node[ni].left = left;
		node[left].parent = ni;
		calcSize(ni);
	}

	void setRight(int ni, int right) {
		node[ni].right = right;
		node[right].parent = ni;
		calcSize(ni);
	}

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
			node[parent].left = 0;
			node[parent].right = 0;
			allocator.releaseNode(parent);
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

	void output(int ni) {
		if (node[ni].left) {
			output(node[ni].left);
		}
		//cout << "[" << node[ni].key << ":" << node[ni].number << "," << node[ni].number + node[ni].length - 1 << "],";
		if (node[ni].length != 1)
			std::cout << "[" << node[ni].number << "," << node[ni].number + node[ni].length - 1 << "],";
		else
			std::cout << node[ni].number << ",";

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
};

#define NODE_COUNT (300500 * 5)

struct Phalanx : public Treap
{
	long long pushKeyBase;
	int root[NODE_COUNT];
	int rows;
	int cols;
	Phalanx(Node _node[], int node_id[], int _max)
		:Treap(_node, node_id, _max)
	{

	}

	void initialze(int _rows, int _cols) {
		cols = _cols;
		rows = _rows;
		pushKeyBase = (long long)rows * cols + 1;

		for (int r = 1; r <= rows; ++r) {
			root[r] = allocator.getNode();
			init(root[r], (long long)(r - 1) * cols + 1, cols - 1, (long long)(r - 1) * cols + 1);
		}
		root[0] = 0;
		for (int r = 1; r <= rows; ++r) {
			int n = allocator.getNode();
			init(n, pushKeyBase + n, 1, (long long)r * cols);
			root[0] = insert(root[0], n);
		}
	}

	long long leaveFromTree(int row, int col) {
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
				int second = allocator.getNode();
				init(second, node[found].key + offset + 1, length - offset, node[found].key + offset + 1);
				root[row] = insert(root[row], second);
			}
			return number;
		}
		return 0;
	}

	long long leaveFromMatrix(int row, int col) {
		long long number = 0;
		if (col < cols) {
			number = leaveFromTree(row, col);
			int from = 1;
			int push_node = find(root[0], row, from);
			root[0] = erase(root[0], node[push_node].key);
			root[row] = insert(root[row], push_node);
		
			int end_node = allocator.getNode();
			init(end_node, pushKeyBase + end_node, 1, number);
			root[0] = insert(root[0], end_node);
		}
		else if(col == cols){
			int from = 1;
			int push_node = find(root[0], row, from);
			number = node[push_node].number;
			root[0] = erase(root[0], node[push_node].key);
		
			int end_node = allocator.getNode();
			init(end_node, pushKeyBase + end_node, 1, number);
			root[0] = insert(root[0], end_node);
		}
		return number;
	}


	void outputData()
	{
		for (int r = 1; r <= rows; r++) {
			output(root[r]);
			int find_from = 1;
			int found = find(root[0], r, find_from);
			std::cout << node[found].number << std::endl;
		}
	}
};



Node nodes[NODE_COUNT];
int nodeId[NODE_COUNT];

Phalanx phalanx(nodes, nodeId, NODE_COUNT);


int main() {
	int rows = 0;
	int cols = 0;
	int events = 0;
#ifdef TEST
	rows = 5;
	cols = 5;
#else
	freopen("phalanx20.in", "r", stdin);
	freopen("phalanx.out", "w", stdout);
	scanf("%d%d%d", &rows, &cols, &events);
#endif

	phalanx.initialze(rows, cols);


#ifdef TEST
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(1, 5) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(1, 1) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(1, 2) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(1, 1) << endl;
	phalanx.outputData();
	
	cout << phalanx.leaveFromMatrix(1, 4) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(1, 3) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(2, 1) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(2, 1) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(2, 1) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(2, 1) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(2, 1) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(2, 5) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(3, 4) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(3, 4) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(3, 4) << endl;
	phalanx.outputData();

	cout << phalanx.leaveFromMatrix(3, 4) << endl;
	phalanx.outputData();
#else
	int r, c;
	for (int i = 1; i <= events; ++i)
	{
		scanf("%d%d", &r, &c);
		printf("%lld\n", phalanx.leaveFromMatrix(r, c));
	}
	fclose(stdin);
	fclose(stdout);
#endif
 	return 0;
}
