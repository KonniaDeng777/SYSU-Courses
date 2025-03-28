//g++ main1.cpp -o main1
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void multiplyMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B, vector<vector<int>> &C) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int n = 500;
    vector<vector<int>> A(n, vector<int>(n, 1));
    vector<vector<int>> B(n, vector<int>(n, 1));
    vector<vector<int>> C(n, vector<int>(n, 0));

    auto start = chrono::high_resolution_clock::now();
    multiplyMatrices(A, B, C);
    auto end = chrono::high_resolution_clock::now();

    cout << "Time taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    return 0;
}
