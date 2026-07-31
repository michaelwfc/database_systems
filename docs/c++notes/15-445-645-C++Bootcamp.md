# 15-445/645 C++ Bootcamp
This bootcamp aims to provide a basic introduction to coding in modern C++. 
The features of the C++ language are too vast and expansive to cover in one
bootcamp, and quite frankly, it is learned best through experience. The staff
is certain that 15-445 will make you a more confident C++ programmer! 
However, we do cover some C++ topics that are necessary to know while doing
the programming assignments. This tutorial does not cover basic C/C++ syntax.
It mainly covers C++ programming features, particularly concepts that do not exist in C.

Feedback for the C++ bootcamp is always appreciated! Feel free to submit issues/PRs.

## Format
The bootcamp consists of C++ code files, located in `src/`, that are meant
to be read in depth. Each of these files can be compiled into an executable
with the same name. Use CMake to build these executables. This set of commands
should build all the executables. After running these commands, these executables
should be located in the `build` directory.

```bash
$ mkdir build
$ cd build
# $ cmake ..
cmake .. -DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake
$ make -j8
```
For instance, the `src/references.cpp` file compiles into the `references`
executable, located in `./build`. The same holds for every file in the source
directory.

## Files
There are fifteen files in the `src/` directory, each which cover different
concepts. They are meant to be read in the order below, since each file 
builds up on the previous one. However, if you know some modern C++ concepts
and are looking to refresh your knowledge, it is probably okay to start by
reading the files on concepts you are unfamiliar about.

### References and Move Semantics
- `references.cpp`: Covers C++ references.
- `move_semantics.cpp`: Covers C++ move semantics.
- `move_constructors.cpp`: Covers C++ class move constructors and move assignment operators.

### C++ Templates
- `templated_functions.cpp`: Covers C++ templated functions.
- `templated_classes.cpp` Covers C++ templated classes.

### Misc
- `wrapper_class.cpp`: Covers C++ wrapper classes.
- `iterator.cpp`: Covers implementing a basic C++ style iterator.
- `namespaces.cpp`: Covers C++ namespaces.

### C++ Standard Library (STL) Containers
- `vectors.cpp`: Covers `std::vector`.
- `set.cpp`: Covers `std::set`.
- `unordered_map.cpp`: Covers `std::unordered_map`.
- `auto.cpp`: Covers the usage of the C++ keyword `auto`, including using `auto` to iterate through C++ STL containers.

### C++ Standard Library (STL) Memory
- `unique_ptr.cpp`: Covers `std::unique_ptr`.
- `shared_ptr.cpp`: Covers `std::shared_ptr`.

### C++ Standard Library (STL) Synch Primitives
- `mutex.cpp`: Covers `std::mutex`.
- `scoped_lock.cpp`: Covers `std::scoped_lock`.
- `condition_variable.cpp`: Covers `std::condition_variable`.
- `rwlock.cpp`: Covers the usage of several C++ STL synchronization primitive libraries (`std::shared_mutex`, `std::shared_lock`, `std::unique_lock`) to create a reader-writer's lock implementation. 

### Demo Code for 15-445/645 Bootcamp
- `spring2024/s24_my_ptr.cpp`: Covers the code used in Spring 2024 bootcamp.

## Other Resources
There are many other resources that will be helpful while you get accquainted to C++.
I list a few here!
- [https://en.cppreference.com/w/](https://en.cppreference.com/w/): Unofficial but quite accurate summary and examples of both C++ and C standards.
- [https://cplusplus.com/](https://cplusplus.com/): Contains both a C++ language [tutorial](https://cplusplus.com/doc/tutorial/) and a C++ library [reference](https://cplusplus.com/reference/).
- [Modern C++ Tutorial](https://github.com/changkun/modern-cpp-tutorial). This GitHub repo contains
some information and exercises that are useful!

## Appendix: C++ Documentation for Topics Covered in the Bootcamp
This documentation may be useful to you! It's very comprehensive (much more comprehensive than this
bootcamp) but it may lack some readability. Overall, I think it's still a good idea to try to read
and understand this documentation, especially when working on the projects. Although the bootcamp
tries to be as comprehensive as possible, it still only covers the bare bones of using modern C++.

- [References](https://en.cppreference.com/w/cpp/language/reference)
- [std::move](https://en.cppreference.com/w/cpp/utility/move)
- [Move Constructors](https://en.cppreference.com/w/cpp/language/move_constructor) and [Move Assignment Operators](https://en.cppreference.com/w/cpp/language/move_assignment)
- [Templated Functions](https://en.cppreference.com/w/cpp/language/function_template)
- [Templated Classes](https://en.cppreference.com/w/cpp/language/class_template)
- [Iterators](https://en.cppreference.com/w/cpp/iterator)
- [Namespaces](https://en.cppreference.com/w/cpp/language/namespace)
- [std::vector](https://en.cppreference.com/w/cpp/container/vector)
- [std::set](https://en.cppreference.com/w/cpp/container/set)
- [std::unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map)
- [auto](https://en.cppreference.com/w/cpp/language/auto)
- [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)
- [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)
- [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)
- [std::scoped_lock](https://en.cppreference.com/w/cpp/thread/scoped_lock)
- [std::condition_variable](https://en.cppreference.com/w/cpp/thread/condition_variable)
- [std::shared_mutex](https://en.cppreference.com/w/cpp/thread/shared_mutex)
- [std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock)
- [std::unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock)


# References and Move Semantics

## 1. What is C++ reference?
Let's explain C++ **reference (`&`)** from first principles. This is one of the most important C++ concepts, especially for **CMU 15-445 Database Systems / BusTub**, because database engines rely heavily on references, pointers, move semantics, RAII, and avoiding unnecessary copies.

Your example is actually teaching two very important ideas:

1. **Reference as an alias**
2. **Pass-by-reference vs pass-by-value**

---

### 1. Why does C++ need references?

First, consider normal variables.

```cpp
int a = 10;
```

Memory looks like:

```
Stack

Address        Value
+---------+
| 0x1000  |  10
+---------+
      ^
      |
      a
```

`a` is a name associated with a memory location.

---

Now suppose we want another name for the same variable.

In C:

```c
int a = 10;
int *b = &a;
```

You need a pointer.

```
        +------+
a ----> | 10   | 0x1000
        +------+
           ^
           |
           b
```

Access:

```c
*b = 20;
```

C++ introduced references to make this cleaner:

```cpp
int a = 10;
int &b = a;
```

Meaning:

> `b` is another name for `a`.

Memory:

```
          +------+
a ------> | 10   |
b ------> |      |
          +------+
```

There is only **one integer**.

---

### 2. Reference declaration syntax

General form:

```cpp
type &reference_name = existing_variable;
```

Example:

```cpp
int a = 10;

int &b = a;
```

Breakdown:

```
int        -> referenced object type
&
b          -> reference name
a          -> object being referenced
```

---

### 3. Reference is NOT a new object

Important:

```cpp
int a = 10;
int &b = a;
```

Does NOT create:

```
a:
+------+
| 10   |
+------+

b:
+------+
| 10   |
+------+
```

No copy happens.

Instead:

```
a
|
v
+------+
| 10   |
+------+
^
|
b
```

Both names refer to the same memory.

---

### 4. Your first example

```cpp
int a = 10;

int &b = a;
```

Now:

```cpp
std::cout << b;
```

prints:

```
10
```

because:

```
b --> a --> 10
```

---

Now:

```cpp
b = 20;
```

What happens?

It changes `a`.

```cpp
std::cout << a;
```

Output:

```
20
```

Because:

```
       +------+
a ---> | 20   |
b ---> |      |
       +------+
```

---

### 5. Reference cannot be reseated

This is a major difference from pointers.

Pointer:

```cpp
int a = 10;
int c = 20;

int *p = &a;

p = &c;
```

Allowed.

Now:

```
p ---> c
```

---

Reference:

```cpp
int a = 10;
int c = 20;

int &r = a;

r = c;
```

Many beginners think:

```
r ---> c
```

Wrong.

Actually:

```
r ---> a
```

Assignment means:

```cpp
a = c;
```

Result:

```
a = 20
c = 20
```

The reference still refers to `a`.

---

### 6. Reference must be initialized

This is illegal:

```cpp
int &x;
```

Why?

Because a reference must always refer to something.

A pointer can be empty:

```cpp
int *p = nullptr;
```

but:

```cpp
int &r = ???;
```

doesn't make sense.

---

### 7. Pass by value vs pass by reference

Now your functions.

---

#### Pass by value

Your function:

```cpp
int double_value(int x)
{
    x *= 2;
    return x;
}
```

Call:

```cpp
int c = 6;

int r = double_value(c);
```

Memory:

Before:

```
main stack:

c
|
v
+-----+
|  6  |
+-----+


function stack:

x
|
v
+-----+
|  6  |
+-----+
```

A copy happens.

Two integers exist.

Then:

```cpp
x *= 2;
```

Only changes:

```
x = 12
```

Original:

```
c = 6
```

unchanged.

Output:

```
before: c is 6
after: c is 6
```

---

### 8. Pass by reference

Your function:

```cpp
void add_three(int &x)
{
    x = x + 3;
}
```

Call:

```cpp
int a = 10;

add_three(a);
```

Now:

```
main:

a
|
v
+------+
| 10   |
+------+

function:

x
|
+------+
```

No copy.

`x` is another name for `a`.

Execution:

```cpp
x = x + 3;
```

is equivalent to:

```cpp
a = a + 3;
```

So:

```
a = 13
```

Output:

```
a is 13
b is 13
```

because:

```cpp
int &b = a;
```

still refers to `a`.

---

### 9. Why use references?

#### Reason 1: Avoid copying

Imagine:

```cpp
struct Tuple {
    int id;
    std::string name;
    std::vector<int> values;
};
```

Function:

```cpp
void process(Tuple t)
{

}
```

Calling:

```cpp
process(tuple);
```

copies:

```
Tuple
 |
 +-- id
 |
 +-- string
 |
 +-- vector
       |
       +-- heap memory
```

Expensive.

Instead:

```cpp
void process(const Tuple &t)
{

}
```

No copy.

Only:

```
t ----+
      |
tuple-+
```

---

### 10. const reference

Very common in production C++:

```cpp
void print(const std::string &s)
{
    std::cout << s;
}
```

Meaning:

```
reference:
    yes

modify:
    no
```

You get:

* no copy
* cannot accidentally modify data

Example:

```cpp
std::string name="Michael";

print(name);
```

No string copy.

---

### 11. Reference vs pointer

Comparison:

|                         | Reference    | Pointer         |
| ----------------------- | ------------ | --------------- |
| Syntax                  | `int &x`     | `int *x`        |
| Null allowed            | ❌            | ✅               |
| Can change target       | ❌            | ✅               |
| Must initialize         | ✅            | ❌               |
| Memory address operator | hidden       | explicit        |
| Use                     | normal alias | optional object |

Example:

Reference:

```cpp
int &r=a;
```

Pointer:

```cpp
int *p=&a;
```

Access:

Reference:

```cpp
r=5;
```

Pointer:

```cpp
*p=5;
```

---

### 12. References inside database systems

In BusTub / PostgreSQL-style C++ code, you see references everywhere.

Example:

```cpp
class BufferPoolManager {
public:

Page &FetchPage(page_id_t id);

};
```

Why return reference?

Because:

```cpp
Page page = FetchPage(id);
```

would copy a whole page.

A database page:

```
Page
|
+----------------+
| 4096 bytes     |
+----------------+
```

Copying thousands of pages is expensive.

Instead:

```cpp
Page &page = FetchPage(id);
```

means:

```
page
 |
 v
buffer pool page
```

Zero copy.

---

### 13. Reference and move semantics connection

Your bootcamp also has:

```
move_semantics.cpp
move_constructors.cpp
```

because modern C++ uses:

```cpp
T&&
```

called:

> rvalue reference

Example:

```cpp
std::vector<int> v1;

auto v2 = std::move(v1);
```

Instead of copying:

```
v1 data
 |
 copy
 |
v2 data
```

move transfers ownership:

```
v1
 |
 +-----> heap data

move

v2
 |
 +-----> heap data
```

This is the foundation of high-performance C++.

---

### Summary

For your example:

```cpp
int a = 10;
int &b = a;
```

means:

```
          +------+
a ------> | 10   |
b ------> |      |
          +------+
```

`b` is another name for `a`.

---

Function comparison:

##### Pass by value

```cpp
int double_value(int x)
```

```
copy created
caller unchanged
```

---

##### Pass by reference

```cpp
void add_three(int &x)
```

```
same object
no copy
caller modified
```

---

For CMU 15-445, the practical rule is:

* Small primitive types (`int`, `double`) → pass by value
* Large objects (`vector`, `string`, `Page`, `Tuple`) → `const T&`
* Need modification → `T&`
* Need ownership transfer → `T&&`
* Need optional object → `T*`

Understanding references is the gateway to understanding **RAII, smart pointers, move semantics, and database engine performance in C++**.


## 2. What is rvalue reference and move semantics connection?



# RAII (Resource Acquisition Is Initialization)

# Smart Pointers

