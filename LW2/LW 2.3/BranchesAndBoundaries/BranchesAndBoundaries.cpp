// BranchesAndBoundaries.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <optional>
#include <vector>
#include <cctype>
#include <map>

using WayPair = std::map <int, int>;

struct Args
{
	std::string fileName;
};

struct BranchPoint
{
	int x;
	int y;
	int weight;
};

struct BestWay
{
	int sumOfWay;
	WayPair wayPair;
};

std::optional<Args> ParseArgs(int argc, char* argv[], std::ostream& outStream)
{
	if (argc != 2)
	{
		outStream << "Invalid arguments count.\n";
		outStream << "Usage: BranchesAndBoundaries.exe <input file name>\n";
		return std::nullopt;
	}
	Args args;
	args.fileName = argv[1];
	return args;
}

int count_digits(const std::string &s)
{
	return std::count_if(s.begin(), s.end(),
		[](unsigned char c) { return std::isdigit(c); } // correct
	);
}

void FillMatrix(int **&x, int matrixLen, std::istream &inputFile)
{
	std::string line;

	for (int i = 0; i < matrixLen; i++)
	{
		for (int j = 0; j < matrixLen; j++)
		{
			if (i == 0 || j == 0)
			{
				if (i == 0)
					x[i][j] = j;
				if (j == 0)
					x[i][j] = i;
			}
			else
			{
				inputFile >> line;
				if (count_digits(line) == line.length())
				{
					x[i][j] = std::stoi(line);
				}
				else
				{
					x[i][j] = -1;
				}
			}
		}
	}
}

void PrintMatrix(int **&x, int matrixLen)
{
	for (int i = 0; i < matrixLen; i++)
	{
		for (int j = 0; j < matrixLen; j++)
			std::cout << x[i][j] << "\t";
		std::cout << "\n";
	}
}

int GetMinInLine(int **&x, int iLen, int iLine)
{
	int iMin = INT_MAX;
	for (int i = 1; i < iLen; i++)
	{
		if (x[iLine][i] >= 0 && x[iLine][i] < iMin)
			iMin = x[iLine][i];
	}
	return iMin;
}

int GetMinInRow(int **&x, int iLen, int iLine)
{
	int iMin = INT_MAX;
	for (int i = 1; i < iLen; i++)
	{
		if (x[i][iLine] >= 0 && x[i][iLine] < iMin)
			iMin = x[i][iLine];
	}
	return iMin;
}

void minusLineValueDj(int **&x, int iLen, int iLine, int minusValue)
{
	for (int i = 1; i < iLen; i++)
	{
		if (x[iLine][i] > 0)
			x[iLine][i] = x[iLine][i] - minusValue;
	}
}

void minusRowValueDi(int**& x, int iLen, int iLine, int minusValue)
{
	for (int i = 1; i < iLen; i++)
	{
		if (x[i][iLine] > 0)
			x[i][iLine] = x[i][iLine] - minusValue;
	}
}

int GetConstPrivJ(int **&x, int matrixLen, int nowLine, int outValue)
{
	int iMin = -1;
	for (int j = 1; j < matrixLen; j++)
	{
		if (x[j][nowLine] >= 0 && j != outValue)
		{
			if (iMin == -1)
				iMin = x[j][nowLine];
			else if (x[j][nowLine] < iMin)
				iMin = x[j][nowLine];
		}
	}
	return (iMin != -1) ? iMin : 0;
}

int GetConstPrivI(int **&x, int matrixLen, int nowRow, int outValue)
{
	int iMin = -1;
	for (int i = 1; i < matrixLen; i++)
	{
		if (x[nowRow][i] >= 0 && i != outValue)
		{
			if (iMin == -1)
			    iMin = x[nowRow][i];
			else if(x[nowRow][i] < iMin)
				iMin = x[nowRow][i];
		}
	}
	return (iMin != -1) ? iMin : 0;
}

BranchPoint GetBranchPoint(int**& x, int matrixLen)
{
	std::vector<int> Di; 
	std::vector<int> Dj;
	Di.push_back(-1);
	Dj.push_back(-1);
	BranchPoint bPoint;
	for (int i = 1; i < matrixLen; i++)
	{
		int iMinX = INT_MAX;
		for (int j = 1; j < matrixLen; j++)
		{
			if (x[i][j] == 0)
			{
				int iMinX2 = GetConstPrivI(x, matrixLen, i, j);
				if (iMinX >= iMinX2)
					iMinX = iMinX2;
			}
		}
		Di.push_back(iMinX);
	}
	for (int j = 1; j < matrixLen; j++)
	{
		int iMinY = INT_MAX;
		for (int i = 1; i < matrixLen; i++)
		{
			if (x[i][j] == 0)
			{
				int iMinY2 = GetConstPrivJ(x, matrixLen, j, i);
				if (iMinY >= iMinY2)
					iMinY = iMinY2;
			}
		}
		Dj.push_back(iMinY);
	}
	bPoint.x = 0;
	bPoint.y = 0;
	bPoint.weight = 0;
	for (int i = 1; i < matrixLen; i++)
	{
		for (int j = 1; j < matrixLen; j++)
		{
			if (x[i][j] == 0)
			{
				if (Di[i] + Dj[j] >= bPoint.weight)
				{
					bPoint.weight = Di[i] + Dj[j];
					bPoint.x = i;
					bPoint.y = j;
				}
			}
		}
	}
	return bPoint;
}

void FillNewMatrix(int **&stratMatrix, int **&newMatrix, int matrixLen, BranchPoint bPoint)
{
	int p1, p2;
	p1 = stratMatrix[bPoint.x][0];
	p2 = stratMatrix[0][bPoint.y];
	int tempValue = 0;
	for (int i = 0; i < matrixLen; i++)
	{
		for (int j = 0; j < matrixLen; j++)
		{
			if ((stratMatrix[i][0] == p2) && (stratMatrix[0][j] == p1))
			{
				tempValue = -1;
			}
			else
			{
				tempValue = stratMatrix[i][j];
			}
			if (i < bPoint.x && j < bPoint.y)
				newMatrix[i][j] = tempValue;
			if (i > bPoint.x && j < bPoint.y)
				newMatrix[i - 1][j] = tempValue;
			if (i < bPoint.x && j > bPoint.y)
				newMatrix[i][j - 1] = tempValue;
			if (i > bPoint.x && j > bPoint.y)
				newMatrix[i - 1][j - 1] = tempValue;
		}
	}
}

void SetMinusToMatrixOnPoint(int **&x, int matrixLen, BranchPoint &bPoint)
{
	int p1, p2;
	p1 = x[bPoint.x][0];
	p2 = x[0][bPoint.y];

	for (int i = 1; i < matrixLen; i++)
	{
		for (int j = 1; j < matrixLen; j++)
		{
			if ((x[i][0] == p1) && (x[0][j] == p2))
			{
				x[i][j] = -1;
			}
		}
	}
}

void CopyMapValues(WayPair &fromPair, WayPair &toPair)
{
	toPair.clear();
	toPair.insert(fromPair.begin(), fromPair.end());
}

void BranchesAndBoundaries(int**& x, int matrixLen, WayPair pathPair, int stepSumOfPath, BestWay &bestWay)
{
 	//PrintMatrix(x, matrixLen);
	WayPair pathPair2;
	CopyMapValues(pathPair, pathPair2);
	std::vector<int> Di;
	Di.push_back(-1);
	for (int i = 1; i < matrixLen; i++)
		Di.push_back(GetMinInLine(x, matrixLen, i));
	for (int i = 1; i < matrixLen; i++)
		minusLineValueDj(x, matrixLen, i, Di[i]);
	std::vector<int> Dj;
	Dj.push_back(-1);
	for (int i = 1; i < matrixLen; i++)
		Dj.push_back(GetMinInRow(x, matrixLen, i));
	for (int i = 1; i < matrixLen; i++)
		minusRowValueDi(x, matrixLen, i, Dj[i]);
	int nowLimit = 0;
	for (int i = 1; i < matrixLen; i++)
	{
		if (Di[i] == INT_MAX || Dj[i] == INT_MAX || nowLimit == INT_MAX)
			nowLimit = INT_MAX;
		else
			nowLimit += Di[i] + Dj[i];
	}
	BranchPoint bPoint = GetBranchPoint(x, matrixLen);

	if (nowLimit != INT_MAX)
		stepSumOfPath += nowLimit;
	else
		stepSumOfPath = INT_MAX;
	if (stepSumOfPath != INT_MAX)
	{
		if (matrixLen > 2)
		{
			pathPair.emplace(x[bPoint.x][0], x[0][bPoint.y]);
		}
		else
			pathPair.emplace(x[1][0], x[0][1]);
		if (matrixLen == 2)
		{
			if (bestWay.sumOfWay == -1 || stepSumOfPath < bestWay.sumOfWay)
			{
				bestWay.sumOfWay = stepSumOfPath;
				CopyMapValues(pathPair, bestWay.wayPair);
			}
		}
	}
 	if (matrixLen > 2 && stepSumOfPath != INT_MAX)
	{
		
		int** x1;
		x1 = new int* [matrixLen - 1];
		for (int i = 0; i < matrixLen - 1; i++)
		{
			x1[i] = new int[matrixLen];
		}
		x[bPoint.x][bPoint.y] = -1;
		
		FillNewMatrix(x, x1, matrixLen, bPoint);
		BranchesAndBoundaries(x1, matrixLen - 1, pathPair, stepSumOfPath, bestWay);
		for (int i = 0; i < matrixLen - 1; i++)
		{
			delete[] x1[i];
		}
		delete[] x1;
		if (stepSumOfPath != INT_MAX)
		{
			if (bestWay.sumOfWay < 0 || stepSumOfPath + bPoint.weight < bestWay.sumOfWay)
			{
				SetMinusToMatrixOnPoint(x, matrixLen, bPoint);
				BranchesAndBoundaries(x, matrixLen, pathPair2, stepSumOfPath, bestWay);
			}
		}
	}
}

void ShowResult(BestWay &bestWay, std::ostream &outStream)
{
	const int &Ref1 = (bestWay.wayPair.begin()->first);
	int key = Ref1;
	outStream << "Best way is ";
	for (size_t i = 0; i < bestWay.wayPair.size(); i++)
	{
		auto search = bestWay.wayPair.find(key);
		outStream << search->first << "-" << search->second << " ";
		/*outStream << search->first << "-";;
		if (i == bestWay.wayPair.size() - 1)
			outStream << search->second << " ";*/
		key = search->second;
	}
	outStream << "and Sum of Best Way is " << bestWay.sumOfWay << '\n';
}

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv, std::cout);
	std::ifstream inputFile(args->fileName);
	if (inputFile.is_open() && inputFile.peek() != EOF)
	{
		int matrixLen;
		std::string line;
		inputFile >> matrixLen;
		matrixLen++; // + столбец и строка для наименований (сами данные - нумерация с 1го)
		int **x;
		x = new int *[matrixLen];
		for (int i = 0; i < matrixLen; i++)
		{
			x[i] = new int[matrixLen];
		}
		FillMatrix(x, matrixLen, inputFile);
		inputFile.close();
		//PrintMatrix(x, matrixLen);
		WayPair pathPair;
		BestWay bestWay;
		bestWay.sumOfWay = -1;
		BranchesAndBoundaries(x, matrixLen, pathPair, 0, bestWay);
		ShowResult(bestWay, std::cout);

		for (int i = 0; i < matrixLen; i++)
		{
			delete[] x[i];
		}
		delete[] x;
	}
	else
	{
		return 1;
	}
}
