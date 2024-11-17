#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>

using namespace std;

void sortIntegers(vector<int>& nums) {
    sort(nums.begin(), nums.end());
}

void printIntegers(const  vector<int>& nums) {
    for (int num : nums) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    vector<int> nums;
    int n, num;

    
     cout << "Enter the number of integers to sort: ";
     cin >> n;
     cout << "Enter " << n << " integers:" <<  endl;

    for (int i = 0; i < n; ++i) {
         cin >> num;
        nums.push_back(num);
    }

    
    PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo;

    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    
     string command = "cmd /c \"";
    for (int i = 0; i < n; ++i) {
        command +=  to_string(nums[i]) + " ";
    }
    command += "\"";

    
    if (!CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
         cerr << "CreateProcess failed! Error: " << GetLastError() <<  endl;
        return 1;
    }

    
     cout << "Parent process (PID: " << GetCurrentProcessId() << ") sorting..." <<  endl;
    sortIntegers(nums);
     cout << "Parent sorted integers: ";
    printIntegers(nums);

    
    WaitForSingleObject(processInfo.hProcess, INFINITE);
     cout << "Child process has finished." <<  endl;

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    return 0;
}
