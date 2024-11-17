#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

class PageReplacement {
public:
    static int FIFO(const vector<int>& pages, int frameCount) {
        unordered_map<int, bool> pageMap;
        queue<int> pageQueue;
        int pageFaults = 0;

        for (int page : pages) {
            if (!pageMap[page]) {
                if (pageQueue.size() == frameCount) {
                    int oldestPage = pageQueue.front();
                    pageQueue.pop();
                    pageMap[oldestPage] = false;
                }
                pageQueue.push(page);
                pageMap[page] = true;
                pageFaults++;
            }
        }
        return pageFaults;
    }

    static int LRU(const vector<int>& pages, int frameCount) {
        unordered_map<int, int> pageMap;
        int pageFaults = 0;
        int time = 0;

        for (int page : pages) {
            if (pageMap.find(page) == pageMap.end()) {
                if (pageMap.size() == frameCount) {
                    int lruPage = -1, lruTime = time;
                    for (const auto& entry : pageMap) {
                        if (entry.second < lruTime) {
                            lruTime = entry.second;
                            lruPage = entry.first;
                        }
                    }
                    pageMap.erase(lruPage);
                }
                pageFaults++;
            }
            pageMap[page] = time++;
        }
        return pageFaults;
    }

    static int Optimal(const vector<int>& pages, int frameCount) {
        unordered_map<int, bool> pageMap;
        int pageFaults = 0;

        for (int i = 0; i < pages.size(); i++) {
            if (!pageMap[pages[i]]) {
                if (pageMap.size() == frameCount) {
                    int farthest = -1, indexToReplace = -1;
                    for (auto& entry : pageMap) {
                        int j;
                        for (j = i + 1; j < pages.size(); j++) {
                            if (pages[j] == entry.first) {
                                break;
                            }
                        }
                        if (j > farthest) {
                            farthest = j;
                            indexToReplace = entry.first;
                        }
                    }
                    pageMap.erase(indexToReplace);
                }
                pageMap[pages[i]] = true;
                pageFaults++;
            }
        }
        return pageFaults;
    }
};

int main() {
    vector<int> referenceString = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3};
    int frameCount = 3;

    cout << "FIFO Page Faults: " << PageReplacement::FIFO(referenceString, frameCount) << endl;
    cout << "LRU Page Faults: " << PageReplacement::LRU(referenceString, frameCount) << endl;
    cout << "Optimal Page Faults: " << PageReplacement::Optimal(referenceString, frameCount) << endl;

    return 0;
}
