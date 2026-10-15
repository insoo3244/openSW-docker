#include<iostream>
#include<string>

using namespace std;

class node {
private:
	friend class BSTSet;
	int key;
	int height;
	node* parent;
	node* left;
	node* right;
public:
	node(int key, int height, node* parent)
		:key(key), height(height), parent(parent), left(nullptr), right(nullptr) {}
};

class BSTSet {
private:
	int n;
	node* root_node;
	void clear(node* Node);
	void update_height(node* Node);
	node* find_node(int key);
	void erase(node* Node);
	node* get_successor(node* Node);
	int height(node* Node);
	int subtree_size(node* Node);
	void inorder(node* Node);          // 중위 순회 (내부용)
public:
	BSTSet();
	~BSTSet();

	void height(int key);
	void subtree_size(int key);

	void insert(int key);
	void erase(int key);
	bool contains(int key);
	int size();                        // O(1)
	bool empty();                      // O(1)
	void print();                      // 중위 순회 출력 O(n)
};

BSTSet::BSTSet()
	:n(0), root_node(nullptr) {}
BSTSet::~BSTSet() {
	if (!empty()) {
		clear(root_node);
		n = 0;
	}
}

int BSTSet::height(node* Node) {
	if (Node == nullptr) {
		return 0;
	}
	int lefth = height(Node->left);
	int righth = height(Node->right);
	return (lefth > righth ? lefth : righth) + 1;
}
int BSTSet::subtree_size(node* Node) {
	if (Node == nullptr) { return 0; }
	return subtree_size(Node->left) + subtree_size(Node->right) + 1;
}
void BSTSet::height(int key) {
	node* target = find_node(key);
	if (target == nullptr) {
		cout << "invalid" << endl;
		return;
	}
	cout << target->height << endl;
}
void BSTSet::subtree_size(int key) {
	node* target = find_node(key);
	if (target == nullptr) {
		cout << "invalid" << endl;
		return;
	}
	cout << subtree_size(target) << endl;
}
void BSTSet::clear(node* Node) {
	if (Node == nullptr) { return; }
	clear(Node->left);
	clear(Node->right);
	delete Node;
}
node* BSTSet::find_node(int key) {
	node* trv = root_node;
	while (trv != nullptr) {
		if (key == trv->key) {
			return trv;
		}
		if (key < trv->key) {
			trv = trv->left;
		}
		else {
			trv = trv->right;
		}
	}
	return nullptr;
}
void BSTSet::erase(node* Node) {
	if (Node == nullptr) { cout << "invalid" << endl; return; }
	n--;
	if (Node->left != nullptr && Node->right != nullptr) {
		node* successor = get_successor(Node);
		Node->key = successor->key;        // value가 없으므로 key만 복사
		Node = successor;
	}
	node* child;
	if (Node->left != nullptr) {
		child = Node->left;
	}
	else {
		child = Node->right;
	}
	if (Node == root_node) {
		root_node = child;
		if (child != nullptr) {
			child->parent = nullptr;
		}
	}
	else {
		node* par = Node->parent;
		if (child != nullptr) {
			child->parent = par;
		}
		if (Node == par->left) {
			par->left = child;
		}
		else {
			par->right = child;
		}
	}
	update_height(Node->parent);           // 삭제 후 높이 갱신 (버그 수정)
	delete Node;
}
node* BSTSet::get_successor(node* Node) {
	if (Node->right) {
		node* trv = Node->right;
		while (trv->left) {
			trv = trv->left;
		}
		return trv;
	}
	node* trv = Node;
	node* Par = Node->parent;
	while (Par && trv == Par->right) {
		trv = Par;
		Par = Par->parent;
	}
	return Par;
}
void BSTSet::insert(int key) {
	if (empty()) {
		root_node = new node(key, 1, nullptr);
		n++;
		return;
	}
	node* trv = root_node;
	node* par = nullptr;
	while (trv != nullptr) {
		if (trv->key == key) {
			return;                        // Set이므로 중복 무시
		}
		par = trv;
		if (key < trv->key) {
			trv = trv->left;
		}
		else {
			trv = trv->right;
		}
	}
	node* new_node = new node(key, 1, par);
	if (key < par->key) {
		par->left = new_node;
	}
	else {
		par->right = new_node;
	}
	update_height(par);
	++n;
}
void BSTSet::update_height(node* Node) {
	while (Node != nullptr) {
		int lefth = 0;
		int righth = 0;
		if (Node->left != nullptr) {
			lefth = Node->left->height;
		}
		if (Node->right != nullptr) {
			righth = Node->right->height;
		}
		Node->height = (lefth > righth ? lefth : righth) + 1;
		int sub = lefth - righth;
		if (sub < 0) {
			sub *= -1;
		}
		if (sub >= 2) {
			/*회전 함수 바로 호출후 넘어가기*/
		}
		Node = Node->parent;
	}
}
void BSTSet::erase(int key) {
	erase(find_node(key));
}
bool BSTSet::contains(int key) {
	return find_node(key) != nullptr;
}
int BSTSet::size() {
	return n;                              // n을 계속 관리하므로 O(1)
}
bool BSTSet::empty() {
	return n == 0;                         // O(1)
}
void BSTSet::inorder(node* Node) {
	if (Node == nullptr) { return; }
	inorder(Node->left);                   // 1. 왼쪽 (작은 값)
	cout << Node->key << " ";              // 2. 자기 자신
	inorder(Node->right);                  // 3. 오른쪽 (큰 값)
}
void BSTSet::print() {
	inorder(root_node);
	cout << endl;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int N;
	int a;
	string name;
	cin >> N;
	while (N--) {
		cin >> a;
		BSTSet s;
		while (a--) {
			cin >> name;
			if (name == "insert") {
				int q;
				cin >> q;
				s.insert(q);
			}
			else if (name == "erase") {
				int q;
				cin >> q;
				s.erase(q);
			}
			else if (name == "check") {
				int q;
				cin >> q;
				cout << (s.contains(q) ? 1 : 0) << endl;
			}
			else if (name == "empty") {
				cout << (s.empty() ? 1 : 0) << endl;
			}
			else if (name == "size") {
				cout << s.size() << endl;
			}
			else if (name == "print") {
				s.print();
			}
			else if (name == "height") {
				int q;
				cin >> q;
				s.height(q);
			}
			else if (name == "subtree_size") {
				int q;
				cin >> q;
				s.subtree_size(q);
			}
		}
	}
}