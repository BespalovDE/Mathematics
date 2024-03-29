﻿// johnson-trotter.cpp : алгоритм Джонсона-Троттера

#include <iostream>
#include <vector>
#include <boost/timer.hpp>

void ShowStep(const std::vector<size_t>& v)
{
    copy(++v.begin(), --v.end(), std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << std::endl;
}

int main()
{
    //int n = 10; // 0.018   11 - 0,2  12 - 2,3   13 - 29,897   14 - 417,473  => 15 - 6262.095  20 - 11.65*10^9   50 - 1.456*10^55   100 - 4.469*10^148
    int n = 10; // 0.018
    std::vector<size_t> p;
    std::vector<size_t> d;
    std::vector<size_t> r;
    p.push_back(n + 1);
    r.push_back(0);
    d.push_back(0);
    for (int i = 1; i <= n; i++)
    {
        p.push_back(i);
        r.push_back(i);
        d.push_back(-1);
    }
    p.push_back(n + 1);
    d[1] = 0;
    int m = n + 1;
    boost::timer t;
    t.restart();
    while (m != 1)
    {
        //ShowStep(p);
        m = n;
        while (p[r[m] + d[m]] > m)
        {
            d[m] = 0-d[m];
            m = m - 1;
        }
        std::swap(p[r[m]], p[r[m] + d[m]]);
        std::swap(r[p[r[m]]], r[m]);
    }
    double duration = t.elapsed();
    std::cout << duration << std::endl;
}
