// SpanningTrees.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using Matrix = boost::numeric::ublas::matrix<double>;

struct Args
{
	std::string fileName;
};

std::optional<Args> ParseArgs(int argc, char* argv[], std::ostream& outStream)
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

bool FillMatrix(Matrix &m, int matrixLen, std::istream &inputFile)
{
	std::string line;

	for (int i = 0; i < matrixLen; i++)
	{
		for (int j = 0; j < matrixLen; j++)
		{
			inputFile >> line;
			try
			{
				m(i, j) = std::atof(line.c_str());
			}
			catch (const std::exception& /* error*/)
			{
				return false;
			}
		}
	}
	return true;
}

bool FillKirchhoffMatrices(Matrix &inputMatrix, Matrix &kirchhofftMatrix)
{
	for (unsigned  i = 0; i < inputMatrix.size1(); i++)
	{
		double relations = 0.0;
		for (unsigned j = 0; j < inputMatrix.size1(); j++)
		{
			if (inputMatrix(i, j) > DBL_EPSILON)
			{
				double d1 = inputMatrix(i, j);
				relations++;
				kirchhofftMatrix(i, j) = -1.0;
			}
			else
			{
				kirchhofftMatrix(i, j) = 0.0;
			}
		}
		if (relations > DBL_EPSILON)
		{
			kirchhofftMatrix(i, i) = relations;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void PrintMatrix(Matrix &x)
{
	for (int i = 0; i < x.size1(); i++)
	{
		for (int j = 0; j < x.size1(); j++)
			std::cout << x(i,j) << "\t";
		std::cout << "\n";
	}
}

void ConvertRow(Matrix &kirchhofftMatrix, int i, int j)
{
	double value = kirchhofftMatrix(i, j);
	double valueAbove = kirchhofftMatrix(j, j);
	double divider = -kirchhofftMatrix(i, j) / kirchhofftMatrix(j, j);
	for (int x = j; x < kirchhofftMatrix.size1(); x++)
	{
		kirchhofftMatrix(i, x) = kirchhofftMatrix(i, x) + kirchhofftMatrix(j, x) * divider;
	}
}

int GetSpanningTreesNumber(Matrix &kirchhofftMatrix)
{
	Matrix oprMatrix(kirchhofftMatrix.size1() - 1, kirchhofftMatrix.size1() - 1);
	for (unsigned i = 0; i < kirchhofftMatrix.size1() - 1; i++)
	{
		for (unsigned j = 0; j < kirchhofftMatrix.size1() - 1; j++)
		{
			oprMatrix(i, j) = kirchhofftMatrix(i, j);
		}
	}
	for (unsigned j = 0; j < oprMatrix.size1(); j++)
	{
		if (j + 1 < oprMatrix.size1())
		{
			for (unsigned i = j + 1; i < oprMatrix.size1(); i++)
			{
				if (abs(oprMatrix(i, j)) > DBL_EPSILON)
				{
					ConvertRow(oprMatrix, i, j);
					/*PrintMatrix(oprMatrix); // отладка
					std::cout << std::endl;*/
				}
			}
		}
	}
	double result = 0;
	for (unsigned i = 0; i < oprMatrix.size1(); i++)
	{
		if (i == 0)
		{
			result = oprMatrix(i, i);
		}
		else
		{
			result *= oprMatrix(i, i);
		}
	}
	//std::cout << result << std::endl;
	return result;
}


int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv, std::cout);
	std::ifstream inputFile(args->fileName);
	int matrixLen = 0;
	if (inputFile.is_open() && inputFile.peek() != EOF)
	{
		std::string line;
		inputFile >> line;
		try
		{
			matrixLen = std::stoi(line);
		}
		catch (const std::exception& /* error*/)
		{
			std::cout << "File has is not numeric values!" << std::endl;
			return 1;
		}
		if (matrixLen > 1)
		{
			Matrix inputMatrix(matrixLen, matrixLen);
			if (!FillMatrix(inputMatrix, matrixLen, inputFile))
			{
				std::cout << "File has is not numeric values!" << std::endl;
				return 1;
			}
			Matrix kirchhofftMatrix(matrixLen, matrixLen);
			if (FillKirchhoffMatrices(inputMatrix, kirchhofftMatrix))
			{
				std::cout << "There are " << GetSpanningTreesNumber(kirchhofftMatrix) << " spanning trees.";
				//std::cout << inputMatrix << std::endl;
				//std::cout << std::endl;
				//std::cout << kirchhofftMatrix << std::endl;
			}
			else
			{
				std::cout << "There are more than one graph!" << std::endl;
				return 1;
			}
		}
		else
		{
			std::cout << "Matrix length < 2!" << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "Empty file!" << std::endl;
		return 1;
	}
}