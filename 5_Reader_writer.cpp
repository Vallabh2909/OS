#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// Shared resources
int sharedData = 0; // Shared data between readers and writers
int read_count = 0; // Number of readers currently reading

// Mutex and condition variable for synchronization
pthread_mutex_t mutex_data; // Mutex for shared data access
pthread_mutex_t mutex_read_count; // Mutex for read_count synchronization
pthread_cond_t cond_writer; // Condition variable for writer

// Reader function
void* reader(void* arg) {
    int id = *(int*)arg;

    while (true) {
        // Reader enters
        pthread_mutex_lock(&mutex_read_count); // Lock the read_count mutex

        // If it's the first reader, lock the data mutex
        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&mutex_data); // First reader locks the data
        }

        pthread_mutex_unlock(&mutex_read_count); // Unlock the read_count mutex

        // Reading section
        cout << "Reader " << id << " is reading data: " << sharedData << endl;
        sleep(1); // Simulate reading time

        // Reader exits
        pthread_mutex_lock(&mutex_read_count); // Lock the read_count mutex
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&mutex_data); // Last reader unlocks the data
        }
        pthread_mutex_unlock(&mutex_read_count); // Unlock the read_count mutex

        sleep(1); // Simulate time between reading
    }

    return nullptr;
}

// Writer function
void* writer(void* arg) {
    int id = *(int*)arg;

    while (true) {
        // Writer enters
        pthread_mutex_lock(&mutex_data); // Lock the data mutex (exclusive access)

        // Writing section
        sharedData++;
        cout << "Writer " << id << " is writing data: " << sharedData << endl;
        sleep(2); // Simulate writing time

        // Writer exits
        pthread_mutex_unlock(&mutex_data); // Unlock the data mutex

        sleep(1); // Simulate time between writing
    }

    return nullptr;
}

int main() {
    const int NUM_READERS = 3;
    const int NUM_WRITERS = 2;

    // Initialize mutex and condition variable
    pthread_mutex_init(&mutex_data, nullptr);
    pthread_mutex_init(&mutex_read_count, nullptr);
    pthread_cond_init(&cond_writer, nullptr);

    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        int* id = new int(i + 1);
        pthread_create(&readers[i], nullptr, reader, id);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        int* id = new int(i + 1);
        pthread_create(&writers[i], nullptr, writer, id);
    }

    // Wait for threads to finish (they run infinitely in this case)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], nullptr);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], nullptr);
    }

    // Cleanup
    pthread_mutex_destroy(&mutex_data);
    pthread_mutex_destroy(&mutex_read_count);
    pthread_cond_destroy(&cond_writer);

    return 0;
}
