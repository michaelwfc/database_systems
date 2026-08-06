/**
 * @file condition_variable.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for C++ STL condition variable.
 */

// This program shows a small example of the usage of std::condition_variable.
// The std::condition_variable class provides the condition variable
// synchronization primitive. The condition variable primitive allows threads
// to wait until a particular condition before they grab a mutex. 
// It also  allows other threads to signal waiting threads to alert them that the condition may be true. 
// thread coordination: A thread sleeps until some condition becomes true, instead of repeatedly checking the condition in a loop.
// 
//           +----------------+
//           |  condition     |
//           |  variable cv   |
//           +----------------+
//                   |
//                   |
//         notify / wait
//                   |
//                   v

// +---------+     protects     +---------+
// | mutex m | <--------------> | shared  |
// |         |                  | state   |
// +---------+                  +---------+

//                               |
//                               v

//                           count == 2

// | Component          | Purpose                    |
// | ------------------ | -------------------------- |
// | mutex              | protect shared data        |
// | condition variable | sleep/wakeup mechanism     |
// | predicate          | the condition you wait for |

// A mutex answers: "Who can enter this critical section?"
// Condition variable answers: "When should this thread continue?"

// For a more detailed introduction of C style condition variables, see
// https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf.

// This program runs three threads. Two of these threads run a function that
// increments a global count variable by 1, atomically, and notifies the
// waiting thread when the count variable is 2. When the count variable is
// 2, the waiting thread wakes up, acquires the lock, and prints the count
// value.

// Includes the condition variable library header.
#include <condition_variable>
// Includes std::cout (printing) for demo purposes.
#include <iostream>
// Includes the mutex library header.
#include <mutex>
// Includes the thread library header.
#include <thread>

// Defining a global count variable, a mutex, and a condition variable to
// be used by both threads.
int count = 0;
std::mutex m;

// This is the syntax for declaring and default initializing a condition 
// variable.
std::condition_variable cv;

// In this function, a thread increments the count variable by
// 1. It also will notify one waiting thread if the count value is 2.
// It is ran by two of the threads in the main function. 

// Your example is a classic producer-consumer synchronization pattern:
// Two threads (t1, t2) produce/increment count
// One thread (t3) waits until count == 2
// When the condition is satisfied, the waiting thread wakes up and continues
// Thread A:  change count ,  wake Thread B
// Thread B:  sleep until count == 2
void add_count_and_notify() {
  // acquire mutex
  std::scoped_lock slk(m);
  // modify shared state
  count += 1;
  std::cout << "run add_count_and_notify in thread, count = " << count <<std::endl;
  if (count == 2) {

    std::cout << "run cv.notifiy_one()"<< std::endl;

    // notify waiting thread: "Something changed. Wake one waiting thread and let it check the condition."
    // notify_one() does NOT mean wake up because condition is true,
    // It only means: wake up and check again
    // Why notify while holding the lock?
    cv.notify_one();
  }
}

// This function, ran by the waiting thread, waits on the condition count == 2. 
// After that, it grabs the mutex m and executes code in the critical section.
// Condition variables need an std::unique_lock object to be constructed.
// std::unique_lock is a type of C++ STL synchronization primitive that
// gives more flexibility and features, including the usage with
// condition variables. Particularly, it is moveable but not copy-constructible
// or copy-assignable.
void waiter_thread() {
  //  Because condition_variable needs to:
      // wake up
      // lock mutex
      // wait()
      // - if predicate is ture, execute
      // - if predicate,  unlock mutex, sleep
  std::unique_lock lk(m);

  // wait(): release lock + sleep + reacquire lock
  // cv.wait(lk, predicate) does not guarantee that it wakes only when count becomes 2. 
  // It can wake up for other reasons, then it checks the predicate again. 
  // If the predicate is false, it sleeps again.
  cv.wait(lk, []{return count == 2;});
  // logically equivalent to:
  // while (count != 2) {
  //     cv.wait(lk);
  // }


  // this line will execute only after cv.wait() returns, and cv.wait() returns only when the predicate:
  // [] { return count == 2; } evaluates to true.
  std::cout << "run waited_thread, Printing count: " << count << std::endl;
}

// The main method constructs three thread objects and has two of them run the
// add_count_and_notify function in parallel. After these threads are finished
// executing, we print the count value, from the waiter thread, showing that
// both increments, along with the conditional acquisition in the waiter
// thread, worked successfully.
int main() {
  std::thread t1(add_count_and_notify);
  std::thread t2(add_count_and_notify);
  std::thread t3(waiter_thread);
  t1.join();
  t2.join();
  t3.join();
  return 0;
}

// Thread 3

// lock(m)
//         |
//         |
//         v
// check count
// count == 2?
//        no
//         |
//         |
//         v
// unlock(m)
// sleep
//         |
//         |
//         |
//         v
//         waiting queue


// -----------------------------
// Thread 2
// count = 2
// notify_one()

// -----------------------------
// Thread 3 wakes
// lock(m)
// check count again
// count == 2
// continue
