#include <iostream>
#include <assert.h>
#include <vector>
#include <queue>

template <class T>
bool isLessDefault(const T& first, const T& second) {
	return first < second;
}

template <class T>
void defaultPrint(const T& elem) {
	std::cout << elem << ' ';
}

template <class T>
class BTree {
public:
	BTree(size_t _t, bool (*IsLess)(const T&, const T&) = isLessDefault) : t(_t), isLess(IsLess) { assert(_t >= 2); };
	BTree(const BTree&) = delete;
	~BTree() { delete root; }
	BTree operator= (const BTree&) = delete;

	void Insert(const T& key);
	bool Find(const T& key) { return search(key, root); };
	void BFS(void(*manipulate)(const T&), void(*doSmth)());

private:
	struct Node {
		bool IsLeaf;
		std::vector<T> Keys;
		std::vector<Node*> Children;

		explicit Node(bool isLeaf) : IsLeaf(isLeaf) {}
		~Node() { for (Node* child : Children) { delete child; } }
	};

	Node* root = nullptr;
	size_t t;
	bool (*isLess)(const T&, const T&);

	void splitChild(Node* p, int pos);
	void insertNonFull(Node* node, const T& key);
	bool isFull(Node* node) { return node->Keys.size() == 2 * t - 1; };
	bool search(const T& key, Node* node);
};

template <class T>
bool BTree<T>::search(const T& key, Node* node) {
	int i = 0;
	while (i < node->Keys.size() && isLess(node->Keys[i], key))
		i++;
	if (i < node->Keys.size() && key == node->Keys[i])
		return true;

	if (node->IsLeaf)
		return false;

	return search(key, node->Children[i]);
}


template <class T>
void BTree<T>::splitChild(Node* X, int pos) {
	Node* Y = X->Children[pos];
	Node* Z = new Node(Y->IsLeaf); 
	T medianKey = Y->Keys[t-1];

	Z->Keys.insert(Z->Keys.begin(), Y->Keys.begin() + t, Y->Keys.end());
	Y->Keys.erase(Y->Keys.begin() + t - 1, Y->Keys.end());

	if (!Y->IsLeaf) {
		Z->Children.insert(Z->Children.begin(), Y->Children.begin() + t, Y->Children.end());
		Y->Children.erase(Y->Children.begin() + t, Y->Children.end());
	}

	X->Children.insert(X->Children.begin() + pos + 1, Z);
	X->Keys.insert(X->Keys.begin() + pos, medianKey);
}

template <class T>
void BTree<T>::Insert(const T& key) {
	if (!root) {
		root = new Node(true);
		root->Keys.push_back(key);
		return;
	}

	if (isFull(root)) {
		Node* newRoot = new Node(false);
		newRoot->Children.push_back(root);
		root = newRoot;
		splitChild(root, 0);
	}

	insertNonFull(root, key);
}

template <class T>
void BTree<T>::insertNonFull(Node* node, const T& key) {
	int pos = node->Keys.size() - 1;
	if (node->IsLeaf) {
		node->Keys.resize(node->Keys.size() + 1);
		while (pos >= 0 && isLess(key, node->Keys[pos])) {
			node->Keys[pos + 1] = node->Keys[pos];
			pos--;
		}
		node->Keys[pos + 1] = key;
	}
	else {
		while (pos >= 0 && isLess(key, node->Keys[pos])) {
			--pos;
		}
		if (isFull(node->Children[pos + 1])) {
			splitChild(node, pos + 1);
			if (isLess(node->Keys[pos + 1], key)) {
				++pos;
			}
		}
		insertNonFull(node->Children[pos + 1], key);
	}
}

template <class T>
void BTree<T>::BFS(void(*manipulate)(const T&), void(*doSmth)()) {
	if (!root)
		return;

	std::queue <Node*> q;
	q.push(root);
	int childNodes = 1;
	int newChiledNodes = 0;

	while (!q.empty()) {
		Node* node = q.front();

		childNodes--;
		newChiledNodes += node->Children.size();

		q.pop();
		for (int i = 0; i < node->Keys.size(); i++)
			manipulate(node->Keys[i]);

		for (int i = 0; i < node->Children.size(); i++)
			q.push(node->Children[i]);

		if (childNodes <= 0) {
			doSmth();			// хотелось оставить обход внутри класса, 
								// поэтому пришлось выкручиватся как-то так...
			childNodes = newChiledNodes;
			newChiledNodes = 0;
		}
	}
};

void doEndl() {
	std::cout << std::endl;
}


int main()
{
	int n = 2;
	std::cin >> n;
	BTree<int> tree(n);
	
	int val = 0;
	while (std::cin >> val)
		tree.Insert(val);

	//if (tree.Find(5))
	//	std::cout << "FOUND" << std::endl;
	//else
	//	std::cout << "NOT FOUND" << std::endl;

	tree.BFS(defaultPrint, doEndl);

	return 0;
}