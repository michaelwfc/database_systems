/**
 * @file mutex.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for C++ STL mutex.

For C++ database programming:

std::thread → create workers
std::mutex → protect shared structures
std::lock_guard → RAII-safe locking
std::condition_variable → thread communication
std::atomic → lightweight synchronization

 */

// This program shows a small example of the usage of std::mutex. The
// std::mutex class provides the mutex synchronization primitive.

// Includes std::cout (printing) for demo purposes.
#include <iostream>
// Includes the mutex library header.
#include <mutex>
// Includes the thread library header.
#include <thread>

// Defining a global count variable and a mutex to be used by both threads.
int count = 0;

// This is the syntax for declaring and default initializing a mutex.
// A mutex (short for mutual exclusion) is a synchronization primitive that allows only one thread at a time to access a shared resource.
// The purpose of a mutex is to prevent data races when multiple threads access and modify the same data concurrently.

std::mutex m;

// The add_count function allows for a thread to increment the count variable
// by 1, atomically.
void add_count() {
  // Acquire the lock before accessing count, the shared resource.

  m.lock();
  // A mutex creates a critical section.
  // A critical section is a piece of code where only one thread can execute at a time.
  count += 1;
  // Release the lock after accessing count, the shared resource.
  m.unlock();
}

// The main method constructs two thread objects and has them both run the
// add_count function in parallel. After these threads are finished executing,
// we print the count value, showing that both increments worked successfully.
// The std::thread library is the C++ STL library used to construct threads.
// You may view it as a C++ equivalent of the pthread library in C.
int main() {
  std::thread t1(add_count);
  std::thread t2(add_count);
  t1.join();
  t2.join();

  std::cout << "Printing count: " << count << std::endl;
  return 0;
}