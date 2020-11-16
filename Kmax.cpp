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
    long long Size = 1;
private:
    void checkDepthSize(){
        long long DepthL = 0;
        long long SizeL = 0;
        long long DepthR = 0;
        long long SizeR = 0;
        if(!leftLeaf()){
            DepthL = leftChild->Depth;
            SizeL = leftChild->Size;
        }
        if(!rightLeaf()){
            DepthR = rightChild->Depth;
            SizeR = rightChild->Size;
        }
        Depth = std::max(DepthL, DepthR) + 1;
        Size = SizeL + SizeR + 1;
    }

    void removeLeft(){
        if(!leftLeaf()) {
            leftChild->parent = nullptr;
            leftChild = nullptr;
        }
    }

    void removeRight(){
        if(!rightLeaf()) {
            rightChild->parent = nullptr;
            rightChild = nullptr;
        }
    }

    void removeParent(){
        if(!root()){
            if(parent->leftChild == this){
                parent->removeLeft();
            }
            else {
                if (parent->rightChild == this) {
                    parent->removeRight();
                }
            }
        }
    }

    void connectLeft(Element* element){
        removeLeft();
        if(element != nullptr) {
            element->removeParent();
            element->parent = this;
        }
        leftChild = element;
    }

    void connectRight(Element* element){
        removeRight();
        if(element != nullptr) {
            element->removeParent();
            element->parent = this;
        }
        rightChild = element;
    }

    void connect(Element* element){
        if(element->value < value){
            connectLeft(element);
        }
        if(element->value > value){
            connectRight(element);
        }
        checkDepthSizeLong();
    }

    void rotateLeft(){
        Element* par = parent;
        bool left = false;
        bool right = false;
        if(par != nullptr) {
            if (par->leftChild == this){
                left = true;
            }
            if (par->rightChild == this){
                right = true;
            }
        }
        Element* center = leftChild->rightChild;
        leftChild->connectRight(this);
        if(left) {
            par->connectLeft(leftChild);
        }
        if(right) {
            par->connectRight(leftChild);
        }
        connectLeft(center);
        checkDepthSize();
        parent->checkDepthSize();
    }

    void rotateRight(){
        Element* par = parent;
        bool left = false;
        bool right = false;
        if(par != nullptr) {
            if (par->leftChild == this){
                left = true;
            }
            if (par->rightChild == this){
                right = true;
            }
        }
        Element* center = rightChild->leftChild;
        rightChild->connectLeft(this);
        if(left) {
            par->connectLeft(rightChild);
        }
        if(right) {
            par->connectRight(rightChild);
        }
        connectRight(center);
        checkDepthSize();
        parent->checkDepthSize();
    }

    void balanceWeak(){
        if(delta(this) == 2){
            if(delta(leftChild) == -1){
                leftChild->rotateRight();
                rotateLeft();
            }
            else{
                rotateLeft();
            }
        }
        if(delta(this) == -2){
            if(delta(rightChild) == 1){
                rightChild->rotateLeft();
                rotateRight();
            }
            else{
                rotateRight();
            }
        }
        checkDepthSize();
    }

    void eraseLeaf(){
        if(leftLeaf() & rightLeaf()){
            Element* par = parent;
            removeParent();
            if(par != nullptr){
                par->checkDepthSizeLong();
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
                return leftChild->binFind(x);
            }
        }
        if(value < x){
            if(rightLeaf()){
                return this;
            }
            else{
                return rightChild->binFind(x);
            }
        }
        else{
            return this;
        }
    }

    void insert(Element* element){
        Element* leaf = binFind(element->value);
        leaf->connect(element);
    }

    bool exists(long long x){
        Element* leaf = binFind(x);
        return leaf->value == x;
    }

    void balanceStrong(){
        Element* par = parent;
        balanceWeak();
        if(par != nullptr){
            par->balanceStrong();
        }
    }

    void eraseHalfLeaf() {
        if(leftLeaf() & !rightLeaf()){
            rotateRight();
            checkDepthSizeLong();
        }
        if(!leftLeaf() & rightLeaf()){
            rotateLeft();
            checkDepthSizeLong();
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
            if (par->leftChild == this){
                left = true;
            }
            if (par->rightChild == this){
                right = true;
            }
        }
        bool leftE = false;
        bool rightE = false;
        if(parE != nullptr){
            if (parE->leftChild == element){
                leftE = true;
            }
            if (parE->rightChild == element){
                rightE = true;
            }
        }
        if(leftE){
            parE->connectLeft(this);
        }
        if(rightE){
            parE->connectRight(this);
        }
        element->connectLeft(leftC);
        element->connectRight(rightC);
        if(left) {
            par->connectLeft(element);
        }
        if(right) {
            par->connectRight(element);
        }
        if(leftFix){
            element->connectLeft(this);
            parent = element;
        }
        if(rightFix){
            element->connectRight(this);
            parent = element;
        }
        connectLeft(leftCE);
        connectRight(rightCE);
    }

    void checkDepthSizeLong() {
        checkDepthSize();
        if(!root()){
            parent->checkDepthSizeLong();
        }
    }

    Element* sizeFind(long long k){
        long long SizeR = 0;
        if(!rightLeaf()){
            SizeR = rightChild->Size;
        }
        if(SizeR + 1 > k){
            return rightChild->sizeFind(k);
        }
        if(SizeR + 1 < k){
            return leftChild->sizeFind(k - SizeR - 1);
        }
        else{
            return this;
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
            DepthL = element->leftChild->Depth;
        }
        if(!(*element).rightLeaf()){
            DepthR = element->rightChild->Depth;
        }
        return DepthL - DepthR;
    }
}

Element* Next(Element* element, long long x){
    if(element == nullptr){
        return nullptr;
    }
    else{
        if(element->value <= x){
            return Next(element->rightChild, x);
        }
        else{
            Element* check = Next(element->leftChild, x);
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
        leafX->value = x;
        if(size == 0){
            root = leafX;
        }
        else{
            root->insert(leafX);
            leafX->balanceStrong();
            if(!root->root()){
                root = root->parent;
            }
        }
        size++;
    }

    bool exists(long long x) const{
        if(root != nullptr) {
            return root->exists(x);
        }
        else{
            return false;
        }
    }

    void erase(long long x){
        if(root != nullptr) {
            Element *pointerDeath = root->binFind(x);
            if(pointerDeath->value == x) {
                Element* next = Next(pointerDeath, x);
                if(root == pointerDeath){
                    if(root->rightLeaf()){
                        root = pointerDeath->leftChild;
                    }
                    else{
                        root = pointerDeath->rightChild;
                    }
                }
                if(next == nullptr){
                    next = pointerDeath;
                }
                if(pointerDeath != next){
                    pointerDeath->swap(next);
                }
                Element* par = pointerDeath->parent;
                pointerDeath->eraseHalfLeaf();
                if(par != nullptr){
                    par->balanceStrong();
                }
                size--;
                if(root != nullptr){
                    while(!root->root()){
                        root = root->parent;
                    }
                }
            }
        }
    }

    long long kMax (long long k){
        if(root != nullptr) {
            return root->sizeFind(k)->value;
        }
        else return 0;
    }
};

int main() {
    AVLTree Tree;
    std::string insert1 = "+1";
    std::string insert2 = "1";
    std::string erase = "-1";
    std::string kMax = "0";
    std::string move = "start";
    long long x;
    long long n;
    std::cin >> n;
    for(long long i = 0; i < n; i++){
        std::cin >> move;
        if(move == insert1 || move == insert2){
            std::cin >> x;
            if(!Tree.exists(x)) {
                Tree.insert(x);
            }
        }
        if(move == erase){
            std::cin >> x;
            Tree.erase(x);
        }
        if(move == kMax){
            std::cin >> x;
            std::cout << Tree.kMax(x) << std::endl;
        }
    }
    return 0;
}