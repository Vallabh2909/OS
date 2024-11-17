#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int P = 5;
const int R = 3;

void calculateNeed(int need[P][R], int maxm[P][R], int allot[P][R]) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = maxm[i][j] - allot[i][j];
}

bool isSafe(int processes[], int avail[], int maxm[][R], int allot[][R]) {
    int need[P][R];
    calculateNeed(need, maxm, allot);

    vector<bool> finish(P, false);
    vector<int> safeSeq(P);
    int work[R];

    for (int i = 0; i < R; i++)
        work[i] = avail[i];

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < R; j++)
                    if (need[p][j] > work[j])
                        break;

                if (j == R) {
                    for (int k = 0; k < R; k++)
                        work[k] += allot[p][k];

                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "System is not in a safe state." << endl;
            return false;
        }
    }

    cout << "System is in a safe state.\nSafe sequence is: ";
    for (int i = 0; i < count; i++)
        cout << safeSeq[i] << " ";
    cout << endl;

    return true;
}

void generateRandomMatrices(int maxm[P][R], int allot[P][R], int avail[R]) {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            maxm[i][j] = rand() % 10 + 1;
            allot[i][j] = rand() % (maxm[i][j] + 1);
        }
    }

    for (int j = 0; j < R; j++) {
        avail[j] = rand() % 10 + 1;
    }
}

int main() {
    srand(time(0));

    int processes[] = {0, 1, 2, 3, 4};

    int avail[R];
    int maxm[P][R], allot[P][R];

    generateRandomMatrices(maxm, allot, avail);

    cout << "Maximum demand matrix (maxm):\n";
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            cout << maxm[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nAllocated resources matrix (allot):\n";
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            cout << allot[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nAvailable resources (avail):\n";
    for (int i = 0; i < R; i++) {
        cout << avail[i] << " ";
    }
    cout << endl << endl;

    isSafe(processes, avail, maxm, allot);

    return 0;
}
