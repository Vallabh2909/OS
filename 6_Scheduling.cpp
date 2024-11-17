#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Process {
    int id;  // Process ID
    int arrival_time;  // Arrival Time
    int burst_time;  // Burst Time
    int priority;  // Priority for priority-based algorithms
    int start_time;  // Time when the process starts execution
    int finish_time;  // Finish Time
    int turnaround_time;  // Turnaround Time
    int waiting_time;  // Waiting Time
    int remaining_time;  // Remaining Time for preemptive algorithms

    // Constructor to initialize a Process
    Process(int id, int arrival_time, int burst_time, int priority)
        : id(id), arrival_time(arrival_time), burst_time(burst_time), priority(priority), 
          start_time(0), finish_time(0), turnaround_time(0), waiting_time(0), remaining_time(burst_time) {}
};

// FCFS Algorithm
void FCFS(vector<Process>& processes) {
    int current_time = 0;
    for (auto& p : processes) {
        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }
        p.start_time = current_time;
        p.finish_time = current_time + p.burst_time;
        p.turnaround_time = p.finish_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        current_time = p.finish_time;
    }
}

// SJF Non-Preemptive
void SJF_NonPreemptive(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    int current_time = 0;
    vector<Process> ready_queue;
    int idx = 0;
    while (idx < processes.size() || !ready_queue.empty()) {
        while (idx < processes.size() && processes[idx].arrival_time <= current_time) {
            ready_queue.push_back(processes[idx]);
            idx++;
        }
        
        if (!ready_queue.empty()) {
            sort(ready_queue.begin(), ready_queue.end(), [](const Process& a, const Process& b) {
                return a.burst_time < b.burst_time;
            });
            Process& p = ready_queue[0];
            p.start_time = current_time;
            p.finish_time = current_time + p.burst_time;
            p.turnaround_time = p.finish_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - p.burst_time;
            current_time = p.finish_time;
            ready_queue.erase(ready_queue.begin());
        } else {
            current_time++;
        }
    }
}

// Round Robin Algorithm
void RoundRobin(vector<Process>& processes, int quantum) {
    queue<Process*> ready_queue;
    int current_time = 0;
    int n = processes.size();
    int completed = 0;

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                ready_queue.push(&processes[i]);
            }
        }

        if (!ready_queue.empty()) {
            Process* p = ready_queue.front();
            ready_queue.pop();

            if (p->remaining_time == p->burst_time) {
                p->start_time = current_time;
            }

            int time_slice = min(p->remaining_time, quantum);
            p->remaining_time -= time_slice;
            current_time += time_slice;

            if (p->remaining_time == 0) {
                p->finish_time = current_time;
                p->turnaround_time = p->finish_time - p->arrival_time;
                p->waiting_time = p->turnaround_time - p->burst_time;
                completed++;
            } else {
                ready_queue.push(p);
            }
        } else {
            current_time++;
        }
    }
}

// Utility function to print results
void printResults(const vector<Process>& processes) {
    cout << "Process ID | Arrival Time | Burst Time | Start Time | Finish Time | Turnaround Time | Waiting Time" << endl;
    for (const auto& p : processes) {
        cout << p.id << "          | " << p.arrival_time << "          | " << p.burst_time << "        | "
             << p.start_time << "        | " << p.finish_time << "          | " << p.turnaround_time << "              | " << p.waiting_time << endl;
    }
}

int main() {
    // Example processes for testing
    vector<Process> processes = {
        Process(1, 0, 6, 2),
        Process(2, 1, 8, 1),
        Process(3, 2, 7, 3),
        Process(4, 3, 3, 4)
    };

    cout << "First Come First Serve (FCFS):" << endl;
    FCFS(processes);
    printResults(processes);

    // Reset process data
    processes = {
        Process(1, 0, 6, 2),
        Process(2, 1, 8, 1),
        Process(3, 2, 7, 3),
        Process(4, 3, 3, 4)
    };

    cout << "\nShortest Job First (Non-Preemptive):" << endl;
    SJF_NonPreemptive(processes);
    printResults(processes);

    // Reset process data
    processes = {
        Process(1, 0, 6, 2),
        Process(2, 1, 8, 1),
        Process(3, 2, 7, 3),
        Process(4, 3, 3, 4)
    };

    cout << "\nRound Robin:" << endl;
    RoundRobin(processes, 4); // Quantum = 4
    printResults(processes);

    return 0;
}