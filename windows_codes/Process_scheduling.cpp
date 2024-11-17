#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Process {
    int pid, burst, arrival, priority, startTime, finishTime, waitingTime, turnaroundTime, remainingTime;
};

void computeTimes(vector<Process>& processes) {
    for (auto& process : processes) {
        process.turnaroundTime = process.finishTime - process.arrival;
        process.waitingTime = process.turnaroundTime - process.burst;
    }
}

void fcfs(vector<Process> processes) {
    cout << "\nFirst Come First Serve:\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) { return a.arrival < b.arrival; });

    int currentTime = 0;
    for (auto& process : processes) {
        if (currentTime < process.arrival) currentTime = process.arrival;
        process.startTime = currentTime;
        process.finishTime = currentTime + process.burst;
        currentTime += process.burst;
    }
    computeTimes(processes);

    for (auto& process : processes) {
        cout << "P" << process.pid << ": Finish=" << process.finishTime
             << ", Turnaround=" << process.turnaroundTime
             << ", Waiting=" << process.waitingTime << "\n";
    }
}

void sjfNonPreemptive(vector<Process> processes) {
    cout << "\nShortest Job First (Non-Preemptive):\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) { return a.arrival < b.arrival; });

    int currentTime = 0;
    while (!processes.empty()) {
        auto it = min_element(processes.begin(), processes.end(), [&](Process a, Process b) {
            return (a.arrival <= currentTime && a.burst < b.burst) || (a.arrival <= currentTime && b.arrival > currentTime);
        });
        if (it->arrival > currentTime) currentTime = it->arrival;
        it->startTime = currentTime;
        it->finishTime = currentTime + it->burst;
        currentTime += it->burst;

        cout << "P" << it->pid << ": Finish=" << it->finishTime
             << ", Turnaround=" << it->finishTime - it->arrival
             << ", Waiting=" << it->finishTime - it->arrival - it->burst << "\n";

        processes.erase(it);
    }
}

void priorityNonPreemptive(vector<Process> processes) {
    cout << "\nPriority (Non-Preemptive):\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) { return a.arrival < b.arrival; });

    int currentTime = 0;
    while (!processes.empty()) {
        auto it = min_element(processes.begin(), processes.end(), [&](Process a, Process b) {
            return (a.arrival <= currentTime && a.priority < b.priority) || (a.arrival <= currentTime && b.arrival > currentTime);
        });
        if (it->arrival > currentTime) currentTime = it->arrival;
        it->startTime = currentTime;
        it->finishTime = currentTime + it->burst;
        currentTime += it->burst;

        cout << "P" << it->pid << ": Finish=" << it->finishTime
             << ", Turnaround=" << it->finishTime - it->arrival
             << ", Waiting=" << it->finishTime - it->arrival - it->burst << "\n";

        processes.erase(it);
    }
}

void roundRobin(vector<Process> processes, int quantum) {
    cout << "\nRound Robin:\n";
    queue<Process*> q;
    for (auto& process : processes) process.remainingTime = process.burst;

    int currentTime = 0;
    while (true) {
        bool done = true;
        for (auto& process : processes) {
            if (process.remainingTime > 0) {
                done = false;
                if (process.remainingTime > quantum) {
                    currentTime += quantum;
                    process.remainingTime -= quantum;
                } else {
                    currentTime += process.remainingTime;
                    process.finishTime = currentTime;
                    process.remainingTime = 0;
                }
            }
        }
        if (done) break;
    }
    computeTimes(processes);
    for (auto& process : processes) {
        cout << "P" << process.pid << ": Finish=" << process.finishTime
             << ", Turnaround=" << process.turnaroundTime
             << ", Waiting=" << process.waitingTime << "\n";
    }
}

int main() {
    vector<Process> processes = {
        {1, 6, 0, 2}, {2, 8, 1, 1}, {3, 7, 2, 3}, {4, 3, 3, 2}
    };
    int quantum = 2;

    fcfs(processes);
    sjfNonPreemptive(processes);
    priorityNonPreemptive(processes);
    roundRobin(processes, quantum);

    return 0;
}
