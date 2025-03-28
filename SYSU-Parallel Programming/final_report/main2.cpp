/*
mpic++ main2.cpp -o main2
mpirun -np 4 ./main2
*/
#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

void multiplyMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B, vector<vector<int>> &C, int startRow, int endRow) {
    int n = A.size();
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 500;
    vector<vector<int>> A(n, vector<int>(n, 1));
    vector<vector<int>> B(n, vector<int>(n, 1));
    vector<vector<int>> C(n, vector<int>(n, 0));

    int rowsPerProcess = n / size;
    int startRow = rank * rowsPerProcess;
    int endRow = (rank == size - 1) ? n : (rank + 1) * rowsPerProcess;

    multiplyMatrices(A, B, C, startRow, endRow);

    if (rank != 0) {
        for (int i = startRow; i < endRow; ++i) {
            MPI_Send(&C[i][0], n, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    } else {
        for (int i = 1; i < size; ++i) {
            int start = i * rowsPerProcess;
            int end = (i == size - 1) ? n : (i + 1) * rowsPerProcess;
            for (int j = start; j < end; ++j) {
                MPI_Recv(&C[j][0], n, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
