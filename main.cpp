#include "ConcatStringTree.h"
//g++ -o main main.cpp ConcatStringTree.cpp -I . -std=c++11
int main() {
    ConcatStringTree s1("Hello");
    ConcatStringTree s2(",_t");
    ConcatStringTree s3 = s1.concat(s2);
    cout << s3.toStringPreOrder() << endl;
    ConcatStringTree s4 = s3.reverse().subString(1,3);
    cout << s4.toStringPreOrder();
    system("pause");
}