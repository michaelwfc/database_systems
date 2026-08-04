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

- no copy
- cannot accidentally modify data

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
| Null allowed            | ❌           | ✅              |
| Can change target       | ❌           | ✅              |
| Must initialize         | ✅           | ❌              |
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

- Small primitive types (`int`, `double`) → pass by value
- Large objects (`vector`, `string`, `Page`, `Tuple`) → `const T&`
- Need modification → `T&`
- Need ownership transfer → `T&&`
- Need optional object → `T*`

Understanding references is the gateway to understanding **RAII, smart pointers, move semantics, and database engine performance in C++**.

## 2. What is rvalue reference and move semantics connection?

Move semantics is one of the most important C++11 features. It is also one of the concepts that separates **modern C++** from C-style programming.

Since you are studying CMU 15-445/645, this concept is especially important because database systems heavily use move semantics in:

- buffer managers
- page objects
- tuples
- iterators
- execution operators
- STL containers
- smart pointers

Let's build the understanding from first principles.

---

### 1. Motivation: Why do we need move semantics?

Before C++11:

```cpp
std::vector<int> a = {1,2,3,4};

std::vector<int> b = a;
```

means:

```
a
 |
 | owns
 v

Heap memory
+-----------+
|1 2 3 4    |
+-----------+


copy


b
 |
 | owns
 v

Heap memory
+-----------+
|1 2 3 4    |
+-----------+
```

Two separate allocations.

The cost:

```
O(n)
```

because every element must be copied.

---

Imagine:

```cpp
std::vector<int> create_vector() {
    std::vector<int> v(1000000);
    return v;
}

auto x = create_vector();
```

Before C++11:

```
create_vector()

stack:
v
 |
 v
heap:
1 million integers


return


copy everything to x

v destroyed
```

Huge unnecessary cost.

---

C++11 introduced:

> If an object is temporary and will be destroyed soon, steal its resources instead of copying them.

That is **move semantics**.

---

### 2. Core idea

Copy:

```
copy data
========>

source
 |
 +----+
      |
      v

destination
```

Move:

```
transfer ownership
================>

source ---------> destination


source gives up resource
```

Example:

```
Before move:

int_array

stack:
+-------------+
| vector      |
| ptr --------+----+
| size=4      |    |
| capacity=4  |    |
+-------------+    |
                   |
                   v
              +---------+
              |1 2 3 4 |
              +---------+



After move:

stealing_ints

stack:
+----------------+
| vector         |
| ptr --------+  |
| size=4      |  |
+-------------+  |
                 |
                 v
             +---------+
             |1 2 3 4 |
             +---------+


int_array

+-------------+
| ptr=null    |
| size=0      |
+-------------+
```

No data copied.

Only pointer ownership transferred.

---

### 3. First concept: lvalue and rvalue

Your code comment:

> lvalues are objects that refer to a location in memory

Good simplified explanation.

Let's make it precise.

#### lvalue

An expression that has a stable identity.

Example:

```cpp
int a = 10;

a
```

`a` has:

```
address
```

You can:

```cpp
int *p=&a;
```

Therefore:

```
a = lvalue
```

---

#### rvalue

Temporary value.

Example:

```cpp
10
```

Memory:

```
temporary object
(no name)
```

Examples:

```cpp
a + 5

std::vector<int>{1,2,3}

function_return_value()
```

These are rvalues.

---

### 4. Reference types

C++ has two important references:

#### lvalue reference

```cpp
int &x = a;
```

means:

```
x
 |
 |
 v

a
```

Example:

```cpp
int a=10;

int &b=a;

b=20;

cout<<a;
```

Output:

```
20
```

They share the same object.

---

#### rvalue reference

New in C++11:

```cpp
int &&x = 10;
```

means:

```
x refers to temporary object
```

Example:

```cpp
int &&x = 10;

cout<<x;
```

Output:

```
10
```

---

### 5. std::move()

std::move(x) returns an rvalue reference (T&&), and this rvalue reference is an rvalue expression.
Very important:

Many beginners misunderstand:

> std::move moves data

Wrong.

`std::move` does NOT move anything.

It only performs a cast:

```cpp
std::move(x)
```

means:

```
treat x as an rvalue
```

Implementation roughly:

```cpp
template<class T>
typename remove_reference<T>::type&& move(T&& t)
{
    return static_cast<...>(t);
}

// It returns:
T&&

```

It changes the compiler's interpretation.

---

Example:

```cpp
std::vector<int> a={1,2,3};

std::vector<int> b=a;
```

calls:

```
copy constructor
```

because:

```
a is lvalue
```

---

Now:

```cpp
std::vector<int> b=std::move(a);
```

calls:

```
move constructor
```

because:

```
std::move(a)
        |
        v
     rvalue
```

---

### 6. Example 1 and Move constructor

```cpp
std::vector<int> int_array = {1,2,3,4};


std::vector<int> stealing_ints =
        std::move(int_array);
```

Move constructor:
rvalue reference: It sees `std::vector<int>&&` from `std::move(int_array)`
Grammar: The destination object does not exist yet. `std::vector<int> stealing_ints = ...`
The compiler asks: How do I construct stealing_ints from this rvalue? new object creation
Compiler chooses: The move constructor. `vector(vector&& other);`
Result: ownership transferred

Vector Move constructor Internally approximately:

```cpp
vector(vector&& other)
{
    this->ptr = other.ptr;
    this->size = other.size;

    other.ptr = nullptr;
    other.size = 0;
}
```

Memory Before:

```
int_array

stack:
ptr ----+
size 4  |
        |
        v

heap:
[1][2][3][4]
```

---

Memory After:

```
stealing_ints

+--------+
| ptr ---|------+
| size 4 |      |
+--------+      |
                v
             [1 2 3 4]


int_array

+---------+
| ptr null|
| size 0  |
+---------+
```

No copying.

---

### 7. Example 2 & reference binding

Code:

```cpp
std::vector<int> &&rvalue_stealing_ints =
        std::move(stealing_ints);
```

This is subtle.
Your observation is correct: The same std::move() call appears to have different behavior. Why?
The answer:
`std::move` itself never changes the object. The difference comes from what the returned rvalue reference is used for.
The return type is important because it changes which operation the compiler selects afterward.
`std::vector<int> &&` is a reference type. You are not creating a new vector. You are creating an alias.
You are NOT moving again. You are creating a reference.

The return type: `std::vector<int>&& b = std::move(a);`
Grammar: reference binding
Compiler chooses: reference initialization
Result: only alias created

Equivalent:

```cpp
int a=10;
int &x=a;
// not
int y=a;

```

```cpp
vector<int>&& rvalue_stealing_ints
          =
       stealing_ints;
```

No constructor.
No move.
No ownership transfer.

---

Memory:

```
stealing_ints
+--------+
| ptr ---|------+
| size 4 |      |
+--------+      |
                v
             [1 2 3 4]
             heap data



rvalue_stealing_ints
 |
 |
 +---- same object
```

Both names refer to the same vector.

### 8. Function example 1

Your function:

```cpp
void move_add_three_and_print(
        std::vector<int> &&vec)
{


    // vec is a lvalue. Any named variable is an lvalue expression.
    // Without std::move:  std::vector<int> vec1 = vec;
    // The compiler sees: vec is an lvalue. Therefore copy constructor is selected.
    // With std::move:  std::vector<int> vec1 = std::move(vec);
    // The compiler sees: vec is an rvalue. Therefore move  constructor is selected.


    std::vector<int> vec1 =
          std::move(vec);

    vec1.push_back(3);
}
```

Call:

```cpp
std::vector<int> int_array2={1,2,3,4};

move_add_three_and_print(
        std::move(int_array2));
```

This is an excellent question. You have reached one of the most confusing parts of C++ move semantics:
"If the function parameter is already std::vector<int>&&, why do we need std::move(vec) again?"

The answer is:
The type of vec is an rvalue reference, but the expression vec itself is an lvalue. Any named variable is an lvalue expression.
This sounds contradictory at first, so let's carefully separate type and value category.

Step-by-step:

#### Step 1： pass by rvalue reference

The parameter type is: `std::vector<int>&&`
This is called an rvalue reference
It means: "This function accepts a temporary object or an object explicitly converted into an rvalue. I may take ownership of its resources."

The type of vec is an rvalue reference, but the expression vec itself is an lvalue. Any named variable is an lvalue expression.
vec is not a new vector. It is another name for the same object.

use `T&&` parameters avoids unnecessary copies. Cost O(1) Only pointer transfer.

Instead of pass by value as `void f(std::vector<int> vec)`, copy constructor happens.

`std::vector<int>&& ve`c does not mean "vec is already moved". It means "vec is allowed to be moved from". The actual move only happens when another operation (move constructor/assignment) consumes that rvalue reference.

- The first && gives permission.
- The second std::move() actually activates the move.

so the parameter binds:

```
int_array2
     |
     |
     v

+----------------+
| vector object  |
| ptr -----------|------> heap [1,2,3,4]
+----------------+


vec
 |
 |
 +------ same object
```

No copy. No move yet. vec is just another name.

---

#### Step 2: std::move

std::move(x) returns an rvalue reference (T&&), and this rvalue reference is an rvalue expression.

The expression has:
type: `std::vector<int>&&`
value category: `xvalue`

"xvalue" means: an object with identity, but whose resources may be moved from.

##### code

| Code                          | What happens                            |
| ----------------------------- | --------------------------------------- |
| `std::move(x)`                | returns `T&&` (xvalue expression)       |
| `T&& x = std::move(a)`        | creates rvalue reference alias          |
| `T y = std::move(a)`          | invokes move constructor                |
| `func(T&&)`                   | accepts movable objects without copying |
| `func(T)`                     | copies lvalues, moves temporaries       |
| named `T&& x` inside function | `x` is an lvalue                        |

##### C++ value categories:

```
                Expression
                    |
        +-----------+-----------+
        |                       |
      glvalue                 rvalue
        |                       |
   +----+----+             +----+----+
   |         |             |         |
 lvalue    xvalue       prvalue
```

Examples:
| Expression | Category |
| --------------- | -------- |
| `v` | lvalue |
| `std::move(v)` | xvalue |
| `vector<int>{}` | prvalue |

Pass:

```cpp
std::move(int_array2)
```

No move yet.

Only cast:

```
int_array2 becomes rvalue
```

---

#### Step 3: move constructor

Function receives:

```cpp
vec
```

Important:

`vec` itself is an lvalue!

Inside:

```cpp
std::vector<int> vec1 =
        std::move(vec);
```

Now move happens.

Result:

```
vec1
 |
 v
[1 2 3 4]


vec
 |
 v
empty
```



---

### 9. Function example 2

Call:

```cpp
// std::move(int_array3) creates an xvalue expression.
// The function accepts it.
add_three_and_print(
        std::move(int_array3));

```

Inside:

```cpp
// also modifies the original object.
vec.push_back(3);


vec
 |
 v
same vector as int_array3
```

No ownership transfer.

You modify the original object.

After:

```
int_array3

[1 2 3 4 3]
```

because:

```
reference only
```

---



### 11. Why databases care about move semantics

Example BusTub:

```cpp
Page page;
```

A page contains:

```
Page
 |
 +-- page_id
 |
 +-- data pointer
 |
 +-- latch
```

Copying:

```
Page A

copy

Page B

duplicate memory
```

Dangerous.

Moving:

```
Page A
 |
 ownership
 |
 v

Page B

A becomes empty
```

This avoids:

- expensive memory copies
- double free bugs
- unnecessary allocations

---

### 12. Common mistakes

#### Mistake 1

Thinking:

```cpp
std::move(x)
```

moves immediately.

Wrong.

It only casts.

---

#### Mistake 2

Using moved objects:

```cpp
auto b=std::move(a);

cout<<a[0];
```

Undefined assumption.

After move:

```
a
```

is valid but unspecified.

Usually:

```
empty
```

but not guaranteed.

---

#### Mistake 3

Moving const objects

```cpp
const vector<int> a;

auto b=std::move(a);
```

Usually cannot move.

Why?

Move needs permission to modify source.

---

### 13. Mental model

Remember this:

```
COPY

source
 |
 | duplicate resources
 |
 v
destination


MOVE

source
 |
 | give ownership
 |
 v
destination


std::move()

source
 |
 | tell compiler:
 | "you may steal this"
 |
 v

move constructor
```

---

For CMU 15-445, the most important takeaway is:

> **Move semantics is an ownership transfer mechanism. `std::move` does not move data; it enables move constructors/assignment operators to transfer expensive resources instead of copying them.**

This concept will appear everywhere in BusTub: `Tuple`, `Page`, `BufferPoolManager`, iterators, executors, and STL containers.

---


## Q3:  What does ownership transfer mean?


Modern C++ often uses:

```cpp
class MyClass {
    // This is called the: pass-by-value and move idiom
    // why This handles both cases elegantly?
public:
    MyClass(std::vector<int> data)
        : data_(std::move(data))
    {}

    // what is different?
public:
    MyClass(std::vector<int>&& data)
        : data_(std::move(data))
    {}

private:
    std::vector<int> data_;
};
```

This is a very good question because **"ownership transfer"** is the core idea behind move semantics, but the word "ownership" is often used loosely. Let's make it precise.

There are two separate concepts:

1. **Ownership of resources**
2. **Ownership of the C++ object itself**

In move semantics, we usually mean:

> The ownership of the internal resources managed by an object is transferred from one object to another.

---

### 1. What is ownership?

Consider:

```cpp
std::vector<int> v = {1,2,3,4};
```

A vector object has two parts:

#### Stack object

```text
v
+----------------+
| pointer        |----+
| size = 4       |    |
| capacity = 4   |    |
+----------------+    |
                      |
                      v
                Heap memory
                +-------------+
                | 1 2 3 4     |
                +-------------+
```

The vector object `v` **owns** the heap memory.

Meaning:

* `v` is responsible for:

  * freeing the memory
  * managing the lifetime
  * modifying the elements

When:

```cpp
v.~vector()
```

runs:

```text
delete heap memory
```

---

### 2. What happens during copy?

Example:

```cpp
std::vector<int> a = {1,2,3,4};

std::vector<int> b = a;
```

Copy means:

```
Before:

a
 |
 v
[1 2 3 4]


After:

a
 |
 v
[1 2 3 4]


b
 |
 v
[1 2 3 4]
```

Two owners.

Two heap allocations.

Each vector destroys its own memory:

```text
a destructor
     |
     v
 delete memory A


b destructor
     |
     v
 delete memory B
```

---

### 3. What happens during move?

Example:

```cpp
std::vector<int> a = {1,2,3,4};

std::vector<int> b = std::move(a);
```

Move means:

```
Before:

a
 |
 v
[1 2 3 4]


After:

b
 |
 v
[1 2 3 4]


a
 |
 v
nullptr
```

The heap memory did not move.

The pointer moved.

Approximately:

```cpp
vector(vector&& other)
{
    this->ptr = other.ptr;

    other.ptr = nullptr;
}
```

Now:

```text
b owns the memory

a no longer owns the memory
```

That is ownership transfer.

---



### 4. Now analyze your class: 

#### Version 1: pass-by-value

```cpp
class MyClass {

public:
    MyClass(std::vector<int> data)
        : data_(std::move(data))
    {}

private:
    std::vector<int> data_;
};
```

This is called:

> pass-by-value and move idiom

---

##### Case A: caller has lvalue

Example:

```cpp
std::vector<int> v={1,2,3};

MyClass obj(v);
```

Step 1:

Parameter creation:

```cpp
std::vector<int> data
```

Because `v` is an lvalue:

```
copy happens
```

Memory:

```
v
 |
 v
[1 2 3]


data
 |
 v
[1 2 3]
```

---

Step 2:

Constructor:

```cpp
data_(std::move(data))
```

Now:

```
data
 |
 v
[1 2 3]


move


data_
 |
 v
[1 2 3]


data
empty
```

Final:

```
v
 |
 v
[1 2 3]


obj.data_
 |
 v
[1 2 3]
```

Cost:

```
1 copy + 1 move
```

---

##### Case B: caller has rvalue

Example:

```cpp
MyClass obj(std::vector<int>{1,2,3});
```

The temporary vector can be moved.

The compiler constructs:

```
data
 |
 v
temporary vector resources
```

Then:

```cpp
data_(std::move(data))
```

moves into member:

```
obj.data_
 |
 v
[1 2 3]
```

Cost:

```
2 moves
```

No deep copy.

---

### 5. Version 2: rvalue reference constructor

Your second version:

```cpp
class MyClass {

public:
    MyClass(std::vector<int>&& data)
        : data_(std::move(data))
    {}

private:
    std::vector<int> data_;
};
```

This only accepts rvalues.

Example:

```cpp
std::vector<int> v={1,2,3};

MyClass obj(std::move(v));
```

Works.

But:

```cpp
MyClass obj(v);
```

does NOT compile.

Why?

Because:

```
v
```

is an lvalue.

---

### 6. Comparison

|                           | pass by value | rvalue reference               |
| ------------------------- | ------------- | ------------------------------ |
| Function                  | `vector data` | `vector&& data`                |
| Accept lvalue             | yes           | no                             |
| Accept rvalue             | yes           | yes                            |
| Implementation complexity | simple        | more overloads needed          |
| lvalue cost               | copy          | impossible                     |
| rvalue cost               | move          | move                           |
| Modern C++ style          | very common   | used for strict ownership APIs |

---

### 7. Why is pass-by-value elegant?

Because instead of writing:

```cpp
class MyClass {

public:

MyClass(const vector<int>& data)
{
    data_=data;
}


MyClass(vector<int>&& data)
{
    data_=std::move(data);
}

};
```

you write:

```cpp
class MyClass {

public:

MyClass(vector<int> data)
    : data_(std::move(data))
{}

};
```

One constructor handles both.

---

### 8. Why not always use pass-by-value?

Because sometimes copying is unacceptable.

Example database system:

```cpp
BufferPoolManager::FetchPage(page_id)
```

Imagine:

```cpp
Page page = FetchPage();
```

A Page may contain:

```
4096 bytes page data
+
metadata
+
latch
```

You do not want accidental copies.

For low-level systems code:

```cpp
Page(Page&& other)
```

is preferred.

For application code:

```cpp
MyClass(std::vector<int> data)
```

is often cleaner.

---

### 9. Connection to `unique_ptr`

The clearest ownership example:

```cpp
std::unique_ptr<int> p =
    std::make_unique<int>(10);
```

`p` owns:

```
heap integer
```

You cannot copy:

```cpp
auto q = p;   // ERROR
```

because two owners would exist.

You must move:

```cpp
auto q = std::move(p);
```

After:

```
q
 |
 v
heap int


p
 |
 nullptr
```

Ownership transferred.

---

### Final mental model

Think of a C++ object as:

```
Object
 |
 +-- metadata (size, pointer)
 |
 +-- owned resource (heap memory, file handle, lock, socket)
```

Copy:

```
duplicate resource

A owns resource A
B owns resource B
```

Move:

```
transfer resource

A gives resource
B becomes owner
A becomes empty
```

So your constructor:

```cpp
MyClass(std::vector<int> data)
    : data_(std::move(data))
{}
```

means:

> "Give me a vector. I will take ownership of its contents efficiently."

And:

```cpp
MyClass(std::vector<int>&& data)
```

means:

> "Only give me vectors whose owners explicitly allow me to steal their resources."

The first is a flexible ownership-taking API; the second is a strict ownership-transfer API.



## Q4: What is Rule of Five

Move semantics introduced new special functions.

A class managing resources usually needs:

```cpp
class Buffer {

public:

// constructor
Buffer();

// destructor
~Buffer();

// copy constructor
Buffer(const Buffer& other);

// move constructor
Buffer(Buffer&& other);

// copy assignment
operator=(const Buffer& other);
// move assignment
operator=(Buffer&& other);

};
```

This is called: Rule of Five


This is an excellent next step. The **Rule of Five** is the natural continuation after understanding move semantics.

The short version:

> **The Rule of Five says: if a C++ class manages a resource and you need to explicitly define one of the special member functions, you probably need to define all five of them.**

The five functions are:

1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

They control **object lifetime and resource ownership transfer**.

---

### 1. Why does Rule of Five exist?

C++ objects often own resources:

* heap memory (`new/delete`)
* files (`FILE*`)
* sockets
* mutexes
* database pages
* GPU memory
* OS handles

Example:

```cpp
class Buffer {
private:
    int* data_;
    size_t size_;
};
```

Imagine:

```cpp
Buffer b1(1000);
```

Memory:

```
Stack:

b1
+-------------+
| data_       |
|   |
+---|---------+
    |
    v
Heap:

+----------------+
| 1000 integers  |
+----------------+
```

`b1` owns that memory.

When `b1` dies:

```cpp
~Buffer()
```

must release it.

---

### 2. The compiler-generated functions are dangerous

Suppose you write:

```cpp
class Buffer {

public:
    Buffer(size_t size)
    {
        data_ = new int[size];
    }

    ~Buffer()
    {
        delete[] data_;
    }

private:
    int* data_;
};
```

Now:

```cpp
Buffer a(100);

Buffer b = a;
```

What happens?

The compiler creates a default copy constructor:

```cpp
Buffer(const Buffer& other)
{
    data_ = other.data_;
}
```

Result:

```
a
 |
 +----+
      |
      v
   [memory]


b
 |
 +----+
      |
      v
   [same memory]
```

Two objects own the same memory.

Then:

```
b destructor
       |
       v
 delete memory


a destructor
       |
       v
 delete same memory AGAIN
```

Problem:

```
double free
undefined behavior
crash
```

This is why resource-owning classes need custom copy/move operations.

---

### 3. The five functions

Let's build a correct Buffer.

---

#### 0. Constructor

Creates a resource.

```cpp
Buffer(size_t size)
{
    size_ = size;
    data_ = new int[size];
}
```

Example:

```cpp
Buffer b(100);
```

Memory:

```
b
 |
 v
heap[100]
```

---

#### 1. Destructor

Releases resource.

```cpp
~Buffer()
{
    delete[] data_;
}
```

Guarantee:

```
constructor
      |
      |
      v
resource exists


destructor
      |
      |
      v
resource destroyed
```

---

#### 2. Copy constructor

Meaning:

> Create a new independent object from another object.

Signature:

```cpp
Buffer(const Buffer& other)
```

Implementation:

```cpp
Buffer(const Buffer& other)
{
    size_ = other.size_;

    data_ = new int[size_];

    memcpy(
        data_,
        other.data_,
        size_ * sizeof(int)
    );
}
```

Now:

```cpp
Buffer a(100);

Buffer b = a;
```

Memory:

```
a
 |
 v
heap A


b
 |
 v
heap B
```

Independent.

---

#### 3. Copy assignment operator

Different from copy constructor.

Copy constructor:`Buffer b = a;` creates a new object.

Copy assignment: 
```cpp
Buffer b;

b = a;
```
The object already exists.

Implementation:

```cpp
Buffer& operator=(const Buffer& other)
{
    if(this == &other)
        return *this;


    delete[] data_;


    size_=other.size_;

    data_=new int[size_];

    memcpy(
        data_,
        other.data_,
        size_*sizeof(int)
    );

    return *this;
}
```

---

#### 4. Move constructor

Introduced by C++11.

Purpose:

> Transfer ownership instead of copying data.

Signature:

```cpp
Buffer(Buffer&& other)
```

Example:

```cpp
Buffer a(1000);

Buffer b = std::move(a);
```

Instead of:

```
copy 1000 integers
```

we do:

```
move pointer
```

Implementation:

```cpp
Buffer(Buffer&& other)
{
    data_ = other.data_;
    size_ = other.size_;


    other.data_ = nullptr;
    other.size_ = 0;
}
```

Before:

```
a
 |
 v
[1000 ints]


b
```

After:

```
b
 |
 v
[1000 ints]


a
 |
 nullptr
```

Ownership transferred.

---

#### 5. Move assignment operator

Similar but destination already exists.

Example:

```cpp
Buffer a(1000);

Buffer b(10);

b = std::move(a);
```

Implementation:

```cpp
Buffer& operator=(Buffer&& other)
{
    if(this == &other)
        return *this;


    delete[] data_;


    data_=other.data_;
    size_=other.size_;


    other.data_=nullptr;
    other.size_=0;


    return *this;
}
```

Before:

```
a ---> memory A

b ---> memory B
```

After:

```
b ---> memory A

a ---> nullptr
```

Memory B is released.

---

### 4. Complete Rule of Five example

```cpp
class Buffer {

public:

    Buffer(size_t size)
        : size_(size),
          data_(new int[size])
    {}


    ~Buffer()
    {
        delete[] data_;
    }


    // copy constructor
    Buffer(const Buffer& other)
        : size_(other.size_),
          data_(new int[other.size_])
    {
        std::copy(
            other.data_,
            other.data_ + size_,
            data_
        );
    }


    // copy assignment
    Buffer& operator=(const Buffer& other)
    {
        if(this != &other)
        {
            delete[] data_;

            size_=other.size_;
            data_=new int[size_];

            std::copy(
                other.data_,
                other.data_+size_,
                data_
            );
        }

        return *this;
    }


    // move constructor
    Buffer(Buffer&& other) noexcept
        : data_(other.data_),
          size_(other.size_)
    {
        other.data_=nullptr;
        other.size_=0;
    }


    // move assignment
    Buffer& operator=(Buffer&& other) noexcept
    {
        if(this != &other)
        {
            delete[] data_;

            data_=other.data_;
            size_=other.size_;

            other.data_=nullptr;
            other.size_=0;
        }

        return *this;
    }


private:

    int* data_;
    size_t size_;
};
```

---

### 5. How are they selected?

The compiler chooses based on the expression.

---

#### Case 1: lvalue

```cpp
Buffer a;

Buffer b = a;
```

Calls:

```
copy constructor
```

because:

```
a
|
lvalue
```

---

#### Case 2: rvalue

```cpp
Buffer a;

Buffer b = std::move(a);
```

Calls:

```
move constructor
```

because:

```
std::move(a)
|
xvalue
```

---

#### Case 3: assignment

```cpp
Buffer a;
Buffer b;

b = a;
```

calls:

```
copy assignment
```

---

```cpp
b = std::move(a);
```

calls:

```
move assignment
```

---

### 6. Why `noexcept` matters

You often see:

```cpp
Buffer(Buffer&& other) noexcept
```

Why?

The STL uses move operations internally.

Example:

```cpp
std::vector<Buffer> buffers;
```

When vector grows:

```
old array
   |
   |
move objects
   |
   v
new array
```

If move constructor can throw:

```
vector may choose copy instead
```

because copying provides stronger exception safety.

Therefore:

```cpp
move constructor should usually be noexcept
```

---

### 7. Rule of Five vs Rule of Zero

Modern C++ prefers:

> **Rule of Zero**

Meaning:

> Avoid manually managing resources.

Use RAII classes:

```cpp
std::vector
std::string
std::unique_ptr
std::shared_ptr
```

Example:

```cpp
class DatabasePage {

private:

    std::vector<char> data_;

};
```

You don't need:

```
~DatabasePage()
copy constructor
move constructor
...
```

because `vector` already implements them.

---

### 8. Connection to database systems (15-445)

In CMU 15-445, you will see this everywhere.

Example:

```cpp
class Page {
private:
    char data_[4096];
};
```

Usually:

```cpp
Page(Page&& other)=delete;
```

because pages are tied to buffer pool ownership.

But:

```cpp
class Tuple {
private:
    std::vector<char> data_;
};
```

can use:

```
Rule of Zero
```

because vector handles ownership.

---

### Final mental model

The Rule of Five answers this question:

> "My class owns something outside itself. What happens when objects are created, copied, moved, assigned, and destroyed?"

| Operation        | Meaning                     |
| ---------------- | --------------------------- |
| Constructor      | acquire resource            |
| Destructor       | release resource            |
| Copy constructor | duplicate ownership         |
| Copy assignment  | replace ownership with copy |
| Move constructor | steal ownership             |
| Move assignment  | release old + steal new     |

The deeper principle:

> **Copy means "create another owner." Move means "transfer the existing ownership."**

Once you understand this, C++ resource management, `std::vector`, `unique_ptr`, and database systems code become much easier to reason about.



---


# C++ Templates

---

# RAII (Resource Acquisition Is Initialization)

## Wrapper classes


## std::unique_ptr
