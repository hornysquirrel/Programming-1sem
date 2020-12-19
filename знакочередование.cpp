#include <iostream>

// Реализуйте структуру данных из n элементов a1,a2…an, поддерживающую следующие операции:
//
//присвоить элементу ai значение j;
//найти знакочередующуюся сумму на отрезке от l до r включительно (al−al+1+al+2−…±ar).
//Входные данные
//В первой строке входного файла содержится натуральное число n (1≤n≤105) — длина массива. Во второй строке записаны начальные значения элементов (неотрицательные целые числа, не превосходящие 104).
//
//В третьей строке находится натуральное число m (1≤m≤105) — количество операций. В последующих m строках записаны операции:
//
//операция первого типа задается тремя числами 0 i j (1≤i≤n, 1≤j≤104).
//операция второго типа задается тремя числами 1 l r (1≤l≤r≤n).
//Выходные данные
//Для каждой операции второго типа выведите на отдельной строке соответствующую знакочередующуюся сумму.

class SegmentTree{
    SegmentTree* left;
    SegmentTree* right;
    long long l;
    long long r;
    long long sum;
    bool leaf(){
        return (left == nullptr) && (right == nullptr);
    }
public:
    SegmentTree(long long n, long long value){
        left = nullptr;
        right = nullptr;
        l = n;
        r = n;
        sum = value;
    }

    SegmentTree(SegmentTree& Left, SegmentTree& Right){
        left = &Left;
        right = &Right;
        l = Left.l;
        r = Right.r;
        if(Left.l % 2 == Right.l % 2){
            sum = Left.sum + Right.sum;
        }
        else{
            sum = Left.sum - Right.sum;
        }
    }

    long long getSum(long long L, long long R){
        if(l == L && r == R){
            return sum;
        }
        if(R < right->l){
            return left->getSum(L, R);
        }
        if(L > left->r){
            return right->getSum(L, R);
        }
        long long sumL = left->getSum(L, left->r);
        long long sumR = right->getSum(right->l, R);
        long long Sum;
        if(right->l % 2 == L % 2){
            Sum = sumL + sumR;
        }
        else{
            Sum = sumL - sumR;
        }
        return Sum;
    }

    void set(long long n, long long value){
        if(leaf()){
            sum = value;
        }
        else{
            if(n < right->l){
                left->set(n, value);
            }
            if(n > left->r){
                right->set(n, value);
            }
            if(right->l % 2 == left->l % 2){
                sum = left->sum + right->sum;
            }
            else{
                sum = left->sum - right->sum;
            }
        }
    }
};

SegmentTree& create(long long L, long long R, long long* array){
    if(L == R){
        SegmentTree* tree = new SegmentTree(L, array[L]);
        return *tree;
    }
    long long M = (L + R) / 2;
    SegmentTree* tree = new SegmentTree(create(L, M, array), create(M + 1, R, array));
    return *tree;
}

int main() {
    long long n;
    std::cin >> n;
    long long a[n];
    for(long long i = 0; i < n; i++){
        std::cin >> a[i];
    }
    SegmentTree Tree = create(0, n - 1, a);
    long long m;
    std::cin >> m;
    long long x;
    long long y;
    bool move;
    for(long long i = 0; i < m; i++){
        std::cin >> move;
        std::cin >> x;
        std::cin >> y;
        if(move){
            x--;
            y--;
            std::cout << Tree.getSum(x, y) << std::endl;
        }
        else{
            x--;
            Tree.set(x, y);
        }
    }
    return 0;
}