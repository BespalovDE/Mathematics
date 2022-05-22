// KuhnsAlgorithm.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <map>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using Matrix = boost::numeric::ublas::matrix<int>;
using ResultPair = std::map <int, int>;

struct Args
{
	std::string fileName;
};

struct MatrixLen
{
	int p;
	int q;
};

std::optional<Args> ParseArgs(int argc, char *argv[], std::ostream &outStream)
{
	if (argc != 2)
	{
		outStream << "Invalid arguments count.\n";
		outStream << "Usage: SpanningTrees.exe <input file name>\n";
		return std::nullopt;
	}
	Args args;
	args.fileName = argv[1];
	return args;
}

bool FillMatrix(Matrix &m, const MatrixLen &matrixLen, std::istream &inputFile)
{
	std::string line;
	for (int i = 0; i < matrixLen.p; i++)
	{
		for (int j = 0; j < matrixLen.q; j++)
		{
			m(i, j) = 0;
		}
	}
	int i = 0;
	while (getline(inputFile, line))
	{
		std::istringstream strm(line);
		std::string inputValue;
		while (strm >> inputValue)
		{
			try
			{
				int j = std::atoi(inputValue.c_str());
				if (j > matrixLen.p)
				{
					return false;
				}
				m(i, j - 1) = j;
			}
			catch (const std::exception& /* error*/)
			{
				return false;
			}
		}
		i++;
	}

	return true;
}

bool TryKuhnsAlgoritm(const Matrix &m, const MatrixLen &matrixLen, std::vector<int> &connectionPQ, std::vector<bool> &usedPeak, int v)
{
	if (usedPeak[v])
	{
		return false;
	}
	usedPeak[v] = true;
	for (size_t i = 0; i < matrixLen.q; ++i)
	{
		int to = m(v,i) - 1;
		if (to >= 0)
		if (connectionPQ[to] == -1 || TryKuhnsAlgoritm(m, matrixLen, connectionPQ, usedPeak, connectionPQ[to]))
		{
			connectionPQ[to] = v;
			return true;
		}
	}
	return false;
}

void GetKuhnsAlgoritm(const Matrix &m, const MatrixLen &matrixLen, ResultPair &resultPair)
{
	std::vector<int> connectionPQ; // связь долей через ребро - это номер вершины первой доли, связанной ребром с вершиной i второй доли
	std::vector<bool> usedPeak; // массив посещённых вершин в обходе в глубину
	connectionPQ.assign(matrixLen.q, -1);
	for (int v = 0; v < matrixLen.p; v++) 
	{
		usedPeak.assign(matrixLen.p, false);
		TryKuhnsAlgoritm(m, matrixLen, connectionPQ, usedPeak, v);
		/*copy(connectionPQ.begin(), connectionPQ.end(), std::ostream_iterator<int>(std::cout, " \n"));
		std::cout << "\n";*/
	}
	for (int i = 0; i < matrixLen.q; ++i)
	{
		if (connectionPQ[i] != -1)
		{
			resultPair.emplace(connectionPQ[i] + 1, i + 1);
			//printf("%d %d\n", connectionPQ[i] + 1, i + 1);
		}
	}
}

void ShowResult(ResultPair &resultPair, int lenP, std::ostream &outStream)
{
	outStream << "NO" << std::endl;
	for (int i = 1; i <= lenP; i++)
	{
		auto search = resultPair.find(i);
		if (search != resultPair.end())
		{
			outStream << search->first << " " << search->second << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv, std::cout);
	std::ifstream inputFile(args->fileName);
	int matrixLen = 0;
	if (inputFile.is_open() && inputFile.peek() != EOF)
	{
		std::string line;
		getline(inputFile, line);
		std::istringstream strm(line);
		MatrixLen matrixLen;
		matrixLen.p = 0;
		matrixLen.q = 0;
		strm >> matrixLen.p;
		strm >> matrixLen.q;
		if (matrixLen.p == 0 || matrixLen.q == 0)
		{
			std::cout << "File has is not numeric values!" << std::endl;
			return 1;
		}
		Matrix inputMatrix(matrixLen.p, matrixLen.q);
		if (!FillMatrix(inputMatrix, matrixLen, inputFile))
		{
			std::cout << "File has the not correct values!" << std::endl;
			return 1;
		}
		//std::cout << inputMatrix << std::endl;
		ResultPair resultPair;
		GetKuhnsAlgoritm(inputMatrix, matrixLen, resultPair);
		if (matrixLen.p == matrixLen.q && resultPair.size() == matrixLen.p)
		{
			std::cout << "YES" << std::endl;
			return 0;
		}
		ShowResult(resultPair, matrixLen.p, std::cout);
	}
	else
	{
		std::cout << "Empty file!" << std::endl;
		return 1;
	}
}
