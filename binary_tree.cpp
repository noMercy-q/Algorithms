#include <iostream>
#include <assert.h>
#include <queue>
#include <stack>

template <class T>
bool isLessDefault(const T& first, const T& second) {
	return first < second;
}

template <class T>
void printDefault(const T& elem) {
	std::cout << elem << ' ';
}

template <class T>
class Tree {
public:
	Tree(bool (*IsLess)(const T&, const T&) = isLessDefault) { isLess = IsLess; };
	Tree(const Tree&) = delete;
	~Tree();
	Tree& operator= (const Tree&) = delete;

	void Add(const T& el);
	void preOrder(void (*manipulate)(const T& elem)); // чтобы не печатать из метода,
													// решил передавать в метод обхода нужный обработчик

private:
	struct Node {
		Node(const T& element) : data(element) {};

		T data;
		Node* left = nullptr;
		Node* right = nullptr;
	};

	Node* root = nullptr;
	bool (*isLess)(const T&, const T&);
};

template <class T>
void Tree<T>::Add(const T& el) {

	Node* cur = root;
	if (!cur) {
		root = new Node(el);
		return;
	}

	while (true) {
		if (isLess(el, cur->data)) {
			if (cur->left)
				cur = cur->left;
			else {
				cur->left = new Node(el);
				return;
			}
		}
		else {
			if (cur->right)
				cur = cur->right;
			else {
				cur->right = new Node(el);
				return;
			}
		}
	}
}

template <class T>
Tree<T>::~Tree() {
	if (!root)
		return;

	std::queue<Node*> q;
	q.push(root);

	while (!q.empty()) {
		Node* cur = q.front();
		q.pop();

		if (cur->left)
			q.push(cur->left);
		if (cur->right)
			q.push(cur->right);

		//std::cout << "Deleted " << cur->data << std::endl;
		delete cur;
	}
}


template <class T>
void Tree<T>::preOrder(void (*manipulate)(const T& elem)) {
	if (!root)
		return;

	Node* cur = root;
	std::stack<Node*> st;

	while (cur || !st.empty()) {
		while (cur) {
			st.push(cur);
			manipulate(cur->data);
			cur = cur->left;
		}
		if (!st.empty()) {
			cur = st.top();
			st.pop();
			cur = cur->right;
		}
	}
}

int main()
{
	Tree<int> tree;

	int n = 0;
	std::cin >> n;

	int val = 0;
	for (int i = 0; i < n; i++) {
		std::cin >> val;
		tree.Add(val);
	}

	tree.preOrder(printDefault);

	return 0;
}