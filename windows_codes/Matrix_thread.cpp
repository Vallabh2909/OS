#include <iostream>
#include <windows.h>
#include <vector>
#include <cstdlib> 
#define NUM_THREADS 4  

using namespace std;

struct ThreadData {
    int row_start;
    int row_end;
    vector<vector<int>>& A;
    vector<vector<int>>& B;
    vector<vector<int>>& C;
    int N;

    
    ThreadData(int row_start, int row_end, vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C, int N)
        : row_start(row_start), row_end(row_end), A(A), B(B), C(C), N(N) {}
};

DWORD WINAPI matrix_multiplication(LPVOID arg) {
    ThreadData* data = (ThreadData*)arg;
    int row_start = data->row_start;
    int row_end = data->row_end;
    vector<vector<int>>& A = data->A;
    vector<vector<int>>& B = data->B;
    vector<vector<int>>& C = data->C;
    int N = data->N;

    
    for (int i = row_start; i < row_end; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0; 
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return 0;
}

int main() {
    int N;

    cout << "Enter the size of the matrices (N x N): ";
    cin >> N;

    
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N));

    cout << "Matrix A:" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Matrix B:" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[i][j] = rand() % 10;
            cout << B[i][j] << " ";
        }
        cout << endl;
    }

    HANDLE threads[NUM_THREADS];
    vector<ThreadData> thread_data;

    int rows_per_thread = N / NUM_THREADS;

    
    for (int i = 0; i < NUM_THREADS; ++i) {
        int row_start = i * rows_per_thread;
        int row_end = (i == NUM_THREADS - 1) ? N : (i + 1) * rows_per_thread;

        thread_data.emplace_back(row_start, row_end, A, B, C, N);

        threads[i] = CreateThread(NULL, 0, matrix_multiplication, (LPVOID)&thread_data[i], 0, NULL);
        if (threads[i] == NULL) {
            cerr << "Error: Unable to create thread." << endl;
            exit(-1);
        }
    }

    
    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

   
    cout << "Result Matrix C (A * B):" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
