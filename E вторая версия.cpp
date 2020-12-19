#include <iostream>


struct Edge{
    long long set;
    long long number;

    Edge ref(){
        Edge r;
        r.set = number;
        r.number = set;
        return r;
    }

    bool operator> (const Edge& edge) const{
        if(set > edge.set){
            return true;
        }
        if(set == edge.set){
            return number > edge.number;
        }
        return false;
    }

    bool operator== (const Edge& edge) const{
        return (set == edge.set) && (number == edge.number);
    }

    bool operator< (const Edge& edge) const{
        if(set < edge.set){
            return true;
        }
        if(set == edge.set){
            return number < edge.number;
        }
        return false;
    }

    bool operator!= (const Edge& edge) const{
        return !(*this == edge);
    }
};

class Element {
    Edge value;
    Element* leftChild = nullptr;
    Element* rightChild = nullptr;

    bool lChild() const{
        return leftChild != nullptr;
    }

    bool lChild(Edge x) const{
        return lChild() && (value > x);
    }

    bool rChild() const{
        return rightChild != nullptr;
    }

    bool rChild(Edge x) const{
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

    bool llGrandChild(Edge x) const{
        if(lChild(x)){
            return leftChild->lChild(x);
        }
        else{
            return false;
        }
    }

    bool lrGrandChild(Edge x) const{
        if(lChild(x)){
            return leftChild->rChild(x);
        }
        else{
            return false;
        }
    }

    bool rlGrandChild(Edge x) const{
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

    bool rrGrandChild(Edge x) const{
        if(rChild(x)){
            return rightChild->rChild(x);
        }
        else{
            return false;
        }
    }
public:
    Element(Edge x){
        value = x;
    }

    Element* splayMax(){
        if(rrGrandChild()){
            Element* ptrR = rightChild;
            Element* ptrRR = rightChild->rightChild->splayMax();
            ptrR->rightChild = ptrRR->leftChild;
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            ptrRR->leftChild = ptrR;
            return ptrRR;
        }
        if(rChild()){
            Element* ptrR = rightChild->splayMax();
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
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
            return ptrLL;
        }
        if(lChild()){
            Element* ptrL = leftChild->splayMin();
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            return ptrL;
        }
        return this;
    }

    std::pair<Element*, Element*> split(Edge x){
        Element* root = splay(x);
        Element* left;
        Element* right;
        std::pair<Element*, Element*> pair;
        if(root->getValue() > x){
            left = root->leftChild;
            root->leftChild = nullptr;
            right = root;
            pair.first = left;
            pair.second = right;
            return pair;
        }
        right = root->rightChild;
        root->rightChild = nullptr;
        left = root;
        pair.first = left;
        pair.second = right;
        return pair;
    }

    Element* mergeRight(Element* right){
        Element* left = splayMax();
        left->rightChild = right;
        return left;
    }

    Element* mergeLeft(Element* left){
        Element* right = splayMin();
        right->leftChild = left;
        return right;
    }

    Edge getValue() const{
        return value;
    }

    friend std::ostream& operator<< (std::ostream &out, const Element &qliphoth);

    Element* splay(Edge x){
        if(llGrandChild(x)){
            Element* ptrL = leftChild;
            Element* ptrLL = leftChild->leftChild->splay(x);
            ptrL->leftChild = ptrLL->rightChild;
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            ptrLL->rightChild = ptrL;
            return ptrLL;
        }
        if(lrGrandChild(x)){
            Element* ptrL = leftChild;
            Element* ptrLR = leftChild->rightChild->splay(x);
            ptrL->rightChild = ptrLR->leftChild;
            leftChild = ptrLR->rightChild;
            ptrLR->leftChild = ptrL;
            ptrLR->rightChild = this;
            return ptrLR;
        }
        if(rlGrandChild(x)){
            Element* ptrR = rightChild;
            Element* ptrRL = rightChild->leftChild->splay(x);
            ptrR->leftChild = ptrRL->rightChild;
            rightChild = ptrRL->leftChild;
            ptrRL->rightChild = ptrR;
            ptrRL->leftChild = this;
            return ptrRL;
        }
        if(rrGrandChild(x)){
            Element* ptrR = rightChild;
            Element* ptrRR = rightChild->rightChild->splay(x);
            ptrR->rightChild = ptrRR->leftChild;
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            ptrRR->leftChild = ptrR;
            return ptrRR;
        }
        if(lChild(x)){
            Element* ptrL = leftChild->splay(x);
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            return ptrL;
        }
        if(rChild(x)){
            Element* ptrR = rightChild->splay(x);
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            return ptrR;
        }
        return this;
    }
};

std::ostream& operator<< (std::ostream &out, const Element &qliphoth){
    if(qliphoth.lChild()){
        out << *qliphoth.leftChild << ' ';
    }
    out << qliphoth.value.number;
    if(qliphoth.rChild()){
        out << ' ' << *qliphoth.rightChild;
    }
    return out;
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

std::pair<Element*, Element*> split(Element* qliphoth, Edge x){
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
public:
    bool empty() const{
        return root == nullptr;
    }

    void showFrom(long long x){
        SplayTree tree;
        Edge l;
        Edge r;
        l.set = x;
        l.number = -1;
        r.set = x + 1;
        r.number = -1;
        std::pair<Element*, Element*> pair1 = split(root, l);
        Element* left = pair1.first;
        Element* center = pair1.second;
        std::pair<Element*, Element*> pair2 = split(center, r);
        center = pair2.first;
        Element* right = pair2.second;
        tree.root = center;
        std::cout << tree << std::endl;
        center = merge(left, center);
        center = merge(center, right);
        root = center;
    }

    void insert(Edge x){
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

    void erase(Edge x){
        if(!empty()){
            root = root->splay(x);
            if(root->getValue() == x){
                std::pair<Element*, Element*> pair1 = split(root, x);
                Element* center = pair1.first;
                Element* right = pair1.second;
                Edge x0 = x;
                x0.number--;
                std::pair<Element*, Element*> pair2 = split(center, x0);
                center = pair2.second;
                Element* left = pair2.first;
                delete center;
                root = merge(left, right);
            }
        }
    }

    Edge eraseRoot(){
        if(!empty()){
            Edge x = root->getValue();
            erase(root->getValue());
            return x;
        }
        Edge x;
        x.set = -1;
        x.number = -1;
        return x;
    }

    SplayTree clearSet(long long x){
        SplayTree tree;
        Edge l;
        Edge r;
        l.set = x;
        l.number = -1;
        r.set = x + 1;
        r.number = -1;
        std::pair<Element*, Element*> pair1 = split(root, l);
        Element* left = pair1.first;
        Element* center = pair1.second;
        std::pair<Element*, Element*> pair2 = split(center, r);
        center = pair2.first;
        Element* right = pair2.second;
        tree.root = center;
        root = merge(left, right);
        return tree;
    }

    friend std::ostream& operator<< (std::ostream &out, const SplayTree &tree);
};

std::ostream& operator<< (std::ostream &out, const SplayTree &tree){
    if(!tree.empty()) {
        out << *tree.root;
    }
    else{
        out << "-1";
    }
    return out;
}

struct SplayTree2{
    SplayTree FromSetToNumbers;
    SplayTree FromNumbersToSets;

    void insert(long long x, long long y){
        Edge e;
        e.set = x;
        e.number = y;
        FromSetToNumbers.insert(e);
        FromNumbersToSets.insert(e.ref());
    }

    void erase(long long x, long long y){
        Edge e;
        e.set = x;
        e.number = y;
        FromSetToNumbers.erase(e);
        FromNumbersToSets.erase(e.ref());
    }

    void showSet(long long x){
        FromSetToNumbers.showFrom(x);

    }

    void showNumber(long long x){
        FromNumbersToSets.showFrom(x);
    }

    void clearSet(long long x){
        SplayTree tree = FromSetToNumbers.clearSet(x);
        while(!tree.empty()){
            Edge e = tree.eraseRoot();
            erase(e.set, e.number);
        }
    }
};

int main() {
    std::string insert = "ADD";
    std::string erase = "DELETE";
    std::string clear = "CLEAR";
    std::string listSet = "LISTSET";
    std::string listSetOf = "LISTSETSOF";
    std::string move = "start";
    long long n;
    long long m;
    long long k;
    std::cin >> n;
    std::cin >> m;
    std::cin >> k;
    SplayTree2 Tree;
    for(long long i = 0; i < k; i++){
        std::cin >> move;
        if(move == insert){
            long long e;
            long long s;
            std::cin >> e;
            std::cin >> s;
            Tree.insert(s, e);
        }
        if(move == erase){
            long long e;
            long long s;
            std::cin >> e;
            std::cin >> s;
            Tree.erase(s, e);
        }
        if(move == clear){
            long long s;
            std::cin >> s;
            Tree.clearSet(s);
        }
        if(move == listSet){
            long long s;
            std::cin >> s;
            Tree.showSet(s);
        }
        if(move == listSetOf){
            long long e;
            std::cin >> e;
            Tree.showNumber(e);
        }
    }
    return 0;
}