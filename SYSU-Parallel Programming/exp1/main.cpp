#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

void generateRandomMatrix(int** matrix, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

void printMatrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void matrixMultiply(int** matrixA, int** matrixB, int** result, int size, int startRow, int endRow) {
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = 0;
            for (int k = 0; k < size; k++) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

int main(int argc, char** argv) 
{
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int matrixSize, numProcesses;
    if (rank == 0) {
        cout << "Enter matrix size (128-2048): ";
        cin >> matrixSize;
        cout << "Enter number of processes (1-16): ";
        cin >> numProcesses;
    }


    MPI_Bcast(&matrixSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numProcesses, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (matrixSize < 128 || matrixSize > 2048 || numProcesses < 1 || numProcesses > 16) {
        if (rank == 0) {
            cout << "Invalid input. Matrix size should be in range [128, 2048], and number of processes should be in range [1, 16]." << endl;
        }
        MPI_Finalize();
        return 0;
    }

    int rowsPerProcess = matrixSize / numProcesses;
    int startRow = rank * rowsPerProcess;
    int endRow = (rank == numProcesses - 1) ? matrixSize : (startRow + rowsPerProcess);

    int** matrixA = new int*[matrixSize];
    int** matrixB = new int*[matrixSize];
    int** result = new int*[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        matrixA[i] = new int[matrixSize];
        matrixB[i] = new int[matrixSize];
        result[i] = new int[matrixSize];
    }

    if (rank == 0) {
        generateRandomMatrix(matrixA, matrixSize);
        generateRandomMatrix(matrixB, matrixSize);
    }

    for (int i = 0; i < matrixSize; i++) {
        MPI_Bcast(matrixA[i], matrixSize, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(matrixB[i], matrixSize, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int localRows = endRow - startRow;
    int** localResult = new int*[localRows];
    for (int i = 0; i < localRows; i++) {
        localResult[i] = new int[matrixSize];
    }

    double startTime = MPI_Wtime();
    matrixMultiply(matrixA, matrixB, localResult, matrixSize, startRow, endRow);

    MPI_Gather(localResult[0], localRows * matrixSize, MPI_INT, result[0], localRows * matrixSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Matrix A:" << endl;
        printMatrix(matrixA, matrixSize);

        cout << "Matrix B:" << endl;
        printMatrix(matrixB, matrixSize);

        cout << "Result:" << endl;
        printMatrix(result, matrixSize);

        double endTime = MPI_Wtime();
        double totalTime = endTime - startTime;
        cout << "Total time: " << totalTime << " seconds" << endl;
    }

    for (int i = 0; i < matrixSize; i++) {
        delete[] matrixA[i];
        delete[] matrixB[i];
        delete[] result[i];
    }
    delete[] matrixA;
    delete[] matrixB;
    delete[] result;

    MPI_Finalize();
    return 0;
}