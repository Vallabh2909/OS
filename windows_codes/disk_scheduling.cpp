
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class DiskScheduling {
public:
    static int SSTF(const vector<int>& requests, int head) {
        vector<bool> serviced(requests.size(), false);
        int totalHeadMovement = 0;
        int currentPosition = head;

        for (int i = 0; i < requests.size(); i++) {
            int closestIndex = -1;
            int closestDistance = INT_MAX;

            for (int j = 0; j < requests.size(); j++) {
                if (!serviced[j]) {
                    int distance = abs(requests[j] - currentPosition);
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestIndex = j;
                    }
                }
            }

            serviced[closestIndex] = true;
            totalHeadMovement += closestDistance;
            currentPosition = requests[closestIndex];
        }

        return totalHeadMovement;
    }

    static int SCAN(const vector<int>& requests, int head, int diskSize) {
        vector<int> sortedRequests = requests;
        sort(sortedRequests.begin(), sortedRequests.end());
        int totalHeadMovement = 0;

        auto it = lower_bound(sortedRequests.begin(), sortedRequests.end(), head);

        for (auto i = it; i != sortedRequests.end(); i++) {
            totalHeadMovement += abs(head - *i);
            head = *i;
        }

        if (it != sortedRequests.begin()) {
            totalHeadMovement += abs(head - *(--it));
            head = *it;

            while (it != sortedRequests.begin()) {
                totalHeadMovement += abs(head - *(--it));
                head = *it;
            }
        }

        return totalHeadMovement;
    }

    static int CLook(const vector<int>& requests, int head) {
        vector<int> sortedRequests = requests;
        sort(sortedRequests.begin(), sortedRequests.end());
        int totalHeadMovement = 0;

        auto it = lower_bound(sortedRequests.begin(), sortedRequests.end(), head);

        for (auto i = it; i != sortedRequests.end(); i++) {
            totalHeadMovement += abs(head - *i);
            head = *i;
        }

        if (!sortedRequests.empty()) {
            totalHeadMovement += abs(head - sortedRequests.front());
            head = sortedRequests.front();

            for (auto i : sortedRequests) {
                if (i > sortedRequests.front()) {
                    totalHeadMovement += abs(head - i);
                    head = i;
                }
            }
        }

        return totalHeadMovement;
    }
};

int main() {
    vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};
    int initialHeadPosition = 53;
    int diskSize = 200;

    cout << "Total Head Movement using SSTF: " << DiskScheduling::SSTF(requests, initialHeadPosition) << endl;
    cout << "Total Head Movement using SCAN: " << DiskScheduling::SCAN(requests, initialHeadPosition, diskSize) << endl;
    cout << "Total Head Movement using C-Look: " << DiskScheduling::CLook(requests, initialHeadPosition) << endl;

    return 0;
}
