#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <unistd.h>

using namespace std;

// Shared resources
queue<int> buffer;       // Shared buffer
const int BUFFER_SIZE = 5; // Max size of the buffer

// Synchronization primitives
sem_t emptySlots;        // Semaphore to track empty slots in the buffer
sem_t fullSlots;         // Semaphore to track filled slots in the buffer
pthread_mutex_t bufferMutex; // Mutex to protect the buffer

// Producer function
void* producer(void* arg) {
    int id = *(int*)arg;
    int item = 0;

    while (true) {
        // Produce an item
        item++;
        sleep(1); // Simulate production time

        // Wait for an empty slot
        sem_wait(&emptySlots);

        // Lock the buffer
        pthread_mutex_lock(&bufferMutex);

        // Add item to the buffer
        buffer.push(item);
        cout << "Producer " << id << " produced item: " << item << endl;

        // Unlock the buffer
        pthread_mutex_unlock(&bufferMutex);

        // Signal that a new slot is filled
        sem_post(&fullSlots);
    }

    return nullptr;
}

// Consumer function
void* consumer(void* arg) {
    int id = *(int*)arg;

    while (true) {
        // Wait for a filled slot
        sem_wait(&fullSlots);

        // Lock the buffer
        pthread_mutex_lock(&bufferMutex);

        // Remove item from the buffer
        int item = buffer.front();
        buffer.pop();
        cout << "Consumer " << id << " consumed item: " << item << endl;

        // Unlock the buffer
        pthread_mutex_unlock(&bufferMutex);

        // Signal that an empty slot is available
        sem_post(&emptySlots);

        sleep(2); // Simulate consumption time
    }

    return nullptr;
}

int main() {
    // Number of producers and consumers
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 2;

    // Initialize synchronization primitives
    sem_init(&emptySlots, 0, BUFFER_SIZE);
    sem_init(&fullSlots, 0, 0);
    pthread_mutex_init(&bufferMutex, nullptr);

    // Thread IDs
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        int* id = new int(i + 1); // Allocate memory for thread ID
        pthread_create(&producers[i], nullptr, producer, id);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        int* id = new int(i + 1); // Allocate memory for thread ID
        pthread_create(&consumers[i], nullptr, consumer, id);
    }

    // Wait for threads to finish (never happens in this infinite loop)
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], nullptr);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], nullptr);
    }

    // Cleanup
    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);
    pthread_mutex_destroy(&bufferMutex);

    return 0;
}
