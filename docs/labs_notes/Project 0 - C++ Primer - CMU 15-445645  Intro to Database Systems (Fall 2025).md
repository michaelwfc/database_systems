---
title: "Project #0 - C++ Primer - CMU 15-445/645 :: Intro to Database Systems (Fall 2025)"
source: "https://15445.courses.cs.cmu.edu/fall2025/project0/"
author:
  - "[[Andy Pavlo]]"
published: 2025-08-26
created: 2026-07-28
description: "Do not post your project on a public Github repository. Overview All the programming projects this semester will be written on the BusTub database management system. This system is written in C++. To make sure that you have the necessary C++ background, you must complete a simple programming assignment to …"
tags:
  - "clippings"
---
Do not post your project on a public Github repository.

# Overview

All the programming projects this semester will be written on the [BusTub](https://github.com/cmu-db/bustub) database management system. This system is written in C++. To make sure that you have the necessary C++ background, you must complete a simple programming assignment to assess your knowledge of basic C++ features. You will not be given a grade for this project, but you **must complete the project with a perfect score** before being allowed to proceed in the course. Any student unable to complete this assignment before the deadline will be asked to drop the course.

All of the code in this programming assignment must be written in C++. The projects will be specifically written for C++17, but we have found that it is generally sufficient to know C++11. If you have not used C++ before, here are some resources to help:

- [15-445 Bootcamp](https://github.com/cmu-db/15445-bootcamp), which contains several small examples to get you familiar with C++11 features.
- [Learncpp](https://www.learncpp.com/) is a useful resource that includes quizzes to test your knowledge.
- [cppreference](https://en.cppreference.com/w/) has more detailed documentation of language internals.
- [A Tour of C++](https://cmu.primo.exlibrisgroup.com/discovery/fulldisplay?docid=alma991019600108604436&context=L&vid=01CMU_INST:01CMU&search_scope=MyInst_and_CI&isFrbr=true&tab=Everything&lang=en) and [Effective Modern C++](https://cmu.primo.exlibrisgroup.com/discovery/fulldisplay?docid=alma991019578256104436&context=L&vid=01CMU_INST:01CMU&search_scope=MyInst_and_CI&tab=Everything&lang=en) are also digitally available from the CMU library.

If you are using VSCode, we recommend you to install [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools), [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) and [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd). After that, follow this tutorial to learn how to use the visual debugger in VSCode: [Debug a C++ project in VS Code](https://www.youtube.com/watch?v=G9gnSGKYIg4).

If you are using CLion, we recommend you to follow this tutorial: [CLion Debugger Fundamentals](https://www.youtube.com/watch?v=5wGsRdumueU).

If you prefer to use `gdb` for debugging, there are many tutorials available to teach you how to use `gdb`. Here are some that we have found useful:

- [Debugging Under Unix: gdb Tutorial](https://www.cs.cmu.edu/~gilpin/tutorial/)
- [GDB Tutorial: Advanced Debugging Tips For C/C++ Programmers](http://www.techbeamers.com/how-to-use-gdb-top-debugging-tips/)
- [Give me 15 minutes & I'll change your view of GDB](https://www.youtube.com/watch?v=PorfLSr3DDI)

This is a single-person project that will be completed individually (i.e. no groups).

- **Release Date:** Aug 25, 2025
- **Due Date:** Sep 07, 2025 @ 11:59pm

# Project Specification

## Count-min sketch
Consider the following scenario: you are the administrator of a popular blog website, and you've been receiving reports on certain accounting spamming excessively. To map out overall network usage and detect potential DDoS attacks, you want a real-time approach to count how often each IP address appears in the incoming request stream. However, the stream is huge, which makes traditional data structures either too slow or too memory-hungry. This is where the **Count–Min Sketch** comes in!

[Count-min sketch](https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch) (CM Sketch) is a [probablistic](https://en.wikipedia.org/wiki/Randomized_algorithm) data structure that approximates frequency counts of items in a stream using sublinear memory. It maintains a compact 2-dimensional array of counters addressed by d independently seeded hash functions. Each update increments one cell per row, and a query returns the minimum of those counters. Moreover, count-min sketch is mergeable, meaning that the sum of two sketches is equivalent to constructing a single sketch over the concatenation of the corresponding input streams. Count-min sketch is widely used for network traffic monitoring, streaming analytics, and database system optimization.

Count-min sktech is based on the following parameters:

- `width (w)` – Number of columns in the hash matrix; each hash maps an item to an index in \[0, w-1\]. Larger w ⇒ smaller additive error.
- `depth (d)` – Number of rows / independent hash functions. Larger d ⇒ lower probability of a bad overestimate.
- `hash family / seeds` – A way to derive d pairwise-independent hash functions (e.g., by seeding a base hash differently for each row).

To explain how this data structure functions, let follow the example at the beginning and consider the following input stream: `["24.156.99.202", "172.217.22.14", "64.104.78.227", "24.156.99.202"]`. Let's use a count-min sketch with width 4 and depth 3. For each of the 3 rows, hash the string (using that row’s seed) to produce an integer, reduce it modulo 4 to get a column index, and increment the counter at (row, column).

First, initialize the hash matrix (3 rows × 4 columns) with all zeros:

```
0 0 0 0
0 0 0 0
0 0 0 0
```

Now process the stream. We first insert "24.156.99.202". Assume the following hash positions (mod 4):

- hash1 → 2
- hash2 → 0
- hash3 → 3

We update the cells accordingly:

```
0 0 1 0
1 0 0 0
0 0 0 1
```

Next, we insert "172.217.22.14" and increment counters at the hash positions below:

- hash1 → 1
- hash2 → 0 (collision with the first item in row 1)
- hash3 → 2

Update:

```
0 1 1 0
2 0 0 0
0 0 1 1
```

Now we insert "64.104.78.227":

- hash1 → 3
- hash2 → 1
- hash3 → 2 (collision with the second item in row 3)

The table becomes:

```
0 1 1 1
2 1 0 0
0 0 2 1
```

Finally, repeat "24.156.99.202" (same hash positions as before: 2, 0, 3). Increment those cells again:

```
0 1 2 1
3 1 0 0
0 0 2 2
```

Now, let's estimate the frequency of "24.156.99.202". Let's look up the same columns used when inserting that key:

- Row 0, col 2 → **2**
- Row 1, col 0 → **3**
- Row 2, col 3 → **2**

The estimate is the minimum across rows:

```
Estimate("24.156.99.202") = min(2, 3, 2) = 2
```

Why take the minimum? Each row’s counter can be inflated by collisions with other items, but the minimum across independent hash rows gives the tightest upper bound on the true frequency.

## Resources

To understand more about Count-min sketch and why they work,

- A [visual walkthrough](https://www.youtube.com/watch?v=mPxslXpg8wA) illustrating the inner workings of the count-min sketch.
- A [comprehensive overview](https://youtu.be/IgyU0iFIoqM?t=515) of count-min sketch and its practical applications (the full video also covers Bloom Filter and HyperLogLog, two other widely applied data structures for large-scale data processing).
- A concise [paper](https://dsf.berkeley.edu/cs286/papers/countmin-latin2004.pdf) analyzing the time and space bounds of the data structure.

# Instructions

You will have to complete the following task for this project:

## Task #1

Implement a basic Count-min sktech data structure supporting insertion, count estimation, and merging.

In `count_min_sketch.h`, following functions are to be implemented:

- `CountMinSketch(width, depth)`: constructor, creates a count-min sketch with `width` columns (buckets) and `depth` rows (hash functions).
- `CountMinSketch(&&other)`: move constructor, transfers ownership of sketch resources from another instance.
- `operator=(&&other)`: move assignment, moves sketch resources from another instance to this one.
- `Insert(item)`: inserts the specified item into the count-min sketch. **this is expected to be thread-safe**
- `Count(&item)`: returns the estimated frequency of the item.
- `Clear()`: resets the data structure from previous streams.
- `Merge(&other)`: creates a new sketch by combining counter values from two compatible sketches.
- `TopK(k, &candidates)`: practical usage of count-min sketch--given the `candidates` that have been stored in the count-min sktech, return the `k` candidates with the most estimated counts.

Feel free to design your helper functions to assist with the implementation.

## Important Information

- For constructing hash functions for the matrix, please use the seeded hash function `"common/util/hash_util.h"` from the bustub repository. **Please refrain from using hash functions from external libraries or implementing your own, since this might influence whether you pass the test suite we provide!**
- The test suite includes parallel tests. However, **we ONLY expect thread-safe implementation for `Insert(item)`**. In other words, your implementation must correctly handle scenarios where multiple threads simultaneously perform insertions into multiple count-min sketches.
- You may notice the last test compares the performance of your implementation for `Insert(item)` against one that is strictly sequential. **You could only pass this test if the relative speedup of your implementation is larger than `1.2`. We expect you NOT to use only a single global latch to guard the whole data structure.** If you do so, the contention ratio will be effectively around `1.0`. There are many ways to do this. As a hint, try thinking of ways to break down the latch granularity or, even better, not to use a latch at all (you may find [compare\_exchange](https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange.html) helpful if you aim for the latter). If you find this difficult to reason about, try passing other tests with a global latch first before attempting to optimize for this one.



## Setting Up Your Development Environment

First install the packages that BusTub requires:

```
# Linux
$ sudo build_support/packages.sh
# macOS
$ build_support/packages.sh
```

See the [README](https://github.com/cmu-db/bustub/blob/master/README.md) for additional information on how to setup different OS environments.

To build the system from the commandline, execute the following commands:

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make -j\`nproc\`
```

We recommend always configuring CMake in debug mode. This will enable you to output debug messages and check for memory leaks (more on this in below sections).

## Testing

You can test the individual components of this assignment using our testing framework. We use [GTest](https://github.com/google/googletest) for unit test cases. You can disable tests in GTest by adding a `DISABLED_` prefix to the test name. To run the tests from the command-line:

```
$ cd build
$ make -j$(nproc) count_min_sketch_test
$ ./test/count_min_sketch_test
```

In this project, there are no hidden tests. In the future, the provided tests in the starter code are only a subset of the all the tests that we will use to evaluate and grade your project. You should write additional test cases on your own to check the complete functionality of your implementation.

Make sure that you remove the DISABLED\_ prefix from the test names otherwise they will **not** run!

## Formatting

Your code must follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). We use [Clang](https://clang.llvm.org/) to automatically check the quality of your source code. Your project grade will be **zero** if your submission fails any of these checks.

Execute the following commands to check your syntax. The `format` target will automatically correct your code. The `check-lint` and `check-clang-tidy` targets will print errors that you must manually fix to conform to our style guide.

```
$ make format
$ make check-clang-tidy-p0
```

## Memory Leaks

For this project, we use [LLVM Address Sanitizer (ASAN) and Leak Sanitizer (LSAN)](https://clang.llvm.org/docs/AddressSanitizer.html) to check for memory errors. To enable ASAN and LSAN, configure CMake in debug mode and run tests as you normally would. If there is memory error, you will see a memory error report. Note that macOS **only supports address sanitizer without leak sanitizer**.

In some cases, address sanitizer might affect the usability of the debugger. In this case, you might need to disable all sanitizers by configuring the CMake project with:

```
$ cmake -DCMAKE_BUILD_TYPE=Debug -DBUSTUB_SANITIZER= ..
```

## Development Hints

You can use `BUSTUB_ASSERT` for assertions in debug mode. Note that the statements within `BUSTUB_ASSERT` will NOT be executed in release mode. If you have something to assert in all cases, use `BUSTUB_ENSURE` instead.

We will test your implementation in release mode. To compile your solution in release mode,

```
$ mkdir build_rel && cd build_rel
$ cmake -DCMAKE_BUILD_TYPE=Release ..
```

Post all of your questions about this project on Piazza. Do **not** email the TAs directly with questions.

TAs will **not** look into your code or help you debug in this project.

## Grading Rubric

In order to pass this project, you must ensure your code follows the following guidelines:

1. Does the submission successfully execute all of the test cases and produce the correct answer?
2. Does the submission execute without any memory leaks?
3. Does the submission follow the code formatting and style policies?

Note that we will use additional test cases to grade your submission that are more complex than the sample test cases that we provide you in future projects.

## Late Policy

There are no late days for this project.

## Submission

You will submit your implementation to Gradescope:

- **[https://www.gradescope.com/courses/1074751](https://www.gradescope.com/courses/1074751)**

Run this command in `build` directory and it will create a `zip` archive called `project0-submission.zip` that you can submit to Gradescope.

```
$ make submit-p0
```

Although you are allowed submit your answers as many times as you like, you should **not** treat Gradescope as your only debugging tool. Most students submit their projects near the deadline, and thus Gradescope will take longer to process the requests. You may not get feedback in a timely manner to help you debug problems. Furthermore, the output from Gradescope is unlikely to be as informative as the output from a debugger (like `gdb`), provided you invest some time in learning to use it.

CMU students should use the Gradescope course code announced on Piazza.

## Collaboration Policy

- Every student must work individually on this assignment.
- Students are allowed to discuss high-level details about the project with others.
- Students are **not** allowed to copy the contents of a white-board after a group meeting with other students.
- Students are **not** allowed to copy solutions from another person.
- Students are allowed to use generative AI to help with development but they are ultimately responsible for their work.

**WARNING:** All of the code for this project must be your own. You may not copy source code from other students or other sources that you find on the web. Plagiarism **will not** be tolerated. See CMU's [Policy on Academic Integrity](https://www.cmu.edu/policies/student-and-student-life/academic-integrity.html) for additional information.


# QA 
## 1. What is Count-min sketch?

Count–Min Sketch is a **probabilistic frequency estimation data structure** used to answer:

> "How many times has this item appeared in a huge data stream?"

Examples:

- How many requests came from each IP address?
- How many times was a search keyword used?
- What are the most frequent products clicked?
- How many times did a network packet source appear?

The challenge is that the stream may contain **billions of events**, and storing an exact counter for every unique item may require too much memory.

A Count–Min Sketch trades **accuracy for memory efficiency**.

---

### 1. Motivation: Why do we need CMS?

Imagine a website receives:

```
GET /index.html from 192.168.1.10
GET /login from 10.0.0.5
GET /api from 192.168.1.10
...
```

You want:

```
192.168.1.10 -> 10,000,000 requests
10.0.0.5     -> 500 requests
...
```

The normal solution:

```cpp
unordered_map<string, uint64_t> counter;
```

Example:

```
IP Address        Count
-------------------------
192.168.1.10      10000000
10.0.0.5          500
8.8.8.8           200
```

Problem:

Suppose:

```
10 billion requests
1 billion unique IPs
```

The hash table needs:

```
1 billion entries
```

Memory:

```
key string
hash table overhead
counter
pointer
allocator metadata
```

Could easily require hundreds of GB.

For monitoring systems, we often only need:

> "Approximately how many times did this happen?"

That is where CMS helps.

---

### 2. Core Idea

Instead of storing:

```
IP -> count
```

CMS stores:

```
small integer matrix
```

Example:

```
        columns
       0 1 2 3 4

row0   0 0 0 0 0
row1   0 0 0 0 0
row2   0 0 0 0 0
```

This is:

```
depth = 3
width = 5
```

Meaning:

- 3 independent hash functions
- each hash chooses a column


---

### 3. Update Operation

When an item arrives:

```
item = "192.168.1.10"
```

We calculate:

```
hash1(item) % width
hash2(item) % width
hash3(item) % width
```

Suppose:

```
hash1 -> column 2
hash2 -> column 4
hash3 -> column 1
```

Increment:

```
row0,col2++
row1,col4++
row2,col1++
```

Before:

```
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
```

After:

```
0 0 1 0 0
0 0 0 0 1
0 1 0 0 0
```

---

### 4. Query Operation

Question:

```
How many times did "192.168.1.10" appear?
```

We calculate the same hashes:

```
hash1 -> col2
hash2 -> col4
hash3 -> col1
```

Read:

```
row0,col2 = 1
row1,col4 = 1
row2,col1 = 1
```

Return:

```
min(1,1,1)=1
```

---

### 5. Why Use Minimum?

The key property:

#### CMS never underestimates

The answer is always:

```
estimated count >= true count
```

Why?


Suppose two IPs collide.

Example:

```
IP A
hash1 -> column 2


IP B
hash1 -> column 2
```

Matrix:

```
row0:

col0 col1 col2 col3

 0    0    2    0
```

The counter becomes:

```
A count + B count
```

So:

```
stored count = true count + collision noise
```

The value can only increase.

Therefore:

```
CMS error is positive
```

---

#### Why minimum helps

Suppose:

```
depth=3
```

For an item:

```
row0 counter = 100  (many collisions)
row1 counter = 12
row2 counter = 15
```

The real frequency is probably close to:

```
12
```

because fewer collisions happened there.

So:

```
estimate=min(100,12,15)

=12
```

---

### 6. Accuracy Guarantees

The original paper gives:

For width:

\[
w=\frac{e}{\epsilon}
\]

For depth:

\[
d=ln(\frac1\delta)
\]


where:

| Parameter | Meaning |
|-|-|
| ε | error tolerance |
| δ | failure probability |


Guarantee: 
\[estimate \le true\_count+\epsilon N \]


with probability:
\[1-\delta\]

Example:

Stream size:

```
N = 1,000,000,000
```

Choose:

```
ε = 0.001
δ = 0.01
```

Maximum error:

```
εN

=1,000,000
```

Meaning:

A real count:

```
10,000,000
```

may become:

```
10,800,000
```

but never:

```
9,000,000
```

---

### 7. Complexity

#### Update

Need:

```
d hash calculations
```

Time:

\[
O(d)
\]


Usually:

```
d = 3~10
```

so effectively constant.


---

#### Query

Same:

```
read d counters
take minimum
```

Time:

\[
O(d)
\]


---

#### Memory

Matrix:

```
width × depth counters
```

Example:

```
width = 1,000,000
depth = 5
```

Counters:

```
5 million
```

Using uint32:

```
5M × 4 bytes

=20 MB
```

Compare:

HashMap:

```
millions of keys
hundreds of MB or GB
```

---

### 8. Collision Example

Suppose:

```
width=4
depth=3
```

Items:

```
A
B
C
```

Hash:

```
        row0 row1 row2

A        1    2    0
B        1    3    2
C        0    2    2
```

After insertion:

```
row0:

0 2 0 0


row1:

0 0 2 1


row2:

1 0 2 0
```

Query A:

```
row0 col1 =2
row1 col2 =2
row2 col0 =1
```

Answer:

```
min(2,2,1)

=1
```

Correct.

---

### 9. Mergeability

This is one of CMS's most powerful properties.

Suppose:

Server A:

```
CMS_A
```

tracks:

```
requests in Asia
```

Server B:

```
CMS_B
```

tracks:

```
requests in Europe
```

You can merge:

```
CMS_total = CMS_A + CMS_B
```

Matrix addition:

```
CMS_A

1 2
3 4


CMS_B

5 6
7 8


=

6 8
10 12
```

The result is equivalent to processing:

```
Asia stream + Europe stream
```

This makes CMS excellent for distributed systems.

---

### 10. CMS vs Other Streaming Structures

| Data Structure | Purpose | Memory | Accuracy |
|-|-|-|-|
| HashMap | exact counts | huge | exact |
| Bloom Filter | membership | tiny | false positive |
| Count-Min Sketch | frequency | tiny | overestimate |
| HyperLogLog | cardinality | tiny | approximate unique count |
| Top-K Sketch | frequent items | tiny | approximate ranking |

---

### 11. Real Production Uses

#### Network Monitoring

Example:

```
IP -> request count
```

Detect:

```
192.168.1.1 : 500M requests
```

Possible DDoS.

Used in:

- routers
- IDS systems
- telemetry


---

#### Database Query Optimization

Modern databases maintain approximate statistics.

Example:

```
SELECT *
FROM orders
WHERE customer_id=123;
```

Optimizer asks:

```
How many rows match?
```

Exact counting is expensive.

Approximate sketches help estimate cardinality.

---

#### Search Engines

Track:

```
keyword -> frequency
```

Example:

```
"iphone" -> billions
"rare term" -> few
```

---

#### Cache Systems

Estimate:

```
which objects are hot?
```

Used for:

- admission policies
- LFU caches


---

### 12. Implementation View

A simple CMS:

```cpp
class CountMinSketch {
public:

    CountMinSketch(
        size_t width,
        size_t depth
    )
        :
        table(depth,
              vector<uint64_t>(width,0))
    {}

    void add(string key)
    {
        for(size_t i=0;i<depth;i++)
        {
            auto col =
              hash(key,i)%width;

            table[i][col]++;
        }
    }


    uint64_t estimate(string key)
    {
        uint64_t result = UINT64_MAX;

        for(size_t i=0;i<depth;i++)
        {
            auto col =
              hash(key,i)%width;

            result =
              min(result,
                  table[i][col]);
        }

        return result;
    }


private:

    vector<vector<uint64_t>> table;
};
```

---

### 13. Important Engineering Details

#### Counter Overflow

Counters can overflow:

```
uint32_t

max:
4,294,967,295
```

Solutions:

- use uint64
- saturating counters
- periodic decay


---

#### Hash Quality

Bad hash:

```
many collisions
```

causes:

```
large estimation error
```

Common choices:

- MurmurHash
- xxHash
- CityHash


---

#### Choosing Width and Depth

Tradeoff:

```
More memory
       |
       v
Lower error
```

Typical:

```
depth = 5

width:
millions
```

---

### 14. Mental Model

Think of CMS as:

```
A compressed collection of counters
```

Instead of:

```
IP address
      |
      v
 exact counter
```

CMS does:

```
IP address
      |
      |
   hash functions
      |
      v

+----------------+
| counter matrix |
+----------------+

```

You lose identity information but keep frequency information.

---

### Summary

| Concept | Explanation |
|-|-|
| What is CMS? | Approximate frequency counter |
| Main idea | Hash items into a small counter matrix |
| Update | Increment one counter per hash row |
| Query | Take minimum counter |
| Error | Only overestimates |
| Time | O(depth) |
| Memory | O(width × depth) |
| Best for | Huge streams where exact counting is impossible |
| Key property | Mergeable distributed counting |

The core intuition:

> **Count-Min Sketch sacrifices exactness of individual counters in exchange for enormous memory savings while guaranteeing that counts are never underestimated.**