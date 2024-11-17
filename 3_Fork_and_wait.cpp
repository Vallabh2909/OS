#include <iostream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// Function to print the sorted array
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

// Sorting function (Bubble Sort)
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main() {
    vector<int> numbers;
    int n;

    // Input integers
    cout << "Enter the number of integers: ";
    cin >> n;

    cout << "Enter the integers: ";
    for (int i = 0; i < n; i++) {
        int temp;
        cin >> temp;
        numbers.push_back(temp);
    }

    pid_t pid = fork(); // Create a child process

    if (pid == 0) {
        // Child process
        cout << "Child process (PID: " << getpid() << ") is sorting the array using Bubble Sort...\n";
        vector<int> childNumbers = numbers; // Copy array to avoid shared memory
        bubbleSort(childNumbers);
        cout << "Sorted array by child process: ";
        printArray(childNumbers);

        // Simulate child completion
        cout << "Child process exiting.\n";
        exit(0); // Exit the child process
    } else if (pid > 0) {
        // Parent process
        cout << "Parent process (PID: " << getpid() << ") is sorting the array using Bubble Sort...\n";
        vector<int> parentNumbers = numbers; // Copy array to avoid shared memory
        bubbleSort(parentNumbers);
        cout << "Sorted array by parent process: ";
        printArray(parentNumbers);

        // Wait for child process to complete
        cout << "Parent process waiting for child process to complete...\n";
        wait(NULL); // Wait for the child process
        cout << "Child process has completed. Parent process resuming.\n";

        // Simulate parent sleeping to demonstrate zombie state
        cout << "Parent process sleeping to demonstrate zombie state...\n";
        sleep(5);

        cout << "Parent process exiting.\n";
    } else {
        // fork() failed
        cerr << "Fork failed!\n";
        return 1;
    }

    return 0;
}
