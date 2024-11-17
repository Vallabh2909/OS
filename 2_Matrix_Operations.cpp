#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>

#define MAX 4 // Size of the matrix

using namespace std;

// Matrices
int matA[MAX][MAX], matB[MAX][MAX], matC[MAX][MAX]; // For addition and multiplication
int resultAdd[MAX][MAX]; // Resultant matrix for addition
int resultMult[MAX][MAX]; // Resultant matrix for multiplication

// Thread arguments
struct ThreadArgs {
    int row; // Row index
    int col; // Column index
};

// Function for matrix addition (executed by each thread)
void* addMatrices(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int row = args->row;
    int col = args->col;

    resultAdd[row][col] = matA[row][col] + matB[row][col];
    pthread_exit(0);
}

// Function for matrix multiplication (executed by each thread)
void* multiplyMatrices(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int row = args->row;
    int col = args->col;

    resultMult[row][col] = 0;
    for (int k = 0; k < MAX; k++) {
        resultMult[row][col] += matA[row][k] * matB[k][col];
    }
    pthread_exit(0);
}

// Helper function to print a matrix
void printMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    // Initialize matrices with random values
    srand(time(0));
    cout << "Matrix A:" << endl;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            cout << matA[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nMatrix B:" << endl;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matB[i][j] = rand() % 10;
            cout << matB[i][j] << " ";
        }
        cout << endl;
    }

    // Create threads for addition
    pthread_t addThreads[MAX * MAX];
    ThreadArgs addArgs[MAX * MAX];
    int threadIdx = 0;

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            addArgs[threadIdx].row = i;
            addArgs[threadIdx].col = j;
            pthread_create(&addThreads[threadIdx], NULL, addMatrices, &addArgs[threadIdx]);
            threadIdx++;
        }
    }

    // Wait for all addition threads to finish
    for (int i = 0; i < MAX * MAX; i++) {
        pthread_join(addThreads[i], NULL);
    }

    // Create threads for multiplication
    pthread_t multThreads[MAX * MAX];
    ThreadArgs multArgs[MAX * MAX];
    threadIdx = 0;

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            multArgs[threadIdx].row = i;
            multArgs[threadIdx].col = j;
            pthread_create(&multThreads[threadIdx], NULL, multiplyMatrices, &multArgs[threadIdx]);
            threadIdx++;
        }
    }

    // Wait for all multiplication threads to finish
    for (int i = 0; i < MAX * MAX; i++) {
        pthread_join(multThreads[i], NULL);
    }

    // Print results
    cout << "\nResultant Matrix after Addition:" << endl;
    printMatrix(resultAdd);

    cout << "\nResultant Matrix after Multiplication:" << endl;
    printMatrix(resultMult);

    return 0;
}
