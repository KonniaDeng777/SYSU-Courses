#include <iostream>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <ctime>

using namespace std;
using namespace chrono;

const int MAX_THREADS = 16;

struct Matrix 
{
    int rows;
    int cols;
    vector<vector<int> > data;
};

struct ThreadArgs 
{
    int thread_id;
    int num_threads;
    Matrix* A;
    Matrix* B;
    Matrix* C;
};

void* multiply(void* arg) 
{
    ThreadArgs* args = (ThreadArgs*)arg;
    Matrix* A = args->A;
    Matrix* B = args->B;
    Matrix* C = args->C;

    int rows_per_thread = A->rows / args->num_threads;
    int start_row = args->thread_id * rows_per_thread;
    int end_row = (args->thread_id == args->num_threads - 1) ? A->rows : (args->thread_id + 1) * rows_per_thread;

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < B->cols; ++j) {
            for (int k = 0; k < A->cols; ++k) {
                C->data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

Matrix generateRandomMatrix(int rows, int cols) 
{
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data.resize(rows, vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mat.data[i][j] = rand() % 100; 
        }
    }

    return mat;
}

void printMatrix(const Matrix& mat) 
{
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            cout << mat.data[i][j] << " ";
        }
        cout << endl;
    }
}

int main() 
{
    srand(time(NULL));

    int m, n, k;
    cout << "Enter dimensions for matrices (m n k): ";
    cin >> m >> n >> k;

    Matrix A = generateRandomMatrix(m, n);
    Matrix B = generateRandomMatrix(n, k);
    Matrix C;
    C.rows = m;
    C.cols = k;
    C.data.resize(m, vector<int>(k, 0)); 

    int num_threads;
    cout << "Enter number of threads (1-" << MAX_THREADS << "): ";
    cin >> num_threads;
    num_threads = min(num_threads, MAX_THREADS);

    pthread_t threads[MAX_THREADS];
    ThreadArgs thread_args[MAX_THREADS];

    clock_t start = clock();

    for (int i = 0; i < num_threads; ++i) 
    {
        thread_args[i].thread_id = i;
        thread_args[i].num_threads = num_threads;
        thread_args[i].A = &A;
        thread_args[i].B = &B;
        thread_args[i].C = &C;
        pthread_create(&threads[i], NULL, multiply, (void*)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; ++i) 
    {
        pthread_join(threads[i], NULL);
    }

    
    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;

    cout << "Matrix A:" << endl;
    printMatrix(A);
    cout << endl;

    cout << "Matrix B:" << endl;
    printMatrix(B);
    cout << endl;

    cout << "Matrix C:" << endl;
    printMatrix(C);
    cout << endl;

    cout << "Running time: " << duration<< " seconds" << endl;

    return 0;
}
