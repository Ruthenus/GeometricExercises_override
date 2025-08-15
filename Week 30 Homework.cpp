/**
 * 4. Створіть базовий клас Фігура. Створіть похідні від нього Точка,
 * Пряма, Еліпс, Багатокутник. Створіть похідний від Еліпса клас Коло.
 * Створіть похідні від Багатокутника класи Трикутник і Чотирикутник.
 * Створіть похідні від Трикутника класи ЗвичайнийГострокутнийТрикутник,
 * РівнобедренийГострокутнийТрикутник, РівностороннійГострокутнийТрикутник,
 * ЗвичайнийТупокутнийТрикутник, РівнобедренийТупокутнийТрикутник,
 * ЗвичайнийПрямокутнийТрикутник, РівнобедренийПрямокутнийТрикутник.
 * Створіть похідні від Чотирикутника класи Паралелограм, Трапеція і
 * Дельтоїд. Створіть похідні від Паралелограма класи Прямокутник і Ромб.
 * Створіть похідний від Прямокутника клас Квадрат. У базовому класі Фігура
 * потрібно зробити методи Площа, Периметр, Намалювати.
 * У похідних класах - перевизначити ці методи.
 */

#define _USE_MATH_DEFINES  // pi = 3.14159265358979323846
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>  // я купив книгу "Грокаємо функційне програмування",
 // метод заливки Фігури, створення трикутників через шаблонний клас
 // https://devzone.org.ua/post/funktsionalne-prohramuvannia-maye-staty-nashym-holovnym-priorytetom-v-2015-rotsi
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// Попереднє оголошення класу Point для використання в GeometryUtils
class Point;

// Допоміжний клас для повторюваних геометричних обчислень
class GeometryUtils
{
public:
    // Допустима похибка під час порівняння чисел з плаваючою комою
    static constexpr double EPSILON = 1e-10;
    // Обчислення відстані між двома точками p1 і p2
    static double Distance(const Point& p1, const Point& p2);
    // Перевірка, чи паралельні дві лінії, задані точками
    static bool AreParallel(const Point& p1, const Point& p2,
        const Point& p3, const Point& p4);
    // Обчислення кута між трьома точками в градусах
    static double AngleBetweenPoints(const Point& a, const Point& b,
        const Point& c);
    // Перевірка, чи перетинаються два відрізка (p1, p2) і (p3, p4)
    static bool SegmentsIntersect(const Point& p1, const Point& p2,
        const Point& p3, const Point& p4);
    // Перевірка, чи багатокутник, заданий вектором вершин, є опуклим
    static bool IsConvexPolygon(const vector<Point>& vertices);
    // Завжди можна розділити увігнутий багатогокутник на множину 
    // опуклих багатокутників (українська Вікіпедія).
    // Перевірка, чи багатокутник є простим (немає перетинів та дірок)
    static bool IsSimplePolygon(const vector<Point>& vertices);
};


// Абстрактний клас Фігура - базовий клас для всіх фігур
class Shape
{
protected:
    string name;  // назва фігури
    static constexpr int GRID_WIDTH = 20;   // ширина координатної сітки
    static constexpr int GRID_HEIGHT = 20;  // висота сітки

    // Побудова відрізка лінії на сітці за алгоритмом Брезенхема
    static void DrawSegmentOnGrid(vector<string>& grid, const Point& p1,
        const Point& p2);
    // https://de.wikipedia.org/wiki/Rasterung_von_Linien
    // Заповнення області на сітці за умовою isInside
    static void FillShapeOnGrid(vector<string>& grid, const
        function<bool(double, double)>& isInside);
    // https://www.arthuss.com.ua/shop/hrokayemo-funktsiyne-prohramuvannya
    // Обчислення площі багатокутника за всім відомою формулою Гаусса
    static double CalculatePolygonArea(const vector<Point>& vertices);
    // Обчислення периметра багатокутника
    static double CalculatePolygonPerimeter(const vector<Point>& vertices);

public:
    // Конструктор з параметром за замовчуванням:
    // Shape(const string& n = "Фігура") { this->name = n; }
    Shape(const string& n = "Фігура") : name(n) {}  // ефективніший!
    // Конструктор копіювання не потрібен, оскільки Shape є абстрактним класом

    // Методи-аксесори:
    void SetName(const string& n) { this->name = n; }
    string GetName() const { return this->name; }
    // https://stackoverflow.com/questions/2391679/why-do-we-need-virtual-functions-in-c

    // Віртуальні методи для площі, периметра, малювання фігури:
    virtual double Area() const = 0;
    // https://stackoverflow.com/questions/39288839/why-in-c-virtual-and-0-is-both-needed-to-describe-a-method-is-abstract
    virtual double Perimeter() const = 0;
    virtual void Draw() const = 0;
    // Віртуальний метод для перевірки коректності фігури
    virtual bool IsValid() const = 0;
    // Віртуальний метод для зміни вершин (не всі фігури мають вершини!)
    virtual void SetVertices(const vector<Point>& newVertices) {}
    // Віртуальний метод для зміни однієї вершини
    virtual void SetVertex(size_t index, const Point& p) {}
    // https://habr.com/ru/companies/otus/articles/739250/

    // Віртуальний деструктор,
    virtual ~Shape() = default;  // що не потребує спеціальної логіки!
    // https://stackoverflow.com/questions/34383516/should-i-default-virtual-destructors
    // https://stackoverflow.com/questions/13576055/how-is-default-different-from-for-default-constructor-and-destructor
};


// Похідний клас Точка
class Point final : public Shape
{
private:  // похідного класу від точки не буде!
    double x, y;  // координати точки
public:
    // Конструктор за координатами
    Point(double x = 0.0, double y = 0.0, const string& n = "Точка") :
        Shape(n), x(x), y(y) {
    }
    // https://en.cppreference.com/w/cpp/language/initializer_list.html

    // Конструктор за вектором координат із делегуванням
    Point(const vector<double>& coords, const string& n = "Точка") :
        Point(coords.size() >= 2 ? coords[0] : 0.0,
            coords.size() >= 2 ? coords[1] : 0.0, n)
    {
        if (coords.size() < 2)
            throw invalid_argument("Для визначення точки потрібні 2 координати!");
    }
    // https://en.cppreference.com/w/cpp/error/invalid_argument.html


    // Конструктор копіювання з використанням покажчика this:
    //Point(const Point& other) : Shape(other.GetName())
    //{
    //    this->x = other.x; this->y = other.y;
    //}

    // Ефективніший конструктор копіювання
    Point(const Point& other) : Shape(other), x(other.x), y(other.y) {}

    // Додавання оператора порівняння, щоб запрацювало порівняння трикутників
    bool operator==(const Point& other) const {
        return abs(this->x - other.x) < GeometryUtils::EPSILON &&
            abs(this->y - other.y) < GeometryUtils::EPSILON;
    }

    // Сетери
    void SetX(double newX) { this->x = newX; }
    void SetY(double newY) { this->y = newY; }
    void SetCoordinates(double newX, double newY)
    {
        this->x = newX; this->y = newY;
    }
    void SetCoordinates(const vector<double>& coords)
    {
        if (coords.size() < 2) throw
            invalid_argument("Для визначення точки потрібні 2 координати!");
        this->x = coords[0]; this->y = coords[1];
    }
    void SetVertices(const vector<Point>& newVertices) override {
        throw logic_error("Точка не підтримує множину вершин!");
        // https://en.cppreference.com/w/cpp/error/logic_error.html
    }
    // Гетери
    double GetX() const { return x; }
    double GetY() const { return y; }

    // Геометричні методи: площа й периметр точки дорівнюють нулю!
    double Area() const override { return 0.0; }
    double Perimeter() const override { return 0.0; }

    // Візуалізація: точка (0, 0) має бути в центрі сітки
    void Draw() const override
    {
        cout << this->GetName() << " в (" << this->x << ", "
            << this->y << ")\n";
        // Ініціалізація координатної сітки символами '.'
        vector<string> grid(GRID_HEIGHT, string(GRID_WIDTH, '.'));
        // Перетворення координат у індекси сітки – стовпчики та рядки:
        int ix = static_cast<int>(round(this->x + GRID_WIDTH / 2.0));
        int iy = static_cast<int>(round(this->y + GRID_HEIGHT / 2.0));
        // this->x і this->y — це координати точки в декартовій системі!
        // Позначення позиції точки, якщо вона в межах сітки
        if (ix >= 0 && ix < GRID_WIDTH && iy >= 0 && iy < GRID_HEIGHT) {
            grid[iy][ix] = '+';
        }
        // Виведення сітки знизу вгору
        for (int row = GRID_HEIGHT - 1; row >= 0; --row) {
            cout << grid[row] << "\n";
        }
        cout << "\n\n\n";
    }

    // Перевірка, чи точка в межах сітки
    bool IsValid() const override {
        return abs(x) <= GRID_WIDTH / 2.0 && abs(y) <= GRID_HEIGHT / 2.0;
    }

    ~Point() override = default;  // деструктор
};


// Реалізація методів GeometryUtils після визначення Point

// Обчислює евклідову відстань між двома точками.
// https://mathworld.wolfram.com/Distance.html
double GeometryUtils::Distance(const Point& p1, const Point& p2)
{
    return sqrt(pow(p2.GetX() - p1.GetX(), 2) + pow(p2.GetY() - p1.GetY(), 2));
}  // застосування теореми Піфагора


// Приймає за основу п'ятий постулат.
// https://mathworld.wolfram.com/ParallelPostulate.html
bool GeometryUtils::AreParallel(const Point& p1, const Point& p2,
    const Point& p3, const Point& p4)
{
    // https://stackoverflow.com/questions/1560492/how-to-tell-whether-a-point-is-to-the-right-or-left-side-of-a-line
    double v1x, v1y, v2x, v2y;
    v1x = p2.GetX() - p1.GetX(); v1y = p2.GetY() - p1.GetY();
    v2x = p4.GetX() - p3.GetX(); v2y = p4.GetY() - p3.GetY();

    // Два вектори v1(p1, p2) і v2(p3, p4) є паралельними, якщо їхній 
    // векторний добуток дорівнює нулю.
    // https://mathworld.wolfram.com/CrossProduct.html
    if ((v1x == 0 && v1y == 0) || (v2x == 0 && v2y == 0))
        throw invalid_argument("Один із відрізків вироджений!");

    return abs(v1x * v2y - v1y * v2x) < EPSILON;
}

// За відомою з 9 класу теоремою косинусів обчислює кут при вершині b 
// у трикутнику abc. Повертає кут у градусах.
// https://mathworld.wolfram.com/LawofCosines.html
double GeometryUtils::AngleBetweenPoints(const Point& a, const Point& b,
    const Point& c)
{
    double ab = Distance(a, b);
    double bc = Distance(b, c);
    double ca = Distance(c, a);
    if (ab < EPSILON || bc < EPSILON || ca < EPSILON) {
        throw invalid_argument("Точки збігаються, кут не визначений!");
    }

    double cosValue = (ab * ab + bc * bc - ca * ca) / (2 * ab * bc);
    if (cosValue < -1.0 - EPSILON) cosValue = -1.0;
    if (cosValue > 1.0 + EPSILON) cosValue = 1.0;

    return acos(cosValue) * 180.0 / M_PI;
}


// Реалізація класичного геометричного алгоритму на основі 
// орієнтації точок і перевірки колінеарності.
// Перевіряє, чи перетинаються відрізки (p1, p2) і (p3, p4).
// Повертає true, якщо відрізки перетинаються, і false в іншому разі.
// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
bool GeometryUtils::SegmentsIntersect(const Point& p1, const Point& p2,
    const Point& p3, const Point& p4)
{
    // Орієнтація трійки точок дозволяє визначити, чи точка лежить 
    // зліва, справа або на прямій, утвореній двома іншими точками.
    // Якщо орієнтації точок змінюються між парами, то відрізки перетинаються.
    // Якщо орієнтація нульова (тобто точки колінеарні), перевіряється, 
    // чи точка лежить на відрізку.

    // Обчислення орієнтацій для всіх комбінацій точок
    // https://mathworld.wolfram.com/VectorOrientation.html

    // Перевірка на виродженість відрізків
    if (p1 == p2 || p3 == p4) {
        throw invalid_argument("Вироджений відрізок: "
            "кінцеві точки збігаються!");
    }

    double val1 = (p2.GetX() - p1.GetX()) * (p3.GetY() - p2.GetY()) -
        (p2.GetY() - p1.GetY()) * (p3.GetX() - p2.GetX());
    int o1 = (abs(val1) < EPSILON) ? 0 : (val1 > 0 ? 1 : -1);
    // val1 — детермінант, що визначає орієнтацію трійки точок (p1, p2, p3).
    // Формула: val1 = (x2 − x1)(y3 − y2) - (y2 − y1)(x3 − x2),
    // де (x1, y1), (x2, y2), (x3, y3) — координати p1, p2, p3;
    // o1 — результат: 0 (колінеарні), 1 (проти годинникової стрілки), 
    // -1 (за годинниковою стрілкою).
    // Представляє 2D векторний добуток векторів (p2−p1) та (p3−p2).

    // Аналогічно обчислюються o2, o3, o4 для інших трійок точок.
    // https://reference.wolfram.com/language/ref/Cross.html

    // val2 — детермінант, що визначає орієнтацію трійки точок (p1, p2, p4).
    // Формула: (x2 − x1)(y4 − y2) - (y2 − y1)(x4 − x2);
    // Представляє 2D векторний добуток векторів (p2−p1) та (p4−p2).
    double val2 = (p2.GetX() - p1.GetX()) * (p4.GetY() - p2.GetY()) -
        (p2.GetY() - p1.GetY()) * (p4.GetX() - p2.GetX());
    int o2 = (abs(val2) < EPSILON) ? 0 : (val2 > 0 ? 1 : -1);

    // val3 — детермінант, що визначає орієнтацію трійки точок (p3, p4, p1).
    // Формула: (x4 − x3)(y1 − y4) - (y4 − y3)(x1 − x4);
    // Представляє 2D векторний добуток векторів (p4−p3) та (p1−p4).
    double val3 = (p4.GetX() - p3.GetX()) * (p1.GetY() - p4.GetY()) -
        (p4.GetY() - p3.GetY()) * (p1.GetX() - p4.GetX());
    int o3 = (abs(val3) < EPSILON) ? 0 : (val3 > 0 ? 1 : -1);

    // val4 — детермінант, що визначає орієнтацію трійки точок (p3, p4, p2).
    // Формула: (x4 − x3)(y2 − y4) - (y4 − y3)(x2 − x4);
    // Представляє 2D векторний добуток векторів (p4−p3) та (p2−p4).
    double val4 = (p4.GetX() - p3.GetX()) * (p2.GetY() - p4.GetY()) -
        (p4.GetY() - p3.GetY()) * (p2.GetX() - p4.GetX());
    int o4 = (abs(val4) < EPSILON) ? 0 : (val4 > 0 ? 1 : -1);

    // Загальний випадок: відрізки перетинаються, якщо орієнтації різні
    if (o1 != o2 && o3 != o4) {
        return true;
    }
    // https://mathworld.wolfram.com/Line-LineIntersection.html

    // Спеціальні випадки (колінеарність)
    // Перевіряється, чи одна з точок лежить на іншому відрізку:
    if (o1 == 0 &&
        p3.GetX() <= max(p1.GetX(), p2.GetX()) &&
        p3.GetX() >= min(p1.GetX(), p2.GetX()) &&
        p3.GetY() <= max(p1.GetY(), p2.GetY()) &&
        p3.GetY() >= min(p1.GetY(), p2.GetY())) {
        return true;
    }
    if (o2 == 0 &&
        p4.GetX() <= max(p1.GetX(), p2.GetX()) &&
        p4.GetX() >= min(p1.GetX(), p2.GetX()) &&
        p4.GetY() <= max(p1.GetY(), p2.GetY()) &&
        p4.GetY() >= min(p1.GetY(), p2.GetY())) {
        return true;
    }
    if (o3 == 0 &&
        p1.GetX() <= max(p3.GetX(), p4.GetX()) &&
        p1.GetX() >= min(p3.GetX(), p4.GetX()) &&
        p1.GetY() <= max(p3.GetY(), p4.GetY()) &&
        p1.GetY() >= min(p3.GetY(), p4.GetY())) {
        return true;
    }
    if (o4 == 0 &&
        p2.GetX() <= max(p3.GetX(), p4.GetX()) &&
        p2.GetX() >= min(p3.GetX(), p4.GetX()) &&
        p2.GetY() <= max(p3.GetY(), p4.GetY()) &&
        p2.GetY() >= min(p3.GetY(), p4.GetY())) {
        return true;
    }
    // Реалізовано через перевірку координат: точка має бути в межах 
    // мінімальних і максимальних координат кінців відрізка.

    // Відрізки не перетинаються
    return false;

    // Часова складність алгоритму SegmentsIntersect є O(1), оскільки він 
    // виконує фіксовану кількість операцій для будь-яких вхідних даних.
}


// Перевіряє, чи є багатокутник опуклим.
// https://mathworld.wolfram.com/ConvexPolygon.html
// Параметри: vertices - вектор вершин багатокутника типу Point.
// Опуклий багатокутник — це багатокутник, у якому всі внутрішні кути 
// менші за 180 градусів, і жодна з його сторін не "вгинається" всередину.
// В опуклому багатокутнику будь-який відрізок, що з'єднує дві точки 
// всередині, повністю лежить у багатокутнику.
// Алгоритм використовує векторний добуток (cross product) для перевірки 
// опуклості шляхом аналізу орієнтації послідовних трійок вершин.
// https://stackoverflow.com/questions/471962/how-do-i-efficiently-determine-if-a-polygon-is-convex-non-convex-or-complex
// https://math.stackexchange.com/questions/1743995/determine-whether-a-polygon-is-convex-based-on-its-vertices
bool GeometryUtils::IsConvexPolygon(const vector<Point>& vertices)
{
    // Перевірка базового випадку: багатокутник з менш ніж 3 вершинами 
    // неможливий!
    if (vertices.size() < 3) return false;
    // https://mathworld.wolfram.com/HappyEndProblem.html

    // Зберігаємо кількість вершин для зручності:
    size_t n = vertices.size();
    // Змінна для зберігання знаку векторного добутку (1, -1 або 0).
    int sign = 0;

    // Ітеруємося по всіх вершинах, розглядаючи трійки послідовних точок.
    for (size_t i = 0; i < n; ++i) {
        // Вибираємо поточну трійку вершин: p1, p2, p3.
        // Використовуємо модуль (%) для циклічного доступу до вершин.
        Point p1 = vertices[i];
        Point p2 = vertices[(i + 1) % n];
        Point p3 = vertices[(i + 2) % n];

        // Обчислюємо векторний добуток для векторів (p2 - p1) та (p3 - p2).
        double cross = (p2.GetX() - p1.GetX()) * (p3.GetY() - p2.GetY()) -
            (p2.GetY() - p1.GetY()) * (p3.GetX() - p2.GetX());

        // Визначаємо знак векторного добутку:
        // 0 (колінеарність), 1 (проти годинникової), -1 (за годинниковою).
        int currSign = (abs(cross) < EPSILON) ? 0 : (cross > 0 ? 1 : -1);
        // Алгоритм допускає колінеарні точки (currSign = 0), що є правильним, 
        // оскільки опуклий багатокутник може мати колінеарні вершини.

        // Якщо знак ще не встановлено, фіксуємо поточний знак:
        if (sign == 0) sign = currSign;
        // Якщо знак змінюється (і поточний знак не нульовий), 
        else if (currSign != 0 && currSign != sign)
            return false; // багатокутник не опуклий.
    }

    // Якщо всі знаки консистентні, багатокутник опуклий!
    return true;

    // Опуклий багатокутник має властивість, що всі його внутрішні кути 
    // повертають в одному напрямку (за або проти годинникової стрілки) 
    // під час обходу вершин. Водночас зміна знаку векторного добутку 
    // вказує на зміну напрямку повороту, що свідчить про увігнутість 
    // (внутрішній кут > 180°).
}


// Перевіряє, чи є багатокутник простим (тобто не має самоперетинів).
// Багатокутник називається простим (або жордановим многокутником), 
// якщо єдиними точками площини, що належать двом ребрам многокутника, 
// є вершини многокутника.
// https://mathworld.wolfram.com/SimplePolygon.html
// Параметри: vertices: вектор вершин багатокутника типу Point.
// Повертає: true, якщо багатокутник простий, false — якщо має 
// самоперетини або недостатньо вершин.
bool GeometryUtils::IsSimplePolygon(const vector<Point>& vertices)
{
    // Перевірка: багатокутник повинен мати принаймні 3 вершини
    if (vertices.size() < 3) return false;

    // Зберігаємо кількість вершин для зручності:
    size_t n = vertices.size();

    // Перебираємо всі вершини i для формування відрізка (i, i+1):
    for (size_t i = 0; i < n; ++i) {
        // Перебираємо всі вершини j, які не є сусідніми з i, 
        // для формування відрізка (j, j+1):
        for (size_t j = i + 2; j < n; ++j) {
            // Пропускаємо перевірку для суміжних вершин 
            // (перша та остання вершина замикають багатокутник):
            if (i == 0 && j == n - 1) continue;

            // Перевіряємо, чи перетинаються відрізки (i, i+1) та (j, j+1):
            if (SegmentsIntersect(vertices[i], vertices[(i + 1) % n],
                vertices[j], vertices[(j + 1) % n])) {
                // Якщо є перетин, багатокутник не є простим!
                return false;
            }
            // Використання % n забезпечує замикання багатокутника: 
            // остання вершина з'єднується з першою.
        }
    }

    // Якщо самоперетинів не знайдено, багатокутник є простим!
    return true;

    // https://stackoverflow.com/questions/4001745/testing-whether-a-polygon-is-simple-or-complex
    // Базовий алгоритм має часову складність O(n^2), де n — кількість 
    // вершин, оскільки перевіряються всі пари несуміжних відрізків.
    // Алгоритм Бентлі - Оттманна: використовує метод скануючої прямої
    // (sweep line) для пошуку всіх перетинів ребер зі складністю 
    // O((n + k)log⁡n). Це корисно для великих багатокутників, 
    // де k - кількість перетинів невелика.
    // https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm
    // Алгоритм Чазелла: теоретично лінійний час O(n), але складний для 
    // реалізації через велику кількість граничних випадків.
}


// Реалізація статичних методів Shape

// Малює відрізок між двома точками p1 і p2 на дискретній сітці, 
// використовуючи алгоритм Брезенхема.
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
// Сітка представлена як вектор рядків, де кожен рядок — це ряд сітки.
void Shape::DrawSegmentOnGrid(vector<string>& grid, const Point& p1,
    const Point& p2)
{
    // Алгоритм Брезенхема працює за принципом інкрементального наближення!

    // Перевірка коректності сітки перед початком роботи алгоритму
    if (grid.empty() || (grid.size() > 0 && grid[0].empty()) ||
        GRID_HEIGHT <= 0 || GRID_WIDTH <= 0) {
        return;  // ранній вихід для некоректної сітки
    }
    // https://learn.microsoft.com/en-us/cpp/standard-library/vector-class?view=msvc-170

    // Перетворення координат точок p1 і p2 у координати сітки.
    // Зсув початку координат до центру сітки, подальше заокруглення
    // до найближчого цілого числа для вирівнювання по сітці.
    int x1 = static_cast<int>(round(p1.GetX() + GRID_WIDTH / 2.0));
    int y1 = static_cast<int>(round(p1.GetY() + GRID_HEIGHT / 2.0));
    int x2 = static_cast<int>(round(p2.GetX() + GRID_WIDTH / 2.0));
    int y2 = static_cast<int>(round(p2.GetY() + GRID_HEIGHT / 2.0));

    // Виняток, якщо обидві точки знаходяться поза межами сітки
    if ((x1 < 0 || x1 >= GRID_WIDTH || y1 < 0 || y1 >= GRID_HEIGHT) &&
        (x2 < 0 || x2 >= GRID_WIDTH || y2 < 0 || y2 >= GRID_HEIGHT)) {
        throw invalid_argument("Відрізок повністю поза сіткою, "
            "малювання не потрібне!");
    }
    // Можна було б додати окрему обробку для часткового виходу (обрізання).

    // Абсолютна різниця між початковою та кінцевою точками по осях X та Y
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    // Визначення напрямку кроку по осі X: +1 (праворуч) або -1 (ліворуч)
    int sx = x1 < x2 ? 1 : -1;
    // Визначення напрямку кроку по осі Y: +1 (вниз) або -1 (вгору)
    int sy = y1 < y2 ? 1 : -1;

    /*
        // Ініціалізація помилки для алгоритму Брезенхема, яка визначає,
        // чи робити крок по X чи Y.
        int err = dx - dy;  // яка вісь є домінуючою?
    */

    // Обробка виродженого випадку, коли точки збігаються
    if (dx == 0 && dy == 0) {
        // Перевірка, чи точка в межах сітки
        if (x1 >= 0 && x1 < GRID_WIDTH && y1 >= 0 && y1 < GRID_HEIGHT)
            grid[y1][x1] = '+';  // позначення точки
        return;  // завершення, коли відрізок є точкою
    }

    /*  ЗАЦИКЛЕННЯ ПІД ЧАС МАЛЮВАННЯ ТРИКУТНИКІВ З ВИХОДОМ ТОЧКИ ЗА МЕЖІ СІТКИ
     *
     *   Коли значення dx і dy були близькими, помилка err накопичувалася
     *   таким чином, що алгоритм некоректно визначав, коли робити крок
     *   по X чи Y. Це призводило до того, що координати (x1, y1) не досягали
     *   кінцевої точки (x2, y2), викликаючи нескінченний цикл.

        while (true) {
            // Перевірка, чи поточна точка (x1, y1) знаходиться в межах сітки
            if (x1 >= 0 && x1 < GRID_WIDTH && y1 >= 0 && y1 < GRID_HEIGHT)
                grid[y1][x1] = '+';  // позначення символом на сітці
            // Перевірка, чи досягнуто кінцевої точки відрізка (x2, y2)
            if (x1 == x2 && y1 == y2) break;  // відрізок намальовано!
            // Подвоєння помилки для порівняння без дробових чисел
            int err2 = 2 * err;
            // Потрібно зробити крок по осі X, щоб мінімізувати помилку?
            if (err2 > -dy) {
                err -= dy;  // оновлення помилки, щоб збалансувати відхилення
                x1 += sx;   // крок по осі X у напрямку sx (+1 або -1)
            }
            // Потрібно зробити крок по осі Y, щоб мінімізувати помилку?
            if (err < dx) {
                err += dx;  // балансуємо між кроками по осях X і Y
                y1 += sy;   // робимо крок по осі Y у напрямку sy (+1 або -1)
            }
        }
    */

    // Визначення основної осі: X (якщо dx > dy) або Y (якщо dx <= dy)
    bool xMajor = dx > dy;
    // Основна та другорядна відстані для вибору осі
    int majorDelta = xMajor ? dx : dy;  // відстань по основній осі
    int minorDelta = xMajor ? dy : dx;  // відстань по другорядній осі
    // Координати для основної та другорядної осей
    int& majorCoord = xMajor ? x1 : y1;  // основна координата
    int& minorCoord = xMajor ? y1 : x1;  // другорядна координата
    // Напрямки кроків для основної та другорядної осей
    int majorStep = xMajor ? sx : sy;  // крок по основній осі (+1 або -1)
    int minorStep = xMajor ? sy : sx;  // крок по другорядній осі (+1 або -1)
    // Кінцеві координати для перевірки завершення
    int majorEnd = xMajor ? x2 : y2;  // кінцева координата основної осі
    int minorEnd = xMajor ? y2 : x2;  // Кінцева координата другорядної осі

    // Ініціалізація помилки для алгоритму Брезенхема:
    int err = 2 * minorDelta - majorDelta;
    // Помилка визначає, чи робити крок по другорядній осі.

    // Динамічний ліміт ітерацій, залежний від розміру сітки, для захисту 
    // від нескінченного циклу, як у закоментованій реалізації алгоритму:
    const int MAX_ITERATIONS = 2 * max(GRID_WIDTH, GRID_HEIGHT);
    int iteration = 0;  // лічильник ітерацій

    // Основний цикл адаптації алгоритму Брезенхема для малювання відрізка
    while (true) {
        // Перевірка на неперевищення ліміту ітерацій
        if (iteration++ > MAX_ITERATIONS) {
            cout << "Перевищено ліміт ітерацій! Координати: "
                << "x1=" << x1 << ", y1=" << y1 << "\n";
            break;  // вихід із циклу для уникнення зависання консолі (було)
        }
        // Встановлення точки (x1, y1) на сітці, якщо координати в межах
        if (x1 >= 0 && x1 < GRID_WIDTH && y1 >= 0 && y1 < GRID_HEIGHT)
            grid[y1][x1] = '+';
        // Перевірка, чи досягнуто кінцевої точки відрізка (x2, y2)
        if (x1 == x2 && y1 == y2) break;  // відрізок намальовано!
        // Завжди робимо крок по основній осі
        majorCoord += majorStep;  // крок у напрямку majorStep (+1 або -1)
        // Перевірка, чи потрібно зробити крок по другорядній осі
        if (err >= 0) {
            minorCoord += minorStep;  // крок по другорядній осі (+1 або -1)
            err -= 2 * majorDelta;    // оновлення помилки для балансування
        }
        // Оновлення помилки для наступної ітерації
        err += 2 * minorDelta;  // накопичення помилки по другорядній осі
    }

    // https://stackoverflow.com/questions/35422997/understanding-bresenhams-error-accumulation-part-of-the-algorithm
    // Алгоритм Брезенхема є оптимальним для малювання ліній на 
    // дискретній сітці, оскільки він відвідує мінімальну кількість 
    // пікселів, необхідних для створення лінії.
    // Часова складність є лінійною відносно довжини відрізка.
    // Просторова складність залежить від розміру сітки, але сам 
    // алгоритм використовує лише константну додаткову пам'ять O(1).
    // https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm
}


// Метод заповнює сітку символами '+' у тих координатах, де умова 
// isInside(x, y) виконується. 
// Параметр isInside - це функційний об'єкт (std::function), який визначає, 
// чи точка (x, y) належить фігурі.
// Клас-шаблон function зі стандартної бібліотеки - це узагальнений контейнер 
// (обгортка) для будь-якого виклику функції з відповідною сигнатурою. Це 
// дозволяє зберігати, передавати і викликати функції як об'єкти.
// https://learn.microsoft.com/ru-ru/cpp/standard-library/functional?view=msvc-170
// https://learn.microsoft.com/ru-ru/cpp/standard-library/function-class?view=msvc-170
// Парадигма функційного програмування: використання функцій як аргументів 
// для абстракції логіки (функції вищого порядку - higher-order functions).
// Порівняння функційного підходу з ООП:
// https://www.geeksforgeeks.org/functional-programming-paradigm/
void Shape::FillShapeOnGrid(vector<string>& grid,
    const function<bool(double, double)>& isInside)
{
    if (grid.empty() || (grid.size() > 0 && grid[0].empty()) ||
        GRID_HEIGHT <= 0 || GRID_WIDTH <= 0) {
        return;  // ранній вихід для некоректної сітки
    }
    for (double y = -GRID_HEIGHT / 2.0; y <= GRID_HEIGHT / 2.0; ++y) {
        for (double x = -GRID_WIDTH / 2.0; x <= GRID_WIDTH / 2.0; ++x) {
            if (isInside(x, y)) {  // абстрагування від геометрії фігури
                int ix = static_cast<int>(round(x + GRID_WIDTH / 2.0));
                int iy = static_cast<int>(round(y + GRID_HEIGHT / 2.0));
                if (ix >= 0 && ix < GRID_WIDTH && iy >= 0 && iy < GRID_HEIGHT)
                {
                    grid[iy][ix] = '+';  // позначення точки на сітці
                }
            }
        }
    }
    // Загальна алгоритмічна складність: O(GRID_WIDTH × GRID_HEIGHT × f), 
    // де f — складність функції isInside(x, y).
}


// Обчислює площу простого багатокутника за формулою Гаусса (формула 
// шнурування, формула землеміра), яка є дискретною реалізацією теореми Гріна.
// https://mathworld.wolfram.com/PolygonArea.html
// https://www.researchgate.net/publication/391315040_FORMULA_SNURUVANNA_V_ROZRIZI_POZASKILNOI_MATEMATICNOI_OSVITI
// Параметр vertices - вектор об'єктів Point, що представляють вершини 
// багатокутника у порядку (за або проти годинникової стрілки).
double Shape::CalculatePolygonArea(const vector<Point>& vertices)
{
    if (vertices.size() < 3 || !GeometryUtils::IsSimplePolygon(vertices)) {
        return 0.0;
    }
    // Ініціалізація змінної sum для накопичення підписаної площі
    double sum = 0;

    // Перебір усіх вершин
    for (size_t i = 0; i < vertices.size(); ++i) {
        // Отримання індексу наступної вершини, з поверненням до 0 
        // для останньої вершини
        size_t j = (i + 1) % vertices.size();
        // https://www.youtube.com/watch?v=0KjG8Pg6LGk
        // Накопичення перехресних добутків послідовних вершин
        double xi = vertices[i].GetX(), yi = vertices[i].GetY();
        double xj = vertices[j].GetX(), yj = vertices[j].GetY();
        sum += xi * yj - xj * yi;
    }
    // Якщо sum > 0, обхід проти годинникової стрілки; 
    // якщо sum < 0, за годинниковою
    double signed_area = sum / 2.0;  // підписана площа
    return abs(signed_area);

    // Без перевірки на самоперетини формула Гаусса призводила б до 
    // некоректних результатів для складних багатокутників!
}


// Обчислює периметр багатокутника за заданими вершинами.
double Shape::CalculatePolygonPerimeter(const vector<Point>& vertices)
{
    if (vertices.size() < 2 || !GeometryUtils::IsSimplePolygon(vertices)) {
        return 0.0;
    }
    double perimeter = 0;
    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t j = (i + 1) % vertices.size();  // замикає вершини
        perimeter += GeometryUtils::Distance(vertices[i], vertices[j]);
    }
    return perimeter;
}


// Похідний клас Пряма
// Але намалювати нескінченну пряму лінію складніше, аніж відрізок!
// Клас LineSegment реалізує відрізок замість прямої, оскільки:
// 1. Нескінченна пряма не може бути відображена на кінцевій сітці 
// розміром GRID_WIDTH x GRID_HEIGHT.
// 2. Відрізок дозволяє чітко визначити довжину (периметр) і 
// легко малюється за допомогою алгоритму Брезенхема.
// 3. Це спрощує методи Area, Perimeter і Draw, зберігаючи 
// відповідність умові завдання.
class LineSegment : public Shape  // спростимо собі задачу!..
{
private:
    Point p1, p2;  // кінцеві точки відрізка!
public:
    // Конструктор за двома точками
    LineSegment(const Point& p1, const Point& p2, const string& n = "Відрізок")
        : Shape(n), p1(p1), p2(p2)
    {
        if (!IsValid()) {
            throw invalid_argument("Точки збігаються або поза межами сітки!");
        }
    }
    // Конструктор за вектором точок
    LineSegment(const vector<Point>& endpoints, const string& n = "Відрізок")
        : Shape(n)
    {
        if (endpoints.size() < 2) throw
            invalid_argument("Зазначте початок і кінець відрізка!");
        if (endpoints[0] == endpoints[1]) {
            throw invalid_argument("Точки збігаються, відрізок некоректний!");
        }
        this->p1 = endpoints[0]; this->p2 = endpoints[1];
        if (!IsValid()) throw invalid_argument("Точки поза межами сітки!");
    }
    // Конструктор копіювання
    LineSegment(const LineSegment& other) : Shape(other), p1(other.p1),
        p2(other.p2) {
    }

    // Оператор присвоювання
    LineSegment& operator=(const LineSegment& other)
    {
        if (this != &other) {
            Shape::operator=(other);
            this->p1 = other.p1;
            this->p2 = other.p2;
        }
        return *this;
    }

    // Сетери
    void SetEndPoint1(const Point& newP1)
    {
        if (newP1 == p2) throw invalid_argument("Нова точка збігається "
            "з кінцевою, відрізок некоректний!");
        this->p1 = newP1;
        if (!IsValid()) throw invalid_argument("Точка поза межами сітки!");
    }
    void SetEndPoint2(const Point& newP2)
    {
        if (newP2 == p1) throw invalid_argument("Нова точка збігається "
            "з початковою, відрізок некоректний!");
        this->p2 = newP2;
        if (!IsValid()) throw invalid_argument("Точка поза межами сітки!");
    }
    void SetEndPoints(const Point& newP1, const Point& newP2)
    {
        if (newP1 == newP2) throw invalid_argument("Точки збігаються!");
        this->p1 = newP1; this->p2 = newP2;
        if (!IsValid()) throw invalid_argument("Точка поза межами сітки!");
    }

    // Гетери
    const Point& GetEndPoint1() const { return p1; }
    const Point& GetEndPoint2() const { return p2; }

    // Геометричні методи
    double Area() const override { return 0.0; }
    double Perimeter() const override
    {
        return GeometryUtils::Distance(p1, p2);  // тут довжина відрізка!
    }
    double GetSlopeAngleInDegrees() const  // кут нахилу відрізка
    {
        double dx = p2.GetX() - p1.GetX();
        double dy = p2.GetY() - p1.GetY();
        // Якщо dx близький до нуля, відрізок вертикальний
        if (abs(dx) <= GeometryUtils::EPSILON) {
            return (dy >= 0) ? 90.0 : -90.0;  // (кут 90° або - 90°)
        }
        return atan2(dy, dx) * 180.0 / M_PI;  // у градусах
        // https://en.cppreference.com/w/cpp/numeric/math/atan2
    }

    // Візуалізація: ініціалізація сітки, малювання відрізка разом із сіткою
    void Draw() const override
    {
        cout << this->GetName() << " з (" << p1.GetX() << ", "
            << p1.GetY() << ") до (" << p2.GetX() << ", "
            << p2.GetY() << ")\n";

        cout << fixed << setprecision(2);
        // https://en.cppreference.com/w/cpp/io/manip/fixed
        // https://en.cppreference.com/w/cpp/io/manip/setprecision

        cout << "Довжина: " << this->Perimeter() << "\n";
        cout << "Кут нахилу відрізка: " << GetSlopeAngleInDegrees() << "\n";
        vector<string> grid(GRID_HEIGHT, string(GRID_WIDTH, '.'));
        DrawSegmentOnGrid(grid, p1, p2);
        for (int row = GRID_HEIGHT - 1; row >= 0; --row) {
            cout << grid[row] << "\n";
        }
        cout << "\n\n\n";
    }

    // Перевірка коректності
    bool IsValid() const override
    {
        return GeometryUtils::Distance(p1, p2) > GeometryUtils::EPSILON &&
            p1.IsValid() && p2.IsValid();
    }

    ~LineSegment() override = default;  // деструктор
};


// Похідний клас Еліпс
// https://mathworld.wolfram.com/Ellipse.html
// https://math.stackexchange.com/questions/tagged/ellipse
// https://rosettacode.org/wiki/Superellipse
class Ellipsis : public Shape  // тільки латиною - жодної англійської!!!
    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-ellipse
{
private:
    double a, b;  // напівосі еліпса
    Point center; // центр еліпса
protected:
    // Кількість кроків для параметричного малювання еліпса
    static constexpr int PARAMETRIC_STEPS = 50;
    // фіксована кількість кроків може бути недостатньою для великих напівосей,
    // що може призводити до неточного малювання великих еліпсів...

    // Захищений метод для малювання сітки (без виведення інформації)
    void DrawGrid(vector<string>& grid, bool useParametric) const
    {
        if (useParametric) {  // використано параметричні рівняння еліпса
            for (int i = 0; i < PARAMETRIC_STEPS; ++i) {
                // Параметричні кути
                double theta1 = 2 * M_PI * i / PARAMETRIC_STEPS;
                double theta2 = 2 * M_PI * (i + 1) / PARAMETRIC_STEPS;

                // Точки на еліпсі
                Point p1(center.GetX() + a * cos(theta1),
                    center.GetY() + b * sin(theta1));
                Point p2(center.GetX() + a * cos(theta2),
                    center.GetY() + b * sin(theta2));

                // Якщо точки в межах сітки, малюємо відрізок між точками
                if (abs(p1.GetX()) <= GRID_WIDTH / 2.0 &&
                    abs(p1.GetY()) <= GRID_HEIGHT / 2.0 &&
                    abs(p2.GetX()) <= GRID_WIDTH / 2.0 &&
                    abs(p2.GetY()) <= GRID_HEIGHT / 2.0) {
                    DrawSegmentOnGrid(grid, p1, p2);
                }
            }
        }
        else {
            // Сітковий метод - використовує канонічне рівняння еліпса
            // http://geometry.karazin.ua/resources/articles/947405987a57401e402595c767308e2f.pdf
            FillShapeOnGrid(grid, [this](double x, double y) {
                double value = pow((x - center.GetX()) / a, 2) +
                    pow((y - center.GetY()) / b, 2);
                return value <= 1.0 + GeometryUtils::EPSILON;
                // Заливка всередині та на контурі
                });  // те саме функційне програмування!
            // https://habr.com/ru/companies/otus/articles/794821/
        }

    }

public:
    // Конструктор за напівосями та центром (з параметрами за замовчуванням)
    Ellipsis(double a = 1.0, double b = 1.0, const Point& c = Point(),
        const string& n = "Еліпс") : Shape(n), a(a), b(b), center(c)
    {
        if (!IsValid()) {
            throw invalid_argument("Напівосі еліпса повинні бути додатними!");
        }
    }
    // Конструктор за вектором параметрів (напівосі a, b) та точкою центру
    Ellipsis(const vector<double>& params, const Point& c = Point(),
        const string& n = "Еліпс") : Shape(n), center(c)
    {
        if (params.size() < 2) {
            throw invalid_argument("Потрібно вказати обидві напівосі a і b!");
        }
        if (params.size() > 2) {
            throw invalid_argument("Занадто багато параметрів для еліпса!");
        }  // поки що серед параметрів немає фокуса еліпса
        this->a = params[0];
        this->b = params[1];
        if (!IsValid()) {
            throw invalid_argument("Напівосі еліпса повинні бути додатними!");
        }
    }
    // Конструктор копіювання
    Ellipsis(const Ellipsis& other) : Shape(other.GetName()), a(other.a),
        b(other.b), center(other.center) {
    }

    // Оператор присвоювання
    Ellipsis& operator=(const Ellipsis& other)
    {
        if (this != &other) {
            Shape::operator=(other);
            this->a = other.a;
            this->b = other.b;
            this->center = other.center;
        }
        return *this;
    }

    // Сетери
    void SetSemiAxes(double newA, double newB)
    {
        if (newA <= GeometryUtils::EPSILON || newB <= GeometryUtils::EPSILON) {
            throw invalid_argument("Напівосі еліпса повинні бути додатними!");
        }
        this->a = newA;
        this->b = newB;
    }
    void SetSemiAxisA(double newA)
    {
        if (newA <= GeometryUtils::EPSILON) {
            throw invalid_argument("Напіввісь a повинна бути додатною!");
        }
        this->a = newA;
    }
    void SetSemiAxisB(double newB)
    {
        if (newB <= GeometryUtils::EPSILON) {
            throw invalid_argument("Напіввісь b повинна бути додатною!");
        }
        this->b = newB;
    }
    void SetCenter(const Point& newCenter)
    {
        this->center = newCenter;
    }

    // Гетери
    double GetSemiAxisA() const { return a; }
    double GetSemiAxisB() const { return b; }
    const Point& GetCenter() const { return center; }

    // Геометричні методи
    double Area() const override { return M_PI * a * b; }  // доволі очевидно
    double Perimeter() const override
    {
        // Наближення для периметра еліпса (перша формула Рамануджана)
        return M_PI * (3 * (a + b) - sqrt((a + 3 * b) * (3 * a + b)));
    }   // https://mathworld.wolfram.com/Ellipse.html#eqn70

    virtual bool IsCircle() const  // перевіряє, чи є еліпс колом (a == b)
        // додав virtual, бо були помилки E1455, C3668 під час тестування
    {
        return abs(a - b) < GeometryUtils::EPSILON;
    }

    virtual double Eccentricity() const  // обчислює ексцентриситет еліпса
        // додав virtual, щоб похідний клас Коло запрацював без помилок
    {
        // https://mathworld.wolfram.com/Eccentricity.html
        // у літературі a - більша напіввісь, b - менша напіввісь!
        double major = max(a, b);  // semimajor a
        double minor = min(a, b);  // semiminor b
        return sqrt(1.0 - (minor * minor) / (major * major));
    }

    // Візуалізація: ініціалізація сітки, малювання еліпса
    void Draw() const override
    {
        Draw(false);  // за замовчуванням використовуємо сітковий метод
    }

    // Перевантажений метод Draw із вибором методу малювання
    virtual void Draw(bool useParametric) const  // були помилки E1455, C3668
    {
        cout << this->GetName() << " з напівосями a = " << a << ", b = " << b
            << " у центрі (" << center.GetX() << ", " << center.GetY()
            << ") " << (useParametric ? "параметричним" : "сітковим")
            << " методом.\n";

        cout << fixed << setprecision(2);
        cout << "Площа: " << Area() << ", Периметр: " << Perimeter() << "\n";
        cout << "Ексцентриситет: " << Eccentricity() << "\n";

        vector<string> grid(GRID_HEIGHT, string(GRID_WIDTH, '.'));
        DrawGrid(grid, useParametric);

        // Виведення сітки
        for (int row = GRID_HEIGHT - 1; row >= 0; --row) {
            cout << grid[row] << "\n";
        }
        cout << "\n\n\n";
    }

    // Перевірка коректності параметрів еліпса
    bool IsValid() const override
    {
        // Перевірка на додатність напівосей
        if (a <= GeometryUtils::EPSILON || b <= GeometryUtils::EPSILON) {
            return false;
        }
        // Перевірка, чи еліпс вміщається в сітку з урахуванням центру
        if (abs(center.GetX()) + a > GRID_WIDTH / 2.0 ||
            abs(center.GetY()) + b > GRID_HEIGHT / 2.0) {
            return false;
        }
        return true;
    }

    ~Ellipsis() override = default;  // деструктор
};


// Похідний клас від класу Еліпс клас Коло
// https://mathworld.wolfram.com/Circle.html
// https://www.cut-the-knot.org/pythagoras/Munching/circle.shtml
// https://rosettacode.org/wiki/Total_circles_area
class Circle : public Ellipsis
{
private:
    double radius;  // радіус кола
public:
    // Конструктор за радіусом і центром
    Circle(double r, const Point& c = Point(), const string& n = "Коло") :
        Ellipsis(r, r, c, n), radius(r)
    {
        if (!IsValid()) {
            throw invalid_argument("Радіус повинен бути додатним, "
                "а коло за параметрами - не виходити за межі сітки!");
        }
    }
    // Конструктор за діаметром
    Circle(double d, bool isDiameter = false, const Point& c = Point(),
        const string& n = "Коло") : Ellipsis(d / 2.0, d / 2.0, c, n),
        radius(d / 2.0)
    {
        if (!isDiameter) {
            radius = d;  // якщо isDiameter == false, вважаємо d радіусом
            Ellipsis::SetSemiAxes(d, d);
        }
        if (!IsValid()) {
            throw invalid_argument("Параметри повинні бути коректними!");
        }
    }
    // Виклик конструктора для малювання кола з діаметром d == 10:
    /*
    int main()
    {
        Point p1(3, -3);
        Circle c(10, true, p1);  // isDiameter = true
        c.Draw(false);  // круг - це коло із заливкою!
        c.Draw(true);   // правдиве коло

        return(0);
    }
    */

    // Конструктор копіювання
    Circle(const Circle& other) : Ellipsis(other), radius(other.radius) {}

    // Оператор присвоювання
    Circle& operator=(const Circle& other)
    {
        if (this != &other) {
            Ellipsis::operator=(other);
            this->radius = other.radius;
        }
        return *this;
    }

    // Сетер для радіуса
    void SetRadius(double r)
    {
        if (r <= GeometryUtils::EPSILON) {
            throw invalid_argument("Радіус кола повинен бути додатним!");
        }
        this->radius = r;
        Ellipsis::SetSemiAxes(r, r);
    }

    // Сетер для діаметра
    void SetDiameter(double d)
    {
        if (d <= GeometryUtils::EPSILON) {
            throw invalid_argument("Діаметр кола повинен бути додатним!");
        }
        this->radius = d / 2.0;
        Ellipsis::SetSemiAxes(radius, radius);
    }

    // Гетери для радіуса та для діаметра відповідно
    double GetRadius() const { return radius; }
    double GetDiameter() const { return 2.0 * radius; }

    // Перевизначення методу IsCircle базового класу Еліпс
    bool IsCircle() const override { return true; }  // коло це коло!

    // Перевизначення методів геометричних обчислень
    double Area() const override { return M_PI * radius * radius; }
    double Perimeter() const override { return 2 * M_PI * radius; }
    // https://mathworld.wolfram.com/Pi.html
    double Eccentricity() const override { return 0.0; }  // завжди для кола!

    // Перевизначення методів візуалізації

    void Draw() const override { Draw(false); }

    void Draw(bool useParametric = false) const override
    {
        cout << this->GetName() << " з радіусом r = " << this->radius
            << ", з діаметром d = " << this->GetDiameter()
            << " у центрі (" << GetCenter().GetX() << ", "
            << GetCenter().GetY() << ") "
            << (useParametric ? "параметричним" : "сітковим") << " методом.\n";

        cout << fixed << setprecision(2);
        cout << "Площа круга: " << this->Area()
            << ", Довжина кола: " << this->Perimeter() << "\n"
            << "Ексцентриситет: " << this->Eccentricity() << "\n";

        vector<string> grid(GRID_HEIGHT, string(GRID_WIDTH, '.'));
        Ellipsis::DrawGrid(grid, useParametric);

        for (int row = GRID_HEIGHT - 1; row >= 0; --row) {
            cout << grid[row] << "\n";
        }
        cout << "\n\n\n";
    }

    // Перевірка коректності параметрів кола
    bool IsValid() const override
    {
        // Успадкована перевірка класу Еліпс
        // (додатність напівосей і відповідність сітці)
        if (!Ellipsis::IsValid()) return false;
        // Додаткова перевірка радіуса
        return radius > GeometryUtils::EPSILON;
    }

    ~Circle() override = default;  // деструктор
};


// Похідний клас Багатокутник
// https://reference.wolfram.com/language/ref/Polygon.html
// https://mathworld.wolfram.com/topics/Polygons.html
class PolygonVOS667 : public Shape  // ВОС 667 "Навчальних полів" !!!
    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-polygon
{
private:  // змінено з protected на private
    vector<Point> vertices; // вектор вершин багатокутника
public:
    // Конструктор за вектором вершин та назвою
    PolygonVOS667(const vector<Point>& verts,
        const string& n = "Багатокутник") : Shape(n), vertices(verts)
    {
        if (!IsValid()) {
            throw invalid_argument("Некоректний багатокутник!");
        }
    }

    // Конструктор копіювання
    PolygonVOS667(const PolygonVOS667& other) : Shape(other.GetName()),
        vertices(other.vertices) {
    }

    // Оператор присвоювання
    PolygonVOS667& operator=(const PolygonVOS667& other)
    {
        if (this != &other) {
            Shape::operator=(other);
            this->vertices = other.vertices;
        }
        return *this;
    }

    // Оператор порівняння
    bool operator==(const PolygonVOS667& other) const
    {
        return this->vertices == other.vertices &&
            this->GetName() == other.GetName();
    }

    // Установлення всіх вершин
    void SetVertices(const vector<Point>& newVertices) override
    {
        this->vertices = newVertices;
    }

    // Установлення вершини за індексом
    void SetVertex(size_t index, const Point& p) override
    {
        if (index >= vertices.size()) {
            throw out_of_range("Індекс вершини поза межами!");
            // https://en.cppreference.com/w/cpp/error/out_of_range.html
        }
        vertices[index] = p;
    }

    // Отримання вершин багатокутника
    const vector<Point>& GetVertices() const { return vertices; }

    // Обчислення довжин сторін багатокутника
    vector<double> GetPolygonSides() const
    {
        vector<double> sides;
        for (size_t i = 0; i < vertices.size(); ++i) {
            size_t j = (i + 1) % vertices.size();  // замикаємо вершини
            double side_length =
                GeometryUtils::Distance(vertices[i], vertices[j]);
            sides.push_back(side_length);
        }
        return sides;
    }

    // Обчислення кутів між сторонами
    vector<double> GetPolygonAngles() const
    {
        vector<double> angles;
        for (size_t i = 0; i < vertices.size(); ++i) {
            // Визначаємо три вершини: поточну (i), попередню (prev) 
            // і наступну (next)
            size_t prev = (i == 0) ? vertices.size() - 1 : i - 1;
            size_t next = (i + 1) % vertices.size();
            // Обчислюємо кут при поточній вершині (vertices[i])
            double angle = GeometryUtils::AngleBetweenPoints(vertices[prev],
                vertices[i], vertices[next]);
            angles.push_back(angle);
        }
        return angles;
    }

    // Обчислення площі - коректно для простих багатокутників
    double Area() const override { return CalculatePolygonArea(vertices); }

    // Обчислення периметра - коректно для простих багатокутників
    double Perimeter() const override
    {
        return CalculatePolygonPerimeter(vertices);
    }

    // Перевірка опуклості
    bool IsConvex() const  // чи увігнутий?
    {
        return GeometryUtils::IsConvexPolygon(vertices);
    }

    // Перевірка простоти
    bool IsSimple() const
    {   // самоперетинів немає?
        return GeometryUtils::IsSimplePolygon(vertices);
    }

    // Перевірка невиродженості
    bool IsNonDegenerate() const
    {
        return abs(Area()) > GeometryUtils::EPSILON;
        // https://mathworld.wolfram.com/Digon.html
    }

    // Перевірка коректності розташування вершин у межах сітки.
    // Не включає перевірку простоти багатокутника, яка є визначальною
    // для коректних обчислень площі та периметра.
    bool IsValid() const override
    {
        // Перевірка, чи всі вершини в межах сітки, і нічого більше!
        for (const auto& vertex : vertices) {
            if (abs(vertex.GetX()) >
                GRID_WIDTH / 2.0 + GeometryUtils::EPSILON ||
                abs(vertex.GetY()) >
                GRID_HEIGHT / 2.0 + GeometryUtils::EPSILON) {
                return false;
            }
        }
        return true;
    }

    // Візуалізація багатокутника. Хай малює складні багатокутники!
    void Draw() const override
    {
        // Виведення геометричних властивостей
        cout << this->GetName() << " з " << vertices.size() << " вершинами:\n"
            << "- " << (this->IsSimple() ? "простий" : "складний") << ", "
            << (IsNonDegenerate() ? "невироджений" : "вироджений") << ", "
            << (this->IsConvex() ? "опуклий" : "увігнутий") << ".\n";

        cout << fixed << setprecision(2);
        cout << "Площа: " << Area() << ", Периметр: " << Perimeter() << "\n";

        // Виведення координат вершин
        for (size_t i = 0; i < vertices.size(); ++i) {
            cout << "Вершина " << i + 1 << ": (" << vertices[i].GetX()
                << ", " << vertices[i].GetY() << ")\n";
        }

        // Захист від невизначеної поведінки
        if (vertices.size() < 2) {
            cout << "Неможливо намалювати багатокутник із "
                "менше ніж 2 вершинами!\n";
            return;
        }

        // Малювання багатокутника на сітці
        vector<string> grid(GRID_HEIGHT, string(GRID_WIDTH, '.'));
        for (size_t i = 0; i < vertices.size(); ++i) {
            DrawSegmentOnGrid(grid, vertices[i],
                vertices[(i + 1) % vertices.size()]);
        }

        // Виведення сітки у консоль
        for (int row = GRID_HEIGHT - 1; row >= 0; --row) {
            cout << grid[row] << "\n";
        }
        cout << "\n\n\n";
    }

    ~PolygonVOS667() override = default;  // деструктор
};


// Похідний від класу Багатокутник клас Трикутник
// https://mathworld.wolfram.com/Triangle.html
// https://reference.wolfram.com/language/ref/Triangle.html
class Triangle : public PolygonVOS667
{
private:
    vector<double> sides;  // довжини сторін трикутника (3 елементи)
    vector<double> angles; // кути при вершинах у градусах (3 елементи)

    // Метод для оновлення сторін і кутів
    void UpdateSidesAndAngles()
    {
        this->sides = GetPolygonSides();
        this->angles = GetPolygonAngles();
    }

public:
    // Конструктор за трьома точками
    Triangle(const Point& p1, const Point& p2, const Point& p3,
        const string& n = "Трикутник") : PolygonVOS667({ p1, p2, p3 }, n)
    {
        if (p1 == p2 || p2 == p3 || p3 == p1) {
            throw invalid_argument("Точки трикутника не повинні збігатися!");
        }
        UpdateSidesAndAngles();
        if (!IsValid()) throw invalid_argument("Некоректний трикутник!");
    }

    // Конструктор за вектором точок
    Triangle(const vector<Point>& points, const string& n = "Трикутник") :
        PolygonVOS667(points, n)
    {
        if (points.size() != 3) {
            throw invalid_argument("Трикутник повинен мати рівно 3 вершини!");
        }
        if (points[0] == points[1] || points[1] == points[2] ||
            points[2] == points[0]) {
            throw invalid_argument("Точки трикутника не повинні збігатися!");
        }
        UpdateSidesAndAngles();
        if (!IsValid()) throw invalid_argument("Некоректний трикутник!");
    }

    // Конструктор копіювання
    Triangle(const Triangle& other) : PolygonVOS667(other),
        sides(other.sides), angles(other.angles) {
    }

    // Оператор присвоювання
    Triangle& operator=(const Triangle& other)
    {
        if (this != &other) {
            PolygonVOS667::operator=(other);
            this->sides = other.sides;
            this->angles = other.angles;
        }
        return *this;
    }

    // Оператор порівняння з урахуванням обертання трикутника
    bool operator==(const Triangle& other) const
    {
        auto v1 = GetVertices();  // вершини зберігаються в базовому класі
        auto v2 = other.GetVertices();
        // Порівняння з урахуванням циклічного порядку
        for (size_t i = 0; i < 3; ++i) {
            if (v1[0] == v2[i] &&
                v1[1] == v2[(i + 1) % 3] &&
                v1[2] == v2[(i + 2) % 3]) {
                return true;
            }
        }
        return false;
    }

    // Установлення всіх вершин
    void SetVertices(const vector<Point>& newVertices) override
    {
        if (newVertices.size() != 3) {
            throw invalid_argument("Трикутник повинен мати рівно 3 вершини!");
        }
        if (newVertices[0] == newVertices[1] ||
            newVertices[1] == newVertices[2] ||
            newVertices[2] == newVertices[0]) {
            throw invalid_argument("Точки трикутника не повинні збігатися!");
        }
        PolygonVOS667::SetVertices(newVertices);
        this->UpdateSidesAndAngles();
        if (!IsValid()) throw invalid_argument("Некоректний трикутник!");
    }

    // Установлення вершини за індексом
    void SetVertex(size_t index, const Point& p) override
    {
        if (index >= 3) {
            throw out_of_range("Невірний індекс вершини для трикутника!");
        }
        auto tempVertices = GetVertices();
        tempVertices[index] = p;
        if (tempVertices[0] == tempVertices[1] ||
            tempVertices[1] == tempVertices[2] ||
            tempVertices[2] == tempVertices[0]) {
            throw invalid_argument("Точки трикутника не повинні збігатися!");
        }
        PolygonVOS667::SetVertex(index, p);
        this->UpdateSidesAndAngles();
        if (!IsValid()) throw invalid_argument("Некоректний трикутник!");
    }

    // Отримання сторін трикутника
    vector<double> GetTrigonSides() const { return sides; }
    // https://mathworld.wolfram.com/Trigon.html

    // Отримання кутів трикутника
    vector<double> GetTrigonAngles() const { return angles; }

    // Перевірка на гострокутність
    bool IsAcute() const
    {
        for (double a : angles) {
            if (a >= 90.0 + GeometryUtils::EPSILON) return false;
        }
        return true;
    }

    // Перевірка на тупокутність
    bool IsObtuse() const
    {
        for (double a : angles) {
            if (a > 90.0 + GeometryUtils::EPSILON) return true;
        }
        return false;
    }

    // Перевірка на прямокутність
    bool IsRight() const
    {
        for (double a : angles) {
            if (abs(a - 90.0) < GeometryUtils::EPSILON) return true;
            // if (abs(a - 90.0) < 1.0) return true;  // збільшена похибку
        }
        return false;
    }

    // Перевірка на рівнобедреність
    bool IsIsosceles() const
    {
        return abs(sides[0] - sides[1]) < GeometryUtils::EPSILON ||
            abs(sides[1] - sides[2]) < GeometryUtils::EPSILON ||
            abs(sides[2] - sides[0]) < GeometryUtils::EPSILON;
    }

    // Перевірка на рівносторонність
    bool IsEquilateral() const
    {
        return abs(sides[0] - sides[1]) < GeometryUtils::EPSILON &&
            abs(sides[1] - sides[2]) < GeometryUtils::EPSILON;
    }

    //// Обчислення площі
    //double Area() const override { return PolygonVOS667::Area(); }

    // Обчислення площі трикутника за формулою Герона, яку використано
    // задля вищої чисельної точності порівняно з CalculatePolygonArea
    // https://mathworld.wolfram.com/HeronsFormula.html
    double Area() const override
    {
        double a = sides[0], b = sides[1], c = sides[2];
        if (a + b <= c + GeometryUtils::EPSILON ||
            a + c <= b + GeometryUtils::EPSILON ||
            b + c <= a + GeometryUtils::EPSILON) {
            return 0.0;
        }
        double p = (a + b + c) / 2.0;  // напівпериметр
        // Власне, формула Герона з обробкою малих значень
        double area = sqrt(p * (p - a) * (p - b) * (p - c));
        return (area < GeometryUtils::EPSILON) ? 0.0 : area;
    }

    // Обчислення периметра
    double Perimeter() const override
    {
        return sides[0] + sides[1] + sides[2];
    }

    // Перевірка коректності трикутника
    bool IsValid() const override
    {
        const auto& vertices = GetVertices();

        // Перевірка кількості вершин
        if (vertices.size() != 3) return false;

        // Перевірка, чи всі вершини в межах сітки
        if (!PolygonVOS667::IsValid()) return false;

        // Перевірка на збіг точок (для уникнення вироджених трикутників)
        if (vertices[0] == vertices[1] || vertices[1] == vertices[2] ||
            vertices[2] == vertices[0]) {
            return false;
        }

        // Перевірка нерівності трикутника
        if (sides[0] + sides[1] <= sides[2] + GeometryUtils::EPSILON ||
            sides[1] + sides[2] <= sides[0] + GeometryUtils::EPSILON ||
            sides[2] + sides[0] <= sides[1] + GeometryUtils::EPSILON) {
            return false;
        }

        // Перевірка на колінеарність за допомогою векторного добутку
        double cross = (vertices[1].GetX() - vertices[0].GetX()) *
            (vertices[2].GetY() - vertices[0].GetY()) -
            (vertices[1].GetY() - vertices[0].GetY()) *
            (vertices[2].GetX() - vertices[0].GetX());
        if (abs(cross) < GeometryUtils::EPSILON) {
            return false;  // точки колінеарні
        }

        return true;
    }

    // Візуалізація трикутника 
    void Draw() const override;  // визначення методу див. нижче

    ~Triangle() override = default;  // деструктор
};


/**
 * ОЗНАЙОМЛЕННЯ З ФУНКЦІЙНИМ ПРОГРАМУВАННЯМ ДЛЯ ЗАГАЛЬНОГО РОЗВИТКУ
 *
 * Цей код демонструє застосування принципів функційного програмування в С++
 * для створення спеціалізованих трикутників (гострокутний, тупокутний,
 * прямокутний, рівнобедрений тощо) через шаблонний клас SpecializedTriangle.
 * Функційний підхід реалізовано через:
 * - Використання std::function для передачі умов перевірки типу трикутника
 *   як функційних об'єктів у просторі імен TriangleChecks.
 *   https://en.cppreference.com/w/cpp/utility/functional/function
 *   https://www.geeksforgeeks.org/cpp/std-function-in-cpp/
 * - Лямбда-вирази, оголошені як static у просторі імен TriangleChecks,
 *   реалізують специфічну логіку класифікації трикутників за комбінацією
 *   кутів і сторін.
 *   https://en.cppreference.com/w/cpp/language/lambda
 *   https://www.w3schools.com/cpp/cpp_functions_lambda.asp
 * - Уникнення побічних ефектів у методах перевірки шляхом використання
 *   чистих функцій, які завжди повертають однаковий результат для
 *   однакових вхідних даних і не змінюють зовнішній стан.
 *   https://en.wikipedia.org/wiki/Pure_function
 * - Абстрагування логіки перевірки через шаблонний клас, що дозволяє
 *   легко додавати нові типи трикутників без зміни базової структури.
 *
 * Переваги функційного підходу:
 * - Гнучкість: лямбда-вирази в TriangleChecks дозволяють легко визначати
 *   та комбінувати перевірки для різних типів трикутників без створення
 *   окремих класів.
 * - Компактність: уникаємо дублювання коду шляхом використання шаблонного
 *   класу, що скорочує кількість рядків коду.
 * - Читабельність: умови перевірки чітко визначені в просторі імен
 *   TriangleChecks, що полегшує розуміння логіки.
 *
 * Недоліки:
 * - Використання std::function додає невеликі накладні витрати на
 *   продуктивність порівняно з прямими викликами методів через віртуалізацію.
 * - Статичні лямбда-вирази в TriangleChecks забезпечують повторне
 *   використання, але їх централізоване визначення може ускладнити
 *   додавання нових перевірок у великих проєктах через необхідність
 *   редагування одного простору імен.
 *
 * Примітка: За умовою домашнього завдання потрібно створити похідні класи
 * трикутників, що суперечить використанню шаблонного підходу, але цей код
 * демонструє альтернативний підхід через функціональне програмування.
 */


 // Простір імен для зберігання перевірок типів трикутників
namespace TriangleChecks {
    // Оголошення статичних константних функційних об'єктів типу
    // std::function<bool(const Triangle&)>. std::function - це
    // шаблонний клас, який може зберігати будь-який викликаємий об'єкт
    // (функцію, лямбда-вираз тощо), що повертає bool і приймає об'єкт 
    // Triangle як параметр.
    // Містить набір лямбда-виразів для класифікації трикутників 
    // за геометричними ознаками (кути, співвідношення сторін).

    // https://mathworld.wolfram.com/ScaleneTriangle.html

    // ЗвичайнийГострокутнийТрикутник
    // https://mathworld.wolfram.com/AcuteTriangle.html
    // https://mathworld.wolfram.com/OnoInequality.html
    // Лямбда-вираз ScaleneAcuteCheck перевіряє, чи є трикутник гострокутним
    // (усі кути < 90°) і не рівнобедреним - тіло функції у фігурних дужках.
    // Синтаксис: [](const Triangle& t) -> bool {...} визначає анонімну 
    // функцію, яка приймає об'єкт Triangle і повертає bool.
    // [capture](parameters) -> return_type { body }
    // Ключове слове static const забезпечуює, що об'єкт є константою і має
    // статичну тривалість життя (існує протягом усього виконання програми).
    static const function<bool(const Triangle&)> ScaleneAcuteCheck =
        [](const Triangle& t) -> bool {
        return t.IsAcute() && !t.IsIsosceles();
        };

    // РівнобедренийГострокутнийТрикутник
    // https://mathworld.wolfram.com/IsoscelesTriangle.html
    // https://mathworld.wolfram.com/GoldenTriangle.html
    static const function<bool(const Triangle&)> IsoscelesAcuteCheck =
        [](const Triangle& t) -> bool {
        return t.IsAcute() && t.IsIsosceles() && !t.IsEquilateral();
        };


    // РівностороннійГострокутнийТрикутник
    // https://mathworld.wolfram.com/EquilateralTriangle.html
    // https://mathworld.wolfram.com/ReuleauxTriangle.html
    static const function<bool(const Triangle&)> EquilateralAcuteCheck =
        [](const Triangle& t) -> bool {
        return t.IsAcute() && t.IsEquilateral();
        };

    // ЗвичайнийТупокутнийТрикутник
    // https://mathworld.wolfram.com/ObtuseTriangle.html
    static const function<bool(const Triangle&)> ScaleneObtuseCheck =
        [](const Triangle& t) -> bool {
        return t.IsObtuse() && !t.IsIsosceles();
        };

    // РівнобедренийТупокутнийТрикутник
    static const function<bool(const Triangle&)> IsoscelesObtuseCheck =
        [](const Triangle& t) -> bool {
        return t.IsObtuse() && t.IsIsosceles();
        };

    // ЗвичайнийПрямокутнийТрикутник
    // https://mathworld.wolfram.com/RightTriangle.html
    // https://mathworld.wolfram.com/30-60-90Triangle.html
    static const function<bool(const Triangle&)> ScaleneRightCheck =
        [](const Triangle& t) -> bool {
        return t.IsRight() && !t.IsIsosceles();
        };

    // РівнобедренийПрямокутнийТрикутник
    // https://mathworld.wolfram.com/IsoscelesRightTriangle.html
    // https://mathworld.wolfram.com/Polyabolo.html
    static const function<bool(const Triangle&)> IsoscelesRightCheck =
        [](const Triangle& t) -> bool {
        return t.IsRight() && t.IsIsosceles();
        };
}


// Шаблонний клас SpecializedTriangle розширює базовий клас Triangle,
// додаючи можливість перевірки специфічних властивостей трикутника через
// параметр ValidityCheck (функціональний об'єкт, наприклад, лямбда-вираз).
// Шаблонний клас дозволяє визначати тип ValidityCheck під час компіляції,
// забезпечуючи гнучкість у визначенні умов перевірки.
// https://en.cppreference.com/w/cpp/language/class_template
// https://www.geeksforgeeks.org/cpp/templates-cpp/
template<typename ValidityCheck>
class SpecializedTriangle : public Triangle
{
private:
    ValidityCheck isValidType;  // об'єкт перевірки типу трикутника
public:
    // Приймає три об'єкти Point (вершини трикутника), назву трикутника (n)
    // та об'єкт перевірки ValidityCheck (наприклад, лямбда-вираз).
    // Використовує список ініціалізації 
    // (: Triangle(p1, p2, p3, n), isValidType(check)) для виклику 
    // конструктора базового класу Triangle та ініціалізації isValidType.
    SpecializedTriangle(const Point& p1, const Point& p2, const Point& p3,
        const string& n = "Спеціалізований трикутник",
        ValidityCheck check = ValidityCheck())
        : Triangle(p1, p2, p3, n), isValidType(check)
    {
        if (!IsValid()) {
            throw invalid_argument("Некоректний трикутник для типу " + n);
        }
    }

    // Конструктор за вектором точок з перевіркою через ValidityCheck
    SpecializedTriangle(const vector<Point>& points,
        const string& n = "Спеціалізований трикутник",
        ValidityCheck check = ValidityCheck())
        : Triangle(points, n), isValidType(check)
    {
        if (!IsValid()) {
            throw invalid_argument("Некоректний трикутник для типу " + n);
        }
    }

    // Конструктор копіювання
    SpecializedTriangle(const SpecializedTriangle& other)
        : Triangle(other), isValidType(other.isValidType) {
    }

    // Оператор присвоювання
    SpecializedTriangle& operator=(const SpecializedTriangle& other)
    {
        if (this != &other) {
            Triangle::operator=(other);
            this->isValidType = other.isValidType;
        }
        return *this;
    }

    // Перевірка коректності спеціалізованого трикутника
    bool IsValid() const override
    {
        if (!Triangle::IsValid()) return false;
        return isValidType(*this);
    }

    // Перевизначення методу візуалізації
    void Draw() const override { Triangle::Draw(); }

    ~SpecializedTriangle() override = default;  // деструктор
};


// Оголошення псевдонімів типів для спеціалізованих трикутників із 
// передачею умов перевірки через std::function<bool(const Triangle&)> 
// для визначення їхніх властивостей
// https://en.cppreference.com/w/cpp/language/type_alias.html
using ScaleneAcuteTriangle = SpecializedTriangle
<function<bool(const Triangle&)>>;  // ЗвичайнийГострокутнийТрикутник
using IsoscelesAcuteTriangle = SpecializedTriangle
<function<bool(const Triangle&)>>;  // РівнобедренийГострокутнийТрикутник
using EquilateralAcuteTriangle = SpecializedTriangle
<function<bool(const Triangle&)>>;  // РівностороннійГострокутнийТрикутник
using ScaleneObtuseTriangle = SpecializedTriangle
<function<bool(const Triangle&)>>;  // ЗвичайнийТупокутнийТрикутник
using IsoscelesObtuseTriangle = SpecializedTriangle
<function<bool(const Triangle&)>>;  // РівнобедренийТупокутнийТрикутник
using ScaleneRightTriangle = SpecializedTriangle
<function<bool(const Triangle&)>>;  // ЗвичайнийПрямокутнийТрикутник
using IsoscelesRightTriangle = SpecializedTriangle
<function<bool(const Triangle&)>>;  // РівнобедренийПрямокутнийТрикутник


// Візуалізація трикутника та виведення його геометричних властивостей.
// Метод виводить координати вершин, довжини сторін, кути, площу, периметр
// та графічне зображення трикутника на сітці (grid).
// Використовує dynamic_cast для перевірки, чи є об'єкт спеціалізованим
// трикутником, щоб визначити тип за кутами.
// https://www.geeksforgeeks.org/dynamic-_cast-in-cpp/
void Triangle::Draw() const
{
    const auto& vertices = GetVertices();  // координати вершин
    vector<string> grid(GRID_HEIGHT, string(GRID_WIDTH, '.'));

    // Виведення базових геометричних властивостей
    cout << this->GetName() << " - багатокутник з 3 вершинами:\n"
        << "- " << (this->IsSimple() ? "простий" : "складний") << ", "
        << (IsNonDegenerate() ? "невироджений" : "вироджений") << ", "
        << (this->IsConvex() ? "опуклий" : "увігнутий") << ".\n";

    // Визначення типу трикутника за співвідношенням сторін
    string sideType = this->IsEquilateral() ? "Рівносторонній" :
        this->IsIsosceles() ? "Рівнобедрений" : "Звичайний";

    // Визначення типу трикутника за кутами, використовуючи isValidType
    string angleType;
    const SpecializedTriangle<function<bool(const Triangle&)>>* specialized =
        dynamic_cast<const SpecializedTriangle<function<bool(const Triangle&)>>*>(this);
    // безпечне приведення типів у спадковій ієрархії. 
    // https://en.cppreference.com/w/cpp/language/dynamic_cast.html
    // https://stackoverflow.com/questions/3605679/cast-const-class-using-dynamic-cast

    // Якщо об'єкт є спеціалізованим трикутником, використовуємо 
    // відповідні предикати для класифікації
    // https://en.wikipedia.org/wiki/Predicate_(logic)
    if (specialized) {
        // Перевіряємо тип через isValidType
        if (TriangleChecks::ScaleneAcuteCheck(*this)) {
            angleType = "Гострокутний";
        }
        else if (TriangleChecks::ScaleneObtuseCheck(*this) ||
            TriangleChecks::IsoscelesObtuseCheck(*this)) {
            angleType = "Тупокутний";
        }
        else if (TriangleChecks::ScaleneRightCheck(*this) ||
            TriangleChecks::IsoscelesRightCheck(*this)) {
            angleType = "Прямокутний";
        }
        else if (TriangleChecks::EquilateralAcuteCheck(*this)) {
            angleType = "Гострокутний";
        }
        else {
            angleType = "Гострокутний";  // за замовчуванням
        }
    }
    else {
        // Якщо об'єкт не є спеціалізованим, використовуємо 
        // базові методи для класифікації
        angleType = this->IsRight() ? "Прямокутний" :
            this->IsObtuse() ? "Тупокутний" : "Гострокутний";
    }

    // Виводимо площу та периметр з фіксованою точністю
    cout << fixed << setprecision(2);
    cout << "Площа: " << Area() << ", Периметр: " << Perimeter() << "\n";

    // Виведення довжини сторін
    cout << "Сторони трикутника:\n";
    for (size_t i = 0; i < 3; ++i) {
        cout << "Сторона " << i + 1 << ": " << sides[i] << "\n";
    }

    // Виведення значень кутів у градусах
    cout << "Кути трикутника (у градусах):\n";
    for (size_t i = 0; i < 3; ++i) {
        cout << "Кут при вершині " << i + 1 << ": " << angles[i] << "°\n";
    }

    // Виведення координат вершин
    for (size_t i = 0; i < 3; ++i) {
        cout << "Вершина " << i + 1 << ": (" << vertices[i].GetX()
            << ", " << vertices[i].GetY() << ")\n";
    }

    // Малювання трикутника на сітці, з'єднуючи вершини
    for (size_t i = 0; i < 3; ++i) {
        DrawSegmentOnGrid(grid, vertices[i], vertices[(i + 1) % 3]);
    }

    // Виведення сітки знизу вгору (осі координат зворотні)
    for (int row = GRID_HEIGHT - 1; row >= 0; --row) {
        cout << grid[row] << "\n";
    }
    cout << "\n\n\n";  // візуальне розділення для зручності між тестами
}


// Похідний від класу Багатокутник клас Чотирикутник
// https://mathworld.wolfram.com/Quadrilateral.html
// https://www.wolframalpha.com/input/?i=quadrilaterals
class Quadrilateral : public PolygonVOS667
{
protected:
    vector<double> sides;  // довжини сторін чотирикутника (4 елементи)
    vector<double> angles; // кути при вершинах у градусах (4 елементи)

    // Оновлення сторін і кутів після зміни вершин
    void UpdateSidesAndAngles()
    {
        this->sides = GetPolygonSides();
        this->angles = GetPolygonAngles();
    }

public:
    // Конструктор за чотирма точками
    Quadrilateral(const Point& p1, const Point& p2, const Point& p3,
        const Point& p4, const string& n = "Чотирикутник") :
        PolygonVOS667({ p1, p2, p3, p4 }, n)
    {
        if (p1 == p2 || p2 == p3 || p3 == p4 || p4 == p1) {
            throw invalid_argument("Точки чотирикутника не повинні збігатися!");
        }
        UpdateSidesAndAngles();
        if (!IsValid()) throw invalid_argument("Некоректний чотирикутник!");
    }

    // Конструктор за вектором точок
    Quadrilateral(const vector<Point>& points,
        const string& n = "Чотирикутник") : PolygonVOS667(points, n)
    {
        if (points.size() != 4) {
            throw invalid_argument("Чотирикутник повинен мати рівно 4 вершини!");
        }
        if (points[0] == points[1] || points[1] == points[2] ||
            points[2] == points[3] || points[3] == points[0]) {
            throw invalid_argument("Точки чотирикутника не повинні збігатися!");
        }
        UpdateSidesAndAngles();
        if (!IsValid()) throw invalid_argument("Некоректний чотирикутник!");
    }

    // Конструктор копіювання
    Quadrilateral(const Quadrilateral& other) : PolygonVOS667(other),
        sides(other.sides), angles(other.angles) {
    }

    // Оператор присвоювання
    Quadrilateral& operator=(const Quadrilateral& other)
    {
        if (this != &other) {
            PolygonVOS667::operator=(other);
            this->sides = other.sides;
            this->angles = other.angles;
        }
        return *this;
    }

    // Оператор порівняння з урахуванням обертання чотирикутника
    bool operator==(const Quadrilateral& other) const
    {
        auto v1 = GetVertices();
        auto v2 = other.GetVertices();
        // Порівняння з урахуванням циклічного порядку
        for (size_t i = 0; i < 4; ++i) {
            if (v1[0] == v2[i] &&
                v1[1] == v2[(i + 1) % 4] &&
                v1[2] == v2[(i + 2) % 4] &&
                v1[3] == v2[(i + 3) % 4]) {
                return true;
            }
        }
        return false;
    }

    // Установлення всіх вершин
    void SetVertices(const vector<Point>& newVertices) override
    {
        if (newVertices.size() != 4) {
            throw invalid_argument("Чотирикутник повинен мати рівно "
                "4 вершини!");
        }
        if (newVertices[0] == newVertices[1] ||
            newVertices[1] == newVertices[2] ||
            newVertices[2] == newVertices[3] ||
            newVertices[3] == newVertices[0]) {
            throw invalid_argument("Точки чотирикутника не повинні збігатися!");
        }
        PolygonVOS667::SetVertices(newVertices);
        this->UpdateSidesAndAngles();
        if (!IsValid()) throw invalid_argument("Некоректний чотирикутник!");
    }

    // Установлення вершини за індексом
    void SetVertex(size_t index, const Point& p) override
    {
        if (index >= 4) {
            throw out_of_range("Невірний індекс вершини для чотирикутника!");
        }
        auto tempVertices = GetVertices();
        tempVertices[index] = p;
        if (tempVertices[0] == tempVertices[1] ||
            tempVertices[1] == tempVertices[2] ||
            tempVertices[2] == tempVertices[3] ||
            tempVertices[3] == tempVertices[0]) {
            throw invalid_argument("Точки чотирикутника не повинні збігатися!");
        }
        PolygonVOS667::SetVertex(index, p);
        this->UpdateSidesAndAngles();
        if (!IsValid()) throw invalid_argument("Некоректний чотирикутник!");
    }

    // Отримання сторін чотирикутника
    vector<double> GetSides() const { return sides; }

    // Отримання кутів чотирикутника
    vector<double> GetAngles() const { return angles; }

    // Перевірка, чи є чотирикутник паралелограмом
    // https://mathworld.wolfram.com/Parallelogram.html
    bool IsParallelogram() const
    {
        return GeometryUtils::AreParallel(GetVertices()[0], GetVertices()[1],
            GetVertices()[2], GetVertices()[3]) &&
            GeometryUtils::AreParallel(GetVertices()[1], GetVertices()[2],
                GetVertices()[3], GetVertices()[0]);
    }

    // Перевірка, чи є принаймні одна пара паралельних сторін
    // Повертає true, якщо є хоча б одна пара паралельних сторін
    // Примітка: відповідає американському визначенню трапеції (trapezoid),
    // де трапеція має "at least one pair of parallel sides".
    // У європейському/українському контексті трапеція має точно одну пару,
    // що перевіряється в класі Trapezoid::IsValid().
    // https://en.wikipedia.org/wiki/Trapezoid
    // https://mathworld.wolfram.com/Trapezoid.html
    // https://mathworld.wolfram.com/Trapezium.html
    bool Trapezoid() const  // американська трапеція
    {
        return GeometryUtils::AreParallel(GetVertices()[0], GetVertices()[1],
            GetVertices()[2], GetVertices()[3]) ||
            GeometryUtils::AreParallel(GetVertices()[1], GetVertices()[2],
                GetVertices()[3], GetVertices()[0]);
    }
    // Тут логіка не виключає випадки з двома парами (паралелограми).


    // Перевірка, чи є чотирикутник дельтоїдом (повітряним змієм)
    // Дельтоїд - чотирикутник із двома парами сусідніх сторін однакової 
    // довжини https://mathworld.wolfram.com/Kite.html
    bool IsKite() const
    {
        auto sides = GetSides();
        return (abs(sides[0] - sides[1]) < GeometryUtils::EPSILON &&
            abs(sides[2] - sides[3]) < GeometryUtils::EPSILON) ||
            (abs(sides[1] - sides[2]) < GeometryUtils::EPSILON &&
                abs(sides[3] - sides[0]) < GeometryUtils::EPSILON);
    }

    bool IsValid() const override
    {
        const auto& vertices = GetVertices();

        // Перевірка кількості вершин
        if (vertices.size() != 4) return false;
        // Перевірка розташування вершин у межах сітки
        if (!PolygonVOS667::IsValid()) return false;
        // Перевірка на простоту (відсутність самоперетинів)!
        if (!GeometryUtils::IsSimplePolygon(vertices)) return false;
        // Перевірка на невиродженість (площа > 0)
        if (abs(Area()) < GeometryUtils::EPSILON) return false;

        // Явна перевірка на колінеарність
        bool isCollinear = true;  //  вершини не лежать на одній прямій
        for (size_t i = 0; i < 4; ++i) {
            const Point& p1 = vertices[i];
            const Point& p2 = vertices[(i + 1) % 4];
            const Point& p3 = vertices[(i + 2) % 4];
            double cross = (p2.GetX() - p1.GetX()) * (p3.GetY() - p1.GetY()) -
                (p2.GetY() - p1.GetY()) * (p3.GetX() - p1.GetX());
            if (abs(cross) > GeometryUtils::EPSILON) {
                isCollinear = false;  // неколінеарні!
                break;
            }
        }
        if (isCollinear) return false;

        // Перевірка на опуклість (усі внутрішні кути < 180°)
        if (!GeometryUtils::IsConvexPolygon(vertices)) return false;

        return true;
    }

    // Обчислення площі за формулою Гаусса
    double Area() const override { return PolygonVOS667::Area(); }

    // Обчислення периметра
    double Perimeter() const override
    {
        return sides[0] + sides[1] + sides[2] + sides[3];
    }

    // Візуалізація чотирикутника
    void Draw() const override
    {
        const auto& vertices = GetVertices();
        cout << this->GetName() << " з 4 вершинами:\n"
            << "- " << (this->IsSimple() ? "простий" : "складний") << ", "
            << (IsNonDegenerate() ? "невироджений" : "вироджений") << ", "
            << (this->IsConvex() ? "опуклий" : "увігнутий") << ".\n";

        string type = this->IsParallelogram() ? "Паралелограм" :
            this->Trapezoid() ? "Трапеція" :
            this->IsKite() ? "Дельтоїд" : "Звичайний";
        cout << "Тип: " << type << "\n";

        cout << fixed << setprecision(2);
        cout << "Площа: " << Area() << ", Периметр: " << Perimeter() << "\n";

        cout << "Сторони чотирикутника:\n";
        for (size_t i = 0; i < 4; ++i) {
            cout << "Сторона " << i + 1 << ": " << sides[i] << "\n";
        }

        cout << "Кути чотирикутника (у градусах):\n";
        for (size_t i = 0; i < 4; ++i) {
            cout << "Кут при вершині " << i + 1 << ": " << angles[i] << "°\n";
        }

        for (size_t i = 0; i < 4; ++i) {
            cout << "Вершина " << i + 1 << ": (" << vertices[i].GetX()
                << ", " << vertices[i].GetY() << ")\n";
        }

        vector<string> grid(GRID_HEIGHT, string(GRID_WIDTH, '.'));
        for (size_t i = 0; i < 4; ++i) {
            DrawSegmentOnGrid(grid, vertices[i], vertices[(i + 1) % 4]);
        }

        for (int row = GRID_HEIGHT - 1; row >= 0; --row) {
            cout << grid[row] << "\n";
        }
        cout << "\n\n\n";
    }

    ~Quadrilateral() override = default;
};


// Похідний клас Паралелограм
// https://reference.wolfram.com/language/ref/Parallelogram.html
class Parallelogram : public Quadrilateral
{
public:
    // Конструктор за чотирма точками
    Parallelogram(const Point& p1, const Point& p2, const Point& p3,
        const Point& p4, const string& n = "Паралелограм") :
        Quadrilateral(p1, p2, p3, p4, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний паралелограм!");
    }

    // Конструктор за вектором точок
    Parallelogram(const vector<Point>& points,
        const string& n = "Паралелограм") : Quadrilateral(points, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний паралелограм!");
    }

    // Перевірка: чотирикутник має бути паралелограмом і опуклим
    bool IsValid() const override
    {
        // Перевірка базового класу Quadrilateral (включає 
        // опуклість, простоту і колінеарність)
        if (!Quadrilateral::IsValid()) return false;
        // Перевірка на паралельність протилежних сторін
        return IsParallelogram();
    }

    // Площа через добуток сторони на висоту 
    // (використовуємо першу сторону і кут між сусідніми сторонами)
    // https://mathworld.wolfram.com/Parallelogram.html#eqn4
    double Area() const override
    {
        return sides[0] * sides[1] * sin(angles[0] * M_PI / 180.0);
    }

    double Perimeter() const override {
        return 2 * (sides[0] + sides[1]);  // протилежні сторони рівні
    }

    void Draw() const override { Quadrilateral::Draw(); }

    ~Parallelogram() override = default;
};


// Похідний клас Трапеція
// https://resources.wolframcloud.com/FunctionRepository/resources/Trapezoid
// https://mathworld.wolfram.com/Trapezium.html
// https://en.wikipedia.org/wiki/Trapezoid
// https://mathworld.wolfram.com/PyramidalFrustum.html
class Trapezium : public Quadrilateral
{
public:
    // Конструктор за чотирма точками
    Trapezium(const Point& p1, const Point& p2, const Point& p3,
        const Point& p4, const string& n = "Трапеція") :
        Quadrilateral(p1, p2, p3, p4, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректна трапеція!");
    }

    // Конструктор за вектором точок
    Trapezium(const vector<Point>& points, const string& n = "Трапеція") :
        Quadrilateral(points, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректна трапеція!");
    }

    // Перевірка: чотирикутник має бути трапецією з точно однією парою
    // паралельних сторін (європейське/українське визначення).
    // Виключає паралелограми (дві пари паралельних сторін).
    // Примітка: У американському визначенні трапеція (trapezoid) має
    // "at least one pair of parallel sides", що включає паралелограми.
    // Цей код дотримується європейського/українського стандарту.
    // https://en.wikipedia.org/wiki/Trapezoid
    bool IsValid() const override
    {
        // Перевірка базового класу Quadrilateral (включає 
        // опуклість, простоту і колінеарність)
        if (!Quadrilateral::IsValid()) return false;

        // Перевірка кількості пар паралельних сторін
        bool parallelPair1 = GeometryUtils::AreParallel(GetVertices()[0],
            GetVertices()[1], GetVertices()[2], GetVertices()[3]);
        bool parallelPair2 = GeometryUtils::AreParallel(GetVertices()[1],
            GetVertices()[2], GetVertices()[3], GetVertices()[0]);

        // Трапеція має точно одну пару паралельних сторін
        return (parallelPair1 != parallelPair2);
    }

    // Площа через суму паралельних сторін і висоту...
    // https://mathworld.wolfram.com/Trapezoid.html#eqn2

    double Area() const override { return Quadrilateral::Area(); }

    double Perimeter() const override { return Quadrilateral::Perimeter(); }

    void Draw() const override { Quadrilateral::Draw(); }

    ~Trapezium() override = default;
};


// Похідний клас Дельтоїд
// https://mathworld.wolfram.com/Polykite.html
// https://mathworld.wolfram.com/PenroseTiles.html
class Kite : public Quadrilateral
{
public:
    // Конструктор за чотирма точками
    Kite(const Point& p1, const Point& p2, const Point& p3, const Point& p4,
        const string& n = "Дельтоїд") : Quadrilateral(p1, p2, p3, p4, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний дельтоїд!");
    }

    // Конструктор за вектором точок
    Kite(const vector<Point>& points, const string& n = "Дельтоїд") :
        Quadrilateral(points, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний дельтоїд!");
    }

    // Перевірка: чотирикутник має бути дельтоїдом, але не паралелограмом
    bool IsValid() const override
    {
        // Перевірка базового класу Quadrilateral (включає опуклість 
        // і колінеарність)
        if (!Quadrilateral::IsValid()) return false;
        // Дельтоїд має дві пари сусідніх сторін однакової довжини, 
        // але не є паралелограмом
        return IsKite() && !IsParallelogram();
    }

    // Площа через добуток діагоналей
    // https://mathworld.wolfram.com/Kite.html#eqn3
    double Area() const override
    {
        auto vertices = GetVertices();
        double d1 = GeometryUtils::Distance(vertices[0], vertices[2]);
        double d2 = GeometryUtils::Distance(vertices[1], vertices[3]);
        return (d1 * d2) / 2.0;
    }

    double Perimeter() const override
    {
        if (abs(sides[0] - sides[1]) < GeometryUtils::EPSILON &&
            abs(sides[2] - sides[3]) < GeometryUtils::EPSILON) {
            return 2 * (sides[0] + sides[2]);  // AB = BC, CD = DA
        }
        else if (abs(sides[1] - sides[2]) < GeometryUtils::EPSILON &&
            abs(sides[3] - sides[0]) < GeometryUtils::EPSILON) {
            return 2 * (sides[1] + sides[3]); // BC = CD, DA = AB
        }
        else {
            throw runtime_error("Некоректний дельтоїд: "
                "сторони не відповідають визначенню!");
        }
    }

    void Draw() const override { Quadrilateral::Draw(); }

    ~Kite() override = default;
};


// Похідний від класу Паралелограм клас Прямокутник
// https://mathworld.wolfram.com/BlanchesDissection.html
// https://mathworld.wolfram.com/GoldenRectangle.html
// https://mathworld.wolfram.com/PerfectRectangle.html
class Rectangulum : public Parallelogram
    // https://learn.microsoft.com/ru-ru/windows/win32/api/wingdi/nf-wingdi-rectangle
{
public:
    // Конструктор за чотирма точками
    Rectangulum(const Point& p1, const Point& p2, const Point& p3,
        const Point& p4, const string& n = "Прямокутник") :
        Parallelogram(p1, p2, p3, p4, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний прямокутник!");
    }

    // Конструктор за вектором точок
    Rectangulum(const vector<Point>& points, const string& n = "Прямокутник")
        : Parallelogram(points, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний прямокутник!");
    }

    // Перевірка: чотирикутник має бути паралелограмом із прямими кутами
    bool IsValid() const override
    {
        // Перевірка базового класу Parallelogram (включає опуклість, 
        // простоту, колінеарність і паралельність)
        if (!Parallelogram::IsValid()) return false;
        // Перевірка на прямі кути
        for (double angle : angles) {
            if (abs(angle - 90.0) > GeometryUtils::EPSILON) return false;
        }
        return true;
    }

    // Площа через добуток сусідніх сторін
    // https://mathworld.wolfram.com/Rectangle.html#eqn1
    double Area() const override { return sides[0] * sides[1]; }

    double Perimeter() const override { return 2 * (sides[0] + sides[1]); }

    void Draw() const override { Quadrilateral::Draw(); }

    ~Rectangulum() override = default;
};


// Похідний клас Ромб
// https://mathworld.wolfram.com/Rhombus.html
// https://mathworld.wolfram.com/Diamond.html
// https://mathworld.wolfram.com/RhombicHexecontahedron.html
// https://mathworld.wolfram.com/Lozenge.html
class Rhombus : public Parallelogram
{
public:
    // Конструктор за чотирма точками
    Rhombus(const Point& p1, const Point& p2, const Point& p3, const Point& p4,
        const string& n = "Ромб") : Parallelogram(p1, p2, p3, p4, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний ромб!");
    }

    // Конструктор за вектором точок
    Rhombus(const vector<Point>& points, const string& n = "Ромб") :
        Parallelogram(points, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний ромб!");
    }

    // Перевірка: чотирикутник має бути паралелограмом із рівними сторонами
    bool IsValid() const override
    {
        // Перевірка базового класу Parallelogram (включає опуклість, 
        // простоту, колінеарність і паралельність)
        if (!Parallelogram::IsValid()) return false;
        // Перевірка на рівність усіх сторін
        return abs(sides[0] - sides[1]) < GeometryUtils::EPSILON &&
            abs(sides[1] - sides[2]) < GeometryUtils::EPSILON &&
            abs(sides[2] - sides[3]) < GeometryUtils::EPSILON;
    }

    // Площа через добуток діагоналей
    // https://mathworld.wolfram.com/Rhombus.html#eqn3
    double Area() const override
    {
        auto vertices = GetVertices();
        double d1 = GeometryUtils::Distance(vertices[0], vertices[2]);
        double d2 = GeometryUtils::Distance(vertices[1], vertices[3]);
        return (d1 * d2) / 2.0;
    }

    double Perimeter() const override
    {
        return 4 * sides[0];  // усі сторони рівні
    }

    void Draw() const override { Quadrilateral::Draw(); }

    ~Rhombus() override = default;
};


// Похідний від класу Прямокутник клас Квадрат
// https://mathworld.wolfram.com/Square.html
// https://mathworld.wolfram.com/vanAubelsTheorem.html
// https://mathworld.wolfram.com/PythagorassConstant.html
class Square : public Rectangulum
{
public:
    // Конструктор за чотирма точками
    Square(const Point& p1, const Point& p2, const Point& p3, const Point& p4,
        const string& n = "Квадрат") : Rectangulum(p1, p2, p3, p4, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний квадрат!");
    }

    // Конструктор за вектором точок
    Square(const vector<Point>& points, const string& n = "Квадрат") :
        Rectangulum(points, n)
    {
        if (!IsValid()) throw invalid_argument("Некоректний квадрат!");
    }

    // Перевірка: чотирикутник має бути прямокутником із рівними сторонами
    bool IsValid() const override
    {
        // Перевірка базового класу Rectangulum (включає опуклість, 
        // колінеарність, простоту, паралельність і прямі кути)
        if (!Rectangulum::IsValid()) return false;
        // Перевірка на рівність усіх сторін
        return abs(sides[0] - sides[1]) < GeometryUtils::EPSILON &&
            abs(sides[1] - sides[2]) < GeometryUtils::EPSILON &&
            abs(sides[2] - sides[3]) < GeometryUtils::EPSILON;
    }

    // Площа через квадрат сторони
    // https://mathworld.wolfram.com/Square.html#eqn2
    double Area() const override { return sides[0] * sides[0]; }

    double Perimeter() const override { return 4 * sides[0]; }

    void Draw() const override { Quadrilateral::Draw(); }

    ~Square() override = default;
};


int main() {

    // Налаштування кодування консолі для української мови
    SetConsoleOutputCP(1251);

    // Відкриваємо файл для запису
    ofstream outFile("shapes_output.txt");
    // https://en.cppreference.com/w/cpp/io/basic_ofstream.html
    if (!outFile.is_open()) {
        cout << "Помилка: не вдалося відкрити файл для запису!\n";
        return 1;
    }

    // Зберігаємо поточний буфер cout
    streambuf* coutbuf = cout.rdbuf();
    // https://en.cppreference.com/w/cpp/io/basic_streambuf.html
    // Перенаправляємо cout у файл
    cout.rdbuf(outFile.rdbuf());

    try {

        // Тест 1: Точка (Point)
        cout << "=== Тест 1: Точка ===\n";
        Point p1(2, 3, "Точка 1");
        Point p2({ 4, 5 }, "Точка 2");  // конструктор з вектором
        Point p3(p2);   // конструктор копіювання
        Point p4 = p1;  // оператор присвоювання
        Point p5;
        p1.Draw();
        p2.Draw();
        p3.Draw();
        p4.Draw();
        p5.SetCoordinates(-1, -1);  // тест сетера
        p5.Draw();

        // Тест 2: Відрізок (LineSegment)
        cout << "=== Тест 2: Відрізок ===\n";
        LineSegment ls1(Point(-5, 0), Point(5, 0), "Відрізок 1");
        LineSegment ls2({ Point(0, -3), Point(0, 4) }, "Відрізок 2");
        LineSegment ls3 = ls1;
        ls1.Draw();
        ls2.Draw();
        ls3.SetEndPoint1(Point(-7, -4));
        ls3.Draw();

        // Тест 3: Еліпс (Ellipsis)
        cout << "=== Тест 3: Еліпс ===\n";
        Ellipsis e1(6, 4, Point(0, 0), "Еліпс 1");
        Ellipsis e2({ 5, 7 }, Point(1, 1), "Еліпс 2");  // вектор параметрів
        Ellipsis e3 = e1;
        e1.Draw(true);         // параметричний метод
        e2.Draw(false);        // cітковий метод
        e3.SetSemiAxes(2, 3);
        e3.Draw(true);

        // Тест 4: Коло (Circle)
        cout << "=== Тест 4: Коло ===\n";
        Circle c1(5, Point(0, 0), "Коло 1");
        Circle c2(14, true, Point(1, 1), "Коло 2"); // конструктор з діаметром
        Circle c3 = c1;
        c1.Draw(true);
        c2.Draw(false);
        c3.SetRadius(4);
        c3.Draw(true);

        // Тест 5: Багатокутник (PolygonVOS667) - опуклий семикутник
        cout << "=== Тест 5: Опуклий семикутник ===\n";
        vector<Point> convexPoints = {
            Point(-8, -8),  // вершина 1
            Point(-3, -9),  // вершина 2
            Point(0, -6),   // вершина 3
            Point(8, -8),   // вершина 4
            Point(9, 2),    // вершина 5
            Point(4, 9),    // вершина 6
            Point(-6, 7)    // вершина 7
        };
        PolygonVOS667 poly1(convexPoints, "Опуклий 7-кутник");
        PolygonVOS667 poly2 = poly1;  // оператор присвоювання
        poly1.Draw();
        poly2.SetVertex(0, Point(-3, -3));  // тест сетера за індексом
        poly2.Draw();

        // Тест 6: Багатокутник (PolygonVOS667) - складний восьмикутник
        cout << "=== Тест 6: Складний неопуклий восьмикутник ===\n";
        vector<Point> nonConvexPoints = {
            Point(-8, -8),  // вершина 1
            Point(-4, 2),   // вершина 2
            Point(0, -6),   // вершина 3
            Point(8, -2),   // вершина 4
            Point(6, 8),    // вершина 5
            Point(2, -4),   // вершина 6
            Point(-2, 6),   // вершина 7
            Point(-6, -4)   // вершина 8
        };
        PolygonVOS667 poly3(nonConvexPoints,
            "Складний неопуклий восьмикутник");
        PolygonVOS667 poly4 = poly3;
        poly3.Draw();
        poly4.SetVertex(1, Point(0, 3));
        poly4.Draw();

        // Тест 7: Трикутник (Triangle)
        cout << "=== Тест 7: Трикутник ===\n";
        Triangle t1(Point(-4, -4), Point(4, -4), Point(0, 4), "Трикутник");
        Triangle t2({ Point(-3, -3), Point(3, -3), Point(0, 3) },
            "Трикутник 2");  // вектор точок
        Triangle t3 = t1;
        t1.Draw();
        t2.Draw();
        t3.SetVertex(0, Point(-2, -2));
        t3.Draw();

        // Тест 8: Звичайний гострокутний трикутник
        cout << "=== Тест 8: Звичайний гострокутний трикутник ===\n";
        ScaleneAcuteTriangle t4(Point(-5, -5), Point(4, -4), Point(0, 3),
            "Звичайний гострокутний трикутник",
            TriangleChecks::ScaleneAcuteCheck);
        ScaleneAcuteTriangle t5 = t4;
        t4.Draw();
        t5.SetVertex(0, Point(-3, -3));
        t5.Draw();

        // Тест 9: Рівнобедрений гострокутний трикутник
        cout << "=== Тест 9: Рівнобедрений гострокутний трикутник ===\n";
        IsoscelesAcuteTriangle t6({ Point(0, -5), Point(-5, 5), Point(5, 5) },
            "Рівнобедрений гострокутний трикутник",
            TriangleChecks::IsoscelesAcuteCheck);
        IsoscelesAcuteTriangle t7 = t6;
        t6.Draw();
        t7.SetVertex(0, Point(0, -4));
        t7.Draw();

        // Тест 10: Рівносторонній гострокутний трикутник
        cout << "=== Тест 10: Рівносторонній гострокутний трикутник ===\n";
        EquilateralAcuteTriangle t8({ Point(0, -4), Point(-4, 2.92820323028),
            Point(4, 2.92820323028) }, "Рівносторонній гострокутний трикутник",
            TriangleChecks::EquilateralAcuteCheck);
        EquilateralAcuteTriangle t9 = t8;
        t8.Draw();
        t9.SetVertex(0, Point(0, -4));
        t9.Draw();

        // Тест 11: Звичайний тупокутний трикутник
        cout << "=== Тест 11: Звичайний тупокутний трикутник ===\n";
        ScaleneObtuseTriangle t10(Point(-8, 0), Point(8, 0), Point(3, 5),
            "Звичайний тупокутний трикутник",
            TriangleChecks::ScaleneObtuseCheck);
        ScaleneObtuseTriangle t11 = t10;
        t10.Draw();
        t11.SetVertex(0, Point(-7, 0));
        t11.Draw();

        // Тест 12: Рівнобедрений тупокутний трикутник
        cout << "=== Тест 12: Рівнобедрений тупокутний трикутник ===\n";
        IsoscelesObtuseTriangle t12({ Point(0, -4), Point(-9, 2), Point(9, 2) },
            "Рівнобедрений тупокутний трикутник",
            TriangleChecks::IsoscelesObtuseCheck);
        IsoscelesObtuseTriangle t13 = t12;
        t12.Draw();
        t13.SetVertex(0, Point(0, -3));
        t13.Draw();

        // Тест 13: Звичайний прямокутний трикутник
        cout << "=== Тест 13: Звичайний прямокутний трикутник ===\n";
        ScaleneRightTriangle t14({ Point(-8, -8), Point(8, -8), Point(-8, 6) },
            "Звичайний прямокутний трикутник",
            TriangleChecks::ScaleneRightCheck);
        ScaleneRightTriangle t15 = t14;
        t14.Draw();
        t15.SetVertex(1, Point(7, -8));
        t15.SetVertex(2, Point(-8, 8));
        t15.Draw();

        // Тест 14: Рівнобедрений прямокутний трикутник
        cout << "=== Тест 14: Рівнобедрений прямокутний трикутник ===\n";
        IsoscelesRightTriangle t16({ Point(-8, -8), Point(8, -8),
            Point(8, 8) }, "Рівнобедрений прямокутний трикутник",
            TriangleChecks::IsoscelesRightCheck);
        IsoscelesRightTriangle t17 = t16;
        t16.Draw();
        t17.SetVertex(0, Point(-8, 8));
        t17.Draw();

        // Тест 15: Чотирикутник
        cout << "=== Тест 15: Чотирикутник ===\n";
        Quadrilateral q1(Point(-8, -7), Point(6, -8), Point(7, 5),
            Point(-6, 6), "Чотирикутник");
        vector<Point> generalQuadPoints = { Point(-7, -6), Point(5, -7),
            Point(8, 4), Point(-5, 5) };
        Quadrilateral q2(generalQuadPoints, "Чотирикутник 2");
        Quadrilateral q3 = q1;
        q1.Draw();
        q2.Draw();
        q3.SetVertex(0, Point(-7, -6));
        q3.Draw();

        // Тест 16: Паралелограм
        cout << "=== Тест 16: Паралелограм ===\n";
        Parallelogram par(Point(-9, -8), Point(7, -8), Point(9, 2),
            Point(-7, 2), "Паралелограм");
        Parallelogram par2 = par;
        par.Draw();
        vector<Point> newParVertices = { Point(-8, -7), Point(6, -7),
        Point(8, 3), Point(-6, 3) };
        par2.SetVertices(newParVertices);
        par2.Draw();

        // Тест 17: Трапеція
        cout << "=== Тест 17: Трапеція ===\n";
        Trapezium trap(Point(-8, -8), Point(8, -8), Point(6, 2),
            Point(-6, 2), "Трапеція");
        Trapezium trap2 = trap;
        trap.Draw();
        trap2.SetVertex(0, Point(-10, -8));
        trap2.Draw();

        // Тест 18: Дельтоїд
        cout << "=== Тест 18: Дельтоїд ===\n";
        Kite kite(Point(0, -6), Point(-4, 0), Point(0, 6), Point(2, 0),
            "Дельтоїд");
        Kite kite2 = kite;
        kite.Draw();
        vector<Point> newKiteVertices = { Point(0, -10), Point(-4, 0),
            Point(0, 10), Point(2, 0) };
        kite2.SetVertices(newKiteVertices);
        kite2.Draw();

        // Тест 19: Прямокутник
        cout << "=== Тест 19: Прямокутник ===\n";
        Rectangulum rect(Point(-8, -8), Point(8, -8), Point(8, 8),
            Point(-8, 8), "Прямокутник");
        Rectangulum rect2 = rect;
        rect.Draw();
        vector<Point> newRectVertices = { Point(-7, -7), Point(7, -7),
            Point(7, 7), Point(-7, 7) };
        try {
            rect2.SetVertices(newRectVertices);
            rect2.Draw();
        }
        catch (const exception& e) {
            cout << "Помилка: " << e.what() << "\n";
        }

        // Тест 20: Ромб
        cout << "=== Тест 20: Ромб ===\n";
        Rhombus rhomb(Point(0, -8), Point(8, 0), Point(0, 8),
            Point(-8, 0), "Ромб");
        Rhombus rhomb2 = rhomb;
        rhomb.Draw();
        vector<Point> newRhombVertices = { Point(0, -7), Point(7, 0),
            Point(0, 7), Point(-7, 0) };
        try {
            rhomb2.SetVertices(newRhombVertices);
            rhomb2.Draw();
        }
        catch (const exception& e) {
            cout << "Помилка: " << e.what() << "\n";
        }

        // Тест 21: Квадрат
        cout << "=== Тест 21: Квадрат ===\n";
        Square square(Point(-5, -5), Point(5, -5), Point(5, 5),
            Point(-5, 5), "Квадрат");
        Square square2 = square; // Оператор присваювання
        square.Draw();
        vector<Point> newSquareVertices = { Point(-4, -4), Point(4, -4),
            Point(4, 4), Point(-4, 4) };
        try {
            square2.SetVertices(newSquareVertices);
            square2.Draw();
        }
        catch (const exception& e) {
            cout << "Помилка: " << e.what() << "\n";
        }

        // Тест 22: Некоректна фігура (збіг точок)
        cout << "=== Тест 22: Вироджений трикутник (колінеарний) ===\n";
        try {
            ScaleneAcuteTriangle t18(Point(0, 0), Point(1, 1), Point(2, 2),
                "Вироджений трикутник", TriangleChecks::ScaleneAcuteCheck);
            t18.Draw();
        }
        catch (const exception& e) {
            cout << "Помилка: " << e.what() << "\n\n\n";
        }

        cout << "=== Тест 23: Еліпс на межі сітки ===\n";
        try {
            Ellipsis e4(10, 10, Point(0, 0), "Еліпс на межі");
            e4.Draw(true);
        }
        catch (const exception& e) {
            cout << "Помилка: " << e.what() << "\n\n\n";
        }
    }

    catch (const exception& e) {
        cout << "Помилка: " << e.what() << "\n\n";
    }

    // Відновлюємо стандартний вивід
    cout.rdbuf(coutbuf);
    // Закриваємо файл
    outFile.close();

    // Повідомлення про успішне збереження
    cout << "Результати Draw збережено у файл shapes_output.txt\n";

    return 0;
}