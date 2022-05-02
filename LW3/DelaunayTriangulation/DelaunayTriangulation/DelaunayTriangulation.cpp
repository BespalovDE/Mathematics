// DelaunayTriangulation.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Point
{
    double x;
    double y;
};

struct Triangle
{
    Point a;
    Point b;
    Point c;
};

struct CircleAroundTriangle
{
    Triangle t;
    Point center;
    double radius;
};

using PointsVector = std::vector<Point>;
using TriangleVector = std::vector<Triangle>;

Triangle GetBigTriangle(PointsVector &pVector) // функция, находящая треугольник, содержащий все точки множества
{
    double minx = 1000000, maxx = -1000000, miny = 1000000, maxy = -1000000;
    for (PointsVector::size_type i = 0; i != pVector.size(); i++)
    {
        if (minx > pVector[i].x)
        {
            minx = pVector[i].x;
        }
        if (miny > pVector[i].y)
        {
            miny = pVector[i].y;
        }
        if (maxx < pVector[i].x)
        {
            maxx = pVector[i].x;
        }
        if (maxy < pVector[i].y)
        {
            maxy = pVector[i].y;
        }
    }
    double dx = maxx - minx;
    double dy = maxy - miny;
    double dxy = (dx > dy) ? dx : dy;
    double midx = dx * 0.5 + minx;
    double midy = dy * 0.5 + miny;
    return
        {
            {midx - 10 * dxy, midy - 10 * dxy},
            {midx, midy + 10 * dxy},
            {midx + 10 * dxy, midy - 10 * dxy}
        };
}

CircleAroundTriangle GetCircleAroundTriangle(Triangle triangle) // вычисление центра и радиуса описанной окружности вокруг треугольника
{
    double x1 = triangle.a.x, y1 = triangle.a.y;
    double x2 = triangle.b.x, y2 = triangle.b.y;
    double x3 = triangle.c.x, y3 = triangle.c.y;
    double dy12 = abs(y1 - y2), dy23 = abs(y2 - y3);
    double xc, yc;
    if (dy12 < DBL_EPSILON) 
    {
        double m2 = -((x3 - x2) / (y3 - y2));
        double mx2 = (x2 + x3) / 2, my2 = (y2 + y3) / 2;
        xc = (x1 + x2) / 2, yc = m2 * (xc - mx2) + my2;
    }
    else if (dy23 < DBL_EPSILON) 
    {
        double m1 = -((x2 - x1) / (y2 - y1));
        double mx1 = (x1 + x2) / 2, my1 = (y1 + y2) / 2;
        xc = (x2 + x3) / 2, yc = m1 * (xc - mx1) + my1;
    }
    else 
    {
        double m1 = -((x2 - x1) / (y2 - y1)), m2 = -((x3 - x2) / (y3 - y2));
        double mx1 = (x1 + x2) / 2, my1 = (y1 + y2) / 2;
        double mx2 = (x2 + x3) / 2, my2 = (y2 + y3) / 2;
        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        if (dy12 > dy23)
        {
            yc = m1 * (xc - mx1) + my1;
        }
        else
        {
            yc = m2 * (xc - mx2) + my2;
        }
    }
    double dx = x2 - xc, dy = y2 - yc;
    return { triangle,  { xc, yc }, dx * dx + dy * dy };
}

void DelecteMultiplesEdgec(/*std::vector<double>& edges*/PointsVector &edges ) // удаление кратных ребер
{
    for (int j = edges.size() - 1; j >= 0;)
    {
        double b = edges[j].x;
        double a = edges[j].y;
        j--;
        double n, m;
        for (int i = j; i >= 0;)
        {
            n = edges[i].x;
            m = edges[i].y;
            i--;
            auto vBegin = edges.cbegin();
            if (a == m && b == n)
            {
                edges.erase(vBegin + j + 1);
                break;
            }
            vBegin = edges.cbegin();
            if (a == n && b == m)
            {
                edges.erase(vBegin + i + 1);
                break;
            }
        }
    }
    
    /*for (int j = edges.size() - 1; j >= 0;)
    {
        double b = edges[j]; 
        j--;
        double a = edges[j]; 
        j--;
        double n, m;
        for (double i = j; i >= 0;) 
        {
            n = edges[i]; 
            i--;
            m = edges[i]; 
            i--;
            auto vBegin = edges.cbegin();
            if (a == m && b == n) 
            {
                auto vEnd = vBegin + j + 3;
                edges.erase(vBegin + j + 1, vEnd);
                edges.erase(vBegin + i + 1, vEnd);
                break;
            }
            vBegin = edges.cbegin();
            if (a == n && b == m) 
            {
                auto vEnd = vBegin + j + 3;
                edges.erase(vBegin + j + 1, vEnd);
                edges.erase(vBegin + i + 1, vEnd);
                break;
            }
        }
    }*/
}

TriangleVector Triangulate(PointsVector &pVector) {
    int n = pVector.size();
    TriangleVector triangleVector;
    if (n < 3)
    {
        return triangleVector; // треугольников нет
    }
    PointsVector points = pVector; // копия массива

    std::vector<int> indexes;
    for (int i = 0; i < n; i++) 
        indexes.emplace_back(i);
    //sort(indexes.begin(), indexes.end(), [](Point lhs, Point rhs) {return lhs.x < rhs.x; });

    // находим треугольник, содержащий все точки, и вставлем его в конец массива с вершинами
    Triangle bigTriangle = GetBigTriangle(pVector);
    points.emplace_back(bigTriangle.a);
    points.emplace_back(bigTriangle.b);
    points.emplace_back(bigTriangle.c);

    std::vector<CircleAroundTriangle> triangleStructs = { GetCircleAroundTriangle({ points[n], points[n + 1], points[n + 2] }) };
    std::vector<CircleAroundTriangle> ans;
    PointsVector edges;
    
    // перебираем все точки
    for (int i = indexes.size() - 1; i >= 0; i--) 
    {
        
        // перебираем все треугольники
        // если точка находится внутри треугольника, то нужно его удалить
        for (int j = triangleStructs.size() - 1; j >= 0; j--)
        {
            // если точка справа от описанной окружности, то треугольник проверять больше не нужно
            // точки отсортированы и поэтому тоже будут справа
            double dx = points[indexes[i]].x - triangleStructs[j].center.x;
            if (dx > 0 && dx * dx > triangleStructs[j].radius) 
            {
                ans.emplace_back(triangleStructs[j]);
                //cur_points.splice(j, 1);
                auto vBegin = triangleStructs.cbegin();
                triangleStructs.erase(vBegin + j);
                continue;
            }
            
            // если точка вне окружности, то треугольник изменять не нужно
            double dy = points[indexes[i]].y - triangleStructs[j].center.y;
            if (dx * dx + dy * dy - triangleStructs[j].radius > DBL_EPSILON) 
            {
                continue;
            }
            // удаляем треугольник и добавляем его стороны в список ребер

            /*edges.emplace_back(
                 triangleStructs[j].t.a, triangleStructs[j].t.b,
                 triangleStructs[j].t.b, triangleStructs[j].t.c,
                 triangleStructs[j].t.c, triangleStructs[j].t.a
            );*/
            edges.emplace_back(triangleStructs[j].t.a);
            edges.emplace_back(triangleStructs[j].t.b);
            edges.emplace_back(triangleStructs[j].t.c);

            auto vBegin = triangleStructs.cbegin();
            triangleStructs.erase(vBegin + j);
            //cur_points.splice(j, 1);
        }
        // удаляем кратные ребра
        DelecteMultiplesEdgec(edges);
        // создаем новые треугольники последовательно по списку ребер
        for (int j = edges.size() - 1; j >= 0;)
        {
            if (j >= 2)
            {
                Point b = edges[j - 1]; //{ edges[j], edges[j - 1] };
                Point a = edges[j];//{ edges[j] - 2, edges[j - 3] };
                triangleStructs.emplace_back(GetCircleAroundTriangle({ a, b,  pVector[indexes[i]] }));
            }
            j = j - 2;
        }
        edges.clear();
        //edges = [];
    }
    

    TriangleVector tr;
    // формируем массив с триангуляцией
    for (int i = triangleStructs.size() - 1; i >= 0; i--) {
        ans.emplace_back(triangleStructs[i]);
    }
    for (int i = 0; i < ans.size(); i++)
    {
        //if (ans[i].a < n && ans[i].b < n && ans[i].c < n) {
            //tr.emplace_back(ans[i].t.a, ans[i].t.b, ans[i].t.c);
        tr.emplace_back(ans[i].t);
        //}
    }
    return tr;
}


int main()
{
    PointsVector pVector = { {0.0,0.0}, {1.0,1.0}, {0.0,1.0}, {1.0,0.0} };
    TriangleVector triangleVector = Triangulate(pVector);
    std::cout << triangleVector[0].a.x << " " << triangleVector[0].a.y << " " << triangleVector[0].b.x << " " << triangleVector[0].b.y << " " << triangleVector[0].c.x << " " << triangleVector[0].c.y;
}

// формируем массив с триангуляцией
    /*for (int i = triangleStructs.size() - 1; i >= 0; i--) {
        ans.emplace_back(triangleStructs[i]);
    }
    for (int i = 0; i < ans.size(); i++)
    {
        //if (ans[i].a < n && ans[i].b < n && ans[i].c < n) {
            tr.emplace_back(ans[i].t.a, ans[i].t.b, ans[i].t.c);
        //}
    }*/