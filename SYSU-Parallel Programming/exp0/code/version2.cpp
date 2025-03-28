#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

void generateRandomMatrix(vector<vector<double>>& matrix, int rows, int cols) {
    matrix.resize(rows, vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = static_cast<double>(rand()) / RAND_MAX;  // 生成0到1之间的随机数
        }
    }
}

vector<vector<double>> matrixMultiplication(const vector<vector<double>>& matrixA, const vector<vector<double>>& matrixB) {
    int rowsA = matrixA.size();
    int colsA = matrixA[0].size();
    int colsB = matrixB[0].size();

    vector<vector<double>> result(rowsA, vector<double>(colsB));

    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    return result;
}

int main() {
    srand(time(0));  

    int m, n, k;
    cout << "Enter values for m, n, and k (between 512 and 2048):";
    cin >> m >> n >> k;

    vector<vector<double>> matrixA, matrixB;
    generateRandomMatrix(matrixA, m, n);
    generateRandomMatrix(matrixB, n, k);

    clock_t start = clock();
    vector<vector<double>> matrixC = matrixMultiplication(matrixA, matrixB);
    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;
    cout << "Matrix A:" << endl;
    for (const auto& row : matrixA) {
        for (double element : row) {
            cout << element << " ";
        }
        cout << endl;
    }

    cout << "Matrix B:" << endl;
    for (const auto& row : matrixB) {
        for (double element : row) {
            cout << element << " ";
        }
        cout << endl;
    }

    cout << "Matrix C:" << endl;
    for (const auto& row : matrixC) {
        for (double element : row) {
            cout << element << " ";
        }
        cout << endl;
    }

    double totalFlops = 2 * static_cast<double>(m) * n * k;  
    double totalBytes = (m * n + n * k + m * k) * sizeof(double);  

    double flopsPerSecond = totalFlops / duration; 
    double bandwidth = totalBytes / duration;  

    double peakPerformancePercentage = (flopsPerSecond / bandwidth) * 100;  

    cout << "Running Time: " << duration << " seconds" << endl;
    cout << "Floating Point Performance: " << fixed << setprecision(5) << flopsPerSecond << " FLOPS" << endl;
    cout << "Peak Bandwidth: " << fixed << setprecision(2) << bandwidth << " bytes/second" << endl;
    cout << "Peak Performance Percentage: " << fixed << setprecision(5) << peakPerformancePercentage << "%" << endl;

    return 0;
}