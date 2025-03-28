#include <iostream>
#include <ctime>
#include <cstdlib>
#include <mpi.h>

using namespace std;

struct Matrix {
    int rows;
    int cols;
    double *data;
};

void generate_matrices(int m, int n, int k, double *A, double *B) {
    for (int i = 0; i < m * n; ++i)
        A[i] = rand() % 100 + 1; 
    for (int i = 0; i < n * k; ++i)
        B[i] = rand() % 100 + 1;
}

void matrix_multiply(int m, int n, int k, double *A, double *B, double *C) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            C[i * k + j] = 0.0;
            for (int l = 0; l < n; ++l) {
                C[i * k + j] += A[i * n + l] * B[l * k + j];
            }
        }
    }
}

void print_matrix(double *matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i * cols + j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int m, n, k, n_process;
    double *A, *B, *C;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 5) {
        if (rank == 0)
            cerr << "Usage: m n k n_process" << endl;
        MPI_Finalize();
        return 1;
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    k = atoi(argv[3]);
    n_process = atoi(argv[4]);

    if (m < 128 || m > 2048 || n < 128 || n > 2048 || k < 128 || k > 2048 || n_process < 1 || n_process > 16) {
        if (rank == 0)
            cerr << "Invalid input range. m, n, k should be in range [128, 2048] and n_process should be in range [1, 16]." << endl;
        MPI_Finalize();
        return 1;
    }

    if (size != n_process) {
        if (rank == 0)
            cerr << "Number of processes must match the specified n_process argument." << endl;
        MPI_Finalize();
        return 1;
    }

    A = new double[m * n];
    B = new double[n * k];
    C = new double[m * k];

    if (rank == 0) {
        generate_matrices(m, n, k, A, B);
        start_time = MPI_Wtime();
    }

    // Broadcast matrices A and B to all processes
    MPI_Bcast(A, m * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, n * k, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Compute local portion of matrix multiplication
    int local_m = m / size;
    double *local_C = new double[local_m * k];
    matrix_multiply(local_m, n, k, A + rank * local_m * n, B, local_C);

    // Define MPI data type for Matrix struct
    MPI_Datatype matrix_type;
    int block_lengths[2] = {1, 1};
    MPI_Aint offsets[2] = {offsetof(Matrix, rows), offsetof(Matrix, data)};
    MPI_Datatype types[2] = {MPI_INT, MPI_DOUBLE};
    MPI_Type_create_struct(2, block_lengths, offsets, types, &matrix_type);
    MPI_Type_commit(&matrix_type);

    Matrix global_C;
    if (rank == 0) {
        global_C.rows = m;
        global_C.cols = k;
        global_C.data = new double[m * k]; 
    }
    MPI_Gather(local_C, 1, matrix_type, global_C.data, 1, matrix_type, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        end_time = MPI_Wtime();
        double time_taken = end_time - start_time;

        // Output matrices A, B, and C, and computation time
        cout << "Matrix A:" << endl;
        print_matrix(A, m, n);
        cout << "Matrix B:" << endl;
        print_matrix(B, n, k);
        cout << "Matrix C:" << endl;
        print_matrix(global_C.data, global_C.rows, global_C.cols);
        cout << "Running time: " << time_taken << " seconds" << endl;
        delete[] global_C.data;
    }

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] local_C;
    MPI_Type_free(&matrix_type);

    MPI_Finalize();

    return 0;
}
