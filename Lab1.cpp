#include <iostream>
#include <fstream>
#include <windows.h>
#include <omp.h>

void createFileOfRandomNums(char* name, int rows, int columns);
double** readMatrix(char* name);
void multiMatrix(double** a, double** b, double** c);
void multiMatrixWithParallel(double** a, double** b, double** c);
void multiMatrixWithOneParallel(double** a, double** b, double** c);
void multiMatrixWithTwoParallel(double** a, double** b, double** c);
void multiMatrixWithThreeParallel(double** a, double** b, double** c);
void multiMatrixWithOneParallel_2threads(double** a, double** b, double** c);
void multiMatrixWithTwoParallel_2threads(double** a, double** b, double** c);
void multiMatrixWithThreeParallel_2threads(double** a, double** b, double** c);
void multiMatrixWithOneParallel_4threads(double** a, double** b, double** c);
void multiMatrixWithTwoParallel_4threads(double** a, double** b, double** c);
void multiMatrixWithThreeParallel_4threads(double** a, double** b, double** c);
void printMatrix(double** c, int rows, int columns);
void writeMatrixInFile(char* name, double** result, int rows, int columns);
double** createResultMatrixEmpty();
double** createResultMatrixFillZero();
void multiMatrixOneCycle(double** a, double** b, double** c);
void multiMatrixOneCycleWithParallel(double** a, double** b, double** c);

ULONGLONG dwStart;
int rows1 = 1000;
int columns2 = 2000;
int inter21 = 3000;

int main(int argc, char* argv[]) {

	double** a = nullptr, ** b = nullptr, ** c;
	double** cc;

	//createFileOfRandomNums((char*)("matrix1.txt"), rows1, inter21);
	//createFileOfRandomNums((char*)("matrix2.txt"), inter21, columns2);

	// ���� ��������� ������ �������
	a = readMatrix((char*)"matrix1.txt");
	//printMatrix(a, rows1, inter21);
	// ���� ��������� ������ �������
	b = readMatrix((char*)"matrix2.txt");
	//printMatrix(b, inter21, columns2);
	omp_set_nested(1);
	cc = createResultMatrixFillZero();
	multiMatrixOneCycle(a, b, cc);
	writeMatrixInFile((char*)"resultOneCycle.txt", cc, rows1, columns2);
	cc = createResultMatrixFillZero();
	multiMatrixOneCycleWithParallel(a, b, cc);
	writeMatrixInFile((char*)"resultOneCycleParallel.txt", cc, rows1, columns2);

	c = createResultMatrixEmpty();
	//multiMatrix(a,b, c);
	//writeMatrixInFile((char*)"result.txt", c, rows1, columns2);
	//multiMatrixWithParallel(a, b, c);
	//writeMatrixInFile((char*)"result0.txt", c, rows1, columns2);
	//multiMatrixWithOneParallel(a, b, c);
	//writeMatrixInFile((char*)"result1.txt", c, rows1, columns2);
	//multiMatrixWithTwoParallel(a, b, c);
	//writeMatrixInFile((char*)"result2.txt", c, rows1, columns2);
	//multiMatrixWithThreeParallel(a, b, c);
	//writeMatrixInFile((char*)"result3.txt", c, rows1, columns2);
	//multiMatrixWithOneParallel_2threads(a, b, c);
	//writeMatrixInFile((char*)"result4.txt", c, rows1, columns2);
	//multiMatrixWithTwoParallel_2threads(a, b, c);
	//writeMatrixInFile((char*)"result5.txt", c, rows1, columns2);
	//multiMatrixWithThreeParallel_2threads(a, b, c);
	//writeMatrixInFile((char*)"result6.txt", c, rows1, columns2);
	//multiMatrixWithOneParallel_4threads(a, b, c);
	//writeMatrixInFile((char*)"result7.txt", c, rows1, columns2);
	//multiMatrixWithTwoParallel_4threads(a, b, c);
	//writeMatrixInFile((char*)"result8.txt", c, rows1, columns2);
	//multiMatrixWithThreeParallel_4threads(a, b, c);
	//writeMatrixInFile((char*)"result9.txt", c, rows1, columns2);

	return (0);
}

void createFileOfRandomNums(char* name, int rows, int columns) {
	std::ofstream f1(name, std::ios::out | std::ios::app);
	f1 <<rows<<" "<<columns<< std::endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			f1 << (double)(rand()) / rand() << " ";
		}
		f1 << std::endl;
	}
	f1.close();
}

void writeMatrixInFile(char* name, double** result, int rows, int columns) {
	std::ofstream f1(name, std::ios::out | std::ios::app);
	f1 << rows << " " << columns << std::endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			f1 << result[i][j] << " ";
		}
		f1 << std::endl;
	}
	f1.close();
}

double** readMatrix(char* name) {
	std::ifstream file(name);
	int rows, columns;
	double** matrix;
	file >> rows >> columns;
	if (name == "matrix1.txt") {
		rows1 = rows;
		inter21 = columns;
	}
	else {
		inter21 = rows;
		columns2 = columns;
	}
	matrix = new double* [rows]; 
	for (int row = 0; row < rows; row++) {
		matrix[row] = new double [columns]; 
		for (int column = 0; column < columns; column++) {
			file>>matrix[row][column];
		}
	}
	file.close();
	return matrix;
}
double** createResultMatrixEmpty() {
	double** result;
	result = new double* [rows1];
	for (int row = 0; row < rows1; row++) {
		result[row] = new double[columns2];
	}
	return result;
}

double** createResultMatrixFillZero() {
	double** result;
	result = new double* [rows1];
	for (int row = 0; row < rows1; row++) {
		result[row] = new double[columns2];
		for (int column = 0; column < columns2; column++) {
			result[row][column] = 0;
		}
	}
	return result;
}

void multiMatrixOneCycle(double** a, double** b, double** c) {
	std::cout << "Begin multiplay" << std::endl;
	dwStart = GetTickCount64();
	int row1 = 0;
	int column2 = 0;
	int inter = 0;
	int rowCurrentIndex = 0;
	int columnCurrentIndex = 0;
	while(true){
				c[rowCurrentIndex][columnCurrentIndex] += a[row1][inter] * b[inter][column2];
				inter++;
				if (inter >= inter21) {
					inter = 0;
					columnCurrentIndex++;
					column2++;
					if (column2 >= columns2) {
						column2 = 0;
						rowCurrentIndex++;
						columnCurrentIndex = 0;
						row1++;
						if (row1 >= rows1) {
							break;
						}
					}
				}

	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrixOneCycleWithParallel(double** a, double** b, double** c) {
	std::cout << "Begin multiplay" << std::endl;
	dwStart = GetTickCount64();
	int row1 = 0;
	int column2 = 0;
	int inter = 0;
	int currentIndex = 0;
	int rowCurrentIndex = 0;
	int columnCurrentIndex = 0;
#pragma omp parallel
	while (true) {
		c[rowCurrentIndex][columnCurrentIndex] += a[row1][inter] * b[inter][column2];
		inter++;
		if (inter >= inter21) {
			inter = 0;
			columnCurrentIndex++;
			column2++;
			if (column2 >= columns2) {
				column2 = 0;
				rowCurrentIndex++;
				columnCurrentIndex = 0;
				row1++;
				if (row1 >= rows1) {
					break;
				}
			}
		}

	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrix(double** a, double** b, double** c) {
	std::cout << "Begin multiplay" << std::endl;
	dwStart = GetTickCount64();
	for (int row = 0; row < rows1; row++) {
		for (int column = 0; column < columns2; column++) {
			c[row][column] = 0;
			for (int inter = 0; inter < inter21; inter++) {
				c[row][column] += a[row][inter] * b[inter][column];
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}
void multiMatrixWithParallel(double** a, double** b, double** c) {
	std::cout << "Begin multiplay with parallel" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel
	{
		for (int row = 0; row < rows1; row++) {
			for (int column = 0; column < columns2; column++) {
				c[row][column] = 0;
				for (int inter = 0; inter < inter21; inter++) {
					c[row][column] += a[row][inter] * b[inter][column];
				}
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}
void multiMatrixWithOneParallel(double** a, double** b, double** c) {
	std::cout << "\nBegin multiplay with OpenMP : One paralleled loop" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
			for (int column = 0; column < columns2; column++) {
				c[row][column] = 0;
				for (int inter = 0; inter < inter21; inter++) {
					c[row][column] += a[row][inter] * b[inter][column];
				}
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrixWithTwoParallel(double** a, double** b, double** c) {
	std::cout << "\nBegin multiplay with OpenMP : Two paralleled loops" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
#pragma omp parallel for
			for (int column = 0; column < columns2; column++) {
				c[row][column] = 0;
				for (int inter = 0; inter < inter21; inter++) {
					c[row][column] += a[row][inter] * b[inter][column];
				}
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrixWithThreeParallel(double** a, double** b, double** c) {
	
	std::cout << "\nBegin multiplay with OpenMP : Three paralleled loops" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
#pragma omp parallel for
			for (int column = 0; column < columns2; column++) {
				double temp = 0;
#pragma omp parallel for reduction(+:temp) schedule(static,4) 
				for (int inter = 0; inter < inter21; inter++) {
					temp += a[row][inter] * b[inter][column];
				}
				c[row][column] = temp;
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}
void multiMatrixWithOneParallel_2threads(double** a, double** b, double** c) {
	std::cout << "\nBegin multiplay with OpenMP : One paralleled loop on 2 threads" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel num_threads(2)
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
			for (int column = 0; column < columns2; column++) {
				c[row][column] = 0;
				for (int inter = 0; inter < inter21; inter++) {
					c[row][column] += a[row][inter] * b[inter][column];
				}
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrixWithTwoParallel_2threads(double** a, double** b, double** c) {
	std::cout << "\nBegin multiplay with OpenMP : Two paralleled loops on 2 threads" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel num_threads(2)
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
#pragma omp parallel for
			for (int column = 0; column < columns2; column++) {
				c[row][column] = 0;
				for (int inter = 0; inter < inter21; inter++) {
					c[row][column] += a[row][inter] * b[inter][column];
				}
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrixWithThreeParallel_2threads(double** a, double** b, double** c) {
	std::cout << "\nBegin multiplay with OpenMP : Three paralleled loops on 2 threads" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel num_threads(2)
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
#pragma omp parallel for
			for (int column = 0; column < columns2; column++) {
				double temp = 0;
#pragma omp parallel for reduction(+:temp) schedule(static,4)
				for (int inter = 0; inter < inter21; inter++) {
					temp += a[row][inter] * b[inter][column];
				}
				c[row][column] = temp;
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrixWithOneParallel_4threads(double** a, double** b, double** c) {
	std::cout << "\nBegin multiplay with OpenMP : One paralleled loop on 4 threads" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel num_threads(4)
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
			for (int column = 0; column < columns2; column++) {
				c[row][column] = 0;
				for (int inter = 0; inter < inter21; inter++) {
					c[row][column] += a[row][inter] * b[inter][column];
				}
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrixWithTwoParallel_4threads(double** a, double** b, double** c) {
	std::cout << "\nBegin multiplay with OpenMP : Two paralleled loops on 4 threads" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel num_threads(4)
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
#pragma omp parallel for
			for (int column = 0; column < columns2; column++) {
				c[row][column] = 0;
				for (int inter = 0; inter < inter21; inter++) {
					c[row][column] += a[row][inter] * b[inter][column];
				}
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void multiMatrixWithThreeParallel_4threads(double** a, double** b, double** c) {
	std::cout << "\nBegin multiplay with OpenMP : Three paralleled loops on 4 threads" << std::endl;
	dwStart = GetTickCount64();
#pragma omp parallel num_threads(4)
	{
#pragma omp for
		for (int row = 0; row < rows1; row++) {
#pragma omp parallel for
			for (int column = 0; column < columns2; column++) {
				double temp = 0;
#pragma omp parallel for reduction(+:temp) schedule(static, 4)
				for (int inter = 0; inter < inter21; inter++) {
					temp += a[row][inter] * b[inter][column];
				}
				c[row][column] = temp;
			}
		}
	}
	printf_s("For multiply two matrixs: %dx%d on %dx%d, spent %I64d milliseconds\n", rows1, inter21, inter21, columns2, (GetTickCount64() - dwStart));
}

void printMatrix(double** c, int rows, int columns) {
	// ����� ������� ������������

	std::cout << "�������" << std::endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
			std::cout << c[i][j] << " ";
		std::cout << std::endl;
	}
}
