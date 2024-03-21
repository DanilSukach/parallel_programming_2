#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <nlohmann/json.hpp>
#include <omp.h>
using json = nlohmann::json;
using namespace std;

#define PATH_TO_FIRST_MATRIX "./data/matrix1.txt"
#define PATH_TO_SECOND_MATRIX "./data/matrix2.txt"
#define PATH_TO_RESULT_MATRIX "./data/resultMatrix.txt"
#define PATH_TO_RESULT_JSON "./data/data.json"
#define PATH_TO_INPUT_DATA "./data/inputData.txt"

class MatrixMultiplier {
	int** matrix1;
	int** matrix2;
	int** resultMatrix;
	int numberThreads;
	int size;
	chrono::duration<double> time;

	int** generateRandom(int n) {
		int** matrix = createMatrix(n);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				matrix[i][j] = rand() % 100;
			}
		}
		return matrix;
	}

	 void multiplyMatrices(int** temp1, int** temp2, int count, int countThreads) {
		int** temp = createMatrix(count);

		#pragma omp parallel num_threads(countThreads)
		{
			#pragma omp for
			for (int i = 0; i < count; ++i) {
				for (int j = 0; j < count; ++j) {
					temp[i][j] = 0;
					for (int k = 0; k < count; ++k) {
						temp[i][j] += temp1[i][k] * temp2[k][j];
					}
				}
			}
		}
		AllocateMemoryForMatrice(temp,count);
		
	}

	chrono::duration<double> measureMatrixMultiplicationTime(int count, int countThreads) {
		int** temp1 = generateRandom(count);
		int** temp2 = generateRandom(count);
		auto start = chrono::steady_clock::now();
		multiplyMatrices(temp1, temp2, count, countThreads);
		auto end = chrono::steady_clock::now();
		chrono::duration<double> elapsed_seconds = end - start;
		AllocateMemoryForMatrice(temp1,count);
		AllocateMemoryForMatrice(temp2,count);
		return elapsed_seconds;
	}

	void saveResult(int count, vector<double> t, int countThreads) {
		json jsonData;

		ifstream inputFile(PATH_TO_RESULT_JSON);
		if (inputFile.good()) {
			inputFile >> jsonData;
			inputFile.close();
		}

		if (jsonData.find(to_string(countThreads)) != jsonData.end()) {
			jsonData[to_string(countThreads)].push_back({ count,t });
		}
		else {
			jsonData[to_string(countThreads)] = json::array({ { count,t } });
		}
		ofstream outputFile(PATH_TO_RESULT_JSON);

		outputFile << setw(4) << jsonData << endl;
		outputFile.close();
	}
	void AllocateMemoryForMatrice(int** matrix, int count) const {
		if (matrix != nullptr) {
			for (size_t i = 0; i < count; ++i) {
				delete[] matrix[i];
			}
			delete[] matrix;
		}
	}

public:
	MatrixMultiplier() : matrix1(nullptr), matrix2(nullptr), resultMatrix(nullptr), size(0), time(0), numberThreads(1) {}

	chrono::duration<double> getTime() const {
		return time;
	}

	void setThreads(int count) {
		numberThreads = count;
	}

	int** createMatrix(int count) {
		int** matrix = new int* [count];
		for (int i = 0; i < count; ++i) {
			matrix[i] = new int[count];
		}
		return matrix;
	}

	void generateRandomMatrices(int n) {
		size = n;
		matrix1 = createMatrix(n);
		matrix2 = createMatrix(n);

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				matrix1[i][j] = rand() % 100;
				matrix2[i][j] = rand() % 100;
			}
		}
	}

	void multiplyMatrices() {
		resultMatrix = createMatrix(size);
		auto start = chrono::steady_clock::now();
		#pragma omp parallel num_threads(numberThreads)
		{
			#pragma omp for
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; ++j) {
					resultMatrix[i][j] = 0;
					for (int k = 0; k < size; ++k) {
						resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
					}
				}
			}
		}
		auto end = chrono::steady_clock::now();
		time = end - start;
	}

	void saveMatricesToFile() {
		if (matrix1) {
			ofstream file(PATH_TO_FIRST_MATRIX);
			file << size << endl;
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; ++j) {
					file << matrix1[i][j] << " ";
				}
				file << endl;
			}
			file.close();
		}
		if (matrix2) {
			ofstream file(PATH_TO_SECOND_MATRIX);
			file << size << endl;
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; ++j) {
					file << matrix2[i][j] << " ";
				}
				file << endl;
			}
			file.close();
		}
	}

	void saveResultMatrixToFile() {
		if (resultMatrix) {
			ofstream file(PATH_TO_RESULT_MATRIX);
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; ++j) {
					file << resultMatrix[i][j] << " ";
				}
				file << endl;
			}
			file.close();
		}
	}

	void saveInputData() {
		ofstream file(PATH_TO_INPUT_DATA);
		file << "Matrix size: " << size << endl;
		file << "Lead time: " << time.count() << " s" << endl;
		file << "Threads: " << numberThreads;
		file.close();
	}

	void loadMatricesFromFile() {
		ifstream file1(PATH_TO_FIRST_MATRIX);
		if (!file1.is_open()) {
			throw runtime_error("Unable to open file1");
		}
		file1 >> size;
		matrix1 = createMatrix(size);
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				file1 >> matrix1[i][j];
			}
		}
		file1.close();
		ifstream file2(PATH_TO_SECOND_MATRIX);
		if (!file2.is_open()) {
			throw runtime_error("Unable to open file2");
		}
		file2 >> size;
		matrix2 = createMatrix(size);
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				file2 >> matrix2[i][j];
			}
		}
		file2.close();
	}

	void test(int start, int end, int step) {
		vector<double> times;
		for (int k = 15; k <= 24; ++k) {
			cout << k;
			for (int i = start; i <= end; i += step) {
				for (int j = 0; j < 10; ++j) {
					chrono::duration<double> t = measureMatrixMultiplicationTime(i, k);
					times.push_back(t.count());
				}
				saveResult(i, times, k);
				times.clear();
			}
		}
	}
};

void printSelectionPoint() {
	cout << "1. Create matrices " << endl;
	cout << "2. Load matrices from files" << endl;
	cout << "3. Save matrices to files" << endl;
	cout << "4. Save the resulting matrix" << endl;
	cout << "5. Multiply matrices" << endl;
	cout << "6. Test" << endl;
	cout << "0. Exit" << endl;
}

int main() {
	MatrixMultiplier matrix;
	while (true) {
		system("cls");
		printSelectionPoint();
		size_t choice;
		cin >> choice;

		switch (choice) {
		case 1: {
			system("cls");
			int size;
			cout << "Enter matrix size: ";
			cin >> size;
			matrix.generateRandomMatrices(size);
			cout << "Matrices created" << endl;
			system("pause");
			break;
		}
		case 2: {
			system("cls");
			try {
				matrix.loadMatricesFromFile();
				cout << "Matrices loaded" << endl;
			}
			catch (const runtime_error& e) {
				cerr << "Error: " << e.what() << endl;
			}
			system("pause");
			break;
		}
		case 3: {
			system("cls");
			matrix.saveMatricesToFile();
			cout << "Matrices saved" << endl;
			system("pause");
			break;
		}
		case 4: {
			system("cls");
			matrix.saveResultMatrixToFile();
			matrix.saveInputData();
			cout << "Data saved" << endl;
			system("pause");
			break;
		}
		case 5: {
			system("cls");
			int count;
			cout << "Enter the number of threads: ";
			cin >> count;
			matrix.setThreads(count);
			matrix.multiplyMatrices();
			cout << "Time taken: " << matrix.getTime().count() << " s" << endl;
			system("pause");
			break;
		}
		case 6: {
			system("cls");
			int start, end, step;
			cout << "input start point, end point and step" << endl;
			cin >> start >> end >> step;
			matrix.test(start, end, step);
			cout << "Test comleted" << endl;
			system("pause");
			break;
		}
		case 0:
			return 0;
		default:
			cerr << "Invalid choice" << endl;
		}
	}

	return 0;

}