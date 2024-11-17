#include <stdio.h>
#include <windows.h>

int read_count = 0;
int shared_data = 0;

CRITICAL_SECTION cs;
HANDLE wrt;

DWORD WINAPI reader(LPVOID arg) {
    int reader_id = (int)arg;
    int read_operations = 3;

    for (int i = 0; i < read_operations; i++) {

        EnterCriticalSection(&cs);
        read_count++;
        if (read_count == 1) {
            WaitForSingleObject(wrt, INFINITE);
        }
        LeaveCriticalSection(&cs);


        printf("Reader %d is reading the shared data: %d\n", reader_id, shared_data);
        Sleep(1000);


        EnterCriticalSection(&cs);
        read_count--;
        if (read_count == 0) {
            ReleaseSemaphore(wrt, 1, NULL);
        }
        LeaveCriticalSection(&cs);

        Sleep(1500);
    }

    return 0;
}

DWORD WINAPI writer(LPVOID arg) {
    int writer_id = (int)arg;
    int write_operations = 3;

    for (int i = 0; i < write_operations; i++) {
        WaitForSingleObject(wrt, INFINITE);

        shared_data++;
        printf("Writer %d is writing the shared data: %d\n", writer_id, shared_data);
        Sleep(1000);

        ReleaseSemaphore(wrt, 1, NULL);
        Sleep(2000);
    }

    return 0;
}

int main() {

    InitializeCriticalSection(&cs);


    wrt = CreateSemaphore(NULL, 1, 1, NULL);


    int reader_ids[3] = {1, 2, 3};
    int writer_ids[2] = {1, 2};

    HANDLE hReaders[3], hWriters[2];


    for (int i = 0; i < 3; i++) {
        hReaders[i] = CreateThread(NULL, 0, reader, &reader_ids[i], 0, NULL);
    }

    for (int i = 0; i < 2; i++) {
        hWriters[i] = CreateThread(NULL, 0, writer, &writer_ids[i], 0, NULL);
    }


    WaitForMultipleObjects(3, hReaders, TRUE, INFINITE);
    WaitForMultipleObjects(2, hWriters, TRUE, INFINITE);


    DeleteCriticalSection(&cs);
    CloseHandle(wrt);

    return 0;
}