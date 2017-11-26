struct TreeNode
{
	int x = 0;
	int y = 0;
	int r = 0;
	int parent = -1;
	int firstChild = -1;
	int prev = -1;
	int next = -1;
};

TreeNode nodes[200];
int nodeCounter = 1;

bool isInside(int first, int second) {
	///nodes[first].x....
	return true;
}

int addNode(int parent, int new_index)
{
	if (isInside(new_index, parent))
	{	
		int child = nodes[parent].firstChild;
		while (child != -1) {
			int ret = addNode(child, new_index);
			if (ret != -1) {
				return parent;
			}
			if (nodes[child].next == -1) {
				nodes[child].next = new_index;
				nodes[new_index].prev = child;
				nodes[new_index].parent = parent;
				break;
			}
			else {
				child = nodes[child].next;
			}
		}

		return parent;

	}
	else if (isInside(parent, new_index))
	{
		nodes[new_index].firstChild = parent;
		nodes[new_index].parent = nodes[parent].parent;
		nodes[new_index].prev = nodes[parent].prev;
		nodes[new_index].next = nodes[parent].next;
		if (nodes[parent].prev != -1) {
			nodes[nodes[parent].prev].next = new_index;
		}
		if (nodes[parent].next != -1) {
			nodes[nodes[parent].next].prev = new_index;
		}
		nodes[parent].parent = new_index;
		nodes[parent].prev = -1;
		nodes[parent].next = -1;
		nodes[parent].firstChild = -1;
		return new_index;
	}
	else {
		return -1;
	}
}


int main1()
{
	nodes[1].x = 100;
	nodes[1].y = 100;
	nodes[1].r = 100;
	nodeCounter++;

	int new_index = nodeCounter++;
	nodes[new_index].x = 1;
	nodes[new_index].y = 2;
	nodes[new_index].r = 3;

	addNode(1, new_index);

	return 0;
}