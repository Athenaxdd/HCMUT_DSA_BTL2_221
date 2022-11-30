#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"
class ConcatStringNode { //rope data structure https://www.geeksforgeeks.org/ropes-data-structure-fast-string-concatenation/
public:
    static int totalID;
    class ParentAVLNode{
    public:
        ConcatStringNode * parent;
        int parentID;
        int height;
        ParentAVLNode * left;
        ParentAVLNode * right;
        ParentAVLNode(){
            this -> parent = new ConcatStringNode();
            this -> height = 0;
            this -> parentID = 0;
            this -> left = nullptr;
            this -> right = nullptr;
        }
        ParentAVLNode(ConcatStringNode * parentNode){
            this -> parent = parentNode;
            this -> height = 1;
            this -> left = nullptr;
            this -> right = nullptr;
            this -> parentID = parentNode -> ID;
        }
        int updateHeight(ParentAVLNode * node){
            if (node == nullptr) return -1;
            return node -> height = 1 + max(updateHeight(node -> left), updateHeight(node -> right));
        }
        ParentAVLNode * turnRight(ParentAVLNode *a){
            ParentAVLNode * b = a -> left;
            ParentAVLNode * d = b -> right;
            a -> left = d;
            b -> right = a;
            return b;
        }
        ParentAVLNode * turnLeft(ParentAVLNode *a){
            ParentAVLNode * b = a -> right;
            ParentAVLNode * c = b -> left;
            a -> right = c;
            b -> left = a;
            return b;
        }
        int treeLevel(ParentAVLNode *t){
            if (t == NULL) return -1;
            return 1 + max(treeLevel(t -> left), treeLevel(t -> right));
        }
        ParentAVLNode * minValueNode(ParentAVLNode* node) { 
            ParentAVLNode * current = node; 
            while (current -> left)  current = current->left; 
            return current; 
        } 
        ParentAVLNode * updateAVL(ParentAVLNode * node){
            if (abs(treeLevel(node -> left) - treeLevel(node -> right)) > 1){
                if (treeLevel(node -> left) > treeLevel(node -> right)){
                    ParentAVLNode * p = node -> left;
                    if (treeLevel(p -> left) >= treeLevel(p -> right)){
                        node = turnRight(node);
                    } else{
                        node -> left = turnLeft(node->left);
                        node = turnRight(node);
                    }
                } else {
                    ParentAVLNode * p = node -> right;
                    if (treeLevel(p->right) >= treeLevel(p->left)){
                        node = turnLeft(node);
                    } else{
                        node -> right = turnRight(node -> right);
                        node = turnLeft(node);
                    }
                }	
            }
            if (node -> left) node->left = updateAVL(node->left);
            if (node -> right) node->right = updateAVL(node->right);
            return node;
        }
        ParentAVLNode * insertParentAVLNode(ConcatStringNode * otherN, ParentAVLNode * node){
            if (node == NULL){
                node = new ParentAVLNode(otherN);
            } else if (otherN -> ID <= node -> parentID){
                node -> left = insertParentAVLNode(otherN, node -> left);
            } else node -> right = insertParentAVLNode(otherN, node -> right);
            node = updateAVL(node);
            return node;
        }
        ParentAVLNode * deleteParentNode(ParentAVLNode * root, int key){
            if (!root) return nullptr;
            if (key > root -> parentID) root -> right = deleteParentNode(root -> right, key);
            else if (key < root -> parentID) root -> left = deleteParentNode(root -> left, key);
            else {
                if (!(root -> left) || !(root -> right)){
                    ParentAVLNode * temp = root -> left ? root -> left : root -> right;
                    if (temp) {
                        temp = root;
                        root = nullptr;
                    } else root = temp;
                    delete (temp);
                } else {
                    ParentAVLNode * temp = minValueNode(root -> left);
                    root -> parentID = temp -> parentID;
                    root -> left = deleteParentNode (root -> left, temp -> parentID);
                }
            }
            if (root == nullptr) return root;
            root -> height = updateHeight(root);
            root = updateAVL(root);
            return root;
        }
        ParentAVLNode * deleteParentNode(ParentAVLNode * root, ConcatStringNode * otherN){
            root = root -> deleteParentNode(root, (int)(otherN -> ID));
            return root;
        }
    };
    class ParentsTree{
    public:
        int parentTreeSize = 0;
        ParentAVLNode * parentRoot;
        ParentsTree(){
            parentTreeSize = 0;
            this -> parentRoot = nullptr;
        }
        ParentsTree(ConcatStringNode * otherN){
            parentTreeSize = 1;
            this -> parentRoot = new ParentAVLNode(otherN);
        }
        int size() const{
            return parentTreeSize;
        }
        void insertParentTreeNode(ConcatStringNode * otherN){
            this -> parentRoot = this -> parentRoot -> insertParentAVLNode(otherN, this -> parentRoot);
            ++parentTreeSize;
        }
        void deleteParentTreeNode(ConcatStringNode * otherN){
            this -> parentRoot = this -> parentRoot -> deleteParentNode(this -> parentRoot, otherN);
            parentTreeSize--;
        }
        string toStringPreOrderHelper(string s) const{ //very cool ss
            stringstream ans;
            ans << s;
            ans.seekp(-1, ans.cur);
            ans << "]";
            return ans.str();
        };
        string toStringPreOrder(ParentAVLNode * root) const{
            if (!root) return "";
            string s = "(id=" + to_string(root -> parentID) + ")" 
            + ";" + toStringPreOrder(root -> left) + toStringPreOrder(root -> right);
            return s;
        }
        string toStringPreOrder() const{
            string ans = "ParentsTree[";
            return toStringPreOrderHelper(ans + toStringPreOrder(parentRoot));
        }
    };
    string data;
    int length;
    ConcatStringNode * left;
    ConcatStringNode * right;
    int leftLength;
    int ID;
    int strlen(const char * s){
        int length = 0;
        while (s[length] != '\0'){
            length++;
        }
        return length;
    }
    ParentsTree parentTree;
    ConcatStringNode(){
        if (totalID + 1 > 10000000) throw std::overflow_error("Id is overflow");
        totalID += 1;
        this -> ID = totalID;
        this -> data = "";
        this -> length = length;
        this -> left = nullptr;
        this -> right = nullptr;
        this -> leftLength = 0;
        this -> parentTree = ParentsTree();
    };
    ConcatStringNode(const ConcatStringNode * otherN){
        if (totalID + 1 > 10000000) throw std::overflow_error("Id is overflow");
        totalID += 1;
        this -> ID = totalID;
        this -> data = otherN -> data;
        this -> length = otherN -> length;
        this -> left = nullptr;
        this -> right = nullptr;
        this -> parentTree = ParentsTree();
        if (otherN -> left){
            this -> left = new ConcatStringNode(otherN -> left);
            this -> left -> parentTree.insertParentTreeNode(this);
        } 
        if (otherN -> right){
            this -> right = new ConcatStringNode(otherN -> right);
            this -> right -> parentTree.insertParentTreeNode(this);
        }
        this -> leftLength = otherN -> leftLength;
    }
    ConcatStringNode(const char * s){
        if (totalID + 1 > 10000000) throw std::overflow_error("Id is overflow");
        totalID += 1;
        this -> ID = totalID;
        this -> data = s;
        this -> length = strlen(s);
        this -> leftLength = 0;
        this -> left = nullptr;
        this -> right = nullptr;
        this -> parentTree = ParentsTree();
    }
    int getDataLength(){
        return this -> length;
    }
    void setDataLength(int length){
        this -> length = length;
    }
    char getAtIndex(int index){
        if (left == nullptr && right == nullptr) return data[index];
        if (leftLength <= index) return this -> right -> getAtIndex(index - this -> leftLength);
        else return this -> left -> getAtIndex(index);
    }
    void reverseData(){
        int n = getDataLength();
        for (int i = 0; i < n / 2; i++) swap(data[i], data[n - i - 1]);
    }
    ~ConcatStringNode(){
        if (this -> parentTree.size() == 0){
        this -> data = "\0";
        this -> length = 0;
        this -> leftLength = 0;
        this -> ID = 0;
        ConcatStringNode* l = this -> left;
        ConcatStringNode* r = this -> right;
        if (this -> left && this -> right){
            this -> left -> parentTree.deleteParentTreeNode(this);
            this -> right -> parentTree.deleteParentTreeNode(this);
        } else if (this -> left && !this -> right) this -> left -> parentTree.deleteParentTreeNode(this);
        else if (this -> right && !this -> left) this -> right -> parentTree.deleteParentTreeNode(this);
        this -> left = nullptr;
        this -> right = nullptr;
        if (l && l -> parentTree.size() == 0) delete l;
        if (r && r -> parentTree.size() == 0) delete r;
        } else return;
        }
    }

};
class ConcatStringTree {
public:
    ConcatStringNode * root;
    ConcatStringTree(){
        root = nullptr;
    }
    ConcatStringTree(const char * s){
        root = new ConcatStringNode(s);
        root -> parentTree.insertParentTreeNode(root);
    };
    ConcatStringTree(const ConcatStringTree * otherT){
        this -> root = new ConcatStringNode(otherT -> root);
        root -> parentTree.insertParentTreeNode(root);
    }
    int length() const{
        return root -> length;
    };
    int height(ConcatStringNode * root){
        if (root == nullptr) return 0;
        int lh = height(root -> left);
        int rh = height(root -> right);
        if (lh > rh) {
            return lh+1;
        } else {
            return rh+1;
        }
    }
    char get(int index){
        if (index > root -> length|| index < 0) throw out_of_range("Index of string is invalid!");
        return root -> getAtIndex(index);
    };
    bool isCurrentLevel(ConcatStringNode* root, int level, int& checkedLen, char c)
    {
        if (root == nullptr) return false;
        if (level == 1) {
            if (root -> data != "") {
                for (int i = 0; i < root -> length; i++){
                    if (root -> data[i] == c) return true;
                    checkedLen++;
                }
            }
            else checkedLen += root -> length;
            return false;
        }
        else if (level > 1) {
            if (isCurrentLevel(root->left, level - 1, checkedLen, c)) return true;
            if (isCurrentLevel(root->right, level - 1, checkedLen, c)) return true;
        }
    };
    int indexOf(ConcatStringNode* root, char c){
        int h = height(root);
        int i;
        int checkedLen = 0;
        int minCheck = root -> length +1;
        bool check = false;
        for (i = 1; i <= h; i++) {
            check = isCurrentLevel(root, i,checkedLen, c);
            if (check) if (checkedLen < minCheck) minCheck = checkedLen;
            checkedLen = 0;
        }
        if (minCheck == root -> length +1) return -1;
        return minCheck;
    };
    int indexOf(char c){
        return indexOf(this -> root, c);
    };
    string toStringPreOrder(ConcatStringNode * root) const{
        if (!root) return "";
        string s = "(LL=" + to_string(root -> leftLength) + (string)",L=" + to_string(root -> getDataLength()) + ',' + (string)((root -> data == "") ? "<NULL>);" : '"' +root -> data + '"'  + ")" + ";")
            + toStringPreOrder(root -> left) + toStringPreOrder(root -> right);
        return s;
    }
    string toStringPreOrder() const{ //very cool ss
        stringstream ans;
        ans << "ConcatStringTree[" << toStringPreOrder(root) ;
        ans.seekp(-1, ans.cur);
        ans << "]";
        return ans.str();
    };
    string toString(ConcatStringNode * root) const{ //inOrder attempt
        if (root == nullptr) return "";
        return toString(root -> left) + root -> data + toString(root -> right);
    }
    string toString() const{
        string ans = "ConcatStringTree[";
        return ans + '"' + toString(root) + '"' + "]" ;
    };
    ConcatStringTree concat(const ConcatStringTree & otherS) const{ //uh assignment said do this 
        ConcatStringTree * ans = new ConcatStringTree();  
        ans -> root = new ConcatStringNode();
        ans -> root -> right = otherS.root;
        ans -> root -> left = this -> root;
        ans -> root -> leftLength = this -> root -> length;
        ans -> root -> length = this -> root -> length + otherS.root -> length;
        ans -> root -> parentTree.insertParentTreeNode(ans -> root);
        root -> parentTree.insertParentTreeNode(ans -> root);
        otherS.root -> parentTree.insertParentTreeNode(ans -> root);
        return * ans;
    };
    ConcatStringNode * trimTree(ConcatStringNode * root, int from, int to) const{ //Inspired by https://leetcode.com/problems/trim-a-binary-search-tree/
        ConcatStringNode * fromRight = root;
        int initialLength = root -> getDataLength() - to;
        int rightDeleted = 0;
        int totalLeftLength = fromRight -> leftLength;
        while (fromRight -> right || fromRight -> left){
            if (fromRight -> right == nullptr && fromRight -> left == nullptr){
                fromRight -> length -= (initialLength - rightDeleted);
                totalLeftLength -= (fromRight -> left -> length - fromRight -> left -> leftLength);
                fromRight -> leftLength -= (initialLength - rightDeleted);
                fromRight = fromRight -> left;
            } else if (totalLeftLength < to){
                fromRight -> length -= (initialLength - rightDeleted);
                fromRight = fromRight -> right;
                totalLeftLength += fromRight -> leftLength;
            } else{
                fromRight -> length -= (initialLength - rightDeleted);
                rightDeleted += fromRight -> right -> getDataLength();
                totalLeftLength -= (fromRight -> left -> length - fromRight -> left -> leftLength);
                fromRight -> leftLength -= (initialLength - rightDeleted);
                fromRight -> right = nullptr;
                fromRight = fromRight -> left;
            }
        }
        fromRight -> data = fromRight -> data.substr(0, abs(to - totalLeftLength));
        fromRight -> setDataLength(fromRight -> data.length());
        ConcatStringNode * fromLeft = root;
        int totalLeftDeleted = 0;
        while (fromLeft -> left || fromLeft -> right){
            if (fromLeft -> left == nullptr && fromLeft -> right == nullptr){
                fromLeft -> length -= (from - totalLeftDeleted);
                fromLeft = fromLeft -> left;
            } else if(fromLeft -> leftLength + totalLeftDeleted > from){
                fromLeft -> length -= (from - totalLeftDeleted);
                fromLeft -> leftLength -= (from - totalLeftDeleted);
                fromLeft = fromLeft -> left;
            } else {
                totalLeftDeleted += fromLeft -> leftLength;
                fromLeft -> length -= from;
                fromLeft -> left = nullptr;
                fromLeft -> leftLength = 0;
                fromLeft = fromLeft -> right;
            }
        }
        fromLeft -> data = fromLeft -> data.substr(from - totalLeftDeleted, fromLeft -> getDataLength());// - (from - totalLeftDeleted));
        //fromLeft -> length = fromLeft -> getDataLength();
        fromLeft -> setDataLength(fromLeft -> data.length());
        return root;
    }
    ConcatStringTree subString(int from, int to) const{
        if (from >= to) throw logic_error("Invalid range");
        if (from < 0 || to >= this -> root -> length) throw out_of_range("Index of string is invalid");
        ConcatStringTree * ans = new ConcatStringTree();
        ans -> root = new ConcatStringNode(this -> root);
        ans -> root -> parentTree.insertParentTreeNode(ans -> root);
        ans -> root = ans -> trimTree(ans -> root, from, to);
        return * ans;
    };
    ConcatStringNode* invert(ConcatStringNode * root) {
        if(!root) return nullptr;
        if (root -> left) invert(root -> left);
        if (root -> right) invert(root -> right);
        swap (root -> left, root -> right);
        if (root -> left) root -> leftLength = root -> left -> length;
        else root -> leftLength = 0;
        if (root ->data != "") root -> reverseData();
        return root;    
    }
    ConcatStringTree reverse() const{ //invert and reverse
        ConcatStringTree * ans = new ConcatStringTree(this);
        ans -> root -> parentTree.insertParentTreeNode(ans -> root);
        ans -> root = ans -> invert(ans -> root);
        return *ans;
    };

    int getParTreeSize(const string & query) const{
        ConcatStringNode * temp = this -> root;
        for (int i = 0; i < query.length(); ++i){
            if (query[i] == 'l') temp = temp -> left;
            else if (query[i] == 'r') temp = temp -> right;
            else throw std::runtime_error("Invalid character of query");
            if (temp == nullptr) throw runtime_error("Invalid query: reaching NULL");
        }
        return temp -> parentTree.size();
    };
    string getParTreeStringPreOrder(const string & query) const{
        ConcatStringNode * temp = this -> root;
        for (int i = 0; i < query.length(); ++i){
            if (query[i] == 'l') temp = temp -> left;
            else if (query[i] == 'r') temp = temp -> right;
            else throw std::runtime_error("Invalid character of query");
            if (temp == nullptr) throw runtime_error("Invalid query: reaching NULL");   
        }
        return temp -> parentTree.toStringPreOrder();
    };
    ~ConcatStringTree(){
        root -> parentTree.deleteParentTreeNode(root);
        ConcatStringNode * temp = this -> root;
        this -> root = nullptr;
        if (temp -> parentTree.size() == 0 ) delete temp;
    }
};

class ReducedConcatStringTree; // forward declaration
class LitStringHash; // forward declaration

class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;
public:
    HashConfig(){
        this -> p = 0;
        this -> c1 = 0;
        this -> c2 = 0;
        this -> lambda = 0;
        this -> alpha = 0;
        this -> initSize = 0;
    }
    HashConfig(int p, double c1, double c2, double lambda, double alpha, int initSize){
        this -> p = p;
        this -> c1 = c1;
        this -> c2 = c2;
        this -> lambda = lambda;
        this -> alpha = alpha;
        this -> initSize = initSize;
    }
    int getP() const{
        return p;
    }
    void setP(int p){
        this -> p = p;
    }
    double getC1() const{
        return c1;
    }
    void setC1(double c1){
        this -> c1 = c1;
    }
    double getC2() const{
        return c2;
    }
    void setC2(double c2){
        this -> c2 = c2;
    }
    double getLambda() const{
        return lambda;
    }
    void setLambda(double lambda){
        this -> lambda = lambda;
    }
    double getAlpha() const{
        return alpha;
    }
    void setAlpha(double alpha){
        this -> alpha = alpha;
    }
    int getInitSize() const{
        return initSize;
    }
    void setInitSize(int initSize){
        this -> initSize = initSize;
    }
    friend class ReducedConcatStringTree;
    friend class LitStringHash;
};

class LitStringHash {
public: 
    friend class HashConfig;
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;
    LitStringHash(const HashConfig & hashConfig){
        this -> p = hashConfig.getP();
        this -> c1 = hashConfig.getC1();
        this -> c2 = hashConfig.getC2();
        this -> lambda = hashConfig.getLambda();
        this -> alpha = hashConfig.getAlpha();
        this -> initSize = hashConfig.getInitSize();
    };
    int getLastInsertedIndex() const{
        return 0;
    };
    string toString() const{
        return "";
    };
};

class ReducedConcatStringTree  /* */ {
public:
    char * s;
    ReducedConcatStringTree(const char * s, LitStringHash * litStringHash){
        this -> litStringHash = litStringHash;
    };
    LitStringHash * litStringHash;
    string toString(){
        return "";
    }
};

#endif // __CONCAT_STRING_TREE_H__
