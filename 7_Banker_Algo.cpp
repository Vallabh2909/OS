#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to check if the system is in a safe state
bool isSafeState(vector<vector<int>>& allocation, vector<vector<int>>& max, vector<int>& available) {
    int numProcesses = allocation.size();
    int numResources = available.size();
    
    // Calculate the need matrix
    vector<vector<int>> need(numProcesses, vector<int>(numResources));
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    
    // Create a work vector and initialize it to available resources
    vector<int> work = available;
    vector<bool> finish(numProcesses, false);
    
    int completed = 0;
    
    while (completed < numProcesses) {
        bool progressMade = false;
        
        // Try to find a process that can finish with the current work
        for (int i = 0; i < numProcesses; ++i) {
            if (!finish[i]) {
                bool canFinish = true;
                
                // Check if the process can be finished with the current work
                for (int j = 0; j < numResources; ++j) {
                    if (need[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }
                
                // If it can finish, simulate its execution
                if (canFinish) {
                    for (int j = 0; j < numResources; ++j) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    completed++;
                    progressMade = true;
                    break;
                }
            }
        }
        
        // If no progress was made in a full iteration, then it's unsafe
        if (!progressMade) {
            return false;
        }
    }
    
    return true; // All processes can finish, hence system is in a safe state
}

int main() {
    int numProcesses, numResources;
    
    cout << "Enter number of processes: ";
    cin >> numProcesses;
    
    cout << "Enter number of resources: ";
    cin >> numResources;
    
    vector<vector<int>> allocation(numProcesses, vector<int>(numResources));
    vector<vector<int>> max(numProcesses, vector<int>(numResources));
    vector<int> available(numResources);
    
    cout << "Enter allocation matrix:\n";
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            cin >> allocation[i][j];
        }
    }
    
    cout << "Enter maximum matrix:\n";
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            cin >> max[i][j];
        }
    }
    
    cout << "Enter available resources:\n";
    for (int i = 0; i < numResources; ++i) {
        cin >> available[i];
    }
    
    // Check if the system is in a safe state
    if (isSafeState(allocation, max, available)) {
        cout << "The system is in a safe state.\n";
    } else {
        cout << "The system is in an unsafe state.\n";
    }
    
    return 0;
}
