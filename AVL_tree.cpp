#include <iostream>
#include <assert.h>

template <class T>
bool isLessDefault(const T& first, const T& second) {
	return first < second;
}

template <class T>
struct Node {
	T key;
	unsigned char height = 1;
	Node* left = 0;
	Node* right = 0;
	int nodes = 1;

	Node(const T& k) : key(k) {};
};

template <class T>
class AVLTree {
private:

	bool (*isLess)(const T&, const T&);
	Node<T>* root = nullptr;
	
	unsigned char height(Node<T>* p) { return p ? p->height : 0; };
	int getCount(Node<T>* p) { return p ? p->nodes : 0; };

	void fixHeight(Node<T>* p);
	void nodesCount(Node<T>* p);

	int bfactor(Node<T>* p) { return height(p->right) - height(p->left); };
	Node<T>* balance(Node<T>* p);
	Node<T>* rotateRight(Node<T>* p);
	Node<T>* rotateLeft(Node<T>* p);

	void deleteTree(Node<T>* p);
	Node<T>* del(Node<T>* p, T k);
	Node<T>* insert(Node<T>* p, T k);
	Node<T>* findMin(Node<T>* p) { return p->left ? findMin(p->left) : p; }
	Node<T>* removeMin(Node<T>* p);

	Node<T>* findRemoveMin(Node<T>* p, Node<T>*& min);
	const T& kStat(int K, Node<T>* p);

public:

	AVLTree(bool (*IsLess)(const T&, const T&) = isLessDefault) { isLess = IsLess; };
	AVLTree(const AVLTree&) = delete;
	~AVLTree() { deleteTree(root); };
	AVLTree operator= (const AVLTree&) = delete;


	void addKey(T k) { root = insert(root, k); };
	void delKey(T k) { root = del(root, k); };

	/*void printTree(Node<T>* p) {
		if (p->left)
			printTree(p->left);
		
		std::cout << p->key << " nodes:" << p->nodes << std::endl;
		if (p->right)
			printTree(p->right);
	}*/
	/*void print() {
		printTree(root);
	}*/

	const T& getKStat(int K) { return kStat(K, root); };
};

template <class T>
void AVLTree<T>::deleteTree(Node<T>* p) {
	if (p) {
		deleteTree(p->left);
		deleteTree(p->right);
		//std::cout << "Deleted " << p->key << std::endl;
		delete p;
	}
}

template <class T>
void AVLTree<T>:: fixHeight(Node<T>* p) {
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

template <class T>
void AVLTree<T>::nodesCount(Node<T>* p) {
	int cl = getCount(p->left);
	int cr = getCount(p->right);
	p -> nodes = (cl + cr + 1);
}

template <class T>
Node<T>* AVLTree<T>:: balance(Node<T>* p) {
	fixHeight(p);
	nodesCount(p);

	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateRight(p->right);
		return rotateLeft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateLeft(p->left);
		return rotateRight(p);
	}
	return p;
}

template <class T>
Node<T>* AVLTree<T>::rotateRight(Node<T>* p) {
	Node<T>* q = p->left;
	p->left = q->right;
	q->right = p;

	fixHeight(p);
	nodesCount(p);

	fixHeight(q);
	nodesCount(q);

	return q;
}

template <class T>
Node<T>* AVLTree<T>::rotateLeft(Node<T>* q) {
	Node<T>* p = q->right;
	q->right = p->left;
	p->left = q;
	fixHeight(q);
	nodesCount(q);

	fixHeight(p);
	nodesCount(p);

	return p;
}

template <class T>
Node<T>* AVLTree<T>::insert(Node<T>* p, T k) {
	if (!p)
		return new Node<T>(k);

	if (isLess(k, p->key))
		p->left = insert(p->left, k);
	else
		p->right = insert(p->right, k);
	return balance(p);
}

template <class T>
Node<T>* AVLTree<T>::removeMin(Node<T>* p) {
	if (p->left == 0)
		return p->right;
	p->left = removeMin(p->left);
	return balance(p);
}

template <class T>
Node<T>* AVLTree<T>::del(Node<T>* p, T k) {
	if (!p)
		return nullptr;

	if (isLess(k, p->key))
		p->left = del(p->left, k);
	else if (isLess(p->key, k))
		p->right = del(p->right, k);
	else //  k == p->key 
	{
		Node<T>* q = p->left;
		Node<T>* r = p->right;
		//std::cout << p->key << 
		//" deleted with del" << std::endl;
		delete p;
		if (!r) return q;
		//Node<T>* min = findMin(r);
		//min->right = removeMin(r);
		Node<T>* min = nullptr;
		findRemoveMin(r, min);  // возвращаемое значение игнорирую: 
								// внутри метода min уже нашелся и встал на место
		min->left = q;
		return balance(min);
	}

	return balance(p);
}



template <class T>
Node<T>* AVLTree<T>::findRemoveMin(Node<T>*p, Node<T>*&min) {
	if (!p->left) {
		min = p;
		return p->right;
	}
	p->left = findRemoveMin(p->left, min);
	min->right = balance(p); // уже спустились до самого левого элемента, а значит min != NULL
	return min->right;
}

template <class T>
const T& AVLTree<T>::kStat(int K, Node<T>* p) {
	if (!p)
		assert("Bad K value, not enough elements!" && false);

	int leftCount = getCount(p->left);
	if (K == leftCount)
		return p->key;
	if (K < leftCount)
		return kStat(K, p->left);
	else
		return kStat(K - leftCount - 1, p->right);
}


int main()
{
	AVLTree<int> tree;

	int n = 0;
	int val = 0;
	int stat = 0;

	std::cin >> n;
	for (int i = 0; i < n; i++) {
		std::cin >> val >> stat;
		if (val < 0)
			tree.delKey(abs(val));
		else
			tree.addKey(val);

		int res = tree.getKStat(stat);
		std::cout << res << std::endl;
	}

	return 0;
}