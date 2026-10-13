#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;
class node {
private:
   friend class BSTMap;

   // key-value? key x와 노드 x가 서로 일치하게 설계하는 것 아님?
   int key;
   int value;
   int height;

   node* parent;
   node* left;
   node* right;
public:
   node(int key, int value, int height, node*parent)
      :key(key), value(value), height(height), parent(parent), left(nullptr), right(nullptr){ }
};

class BSTMap {
private:
   int n; 
   node* root_node;

   void clear(node* Node);
   void update_height(node* Node); // 높이 갱신
   node* find_node(int key);

   // 아래 max, min 함수는 출력을 겸함
   void find_Max(int x); // 최대 key 찾기
   void find_Min(int x); // 최소 key 찾기

   void erase(node* Node);
   node* get_successor(node* Node);

   int height(node* Node);
   int subtree_size(node* Node);
public:
   BSTMap();
   ~BSTMap();

   void height(int key);
   void subtree_size(int key);

   void insert(int key, int value);
   void erase(int key);
   int check(int key); // = find()
   int num();
   bool empty();
};

BSTMap::BSTMap()
   :n(0), root_node(nullptr){ }
BSTMap::~BSTMap() {
   if (!empty()) {
      clear(root_node);
      n = 0; // 일단?
   }
}

int BSTMap::height(node* Node) {
   int max = 0;
   if (Node == nullptr) {
      return 0;
   }

   // 왼/오 서브트리 높이 비교
   int lefth = height(Node->left);
   int righth = height(Node->right);
   if (max < lefth) {
      max = lefth;
   }
   if (lefth < righth) {
      max = righth;
   }

   return max + 1;
   // return (lefth < righth ? righth + 1 : lefth + 1);
}

int BSTMap::subtree_size(node* Node) {
   if (Node == nullptr) { return 0; }

   return subtree_size(Node->left) + subtree_size(Node->right) + 1; // 자식 서브트리 + 본인 노드 = 서브트리 크기
}

void BSTMap::height(int key) {
   node* target = find_node(key);
   if (target == nullptr) {
      cout << "invalid" << endl;
      return;
   }// 검증용

   cout << target->height << height(target) << endl; // ?
   // cout << target->height << " " << height(target) << endl; 이거 물어보기 : 타겟의 높이와 타겟의 높이 출력?
}

void BSTMap::subtree_size(int key) {
   node* target = find_node(key);
   if (target == nullptr) {
      cout << "invalid" << endl;
      return;
   }

   cout << subtree_size(target) << endl;
}

void BSTMap::clear(node* Node) {
   if (Node == nullptr) { return; }
   if (Node->left != nullptr) { clear(Node->left); }
   if (Node->right != nullptr) { clear(Node->right); }
   delete Node;
}

node* BSTMap::find_node(int key) {
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

void BSTMap::erase(node* Node) {
   if (Node == nullptr) { cout << "invalid" << endl;return; }
   n--;

   // 후행자 정보 복사
   if (Node->left != nullptr && Node->right != nullptr) {
      node* successor = get_successor(Node);
      Node->key = successor->key;
      Node->value = successor->value;
      Node = successor;
   }

   // 유효한 자식으로 초기화
   node* child; // node* child = (Node->left ? Node->left : Node->right);
   if (Node->left != nullptr) {
      child = Node->left;
   }
   else {
      child = Node->right;
   }

   // 삭제할 노드가 루트노드라면 ?
   if (Node == root_node) {
      root_node = child;
      if (child != nullptr) {
         child->parent = nullptr;
      }
   }
   // 일반 노드라면 ?
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
   delete Node;
}
node* BSTMap::get_successor(node* Node) {
   if (Node->right) {
      node* trv = Node->right;
      while (trv->left) {
         trv = trv->left;
      }
      return trv;
   }
   else {
      node* trv = Node;
      node* Par = Node->parent;
      while (Par && trv == Par->right) {
         Par = Par->parent;
         trv = trv->parent;
      }
      return Par;
   }
   /*코드블락이지만 로직 일관성을 위해 작성 <- ?? 이거도 물어보기 */
}

void BSTMap::insert(int key, int value) {
   if (empty()) {
      root_node = new node(key, value, 1, nullptr);
      n++;
      return;
   }

   // 삽입할 자리 탐색
   node* trv = root_node;
   node* par = nullptr;
   while (trv != nullptr) {
      if (trv->key == key) {
         return;
      }
      par = trv;
      if (key < trv->key) {
         trv = trv->left;
      
      }
      else {
         trv = trv->right;
         
      }
   }

   node* new_node = new node(key, value,1, par);
   if (new_node->key < par->key) {
      par->left = new_node;
   }
   else {
      par->right = new_node;
   }

   // 삽입 노드의 부모 노드 높이 갱신
   update_height(par);
   ++n;
}
void BSTMap::update_height(node* Node) {
   while (Node != nullptr) {
      int lefth = 0;
      int righth = 0;
      // int lefth = (Node->left ? : Node->left->height : 0);
      // int right = (Node->right ? : Node->right->height : 0);

      if (Node->left != nullptr) {
         lefth = Node->left->height;
      }/*nullptr역참조 방지로직*/

      if (Node->right != nullptr) {
         righth = Node->right->height;
      }/*여기도 마찬가지로*/

      if (lefth > righth) {
         Node->height = lefth + 1;
      }
      else {
         Node->height = righth + 1;
      }

      // 높이 차 판단 로직 (-1, 0, 1이 아닐 경우)
      int sub = lefth - righth;
      if (sub < 0) { sub *= -1; /*절댓값 작업*/ }
      if (sub >= 2) {
         /*회전 함수 바로 호출후 넘어가기*/
      }
      Node = Node->parent; // ? 
   }
}

void BSTMap::erase(int key) {
   node* target = find_node(key);
   erase(target);
}

int BSTMap::check(int key) { // find() : value 반환 중
   node* target = find_node(key);
   if (target == nullptr) {
      return 0;
   }
   return target->value;
}
int BSTMap::num() { // size()
   return n;
}
bool BSTMap::empty() {
   return num() == 0;
}

void BSTMap::find_Max(int x){
   node* Node = find_node(x);
   if(!Node) { return -1; }

   while(!Node->right){
      Node = Node->right;
   }
   
   // key와 높이를 같이 출력
   cout << Node->key << ' ' << Node->height;
}
void BSTMap::find_Min(int x){
   node* Node = find_node(x);
   if(!Node) { return -1; }

   while(!Node->left){
      Node = Node->left;
   }
   
   // key와 높이를 같이 출력
   cout << Node->key << ' ' << Node->height;
}

int main() {
   int N;/*테스트케이스 개수*/
   int a;/*명령어 개수*/
   string name;
   cin >> N;
   while (N--) {
      cin >> a;
      BSTMap s;
      while (a--) {
         cin >> name;
         if (name == "insert") {
            int q, w;
            cin >> q >> w;
            s.insert(q, w);
         }
         else if (name == "erase") {
            int q;
            cin >> q;
            s.erase(q);
         }
         else if (name == "check") {
            int q;
            cin >> q;
            cout << s.check(q) << endl;
         }
         else if (name == "num") {
            cout << s.num() << endl;
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


// 삽입부 로직
void BSTMap::insert(int key, int value) {
   if (empty()) {
      root_node = new node(key, value,1, nullptr);
      n++;
      return;
   }
   node* trv = root_node;
   node* par = nullptr;
   while (trv != nullptr) {
      if (trv->key == key) {
         return;
      }
      par = trv;
      if (key < trv->key) {
         trv = trv->left;
      
      }
      else {
         trv = trv->right;
         
      }
   }
   node* new_node = new node(key, value,1, par);
   if (new_node->key < par->key) {
      par->left = new_node;
   }
   else {
      par->right = new_node;
   }
   update_height(par);
   ++n;
}
void BSTMap::update_height(node* Node) {
   while (Node!= nullptr) {
      int lefth = 0;
      int righth = 0;
      if (Node->left != nullptr) {
         lefth = Node->left->height;
      }/*nullptr역참조 방지로직*/
      if (Node->right != nullptr) {
         righth = Node->right->height;
      }/*여기도 마찬가지로*/
      if (lefth > righth) {
         Node->height = lefth + 1;
      }
      else {
         Node->height = righth + 1;
      }
      int sub = lefth - righth;
      if (sub < 0) {
         sub *= -1; /*높이 차가 음수일경우 양수로 보정*/
      }
      if (sub >= 2) {
         /*회전 함수 바로 호출후 넘어가기*/
      }
      Node = Node->parent;
   }
} 
// 오늘 완성한 삽입부입니다!!회전 로직만 넣으면 될거같습니다