#include<iostream>

//Реализуйте splay-дерево, которое поддерживает множество S целых чисел, в котором разрешается производить следующие операции:
//
//add(i) — добавить в множество S число i (если он там уже есть, то множество не меняется);
//sum(l,r) — вывести сумму всех элементов x из S, которые удовлетворяют неравенству l≤x≤r.
//Решения, не использующие splay-деревья, будут игнорироваться.
//
//Решения, не использующие операции split и merge, будут оцениваться в 2 балла. Решения, использующие операции split и merge, оцениваются в 3 балла.
//
//Входные данные
//Исходно множество S пусто. Первая строка входного файла содержит n — количество операций (1≤n≤300000). Следующие n строк содержат операции. Каждая операция имеет вид либо «+ i», либо «? l r». Операция «? l r» задает запрос sum(l,r).
//
//Если операция «+ i» идет во входном файле в начале или после другой операции «+», то она задает операцию add(i). Если же она идет после запроса «?», и результат этого запроса был y, то выполняется операция add((i+y)mod.
//
//Во всех запросах и операциях добавления параметры лежат в интервале от 0 до 10^9.
//
//Выходные данные
//Для каждого запроса выведите одно число — ответ на запрос.

class Element {
    long long value;
    Element* leftChild = nullptr;
    Element* rightChild = nullptr;
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
    Element(long long x){
        value = x;
        sum = x;
    }

    Element* splay(long long x){
        if(llGrandChild(x)){
            Element* ptrL = leftChild;
            Element* ptrLL = leftChild->leftChild->splay(x);
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
            Element* ptrL = leftChild;
            Element* ptrLR = leftChild->rightChild->splay(x);
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
            Element* ptrR = rightChild;
            Element* ptrRL = rightChild->leftChild->splay(x);
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
            Element* ptrR = rightChild;
            Element* ptrRR = rightChild->rightChild->splay(x);
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
            Element* ptrL = leftChild->splay(x);
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            checkSum();
            ptrL->checkSum();
            return ptrL;
        }
        if(rChild(x)){
            Element* ptrR = rightChild->splay(x);
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            checkSum();
            ptrR->checkSum();
            return ptrR;
        }
        return this;
    }

    Element* splayMax(){
        if(rrGrandChild()){
            Element* ptrR = rightChild;
            Element* ptrRR = rightChild->rightChild->splayMax();
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
            Element* ptrR = rightChild->splayMax();
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            checkSum();
            ptrR->checkSum();
            return ptrR;
        }
        return this;
    }

    Element* splayMin(){
        if(llGrandChild()){
            Element* ptrL = leftChild;
            Element* ptrLL = leftChild->leftChild->splayMin();
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
            Element* ptrL = leftChild->splayMin();
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            checkSum();
            ptrL->checkSum();
            return ptrL;
        }
        return this;
    }

    std::pair<Element*, Element*> split(long long x){
        Element* root = splay(x);
        Element* left;
        Element* right;
        std::pair<Element*, Element*> pair;
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

    Element* mergeRight(Element* right){
        Element* left = splayMax();
        left->rightChild = right;
        left->checkSum();
        return left;
    }

    Element* mergeLeft(Element* left){
        Element* right = splayMin();
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

long long sum(Element* qliphot){
    if(qliphot == nullptr){
        return 0;
    }
    return qliphot->getSum();
}

Element* merge(Element* left, Element* right){
    if(left != nullptr){
        Element* root = left->mergeRight(right);
        return root;
    }
    if(right != nullptr){
        return right;
    }
    return nullptr;
}

std::pair<Element*, Element*> split(Element* qliphoth, long long x){
    std::pair<Element*, Element*> pair;
    if(qliphoth == nullptr){
        pair.first = nullptr;
        pair.second = nullptr;
        return pair;
    }
    pair = qliphoth->split(x);
    return pair;
}

class SplayTree{
    Element* root = nullptr;
    bool empty(){
        return root == nullptr;
    }
public:
    void insert(long long x){
        if(empty()){
            Element* xRoot = new Element(x);
            root = xRoot;
        }
        else{
            root = root->splay(x);
            if(root->getValue() != x){
                Element* xRoot = new Element(x);
                std::pair<Element*, Element*> pair = split(root, x);
                xRoot->mergeLeft(pair.first);
                xRoot->mergeRight(pair.second);
                root = xRoot;
            }
        }
    }

    long long segSum(long long l, long long r){
        std::pair<Element*, Element*> pair1 = split(root, l - 1);
        Element* left = pair1.first;
        Element* center = pair1.second;
        std::pair<Element*, Element*> pair2 = split(center, r);
        center = pair2.first;
        Element* right = pair2.second;
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