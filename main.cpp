#include "ConcatStringTree.h"

int main() {
    ConcatStringTree s1 = "never_gonna_";
    ConcatStringTree s2 = "give_you_up";
    ConcatStringTree s3 = s1.subString(1, 10);
    cout << s1.toStringPreOrder() << endl;
    cout << s3.toString() << endl;
    cout << s3.reverse().toString() << endl;
    return 0;
}