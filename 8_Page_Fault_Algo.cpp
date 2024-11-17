#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Function to calculate page faults using FIFO
int fifoPageFaults(const vector<int>& reference, int frameCount) {
    unordered_map<int, int> pageMap;  // To store the pages in memory
    vector<int> frame(frameCount, -1);  // Frame to store pages
    int pageFaults = 0;
    int index = 0;  // To keep track of the next page to be replaced

    for (int page : reference) {
        // If page is not already in memory
        if (find(frame.begin(), frame.end(), page) == frame.end()) {
            if (pageMap.size() < frameCount) {
                frame[index] = page;
                pageMap[page] = 1;
                index = (index + 1) % frameCount;  // Circular replacement
            } else {
                // Replace the first page (FIFO)
                pageMap.erase(frame[0]);
                for (int i = 1; i < frameCount; ++i) {
                    frame[i - 1] = frame[i];
                }
                frame[frameCount - 1] = page;
                pageMap[page] = 1;
            }
            pageFaults++;
        }
    }
    return pageFaults;
}

// Function to calculate page faults using LRU
int lruPageFaults(const vector<int>& reference, int frameCount) {
    unordered_map<int, int> pageMap;  // To store the pages in memory
    vector<int> frame;
    int pageFaults = 0;

    for (int page : reference) {
        auto it = find(frame.begin(), frame.end(), page);
        // If page is not already in memory
        if (it == frame.end()) {
            if (frame.size() < frameCount) {
                frame.push_back(page);
            } else {
                // Replace the least recently used page (LRU)
                frame.erase(frame.begin());
                frame.push_back(page);
            }
            pageFaults++;
        } else {
            // Move the page to the most recent position
            frame.erase(it);
            frame.push_back(page);
        }
    }
    return pageFaults;
}

// Function to calculate page faults using Optimal Page Replacement
int optimalPageFaults(const vector<int>& reference, int frameCount) {
    vector<int> frame;
    int pageFaults = 0;

    for (int i = 0; i < reference.size(); ++i) {
        int page = reference[i];
        auto it = find(frame.begin(), frame.end(), page);
        // If page is not already in memory
        if (it == frame.end()) {
            if (frame.size() < frameCount) {
                frame.push_back(page);
            } else {
                // Find the page to replace (Optimal replacement)
                int farthest = -1;
                int indexToReplace = -1;

                for (int j = 0; j < frame.size(); ++j) {
                    int nextUse = -1;
                    for (int k = i + 1; k < reference.size(); ++k) {
                        if (reference[k] == frame[j]) {
                            nextUse = k;
                            break;
                        }
                    }
                    if (nextUse == -1) {
                        indexToReplace = j;
                        break;
                    } else if (nextUse > farthest) {
                        farthest = nextUse;
                        indexToReplace = j;
                    }
                }

                // Replace the page
                frame[indexToReplace] = page;
            }
            pageFaults++;
        }
    }
    return pageFaults;
}

int main() {
    vector<int> reference;
    int frameCount, pageCount;

    cout << "Enter number of pages: ";
    cin >> pageCount;
    
    cout << "Enter reference string (space separated): ";
    reference.resize(pageCount);
    for (int i = 0; i < pageCount; i++) {
        cin >> reference[i];
    }
    
    cout << "Enter number of frames: ";
    cin >> frameCount;
    
    // Calculate page faults for FIFO, LRU, and Optimal
    int fifoFaults = fifoPageFaults(reference, frameCount);
    int lruFaults = lruPageFaults(reference, frameCount);
    int optimalFaults = optimalPageFaults(reference, frameCount);
    
    cout << "Page Faults (FIFO): " << fifoFaults << endl;
    cout << "Page Faults (LRU): " << lruFaults << endl;
    cout << "Page Faults (Optimal): " << optimalFaults << endl;

    return 0;
}
