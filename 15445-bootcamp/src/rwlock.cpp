/**
 * @file rwlock.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for C++ STL std::shared_lock and std::unique_lock
 * (particularly usage of them as RWLocks).
 */

// Although C++ does not have a specific reader-writer's lock library, it is
// possible to emulate one by using the std::shared_mutex, std::shared_lock,
// and std::unique_lock libraries. This program shows a small example on how
// to do this.

// The std::shared_mutex is a mutex that allows for both shared, read-only
// locking, and exclusive, write-only locking. std::shared_lock can be used
// as an RAII-style read lock, and std::unique_lock can be used as a RAII-style
// write lock. scoped_lock.cpp talks about RAII-style locking in C++.

// If you would prefer to review the conceptuals of readers-writers locks and
// the reader-writers problem, you can refer to the 15-213/513/613 slides here:
// https://www.cs.cmu.edu/afs/cs/academic/class/15213-s23/www/lectures/25-sync-advanced.pdf

// The reader-writer problem is one of the classic synchronization problems in operating systems and database systems.
// It appears everywhere:
// Database systems: many transactions read data, fewer transactions update data
// Filesystems: many processes read files, fewer processes write files
// Caches: many threads read cached objects, occasional updates
// Web servers: many requests read configuration, admin updates configuration

// Your C++ example is implementing a Reader-Writer Lock (RWLock) using:
// std::shared_lock = "I only read, others can read with me"
// std::unique_lock = "I modify, nobody else enters"
// std::shared_mutex = provides the synchronization mechanism


// Includes std::cout (printing) for demo purposes.
#include <iostream>
// Includes the mutex library header.
#include <mutex>
// Includes the shared mutex library header.
#include <shared_mutex>
// Includes the thread library header.
#include <thread>

// Normal mutex
// std::mutex m;
// Only one thread:

// Thread A
//     lock
//       |
//       |
//     critical section
//       |
//     unlock


// Thread B
//     waits
// Even:
// Reader 1
// Reader 2
// Reader 3

// must execute:
// Reader1
//   |
// Reader2
//   |
// Reader3
// Very conservative.

// Defining a global count variable and a shared mutex to be used by all threads.
// The std::shared_mutex is a mutex that allows for shared locking, as well as
// exclusive locking.
// shared_mutex
// Allows:
// Reader1
//    |
// Reader2
//    |
// Reader3

// all together

// but:  Writer  blocks everyone

int count = 0;

// Every access to count must hold a lock.
std::shared_mutex m;

// This function uses a std::shared_lock (reader lock equivalent) to gain
// read only, shared access to the count variable, and reads the count
// variable.
void read_value() {
  // This acquires a shared lock.
  // Equivalent conceptually: m.lock_shared();
  std::shared_lock lk(m);
  std::cout << "Reading value " + std::to_string(count) + "\n" << std::flush;
}

// This function uses a std::unique_lock (write lock equivalent) to gain
// exclusive access to the count variable and write to the value.
void write_value() {
  // I need exclusive ownership.
  // Equivalent: m.lock();
  std::unique_lock lk(m);
  count += 3;
}

// The main method constructs six thread objects and has two of them run the
// write_value function, and four of them run the read_value function, all
// in parallel. This means that the output is not deterministic, depending
// on which threads grab the lock first. Run the program a few times, and
// see if you can get different outputs.

// A reader-writer lock follows:
//                  shared data

//                      |
//         +------------+------------+
//         |                         |
//      readers                  writers


// Readers:
// ---------
// Multiple readers allowed

// Reader 1  \
// Reader 2   ---> can run together
// Reader 3  /


// Writers:
// ---------
// Only one writer

// Writer 1
//     |
//     |
// (no readers, no other writers)

int main() {
  std::thread t1(read_value);
  std::thread t2(write_value);
  std::thread t3(read_value);
  std::thread t4(read_value);
  std::thread t5(write_value);
  std::thread t6(read_value);

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();

  return 0;
}