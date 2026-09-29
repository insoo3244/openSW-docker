// 2nd try
#include <iostream>
#include <string>

using namespace std;

class Node{
public:
    Node(int k, Node* p) : key(k), parent(p), left(nullptr), right(nullptr) {}
private:
    int key;
    Node *parent, *left, *right;

    friend class bTree;
};

class bTree{
public:
    bTree() : n(0), rnode(nullptr) { }
    ~bTree() { if(!empty()) { clear(rnode); } } 

    int size() { return n; }
    bool empty() { return size() == 0; }
    void clear(Node* node) {
        if(node->left != nullptr) { clear(node->left); }
        if(node->right != nullptr) { clear(node->right); }

        delete node;
    }

    Node* find(int k){
        Node* v = rnode;
        while(v != nullptr){
            if(k == v->key) { return v; }

            if(k < v->key) { v = v->left; }
            else { v = v->right; }
        }

        return nullptr;
    }

    bool insert(int k){
        if(empty()){
            rnode = new Node(k, nullptr);
            n++;
            return true;
        }

        Node* cur = rnode;
        Node* pn = nullptr;
        while(cur != nullptr){
            if(k == cur->key) { return false; }

            pn = cur;
            if(k < cur->key) { cur = cur->left; }
            else { cur = cur->right; }
        }

        // 여기 자꾸 헷갈림. cur가 pn의 왼&오른 자식을 따지지 말고,
        // k와 부모 key를 대소비교할 것 
        Node* v = new Node(k, pn);
        if(k < pn->key) { pn->left = v; }
        else { pn->right = v; }

        n++;
        return true;
    }

    // 구현 필요없는거 당장 구현 X 
    // 로직만 신경쓰기
    Node* getS(Node* node){
        Node* cur;
        if(node->right) {
            cur = node->right;
            while(cur->left) { cur = cur->left; }

            return cur;
        }

        cur = node;
        Node* pn = node->parent;
        while(pn && pn->right == node){
            cur = pn;
            pn = pn->parent;
        }

        return pn;
    }
    bool erase(int k){
        return erase(find(k));
    }
    bool erase(Node* node){
        if(node->left != nullptr && node->right != nullptr){
            Node* v = getS(node);

            node->key = v->key;
            node = v;
        }

        Node* cn = nullptr;
        if(node->left != nullptr) { cn = node->left; }
        else { cn = node->right; }

        if(node == rnode){
            rnode = cn;
            if(cn != nullptr) { cn->parent = nullptr; }
        }
        else{
            Node* pn = node->parent;
            if(cn != nullptr) { cn->parent = pn; }

            if(node == pn->left) { pn->left = cn; }
            else { pn->right = cn; }
        }

        delete node;
        n--;
        return true;
    }
    
    int cnt;
    int tSize(int k){
        cnt = 0;
        Node* v = find(k);
        if(v == nullptr) { return 0; }

        // 인자로 넘겨주는거 확실하게 신경쓰기
        post(v->left);
        return cnt;
    }
    void post(Node* node){
        if(node == nullptr) { return; }

        if(node->left != nullptr) { post(node->left); }
        if(node->right != nullptr) { post(node->right); }

        cnt++;
    }

private:
    int n;
    Node* rnode;
};

int main(){
    int n, m;
    cin >> n >> m;

    bTree tree;
    while(n--){
        int t1;
        cin >> t1;

        tree.insert(t1);
    }

    while(m--){
        int t1, t2;
        cin >> t1 >> t2;

        cout << tree.tSize(t1) + tree.tSize(t2) << '\n';
    }
}


// 1st try
// #include <iostream>
// #include <string>

// using namespace std;

// class Node{
// public:
//     Node(int k, string e, Node* p) : key(k), elem(e), parent(p), left(nullptr), right(nullptr) { } 
// private:
//     int key;
//     string elem;

//     Node* parent;
//     Node* left;
//     Node* right;

//     friend class bTree;
// };

// class bTree{
// public:
//     bTree() : n(0), rnode(nullptr){ }
//     ~bTree() { if(!empty()) { clear(rnode); } }

//     int size() { return n; }
//     bool empty() { return size() == 0; }
//     void clear(Node* node) {
//         if(node->left != nullptr) { clear(node->left); }
//         if(node->right != nullptr) { clear(node->right); } 

//         delete node;
//     }

//     Node* find_e(int k){
//         Node* cur = rnode;
//         while(cur != nullptr){
//             if(k == cur->key) { return cur; }

//             if(k < cur->key) { cur = cur->left; }
//             else { cur = cur->right; }
//         }

//         return nullptr;
//     }

//     bool insert(int k){
//         if(empty()){
//             rnode = new Node(k, "", nullptr);
//             n++;
//             return true;
//         }

//         Node* cur = rnode;
//         Node* pn = nullptr;
//         while(cur != nullptr){
//             if(k == cur->key) { return false; }
//             pn = cur;
            
//             if(k < cur->key) { cur = cur->left; }
//             else { cur = cur->right; }
//         }

//         Node* cn = new Node(k, "", pn);
//         if(k < pn->key) { pn->left = cn; }
//         else { pn->right = cn; }
        
//         n++;
//         return true;
//     }

//     int cnt;
//     int cntNode(int k){
//         cnt = 0;
//         Node* node = find_e(k);
//         if(node == nullptr){ return 0; }

//         if(node->left == nullptr) { return 0; }
//         post(node->left);

//         return cnt;
//     }

//     void post(Node* node){
//         if(node == nullptr) { return; }
//         if(node->left != nullptr) { post(node->left); }
//         if(node->right != nullptr) { post(node->right); }

//         cnt++;
//     }
// private:
//     int n;
//     Node* rnode;
// };

// int main(){
//     int n, m;
//     cin >> n >> m;

//     bTree tree;
//     while(n--){
//         int t1;
//         cin >> t1;

//         tree.insert(t1);
//     }

//     while(m--){
//         int t1, t2;
//         cin >> t1 >> t2;

//         cout << tree.cntNode(t1) + tree.cntNode(t2) << '\n';
//     }
// }