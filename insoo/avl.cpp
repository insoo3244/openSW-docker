#include <iostream>
#include <string>

using namespace std;

class Node{
public:
    Node(int e, Node* p, int h) 
    : elem(e), parent(p), height(h), left(nullptr), right(nullptr) {}
private:
    int elem;
    Node* parent;
    int height;

    Node* left;
    Node* right;

    friend class AVL;
};

class AVL{
public:
    // 생성자 및 소멸자
    AVL() : n(0), rnode(nullptr){}
    ~AVL() { if(!empty()) clear(rnode); }

    // 크기 반환 및 비어있는가 여부
    int size() { return n; }
    bool empty() { return size() == 0; }

    // 완전 비우기 함수
    void clear(Node* node){
        if(node->left) { clear(node->left); }
        if(node->right) { clear(node->right); }

        delete node;
    }
    
    // 찾기 보조함수 및 찾기 함수
    Node* find_elem(int k){
        Node* v = rnode;

        while(v){
            if(k == v->elem) { return v; }

            if(k < v->elem) { v = v->left; }
            else { v = v->right; }
        }

        return nullptr;
    }
    int find_e(int k){
        Node* node = find_elem(k);
        if(!node) return 0;

        return node->elem;
    }

    // 삽입 (중복 원소 허용X)
    bool insert(int k){
        if(empty()) { // 아무 원소도 없을 때
            rnode = new Node(k, nullptr, 1);
            n++;
            return true;
        }

        // 루트노드부터 삽입할 위치 탐색
        Node* node = rnode;
        Node* pn = nullptr;
        while(node){
            if(k == node->elem) { return false; } // 중복 원소 허용 X

            pn = node;
            if(k < node->elem) { node = node->left; }
            else { node = node->right; }
        }

        // 새 노드 생성 및 삽입
        Node* v = new Node(k, pn, 1);
        if(k < pn->elem) { pn->left = v; }
        else { pn->right = v; }

        n++;
        return true;
    }

    // 후행자 찾기 함수
    Node* getSuccessor(Node* v){
        if(v->right){
            Node* temp = v->right;
            while(temp->left) { temp = temp->left; }

            return temp;
        }
        Node* cur = v;
        Node* pn = v->parent;
        while(pn && cur == pn->right){
            cur = pn;
            pn = cur->parent;
        }
        return pn;
    }
    // 삭제
    bool erase(int k){
        return erase(find_elem(k));
    }
    // 삭제보조함수
    bool erase(Node* node){
        if(!node) { return false; }

        // 왼오른 자식이 다 있을 경우 : node에 후행자 정보 복사
        if(node->left && node->right){
            Node* v = getSuccessor(node);

            node->elem = v->elem;
            node->height = v->height;
            node = v;
        }

        // 자식 노드 cn, 왼쪽부터 고르기
        Node* cn = nullptr;
        if(node->left) { cn = node->left; }
        else { cn = node->right; }

        // 내가 제거할 노드가 루트노드라면 : 자식을 루트노드로 포인터 갱신
        if(node == rnode){
            rnode = cn;
            if(cn) { cn->parent = nullptr; }
        } 
        // 아닌 경우 : 자식과 부모의 대소비교에 의해 왼오른자식 결정
        else{
            Node* pn = node->parent;
            if(cn) { cn->parent = pn; }

            if(node == pn->left) { pn->left = cn; }
            else { pn->right = cn; }
        }

        delete node;
        n--;
        return true;
    }

    // 출력과 중위순회 함수
    void print(){
        if(empty()) { return; }
        in(rnode);
        cout << '\n';
    }
    void in(Node* node){
        if(node == nullptr) { return; }
        if(node->left) { in(node->left); }

        cout << node->elem << ' ';

        if(node->right) { in(node->right); }
    }

private:
    int n;
    Node* rnode;
    
};

int main(){
    int n;
    string cmd;
    cin >> n;

    AVL avl;
    while(n--){
        cin >> cmd;

        if(cmd == "insert"){
            int t; cin >> t;

            avl.insert(t);
        }
        if(cmd == "erase"){
            int t; cin >> t;
            
            avl.erase(t);
        }
        if(cmd == "print"){
            avl.print();
        }
    }
}