#include<iostream>

class Qliphoth {
    long long value;
    Qliphoth* leftChild = nullptr;
    Qliphoth* rightChild = nullptr;
    long long sum;
    long long lSum() const{
        if(lChild()){
            return leftChild->sum;
        }
        return 0;
    }

    long long rSum() const{
        if(rChild()){
            return rightChild->sum;
        }
        return 0;
    }

    void checkSum() {
        sum = value + lSum() + rSum();
    }

    bool lChild() const{
        return leftChild != nullptr;
    }

    bool lChild(long long x) const{
        return lChild() && (value > x);
    }

    bool rChild() const{
        return rightChild != nullptr;
    }

    bool rChild(long long x) const{
        return rChild() && (value < x);
    }

    bool llGrandChild() const{
        if(lChild()){
            return leftChild->lChild();
        }
        else{
            return false;
        }
    }

    bool llGrandChild(long long x) const{
        if(lChild(x)){
            return leftChild->lChild(x);
        }
        else{
            return false;
        }
    }

    bool lrGrandChild(long long x) const{
        if(lChild(x)){
            return leftChild->rChild(x);
        }
        else{
            return false;
        }
    }

    bool rlGrandChild(long long x) const{
        if(rChild(x)){
            return rightChild->lChild(x);
        }
        else{
            return false;
        }
    }

    bool rrGrandChild() const{
        if(rChild()){
            return rightChild->rChild();
        }
        else{
            return false;
        }
    }

    bool rrGrandChild(long long x) const{
        if(rChild(x)){
            return rightChild->rChild(x);
        }
        else{
            return false;
        }
    }
public:
    Qliphoth(long long x){
        value = x;
        sum = x;
    }

    Qliphoth* splay(long long x){
        if(llGrandChild(x)){
            Qliphoth* ptrL = leftChild;
            Qliphoth* ptrLL = leftChild->leftChild->splay(x);
            ptrL->leftChild = ptrLL->rightChild;
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            ptrLL->rightChild = ptrL;
            checkSum();
            ptrL->checkSum();
            ptrLL->checkSum();
            return ptrLL;
        }
        if(lrGrandChild(x)){
            Qliphoth* ptrL = leftChild;
            Qliphoth* ptrLR = leftChild->rightChild->splay(x);
            ptrL->rightChild = ptrLR->leftChild;
            leftChild = ptrLR->rightChild;
            ptrLR->leftChild = ptrL;
            ptrLR->rightChild = this;
            checkSum();
            ptrL->checkSum();
            ptrLR->checkSum();
            return ptrLR;
        }
        if(rlGrandChild(x)){
            Qliphoth* ptrR = rightChild;
            Qliphoth* ptrRL = rightChild->leftChild->splay(x);
            ptrR->leftChild = ptrRL->rightChild;
            rightChild = ptrRL->leftChild;
            ptrRL->rightChild = ptrR;
            ptrRL->leftChild = this;
            checkSum();
            ptrR->checkSum();
            ptrRL->checkSum();
            return ptrRL;
        }
        if(rrGrandChild(x)){
            Qliphoth* ptrR = rightChild;
            Qliphoth* ptrRR = rightChild->rightChild->splay(x);
            ptrR->rightChild = ptrRR->leftChild;
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            ptrRR->leftChild = ptrR;
            checkSum();
            ptrR->checkSum();
            ptrRR->checkSum();
            return ptrRR;
        }
        if(lChild(x)){
            Qliphoth* ptrL = leftChild->splay(x);
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            checkSum();
            ptrL->checkSum();
            return ptrL;
        }
        if(rChild(x)){
            Qliphoth* ptrR = rightChild->splay(x);
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            checkSum();
            ptrR->checkSum();
            return ptrR;
        }
        return this;
    }

    Qliphoth* splayMax(){
        if(rrGrandChild()){
            Qliphoth* ptrR = rightChild;
            Qliphoth* ptrRR = rightChild->rightChild->splayMax();
            ptrR->rightChild = ptrRR->leftChild;
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            ptrRR->leftChild = ptrR;
            checkSum();
            ptrR->checkSum();
            ptrRR->checkSum();
            return ptrRR;
        }
        if(rChild()){
            Qliphoth* ptrR = rightChild->splayMax();
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            checkSum();
            ptrR->checkSum();
            return ptrR;
        }
        return this;
    }

    Qliphoth* splayMin(){
        if(llGrandChild()){
            Qliphoth* ptrL = leftChild;
            Qliphoth* ptrLL = leftChild->leftChild->splayMin();
            ptrL->leftChild = ptrLL->rightChild;
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            ptrLL->rightChild = ptrL;
            checkSum();
            ptrL->checkSum();
            ptrLL->checkSum();
            return ptrLL;
        }
        if(lChild()){
            Qliphoth* ptrL = leftChild->splayMin();
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            checkSum();
            ptrL->checkSum();
            return ptrL;
        }
        return this;
    }

    std::pair<Qliphoth*, Qliphoth*> split(long long x){
        Qliphoth* root = splay(x);
        Qliphoth* left;
        Qliphoth* right;
        std::pair<Qliphoth*, Qliphoth*> pair;
        if(root->getValue() > x){
            left = root->leftChild;
            root->leftChild = nullptr;
            root->checkSum();
            right = root;
            pair.first = left;
            pair.second = right;
            return pair;
        }
        right = root->rightChild;
        root->rightChild = nullptr;
        root->checkSum();
        left = root;
        pair.first = left;
        pair.second = right;
        return pair;
    }

    Qliphoth* mergeRight(Qliphoth* right){
        Qliphoth* left = splayMax();
        left->rightChild = right;
        left->checkSum();
        return left;
    }

    Qliphoth* mergeLeft(Qliphoth* left){
        Qliphoth* right = splayMin();
        right->leftChild = left;
        right->checkSum();
        return right;
    }

    long long getValue() const{
        return value;
    }

    long long getSum() const{
        return sum;
    }
};

long long sum(Qliphoth* qliphot){
    if(qliphot == nullptr){
        return 0;
    }
    return qliphot->getSum();
}

Qliphoth* merge(Qliphoth* left, Qliphoth* right){
    if(left != nullptr){
        Qliphoth* root = left->mergeRight(right);
        return root;
    }
    if(right != nullptr){
        return right;
    }
    return nullptr;
}

std::pair<Qliphoth*, Qliphoth*> split(Qliphoth* qliphoth, long long x){
    std::pair<Qliphoth*, Qliphoth*> pair;
    if(qliphoth == nullptr){
        pair.first = nullptr;
        pair.second = nullptr;
        return pair;
    }
    pair = qliphoth->split(x);
    return pair;
}

class SplayTree{
    Qliphoth* root = nullptr;
    bool empty(){
        return root == nullptr;
    }
public:
    void insert(long long x){
        if(empty()){
            Qliphoth* xRoot = new Qliphoth(x);
            root = xRoot;
        }
        else{
            root = root->splay(x);
            if(root->getValue() != x){
                Qliphoth* xRoot = new Qliphoth(x);
                std::pair<Qliphoth*, Qliphoth*> pair = split(root, x);
                xRoot->mergeLeft(pair.first);
                xRoot->mergeRight(pair.second);
                root = xRoot;
            }
        }
    }

    long long segSum(long long l, long long r){
        std::pair<Qliphoth*, Qliphoth*> pair1 = split(root, l - 1);
        Qliphoth* left = pair1.first;
        Qliphoth* center = pair1.second;
        std::pair<Qliphoth*, Qliphoth*> pair2 = split(center, r);
        center = pair2.first;
        Qliphoth* right = pair2.second;
        long long s = sum(center);
        center = merge(left, center);
        center = merge(center, right);
        root = center;
        return s;
    }
};

int main() {
    SplayTree Tree;
    std::string insert = "+";
    std::string getSum = "?";
    std::string move = "start";
    long long x;
    long long n;
    long long l;
    long long r;
    bool prevGetSum = false;
    long long sum = 0;
    std::cin >> x;
    for(long long i = 0; i < x; i++){
        std::cin >> move;
        if(move == insert){
            std::cin >> n;
            if(prevGetSum){
                n = (n + sum)%1000000000;
            }
            Tree.insert(n);
            prevGetSum = false;
        }
        if(move == getSum){
            std::cin >> l;
            std::cin >> r;
            sum = Tree.segSum(l, r);
            std::cout << sum << std::endl;
            prevGetSum = true;
        }
    }
    return 0;
}