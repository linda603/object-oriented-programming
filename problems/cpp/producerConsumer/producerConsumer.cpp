#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx; // Mutex for synchronization
std::condition_variable cond_var; // Condition variable for producer-consumer signaling
std::queue<int> buffer;
const unsigned int MAX_BUFFER_SIZE = 10; // Max size of buffer

void producer(int val) {
    std::unique_lock<std::mutex> lock(mtx); // Lock the mutex
    cond_var.wait(lock, [] { return buffer.size() < MAX_BUFFER_SIZE; }); // Wait until there's space in buffer
    buffer.push(val);
    std::cout << "Producing " << val << std::endl;
    std::cout << "Buffer size after producing: " << buffer.size() << std::endl;
    lock.unlock(); // Unlock the mutex
    cond_var.notify_one(); // Notify one waiting thread
}

void consumer() {
    std::unique_lock<std::mutex> lock(mtx);
    cond_var.wait(lock, [] { return buffer.size() > 0; }); // Wait unitl there's something in the buffer

    int val = buffer.front(); // Get the front val in the buffer
    buffer.pop(); // Remove val from buffer
    std::cout << "Consuming " << val << std::endl;
    std::cout << "Buffer size after consuming: " << buffer.size() << std::endl;
    lock.unlock();
    cond_var.notify_one();
}

int main() {
    std::thread producerThread([] {
        for (int i = 0; i < 20; i++) {
            producer(i); // Produces 20 items
        }
    });

    // Delay 1 second before starting consumer thread to make sure the buffer would be full with data
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::thread consumerThread([] {
        for(int i = 0; i < 20; i++) {
            consumer(); // Consume 20 items
        }
    });
    producerThread.join(); // Wait for producer thread to finish
    consumerThread.join(); // Wait for consumer thread to finish
    return 0;
}