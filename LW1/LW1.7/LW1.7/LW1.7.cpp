// LW1.7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>

void ClearNuls(std::vector<size_t>& v)
{
    if (v[v.size() - 1] == 0)
    {
        v.pop_back();
    }
}

void ShowStep(const std::vector<size_t>& v)
{
    copy(v.begin(), v.end(), std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << std::endl;
}

void GetMin(const std::vector<size_t>& v, int &minNumber, int &indexMin)
{
    for (int i = 0; i < v.size() - 1; i++)
    {
        if (minNumber > v[i])
        {
            minNumber = v[i];
            indexMin = i;
        }
    }
}

void GetOst(const std::vector<size_t>& v, int &ostEd, int indexMin, int startSum)
{
    for (int i = 0; i <= indexMin; i++)
    {
        ostEd += v[i];
    }
    ostEd = startSum - ostEd;
}

void RecountOst(std::vector<size_t>& v, int& ostEd, int indexMin)
{
    for (int i = indexMin + 1; i < v.size(); i++)
    {
        v[i] = 0;
    }
    for (int i = indexMin + 1; i < v.size(); i++)
    {
        if (ostEd > 0)
        {
            v[i]++;
            ostEd--;
        }
        if (ostEd > 0 && i == v.size() - 1)
        {
            i = indexMin;
        }
    }
}

void Solve(std::vector<size_t>& v, const int& startSum)
{
    while (v[0] < startSum)
    {
        ClearNuls(v);
        ShowStep(v);
        int minNumber = startSum;
        int indexMin = 0;
        GetMin(v, minNumber, indexMin);
        v[indexMin]++;
        int ostEd = 0;
        GetOst(v, ostEd, indexMin, startSum);
        RecountOst(v, ostEd, indexMin);
    }
}


int main()
{
    int number = 15;
    std::vector<size_t> v;
    for (int i = 0; i < number; i++)
    {
        v.push_back(1);
    }
    Solve(v, number);
    return 0;
}




// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
