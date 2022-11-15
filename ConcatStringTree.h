#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"
class ConcatStringNode { //rope data structure https://www.geeksforgeeks.org/ropes-data-structure-fast-string-concatenation/
public:
    string data;
    int length;
    ConcatStringNode * left;
    ConcatStringNode * right;
    int leftLength;
    int ID;
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
        string toStringPreOrder(ParentAVLNode * root) const{
            if (!root) return "";
            string s = "(id=" + to_string(root -> parentID) + ")" 
            + ";" + toStringPreOrder(root -> left) + ";" + toStringPreOrder(root -> right);
        return s;
        }
        string toStringPreOrder() const{
            string ans = "ParentsTree[";
            return ans + toStringPreOrder(parentRoot) + "]";
        }
    };
    ConcatStringNode(){
        if (totalID + 1 > 10000000) throw std::overflow_error("Id is overflow");
        this -> ID = ++totalID;
        this -> data = "";
        this -> length = length;
        this -> left = nullptr;
        this -> right = nullptr;
        this -> leftLength = 0;
    };
    ConcatStringNode(string data, ConcatStringNode * left = nullptr, ConcatStringNode * right = nullptr){
        if (totalID + 1 > 10000000) throw std::overflow_error("Id is overflow");
        this -> ID = ++totalID;
        this -> data = data;
        this -> length = length;
        this -> left = left;
        this -> right = right;
        this -> leftLength = leftLength; 
    }
    ConcatStringNode(const ConcatStringNode * otherN){
        if (totalID + 1 > 10000000) throw std::overflow_error("Id is overflow");
        this -> ID = ++totalID;
        this -> data = otherN -> data;
        this -> length = otherN -> length;
        this -> left = nullptr;
        this -> right = nullptr;
        if (otherN -> left) this -> left = new ConcatStringNode(otherN -> left);
        if (otherN -> right)this -> right = new ConcatStringNode(otherN -> right);
        this -> leftLength = otherN -> leftLength;
    }
    ConcatStringNode(const char * s){
        this -> data = s;
        this -> length = data.length();
        this -> left = nullptr;
        this -> right = nullptr;
        this -> leftLength = 0;
    }
    int getDataLength(){
        return this -> data.length();
    }
    void setDataLength(int length){
        this -> length = length;
    }
    char getAtIndex(int index){
        if (left == nullptr && right == nullptr) return data[index];
        if (leftLength <= index) return  this -> right -> getAtIndex(index - this -> leftLength);
        else return this -> left -> getAtIndex(index);
    }
    void reverseData(){
        int n = getDataLength();
        for (int i = 0; i < n / 2; i++) swap(data[i], data[n - i - 1]);
    }
    ~ConcatStringNode(){

    }
};
class ConcatStringTree {
public:
    ConcatStringNode * root;
    int treeLength = 0;
    ConcatStringTree(){
        root = nullptr;
    }
    ConcatStringTree(const char * s){
        root = new ConcatStringNode(s);
        treeLength += root->getDataLength();
    };
    ConcatStringTree(const ConcatStringTree * otherT){
        this -> root = new ConcatStringNode(otherT -> root);
    }
    int length() const{
        return treeLength;
    };
    int height(ConcatStringNode * root){
        if (root == nullptr) return 0;
        int lh = height(root -> left);
        int rh = height(root -> right);
        if (lh > rh) {
            return ++lh;
        } else {
            return ++rh;
        }
    }
    char get(int index){
        if (index > treeLength || index < 0) throw out_of_range("Index of string is invalid!");
        return root -> getAtIndex(index);
    };
    bool isCurrentLevel(ConcatStringNode* root, int level, int& checkedLen, char c)
    {
        if (root == nullptr) return false;
        if (level == 1) {
            if (root -> data != "") {
                for (int i = 1; i < root -> getDataLength(); i++){
                    if (root -> data[i] == c) return true;
                    checkedLen++;
                }
            }
            else checkedLen += root -> getDataLength();
            return false;
        }
        else if (level > 1) {
            if (isCurrentLevel(root->left, level - 1, checkedLen, c)) return true;
            if (isCurrentLevel(root->right, level - 1, checkedLen, c)) return true;
        }
    };
    int indexOf(ConcatStringNode * root, char c){
        int checkedLen = 0;
        int minLength = root -> getDataLength();
        for (int i = 1; i < height(root); ++i){
            if (isCurrentLevel(root, i, checkedLen, c)) minLength = min(checkedLen, minLength);
            checkedLen = 0;
        }
        if (minLength = root -> getDataLength() + 1) return -1; 
        return minLength;
    }
    int indexOf(char c){
        return indexOf(this -> root, c);    
    };
    string toStringPreOrder(ConcatStringNode * root) const{
        if (!root) return "";
        string s = "(LL=" + to_string(root -> leftLength) + (string)",L=" + to_string(root -> getDataLength()) + ',' + (string)((root -> data == "") ? "<NULL>);" : '<' +root -> data + ">)" + ";")
            + toStringPreOrder(root -> left) + toStringPreOrder(root -> right);
        return s;
    }
    string toStringPreOrder() const{ //very cool ss
        stringstream ans;
        ans << "ConcatStringTree[" << toStringPreOrder(root) ;
        ans.seekp(-1,ans.cur);
        ans << "]";
        return ans.str();
    };
    string toString(ConcatStringNode * root) const{ //inOrder attempt
        if (!root) return "";
        return toString(root -> left) + root -> data + toString(root -> right);
    }
    string toString() const{
        string ans = "ConcatStringTree[";
        return ans + '"' + toString(root) + '"' + "]" ;
    };
    ConcatStringTree concat(const ConcatStringTree & otherS) const{ //uh assignment said do this 
        ConcatStringTree* result = new ConcatStringTree();  
        result -> root = new ConcatStringNode();
        result -> root -> data = "";
        result -> root -> left = this -> root;
        result -> root -> right = otherS.root;
        result -> root -> length = this -> root -> length + otherS.root -> length;
        result -> root -> leftLength = this -> root -> length;
        return *result;
    };
    ConcatStringNode * trimTree(ConcatStringNode * root, int from, int to) const{
        ConcatStringNode * fromRight = root;
        int initialLength = root -> getDataLength() - to;
        int rightDeleted = 0;
        int totalLeftLength = fromRight -> leftLength;
        while (fromRight -> right || fromRight -> left){
            if (fromRight -> right == nullptr || fromRight -> left == nullptr){
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
        fromRight -> length = fromRight -> getDataLength();
        ConcatStringNode * fromLeft = root;
        int totalLeftDeleted = 0;
        while (fromLeft -> left || fromLeft -> right){
            if (fromLeft -> left == nullptr && fromLeft -> right == nullptr){
                fromLeft -> length -= (from - totalLeftDeleted);
                fromLeft = fromLeft -> left;
            } else if(fromLeft -> leftLength + totalLeftDeleted > from){
                fromLeft -> left -= (from - totalLeftDeleted);
                fromLeft -> leftLength -= (from - totalLeftDeleted);
                fromLeft = fromLeft -> left;
            } else {
                totalLeftDeleted += fromLeft -> leftLength;
                fromLeft -> length = from;
                fromLeft -> left = nullptr;
                fromLeft -> leftLength = 0;
                fromLeft = fromLeft -> right;
            }
        }
        fromLeft -> data = fromLeft -> data.substr(from - totalLeftDeleted, fromLeft -> getDataLength() - (from - fromLeft -> leftLength));
        fromLeft -> length = fromLeft -> getDataLength();
        return root;
    }
    ConcatStringTree subString(int from, int to) const{
        if (from >= to) throw logic_error("Invalid range");
        if (from < 0 || to > this -> root -> getDataLength()) throw out_of_range("Index of string is invalid");
        ConcatStringTree * result = new ConcatStringTree();
        result -> root = new ConcatStringNode(this -> root);
        result -> root = result -> trimTree(result -> root, from, to);
        return *result;
    };
    ConcatStringNode* invert(ConcatStringNode * root) {
        if(!root) return nullptr;
        if (root -> left) invert(root -> left);
        if (root -> right) invert(root -> right);
        swap (root -> left, root -> right);
        if (root -> left) root -> leftLength = root -> right -> length;
        else root -> leftLength = 0;
        if (root ->data != "") root -> reverseData();
        return root;    
    }
    ConcatStringTree reverse() const{ //invert and reverse
        ConcatStringTree * result = new ConcatStringTree(this);
        result -> root = result -> invert(result -> root);
        return *result;
    };

    int getParTreeSize(const string & query) const{
    };
    string getParTreeStringPreOrder(const string & query) const;
};

class ReducedConcatStringTree; // forward declaration

class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;

    friend class ReducedConcatStringTree;
};

class ReducedConcatStringTree /* */ {

public:
    class LitStringHash {
    public:
        LitStringHash(const HashConfig & hashConfig);
        int getLastInsertedIndex() const;
        string toString() const;
    };

public:
    ReducedConcatStringTree(const char * s, LitStringHash * litStringHash);
    LitStringHash * litStringHash;
};

#endif // __CONCAT_STRING_TREE_H__