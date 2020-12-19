#include <iostream>
#include <cmath>
#include <vector>
#include <initializer_list>

struct Vector;
struct Point;
class Line;

bool Equality(double x, double y){
    return (std::abs(x - y) < 0.0000001);
}

struct Point{
    double x;
    double y;

    bool operator== (const Point& point) const{
        return (Equality(x, point.x)) && Equality(y, point.y);
    }

    bool operator!= (const Point& point) const{
        return !(*this == point);
    }

    Point& operator+= (const Vector& vector);

    Point(double x, double y): x(x), y(y) {}

    Point(Line l, Line m);

    void rotate(const Point& center, double angle){
        double x1 = x - center.x;
        double y1 = y - center.y;
        x = x1 * std::cos(angle) + y1 * std::sin(angle) + center.x;
        y = y1 * std::cos(angle) - x1 * std::sin(angle) + center.y;
    }
    void reflex(const Point& center){
        x = 2 * center.x - x;
        y = 2 * center.y - y;
    }
    void reflex(const Line& axis);
    void scale(const Point& center, double coefficient){
        double x1 = x - center.x;
        double y1 = y - center.y;
        x = x1 * coefficient + center.x;
        y = y1 * coefficient + center.y;
    }
};

double abs(const Vector& v);

struct Vector{
    double x;
    double y;

    Vector(double x, double y): x(x), y(y) {}

    Vector(const Point& p1, const Point& p2){
        x = p2.x - p1.x;
        y = p2.y - p1.y;
    }

    Vector(const Vector& v) = default;

    Vector& operator= (const Vector& v) = default;

    double operator* (const Vector& v) const{
        return (x * v.y - y * v.x);
    }

    Vector operator*= (double l) {
        x *= l;
        y *= l;
        return *this;
    }

    Vector& operator+= (const Vector& v){
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector operator+ (const Vector& v){
        Vector copy = *this;
        copy += v;
        return copy;
    }

    void normalize() {
        double abs = std::sqrt(x * x + y * y);
        x /= abs;
        y /= abs;
    }
};

double abs(const Vector& v){
    return std::sqrt(v.x * v.x + v.y * v.y);
}

double segment(Point p1, Point p2){
    Vector v(p1, p2);
    return abs(v);
}

Point& Point::operator+= (const Vector& vector){
    x += vector.x;
    y += vector.y;
    return *this;
}

class Line{
    double a;
    double b;
    double c;

    // Уравнение: ax+by+c=0
public:
    Line(Point p, Vector v): a(v.y), b(-v.x){
        c = v.x * p.y - v.y * p.x;
    }

    Line(double a, double c): a(a), b(-1), c(c){}

    Line(const Point& p, double k): a(k) {
        b = -1;
        c = p.y - k * p.x;
    }

    Line(const Point& p1, const Point& p2) {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = p1.x * p2.y - p1.y * p2.x;
    }

    bool operator== (const Line& line) const {
        return Equality(a * line.c, c * line.a) && Equality(b * line.c, c * line.b);
    }

    bool operator!= (const Line& line) const {
        return !(*this == line);
    }

    double insertInEquation(const Point& point) const{
        return a * point.x + b * point.y + c;
    }

    Vector orthogonal() const{
        Vector v(a, b);
        return v;
    }
    Vector parallel() const{
        Vector v(-b, a);
        return v;
    }
    bool containsPoint(Point point) const {
        return insertInEquation(point) == 0;
    }

    Point intersect(const Line& l) const{
        double x = (b * l.c - c * l.b) / (a * l.b - b * l.a);
        double y = (c * l.a - a * l.c) / (a * l.b - b * l.a);
        Point point(x, y);
        return point;
    }
};

void Point::reflex(const Line& axis){
    Vector v = axis.orthogonal();
    double h = axis.insertInEquation(*this);
    v *= (-2 * h / (abs(v) * abs(v)));
    *this += v;
}

Point::Point(Line l, Line m){
    x = l.intersect(m).x;
    y = l.intersect(m).y;
}

class Shape{
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool operator==(const Shape& another) const = 0;
    virtual bool operator!=(const Shape& another) const = 0;
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(Point point) const = 0;
    virtual void rotate(Point center, double angle) = 0;
    virtual void reflex(Point center) = 0;
    virtual void reflex(Line axis) = 0;
    virtual void scale(Point center, double coefficient) = 0;
    virtual ~Shape() = 0;
};

Shape::~Shape() {}

class Ellipse: public Shape {
    Point focus1;
    Point focus2;
protected:
    double sum;
public:
    Ellipse(Point focus1, Point focus2, double sum): focus1(focus1), focus2(focus2), sum(sum) {}

    Point center() const {
        Point p((focus1.x + focus2.x) / 2, (focus1.y + focus2.y) / 2);
        return p;
    }

    std::pair<Point,Point> focuses() const{
        std::pair<Point,Point> focuses(focus1, focus2);
        return focuses;
    }

    double eccentricity() const {
        return segment(focus1, focus2) / sum;
    }

    Line axis() const {
        Line l(focus1, focus2);
        return l;
    }

    std::pair<Line, Line> directrices() const{
        Vector v = axis().orthogonal();
        Point p1 = focus1;
        Point p2 = focus2;
        p1.scale(center(), 1 / (eccentricity() * eccentricity()));
        p2.scale(center(), 1 / (eccentricity() * eccentricity()));
        Line l1(p1, v);
        Line l2(p2, v);
        std::pair<Line,Line> ans(l1, l2);
        return ans;
    }

    double perimeter() const override{
        double a = sum / 2;
        return 4 * a * std::comp_ellint_2(eccentricity());
    }
    double area() const override{
        double a = sum / 2;
        double b = a * std::sqrt(1 - eccentricity() * eccentricity());
        return 3.1415926 * a * b;
    }
    bool operator==(const Shape& another) const override{
        try{
            dynamic_cast<const Ellipse&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Ellipse&>(another);
        return Equality(sum, copy.sum) && (((focus1 == copy.focus1) && (focus2 == copy.focus2)) || ((focus1 == copy.focus2) && (focus2 == copy.focus1)));
    }
    bool operator!=(const Shape& another) const override{
        return !(*this == another);
    }
    bool isCongruentTo(const Shape& another) const override{
        try{
            dynamic_cast<const Ellipse&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Ellipse&>(another);
        return Equality(sum, copy.sum) && (segment(focus1, focus2) == segment(copy.focus1, copy.focus2));
    }
    bool isSimilarTo(const Shape& another) const override{
        try{
            dynamic_cast<const Ellipse&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Ellipse&>(another);
        return Equality(eccentricity(), copy.eccentricity());
    }
    bool containsPoint(Point point) const override{
        return (segment(point, focus1) + segment(point, focus2) <= sum);
    }
    void rotate(Point center, double angle) override{
        focus1.rotate(center, angle);
        focus2.rotate(center, angle);
    }
    void reflex(Point center) override{
        focus1.reflex(center);
        focus2.reflex(center);
    }
    void reflex(Line axis) override{
        focus1.reflex(axis);
        focus2.reflex(axis);
    }

    void scale(Point center, double coefficient) override{
        focus1.scale(center, coefficient);
        focus2.scale(center, coefficient);
        sum *= coefficient;
    }
    ~Ellipse() override = default;
};

class Circle: public Ellipse{
public:
    double radius() const{
        return sum / 2;
    }
    Circle(Point center, double radius) : Ellipse(center, center, 2 * radius) {}
    double perimeter() const override{
        return 2 * 3.1415926 * radius();
    }
    double area() const override{
        return 3.1415926 * radius() * radius();
    }
    bool operator==(const Shape& another) const override{
        try{
            dynamic_cast<const Circle&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Circle&>(another);
        return Equality(radius(), copy.radius()) && (center() == copy.center());
    }
    bool operator!=(const Shape& another) const override{
        return !(*this == another);
    }
    bool isCongruentTo(const Shape& another) const override{
        try{
            dynamic_cast<const Circle&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Circle&>(another);
        return (radius() == copy.radius());
    }
    bool isSimilarTo(const Shape& another) const override{
        try{
            dynamic_cast<const Circle&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Circle&>(another);
        return true;
    }
    bool containsPoint(Point point) const override{
        return (segment(point, center()) <= radius());
    }
};

class Polygon: public Shape{
protected:
    std::vector<Point> vertices;
public:
    Polygon(std::initializer_list<Point> lst){
        vertices = lst;
    }
    Polygon(std::vector<Point>& vertices): vertices(vertices){}
    const std::vector<Point>& getVertices() const{
        return vertices;
    }
    virtual double orArea() const{
        double S = 0;
        for(long long i = 2; i < verticesCount(); i++){
            Vector left(vertices[0], vertices[i - 1]);
            Vector right(vertices[0], vertices[i]);
            S += left * right;
        }
        return S;
    };
    virtual long long index(Point point) const;
    long long verticesCount() const{
        return vertices.size();
    }
    virtual bool isConvex() const{
        for(long long i = 0; i < verticesCount(); i++){
            Vector v1(vertices[i], vertices[(i + 1) % verticesCount()]);
            Vector v2(vertices[(i + 1) % verticesCount()], vertices[(i + 2) % verticesCount()]);
            Vector v3(vertices[(i + 2) % verticesCount()], vertices[(i + 3) % verticesCount()]);
            if(((v1 * v2) * (v2 * v3)) < 0){
                return false;
            }
        }
        return true;
    }
    double perimeter() const override{
        double P = 0.0;
        for(long long i = 0; i < verticesCount(); i++){
            P += segment(vertices[i], vertices[(i + 1) % verticesCount()]);
        }
        return P;
    }
    double area() const override{
        return std::abs(orArea()) / 2;
    }
    bool operator==(const Shape& another) const override{
        try{
            dynamic_cast<const Polygon&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Polygon&>(another);
        if(verticesCount() != copy.verticesCount()){
            return false;
        }
        long long start = -1;
        for(long long i = 0; i < verticesCount(); i++){
            if(copy.vertices[i] == vertices[0]){
                start = i;
            }
        }
        if(start == -1){
            return false;
        }
        bool inOrder = true;
        bool refOrder = true;
        for(long long i = 0; i < verticesCount(); i++){
            if(vertices[i] != copy.vertices[(i + start) % verticesCount()]){
                inOrder = false;
            }
        }
        for(long long i = 0; i < verticesCount(); i++){
            if(vertices[i] != copy.vertices[(verticesCount() + start - i) % verticesCount()]){
                refOrder = false;
            }
        }
        return inOrder || refOrder;
    }
    bool operator!=(const Shape& another) const override{
        return !(*this == another);
    }
    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;
    bool containsPoint(Point point) const override{
        return index(point) != 0;
    }
    void rotate(Point center, double angle) override{
        for(long long i = 0; i < verticesCount(); i++){
            vertices[i].rotate(center, angle);
        }
    }
    void reflex(Point center) override{
        for(long long i = 0; i < verticesCount(); i++){
            vertices[i].reflex(center);
        }
    }
    void reflex(Line axis) override{
        for(long long i = 0; i < verticesCount(); i++){
            vertices[i].reflex(axis);
        }
    }
    void scale(Point center, double coefficient) override{
        for(long long i = 0; i < verticesCount(); i++){
            vertices[i].scale(center, coefficient);
        }
    }
    bool isCongruentInOrder(const Polygon& another, long long d = 0) const{
        if(verticesCount() != another.verticesCount()){
            return false;
        }
        for(long long i = 0; i < verticesCount(); i++){
            if(!Equality(segment(vertices[i], vertices[(i + 3) % verticesCount()]), segment(another.vertices[(d + i) % verticesCount()], another.vertices[(d + i + 3) % verticesCount()]))){
                return false;
            }
            if(!Equality(segment(vertices[i], vertices[(i + 2) % verticesCount()]), segment(another.vertices[(d + i) % verticesCount()], another.vertices[(d + i + 2) % verticesCount()]))){
                return false;
            }
            if(!Equality(segment(vertices[i], vertices[(i + 1) % verticesCount()]), segment(another.vertices[(d + i) % verticesCount()], another.vertices[(d + i + 1) % verticesCount()]))){
                return false;
            }
        }
        return true;
    }
    bool isCongruentInOrderReverse(const Polygon& another, long long d = 0) const{
        if(verticesCount() != another.verticesCount()){
            return false;
        }
        for(long long i = 0; i < verticesCount(); i++){
            if(!Equality(segment(vertices[i], vertices[(i + 3) % verticesCount()]), segment(another.vertices[(verticesCount() + d - i) % verticesCount()], another.vertices[(2 * verticesCount() + d - i - 3) % verticesCount()]))){
                return false;
            }
            if(!Equality(segment(vertices[i], vertices[(i + 2) % verticesCount()]), segment(another.vertices[(verticesCount() + d - i) % verticesCount()], another.vertices[(2 * verticesCount() + d - i - 2) % verticesCount()]))){
                return false;
            }
            if(!Equality(segment(vertices[i], vertices[(i + 1) % verticesCount()]), segment(another.vertices[(verticesCount() + d - i) % verticesCount()], another.vertices[(2 * verticesCount() + d - i - 1) % verticesCount()]))){
                return false;
            }
        }
        return true;
    }
    bool isSimilarInOrder(const Polygon& another, long long d = 0) const{
        if(verticesCount() != another.verticesCount()){
            return false;
        }
        double segment1 = segment(vertices[0], vertices[1]);
        double segment2 = segment(another.vertices[d % verticesCount()], another.vertices[(d + 1) % verticesCount()]);
        for(long long i = 0; i < verticesCount(); i++){
            if(!Equality(segment(vertices[i], vertices[(i + 3) % verticesCount()]) * segment2, segment1 * segment(another.vertices[(d + i) % verticesCount()], another.vertices[(d + i + 3) % verticesCount()]))){
                return false;
            }
            if(!Equality(segment(vertices[i], vertices[(i + 2) % verticesCount()]) * segment2, segment1 * segment(another.vertices[(d + i) % verticesCount()], another.vertices[(d + i + 2) % verticesCount()]))){
                return false;
            }
            if(!Equality(segment(vertices[i], vertices[(i + 1) % verticesCount()]) * segment2, segment1 * segment(another.vertices[(d + i) % verticesCount()], another.vertices[(d + i + 1) % verticesCount()]))){
                return false;
            }
        }
        return true;
    }
    bool isSimilarInOrderReverse(const Polygon& another, long long d = 0) const{
        if(verticesCount() != another.verticesCount()){
            return false;
        }
        double segment1 = segment(vertices[0], vertices[1]);
        double segment2 = segment(another.vertices[d % verticesCount()], another.vertices[(verticesCount() + d - 1) % verticesCount()]);
        for(long long i = 0; i < verticesCount(); i++){
            if(!Equality(segment(vertices[i], vertices[(i + 3) % verticesCount()]) * segment2, segment1 * segment(another.vertices[(verticesCount() + d - i) % verticesCount()], another.vertices[(2 * verticesCount() + d - i - 3) % verticesCount()]))){
                return false;
            }
            if(!Equality(segment(vertices[i], vertices[(i + 2) % verticesCount()]) * segment2, segment1 * segment(another.vertices[(verticesCount() + d - i) % verticesCount()], another.vertices[(2 * verticesCount() + d - i - 2) % verticesCount()]))){
                return false;
            }
            if(!Equality(segment(vertices[i], vertices[(i + 1) % verticesCount()]) * segment2, segment1 * segment(another.vertices[(verticesCount() + d - i) % verticesCount()], another.vertices[(2 * verticesCount() + d - i - 1) % verticesCount()]))){
                return false;
            }
        }
        return true;
    }

    ~Polygon() override = default;
};

class Rectangle: public Polygon{
public:
    Rectangle(Point p0, Point p2, double tg) : Polygon({p0, p0, p2, p2}) {

        if(tg < 1){
            tg = 1 / tg;
        }
        double sin = tg / std::sqrt(1 + tg * tg);
        double cos = std::sqrt(1 - sin * sin);
        Vector diagonal(p0, p2);
        double x1 = cos * cos * diagonal.x - sin * cos * diagonal.y;
        double y1 = sin * cos * diagonal.x + cos * cos * diagonal.y;
        Vector v(x1, y1);
        vertices[1] += v;
        v *= -1;
        vertices[3] += v;
    }
    Point center(){
        Point center = vertices[2];
        center.scale(vertices[0], 0.5);
        return center;
    }
    std::pair<Line, Line> diagonals(){
        Line first(vertices[0], vertices[2]);
        Line second(vertices[1], vertices[3]);
        std::pair<Line, Line> pair(first, second);
        return pair;
    }
    bool isConvex() const override{
        return true;
    }
    double perimeter() const override{
        return 2 * (segment(vertices[0], vertices[1]) + segment(vertices[0], vertices[3]));
    }
    double area() const override{
        return segment(vertices[0], vertices[1]) * segment(vertices[0], vertices[3]);
    }
    bool isCongruentTo(const Shape& another) const override{
        try{
            dynamic_cast<const Rectangle&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Rectangle&>(another);
        double a1 = segment(vertices[0], vertices[1]);
        double b1 = segment(vertices[0], vertices[3]);
        double a2 = segment(copy.vertices[0], copy.vertices[1]);
        double b2 = segment(copy.vertices[0], copy.vertices[3]);
        return ((Equality(a1, a2) && Equality(b1, b2)) || (Equality(a1, b2) && Equality(b1, a2)));
    }
    bool isSimilarTo(const Shape& another) const override{
        try{
            dynamic_cast<const Polygon&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Rectangle&>(another);
        double a1 = segment(vertices[0], vertices[1]);
        double b1 = segment(vertices[0], vertices[3]);
        double a2 = segment(copy.vertices[0], copy.vertices[1]);
        double b2 = segment(copy.vertices[0], copy.vertices[3]);
        return (Equality(a1 / b1, a2 / b2) || Equality(a1 / b1, b2 / a2));
    }
};


class Square: public Rectangle{
public:
    Square(Point p0, Point p2): Rectangle(p0, p2, 1){}

    double perimeter() const override{
        return 2 * segment(vertices[0], vertices[1]);
    }
    double area() const override{
        return segment(vertices[0], vertices[1]) * segment(vertices[0], vertices[1]);
    }
    bool isCongruentTo(const Shape& another) const override{
        try{
            dynamic_cast<const Square&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Square&>(another);
        double a1 = segment(vertices[0], vertices[1]);
        double a2 = segment(copy.vertices[0], copy.vertices[1]);
        return Equality(a1, a2);
    }
    bool isSimilarTo(const Shape& another) const override{
        try{
            dynamic_cast<const Square&>(another);
        }
        catch (...) {
            return false;
        }
        auto copy = dynamic_cast<const Square&>(another);
        return true;
    }
    Circle circumscribedCircle(){
        Circle Omega(center(), segment(center(), vertices[0]));
        return Omega;
    }
    Circle inscribedCircle(){
        Circle omega(center(), segment(vertices[0], vertices[1]) / 2);
        return omega;
    }
};

class Triangle: public Polygon{
public:
    Triangle(Point A, Point B, Point C): Polygon({A, B, C}){}
    bool isConvex() const override{
        return true;
    }
    double orArea() const override{
        Vector v1(vertices[0], vertices[1]);
        Vector v2(vertices[0], vertices[2]);
        return v1 * v2;
    }
    long long index(Point point) const override{
        Vector side0(vertices[0], vertices[1]);
        Vector side1(vertices[1], vertices[2]);
        Vector side2(vertices[2], vertices[0]);
        Vector v0(vertices[0], point);
        Vector v1(vertices[1], point);
        Vector v2(vertices[2], point);
        if((side0 * v0 >= 0.0) && (side1 * v1 >= 0.0) && (side2 * v2 >= 0.0)){
            return 1;
        }
        if((side0 * v0 <= 0.0) && (side1 * v1 <= 0.0) && (side2 * v2 <= 0.0)){
            return -1;
        }
        return 0;
    }
    Point centroid(){
        Point m = vertices[0];
        m.scale(vertices[1], 0.5);
        m.scale(vertices[2], 2.0 / 3);
        return m;
    }
    Point orthocenter(){
        Line side1(vertices[0], vertices[2]);
        Line side2(vertices[0], vertices[1]);
        Line h1(vertices[1], side1.orthogonal());
        Line h2(vertices[2], side2.orthogonal());
        Point orthocenter(h1, h2);
        return orthocenter;
    }
    Point circumcenter(){
        Point circumcenter = orthocenter();
        circumcenter.scale(centroid(), -0.5);
        return circumcenter;
    }
    Point incenter(){
        Vector side01(vertices[0], vertices[1]);
        Vector side02(vertices[0], vertices[2]);
        Vector side10(vertices[1], vertices[0]);
        Vector side12(vertices[1], vertices[2]);
        side10.normalize();
        side12.normalize();
        side01.normalize();
        side02.normalize();
        Line bisector0(vertices[0], side01 + side02);
        Line bisector1(vertices[1], side10 + side12);
        Point incenter(bisector1, bisector0);
        return incenter;
    }
    Line EulerLine(){
        Line line(centroid(), orthocenter());
        return line;
    }
    Circle circumscribedCircle(){
        Point center = circumcenter();
        double radius = segment(center, vertices[0]);
        Circle circle(center, radius);
        return circle;
    }
    Circle inscribedCircle(){
        Point center = incenter();
        double radius = 2 * area() / perimeter();
        Circle circle(center, radius);
        return circle;
    }
    Circle ninePointsCircle(){
        Point center = circumcenter();
        double radius = segment(center, vertices[0]);
        Circle circle(center, radius);
        circle.scale(orthocenter(), 0.5);
        return circle;
    }
};

long long Polygon::index(Point point) const{
    long long index = 0;
    for(long long i = 1; i < verticesCount() - 1; i++){
        Triangle triangle(vertices[0], vertices[i], vertices[i + 1]);
        index += triangle.index(point);
    }
    return index;
}

bool Polygon::isCongruentTo(const Shape& another) const{
    try{
        dynamic_cast<const Polygon&>(another);
    }
    catch (...) {
        return false;
    }
    auto copy = dynamic_cast<const Polygon&>(another);
    for(long long i = 0; i < verticesCount(); i++){
        if(isCongruentInOrder(copy, i)){
            return true;
        }
        if(isCongruentInOrderReverse(copy, i)){
            return true;
        }
    }
    return false;
}
bool Polygon::isSimilarTo(const Shape& another) const{
    try{
        dynamic_cast<const Polygon&>(another);
    }
    catch (...) {
        return false;
    }
    auto copy = dynamic_cast<const Polygon&>(another);
    for(long long i = 0; i < verticesCount(); i++){
        if(isSimilarInOrder(copy, i)){
            return true;
        }
        if(isSimilarInOrderReverse(copy, i)){
            return true;
        }
    }
    return false;
}