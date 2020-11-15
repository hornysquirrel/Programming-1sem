#include<iostream>

class Element;

long long delta(Element*);

Element* Next(Element* element, long long x);

class Element {
public:
    Element* parent = nullptr;
    Element* leftChild = nullptr;
    Element* rightChild = nullptr;
    long long value{};
    bool leftLeaf() const {return leftChild == nullptr;}
    bool rightLeaf() const {return rightChild == nullptr;}
    bool root() const {return parent == nullptr;}

    long long Depth = 1;
private:
    void checkDepth(){
        long long DepthL = 0;
        long long DepthR = 0;
        if(!leftLeaf()){
            DepthL = (*leftChild).Depth;
        }
        if(!rightLeaf()){
            DepthR = (*rightChild).Depth;
        }
        Depth = std::max(DepthL, DepthR) + 1;
    }

    void removeLeft(){
        if(!leftLeaf()) {
            (*leftChild).parent = nullptr;
            leftChild = nullptr;
        }
    }

    void removeRight(){
        if(!rightLeaf()) {
            (*rightChild).parent = nullptr;
            rightChild = nullptr;
        }
    }

    void removeParent(){
        if(!root()){
            if((*parent).leftChild == this){
                (*parent).removeLeft();
            }
            else {
                if ((*parent).rightChild == this) {
                    (*parent).removeRight();
                }
            }
        }
    }

    void connectLeft(Element* element){
        removeLeft();
        if(element != nullptr) {
            (*element).removeParent();
            (*element).parent = this;
        }
        leftChild = element;
    }

    void connectRight(Element* element){
        removeRight();
        if(element != nullptr) {
            (*element).removeParent();
            (*element).parent = this;
        }
        rightChild = element;
    }

    void connect(Element* element){
        if((*element).value < value){
            connectLeft(element);
        }
        if((*element).value > value){
            connectRight(element);
        }
        checkDepthLong();
    }

    void rotateLeft(){
        Element* par = parent;
        bool left = false;
        bool right = false;
        if(par != nullptr) {
            if ((*par).leftChild == this){
                left = true;
            }
            if ((*par).rightChild == this){
                right = true;
            }
        }
        Element* center = (*leftChild).rightChild;
        (*leftChild).connectRight(this);
        if(left) {
            (*par).connectLeft(leftChild);
        }
        if(right) {
            (*par).connectRight(leftChild);
        }
        connectLeft(center);
        checkDepth();
        (*parent).checkDepth();
    }

    void rotateRight(){
        Element* par = parent;
        bool left = false;
        bool right = false;
        if(par != nullptr) {
            if ((*par).leftChild == this){
                left = true;
            }
            if ((*par).rightChild == this){
                right = true;
            }
        }
        Element* center = (*rightChild).leftChild;
        (*rightChild).connectLeft(this);
        if(left) {
            (*par).connectLeft(rightChild);
        }
        if(right) {
            (*par).connectRight(rightChild);
        }
        connectRight(center);
        checkDepth();
        (*parent).checkDepth();
    }

    void balanceWeak(){
        if(delta(this) == 2){
            if(delta(leftChild) == -1){
                (*leftChild).rotateRight();
                rotateLeft();
            }
            else{
                rotateLeft();
            }
        }
        if(delta(this) == -2){
            if(delta(rightChild) == 1){
                (*rightChild).rotateLeft();
                rotateRight();
            }
            else{
                rotateRight();
            }
        }
        checkDepth();
    }

    void eraseLeaf(){
        if(leftLeaf() & rightLeaf()){
            Element* par = parent;
            removeParent();
            if(par != nullptr){
                (*par).checkDepthLong();
            }
            delete this;
        }
    }
public:
    Element* binFind(long long x){
        if(value > x){
            if(leftLeaf()){
                return this;
            }
            else{
                return (*leftChild).binFind(x);
            }
        }
        if(value < x){
            if(rightLeaf()){
                return this;
            }
            else{
                return (*rightChild).binFind(x);
            }
        }
        else{
            return this;
        }
    }

    void insert(Element* element){
        Element* leaf = binFind((*element).value);
        (*leaf).connect(element);
    }

    bool exists(long long x){
        Element* leaf = binFind(x);
        return (*leaf).value == x;
    }

    void balanceStrong(){
        Element* par = parent;
        balanceWeak();
        if(par != nullptr){
            (*par).balanceStrong();
        }
    }

    void eraseHalfLeaf() {
        if(leftLeaf() & !rightLeaf()){
            rotateRight();
            checkDepthLong();
        }
        if(!leftLeaf() & rightLeaf()){
            rotateLeft();
            checkDepthLong();
        }
        eraseLeaf();
    }

    void swap(Element* element){
        bool leftFix = (element == leftChild);
        bool rightFix = (element == rightChild);
        Element* par = parent;
        Element* leftC = leftChild;
        Element* rightC = rightChild;
        Element* parE = (*element).parent;
        Element* leftCE = (*element).leftChild;
        Element* rightCE = (*element).rightChild;
        bool left = false;
        bool right = false;
        if(par != nullptr) {
            if ((*par).leftChild == this){
                left = true;
            }
            if ((*par).rightChild == this){
                right = true;
            }
        }
        bool leftE = false;
        bool rightE = false;
        if(parE != nullptr){
            if ((*parE).leftChild == element){
                leftE = true;
            }
            if ((*parE).rightChild == element){
                rightE = true;
            }
        }
        if(leftE){
            (*parE).connectLeft(this);
        }
        if(rightE){
            (*parE).connectRight(this);
        }
        (*element).connectLeft(leftC);
        (*element).connectRight(rightC);
        if(left) {
            (*par).connectLeft(element);
        }
        if(right) {
            (*par).connectRight(element);
        }
        if(leftFix){
            (*element).connectLeft(this);
            parent = element;
        }
        if(rightFix){
            (*element).connectRight(this);
            parent = element;
        }
        connectLeft(leftCE);
        connectRight(rightCE);
    }

    void checkDepthLong() {
        checkDepth();
        if(!root()){
            (*parent).checkDepthLong();
        }
    }
};

long long delta(Element* element){
    if(element == nullptr){
        return 0;
    }
    else{
        long long DepthL = 0;
        long long DepthR = 0;
        if(!(*element).leftLeaf()){
            DepthL = (*(*element).leftChild).Depth;
        }
        if(!(*element).rightLeaf()){
            DepthR = (*(*element).rightChild).Depth;
        }
        return DepthL - DepthR;
    }
}

Element* Next(Element* element, long long x){
    if(element == nullptr){
        return nullptr;
    }
    else{
        if((*element).value <= x){
            return Next((*element).rightChild, x);
        }
        else{
            Element* check = Next((*element).leftChild, x);
            if(check == nullptr){
                return element;
            }
            else{
                return check;
            }
        }
    }
}

Element* Prev(Element* element, long long x){
    if(element == nullptr){
        return nullptr;
    }
    else{
        if((*element).value >= x){
            return Prev((*element).leftChild, x);
        }
        else{
            Element* check = Prev((*element).rightChild, x);
            if(check == nullptr){
                return element;
            }
            else{
                return check;
            }
        }
    }
}

struct AVLTree {
    Element* root = nullptr;
    long long size = 0;

    void insert(long long x){
        auto leafX = new Element;
        (*leafX).value = x;
        if(size == 0){
            root = leafX;
        }
        else{
            (*root).insert(leafX);
            (*leafX).balanceStrong();
            if(!(*root).root()){
                root = (*root).parent;
            }
        }
        size++;
    }

    bool exists(long long x) const{
        if(root != nullptr) {
            return (*root).exists(x);
        }
        else{
            return false;
        }
    }

    bool nextExists(long long x){
        return (Next(root, x) != nullptr);
    }

    long long next(long long x){
        if(nextExists(x)) {
            return (*Next(root, x)).value;
        }
        else{
            return 0;
        }
    }

    bool prevExists(long long x){
        return (Prev(root, x) != nullptr);
    }

    long long prev(long long x){
        if(prevExists(x)) {
            return (*Prev(root, x)).value;
        }
        else{
            return 0;
        }
    }

    void erase(long long x){
        if(root != nullptr) {
            Element *pointerDeath = (*root).binFind(x);
            if((*pointerDeath).value == x) {
                Element* next = Next(pointerDeath, x);
                if(root == pointerDeath){
                    if((*root).rightLeaf()){
                        root = (*pointerDeath).leftChild;
                    }
                    else{
                        root = (*pointerDeath).rightChild;
                    }
                }
                if(next == nullptr){
                    next = pointerDeath;
                }
                if(pointerDeath != next){
                    (*pointerDeath).swap(next);
                }
                Element* par = (*pointerDeath).parent;
                (*pointerDeath).eraseHalfLeaf();
                if(par != nullptr){
                    (*par).balanceStrong();
                }
                size--;
                if(root != nullptr){
                    while(!(*root).root()){
                        root = (*root).parent;
                    }
                }
            }
        }
    }
};

int main() {
    AVLTree Tree;
    std::string insert = "insert";
    std::string erase = "delete";
    std::string next = "next";
    std::string prev = "prev";
    std::string exists = "exists";
    std::string move = "start";
    long long x;
    while(std::cin >> move){
        if(move == insert){
            std::cin >> x;
            if(!Tree.exists(x)) {
                Tree.insert(x);
            }
        }
        if(move == erase){
            std::cin >> x;
            Tree.erase(x);
        }
        if(move == prev){
            std::cin >> x;
            if(Tree.prevExists(x)) {
                std::cout << Tree.prev(x) << std::endl;
            }
            else{
                std::cout << "none" << std::endl;
            }
        }
        if(move == next){
            std::cin >> x;
            if(Tree.nextExists(x)) {
                std::cout << Tree.next(x) << std::endl;
            }
            else{
                std::cout << "none" << std::endl;
            }
        }
        if(move == exists){
            std::cin >> x;
            if(Tree.exists(x)){
                std::cout << "true" << std::endl;
            }
            else{
                std::cout << "false" << std::endl;
            }
        }
    }
    return 0;
}
