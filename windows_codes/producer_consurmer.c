#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 5
#define MAX_CYCLES 10

int buffer[BUFFER_SIZE];
int count = 0;
int cycles = 0;  

CRITICAL_SECTION cs;

DWORD WINAPI producer(LPVOID arg) {
    int item = 0;
    while (1) {
        Sleep(1000);
        EnterCriticalSection(&cs);

        if (count < BUFFER_SIZE && cycles < MAX_CYCLES) {
            item++;
            buffer[count] = item;
            count++;
            printf("Produced: %d\n", item);
        }

        LeaveCriticalSection(&cs);

        
        if (cycles >= MAX_CYCLES) {
            break;
        }
    }
    return 0;
}

DWORD WINAPI consumer(LPVOID arg) {
    while (1) {
        Sleep(1500);

        EnterCriticalSection(&cs);

        if (count > 0 && cycles < MAX_CYCLES) {
            int item = buffer[count - 1];
            count--;
            printf("Consumed: %d\n", item);

            cycles++;  
        }

        LeaveCriticalSection(&cs);

        if (cycles >= MAX_CYCLES) {
            break;
        }
    }
    return 0;
}

int main() {
    InitializeCriticalSection(&cs);

    HANDLE hProducer = CreateThread(NULL, 0, producer, NULL, 0, NULL);
    HANDLE hConsumer = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

    WaitForSingleObject(hProducer, INFINITE);
    WaitForSingleObject(hConsumer, INFINITE);

    DeleteCriticalSection(&cs);

    return 0;
}
