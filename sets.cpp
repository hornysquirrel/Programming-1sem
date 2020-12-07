#include <iostream>


struct Edge{
    long long from;
    long long to;

    Edge ref(){
        Edge r;
        r.from = to;
        r.to = from;
        return r;
    }

    bool operator> (const Edge& edge) const{
        if(from > edge.from){
            return true;
        }
        if(from == edge.from){
            return to > edge.to;
        }
        return false;
    }

    bool operator== (const Edge& edge) const{
        return (from == edge.from) && (to == edge.to);
    }

    bool operator< (const Edge& edge) const{
        if(from < edge.from){
            return true;
        }
        if(from == edge.from){
            return to < edge.to;
        }
        return false;
    }

    bool operator!= (const Edge& edge) const{
        return !(*this == edge);
    }
};

class Qliphoth {
    Edge value;
    Qliphoth* leftChild = nullptr;
    Qliphoth* rightChild = nullptr;

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
    Qliphoth(Edge x){
        value = x;
    }

    Qliphoth* splayMax(){
        if(rrGrandChild()){
            Qliphoth* ptrR = rightChild;
            Qliphoth* ptrRR = rightChild->rightChild->splayMax();
            ptrR->rightChild = ptrRR->leftChild;
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            ptrRR->leftChild = ptrR;
            return ptrRR;
        }
        if(rChild()){
            Qliphoth* ptrR = rightChild->splayMax();
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
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
            return ptrLL;
        }
        if(lChild()){
            Qliphoth* ptrL = leftChild->splayMin();
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            return ptrL;
        }
        return this;
    }

    std::pair<Qliphoth*, Qliphoth*> split(Edge x){
        Qliphoth* root = splay(x);
        Qliphoth* left;
        Qliphoth* right;
        std::pair<Qliphoth*, Qliphoth*> pair;
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

    Qliphoth* mergeRight(Qliphoth* right){
        Qliphoth* left = splayMax();
        left->rightChild = right;
        return left;
    }

    Qliphoth* mergeLeft(Qliphoth* left){
        Qliphoth* right = splayMin();
        right->leftChild = left;
        return right;
    }

    Edge getValue() const{
        return value;
    }

    friend std::ostream& operator<< (std::ostream &out, const Qliphoth &qliphoth);

    Qliphoth* splay(Edge x){
        if(llGrandChild(x)){
            Qliphoth* ptrL = leftChild;
            Qliphoth* ptrLL = leftChild->leftChild->splay(x);
            ptrL->leftChild = ptrLL->rightChild;
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            ptrLL->rightChild = ptrL;
            return ptrLL;
        }
        if(lrGrandChild(x)){
            Qliphoth* ptrL = leftChild;
            Qliphoth* ptrLR = leftChild->rightChild->splay(x);
            ptrL->rightChild = ptrLR->leftChild;
            leftChild = ptrLR->rightChild;
            ptrLR->leftChild = ptrL;
            ptrLR->rightChild = this;
            return ptrLR;
        }
        if(rlGrandChild(x)){
            Qliphoth* ptrR = rightChild;
            Qliphoth* ptrRL = rightChild->leftChild->splay(x);
            ptrR->leftChild = ptrRL->rightChild;
            rightChild = ptrRL->leftChild;
            ptrRL->rightChild = ptrR;
            ptrRL->leftChild = this;
            return ptrRL;
        }
        if(rrGrandChild(x)){
            Qliphoth* ptrR = rightChild;
            Qliphoth* ptrRR = rightChild->rightChild->splay(x);
            ptrR->rightChild = ptrRR->leftChild;
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            ptrRR->leftChild = ptrR;
            return ptrRR;
        }
        if(lChild(x)){
            Qliphoth* ptrL = leftChild->splay(x);
            leftChild = ptrL->rightChild;
            ptrL->rightChild = this;
            return ptrL;
        }
        if(rChild(x)){
            Qliphoth* ptrR = rightChild->splay(x);
            rightChild = ptrR->leftChild;
            ptrR->leftChild = this;
            return ptrR;
        }
        return this;
    }
};

std::ostream& operator<< (std::ostream &out, const Qliphoth &qliphoth){
    if(qliphoth.lChild()){
        out << *qliphoth.leftChild << ' ';
    }
    out << qliphoth.value.to;
    if(qliphoth.rChild()){
        out << ' ' << *qliphoth.rightChild;
    }
    return out;
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

std::pair<Qliphoth*, Qliphoth*> split(Qliphoth* qliphoth, Edge x){
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
public:
    bool empty() const{
        return root == nullptr;
    }

    void showFrom(long long x){
        SplayTree tree;
        Edge l;
        Edge r;
        l.from = x;
        l.to = -1;
        r.from = x + 1;
        r.to = -1;
        std::pair<Qliphoth*, Qliphoth*> pair1 = split(root, l);
        Qliphoth* left = pair1.first;
        Qliphoth* center = pair1.second;
        std::pair<Qliphoth*, Qliphoth*> pair2 = split(center, r);
        center = pair2.first;
        Qliphoth* right = pair2.second;
        tree.root = center;
        std::cout << tree << std::endl;
        center = merge(left, center);
        center = merge(center, right);
        root = center;
    }

    void insert(Edge x){
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

    void erase(Edge x){
        if(!empty()){
            root = root->splay(x);
            if(root->getValue() == x){
                std::pair<Qliphoth*, Qliphoth*> pair1 = split(root, x);
                Qliphoth* center = pair1.first;
                Qliphoth* right = pair1.second;
                Edge x0 = x;
                x0.to--;
                std::pair<Qliphoth*, Qliphoth*> pair2 = split(center, x0);
                center = pair2.second;
                Qliphoth* left = pair2.first;
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
        x.from = -1;
        x.to = -1;
        return x;
    }

    SplayTree clearFrom(long long x){
        SplayTree tree;
        Edge l;
        Edge r;
        l.from = x;
        l.to = -1;
        r.from = x + 1;
        r.to = -1;
        std::pair<Qliphoth*, Qliphoth*> pair1 = split(root, l);
        Qliphoth* left = pair1.first;
        Qliphoth* center = pair1.second;
        std::pair<Qliphoth*, Qliphoth*> pair2 = split(center, r);
        center = pair2.first;
        Qliphoth* right = pair2.second;
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
    SplayTree FromTo;
    SplayTree ToFrom;

    void insert(long long x, long long y){
        Edge e;
        e.from = x;
        e.to = y;
        FromTo.insert(e);
        ToFrom.insert(e.ref());
    }

    void erase(long long x, long long y){
        Edge e;
        e.from = x;
        e.to = y;
        FromTo.erase(e);
        ToFrom.erase(e.ref());
    }

    void showFrom(long long x){
        FromTo.showFrom(x);

    }

    void showTo(long long x){
        ToFrom.showFrom(x);
    }

    void clearFrom(long long x){
        SplayTree tree = FromTo.clearFrom(x);
        while(!tree.empty()){
            Edge e = tree.eraseRoot();
            erase(e.from, e.to);
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
            Tree.clearFrom(s);
        }
        if(move == listSet){
            long long s;
            std::cin >> s;
            Tree.showFrom(s);
        }
        if(move == listSetOf){
            long long e;
            std::cin >> e;
            Tree.showTo(e);
        }
    }
    return 0;
}