#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <climits>

using namespace std;

// Function to calculate the total seek time for SSTF
int sstfDiskScheduling(vector<int>& requests, int head) {
    int seekCount = 0;
    vector<bool> visited(requests.size(), false);
    int n = requests.size();
    int distance, curTrack;
    
    // Process all requests
    for (int i = 0; i < n; ++i) {
        int minDistance = INT_MAX;
        int index = -1;
        
        // Find the closest request to the current head position
        for (int j = 0; j < n; ++j) {
            if (!visited[j]) {
                distance = abs(requests[j] - head);
                if (distance < minDistance) {
                    minDistance = distance;
                    index = j;
                }
            }
        }
        
        // Update head position
        visited[index] = true;
        seekCount += minDistance;
        head = requests[index];
    }

    return seekCount;
}

// Function to calculate the total seek time for SCAN
int scanDiskScheduling(vector<int>& requests, int head, int diskSize) {
    int seekCount = 0;
    vector<int> left, right;

    // Split requests into left and right of the head
    for (int i = 0; i < requests.size(); i++) {
        if (requests[i] < head) {
            left.push_back(requests[i]);
        } else {
            right.push_back(requests[i]);
        }
    }

    // Sort the left and right vectors
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    // Move to the right first
    seekCount += abs(head - right[0]);
    head = right[0];

    // Move to the end of the disk
    seekCount += right[right.size() - 1] - head;
    head = right[right.size() - 1];

    // Then move to the left
    seekCount += head - left[0];
    
    return seekCount;
}

// Function to calculate the total seek time for C-Look
int clookDiskScheduling(vector<int>& requests, int head, int diskSize) {
    int seekCount = 0;
    vector<int> left, right;

    // Split requests into left and right of the head
    for (int i = 0; i < requests.size(); i++) {
        if (requests[i] < head) {
            left.push_back(requests[i]);
        } else {
            right.push_back(requests[i]);
        }
    }

    // Sort the left and right vectors
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    // Move to the right first
    seekCount += abs(head - right[0]);
    head = right[0];

    // Serve all the right side requests
    seekCount += right[right.size() - 1] - head;
    head = right[right.size() - 1];

    // Now jump to the leftmost request without backtracking
    seekCount += abs(head - left[0]);  // Jump to the leftmost request and serve
    head = left[0];

    // Optionally, you could continue serving the requests in the left side.
    // This is assumed based on your use case, otherwise this step could be skipped.
    if (left.size() > 0) {
        seekCount += left[left.size() - 1] - head;
    }
    
    return seekCount;
}


int main() {
    int n, head, diskSize;

    // Input the number of requests, initial head position, and disk size
    cout << "Enter number of disk requests: ";
    cin >> n;

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter disk size: ";
    cin >> diskSize;

    vector<int> requests(n);
    
    cout << "Enter the disk requests (space separated): ";
    for (int i = 0; i < n; i++) {
        cin >> requests[i];
    }

    // Calculate total seek time for SSTF
    int sstfSeekTime = sstfDiskScheduling(requests, head);
    cout << "Total Seek Time (SSTF): " << sstfSeekTime << endl;

    // Calculate total seek time for SCAN
    int scanSeekTime = scanDiskScheduling(requests, head, diskSize);
    cout << "Total Seek Time (SCAN): " << scanSeekTime << endl;

    // Calculate total seek time for C-Look
    int clookSeekTime = clookDiskScheduling(requests, head, diskSize);
    cout << "Total Seek Time (C-Look): " << clookSeekTime << endl;

    return 0;
}
