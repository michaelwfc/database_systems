# 1. C++ in WSL Ubuntu env

- https://code.visualstudio.com/docs/remote/wsl
- https://code.visualstudio.com/docs/cpp/config-wsl


## Setup Project on WSL

### Local WSL
The architecture is:
```bash
+------------------------------------------------+
| Windows 11                                     |
|                                                |
|  +------------------------------------------+  |
|  | WSL Ubuntu-24.04                         |  |
|  |                                          |  |
|  |  Linux user space                        |  |
|  |  bash                                    |  |
|  |  gcc/clang                               |  |
|  |  gdb                                     |  |
|  |  cmake                                   |  |
|  |                                          |  |
|  +------------------------------------------+  |
|                                                |
+------------------------------------------------+


The filepaht
Windows 11
│
├── VSCode Windows application
│
└── WSL Ubuntu-24.04
       │
       ├── Linux kernel
       │
       └── Ubuntu user space
              │
              └── /mnt/e
                    |
                    E:\ drive

```
So this is:
✅ Local WSL
✅ Ubuntu running locally
✅ VSCode connected to WSL
                    

#### using WSL with Windows filesystem
```bash
C:\Users\michael>wsl --version
WSL 版本： 2.4.4.0
内核版本： 5.15.167.4-1
WSLg 版本： 1.0.65
MSRDC 版本： 1.2.5620
Direct3D 版本： 1.611.1-81528511
DXCore 版本： 10.0.26100.1-240331-1435.ge-release
Windows 版本： 10.0.26200.8875

C:\Users\michael>wsl --update
正在检查更新。
正在将适用于 Linux 的 Windows 子系统更新到版本： 2.7.11。


michael@DESKTOP-2KLOSPO MINGW64 /e/projects/database_systems (main)
$ wsl -l -o
$ wsl --install -d Ubuntu-24.04
$ wsl -l -v
  NAME            STATE           VERSION
  Ubuntu-18.04    Stopped         2
  Ubuntu          Running         2
  Ubuntu-24.04    Stopped         2
  Ubuntu-22.04    Stopped         2

michael@DESKTOP-2KLOSPO MINGW64 /e/projects/database_systems (main)
$ wsl --set-default Ubuntu-24.04
操作成功完成。 

michael@DESKTOP-2KLOSPO MINGW64 /e/projects/database_systems (main)
$ wsl -l
适用于 Linux 的 Windows 子系统分发:
Ubuntu-24.04 (默认值)
Ubuntu-18.04
Ubuntu
Ubuntu-22.0


wsl -d Ubuntu-24.04
# WSL mounts Windows drives under /mnt, so E:\ becomes /mnt/e.
cd /mnt/e/projects/database_systems
# This opens the current folder (/mnt/e/projects/operating_system) in VS Code, using the WSL Ubuntu-24.04 environment.
code .

# inside WSL, VSCode detects: I am inside Linux
# and automatically installs: ~/.vscode-server/ inside Ubuntu.
# You can verify:
ls ~/.vscode-server
# You will see:
bin/
extensions/
# That means you are using VSCode Remote WSL.

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 24.04.4 LTS
Release:        24.04
Codename:       noble

# current project location is not Linux filesystem. It is a Windows filesystem exposed to Linux.
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ pwd
# /mnt/e/projects/database_systems

```


### VSCode Remote WSL
VSCode Remote WSL is a VSCode feature.
It solves this problem:
"I want to use Windows VSCode UI, but I want my compiler/debugger/extensions to run inside Linux."

VSCode Remote WSL means: The VSCode UI runs on Windows, but the VSCode backend/server runs inside WSL.
Architecture:

```bash
Windows
================================================

VSCode UI
(editor windows, menus, keyboard)
   |
   |
   | Remote connection
   |
   v

WSL Ubuntu
================================================

VSCode Server
   |
   |
   +-- C++ extension
   +-- clang++
   +-- gdb
   +-- cmake
   +-- terminal



```
The editor looks like Windows VSCode, but the backend runs in Linux.

#### layers

```bash
Layer 4: Editor
-----------------------------
VSCode Remote WSL


Layer 3: Development tools
-----------------------------
clang++
gdb
cmake
python


Layer 2: Linux environment
-----------------------------
Ubuntu 24.04


Layer 1: Hardware
-----------------------------
Your Windows PC
```


#### From Windows filesystem to Linux filesystem

```bash
# current project location is not Linux filesystem. It is a Windows filesystem exposed to Linux.
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ pwd
# /mnt/e/projects/database_systems
# This translation layer is slower.
WSL Linux process
       |
       |
       v
9P filesystem protocol
       |
       |
       v
NTFS (Windows)


# Native WSL filesystem , Your Linux filesystem is:
# michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ echo ~
/home/michael
# Architecture: WSL Ubuntu
# No Windows filesystem involved. Much faster.
ext4 virtual disk
        |
        |
        v

/home/michael/projects


cd ~/projects
git clone <your-repository-url>
code database_systems


```
#### code database_systems from WSL trigger Remote WSL

```bash
# When you execute inside WSL:
code database_systems
# VSCode detects Current shell: Linux
# Then Windows starts VSCode UI , VSCode installs VSCode Server in WSL
# You can see:
ls ~/.vscode-server
# Example:
.vscode-server/
    bin/
    extensions/

# That server runs inside Linux.
```
#### other VSCode Remote modes:

| Feature        | Remote machine       |
| -------------- | -------------------- |
| Remote WSL     | Local WSL instance   |
| Remote SSH     | Another Linux server |
| Dev Containers | Docker container     |
| Codespaces     | Cloud VM             |
```bash
# Remote WSL
Your PC
 |
 +-- Windows
 |
 +-- WSL Ubuntu

# Remote SSH
Your PC
 |
 +-- VSCode UI
 |
 Internet
 |
 +-- AWS EC2 Linux


#  Dev Container
Your PC
 |
 +-- Docker
       |
       +-- Ubuntu container


```


#### For your CMU 15-445 setup

```bash
Windows 11
    |
    |
    v
WSL Ubuntu-24.04
    |
    |
    v
VSCode Remote WSL
    |
    |
    v
clang++ + gdb + cmake

```


## Update 软件源（apt 仓库）
```bash
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 24.04.3 LTS
Release:        24.04
Codename:       noble



# 要把 Ubuntu 24.04（Noble Numbat）的软件源（apt 仓库）更换为阿里云镜像，你需要修改系统的 APT 源配置文件 /etc/apt/sources.list
# 你看到的提示说明 从 Ubuntu 23.10 开始（包括 24.04），APT 的软件源配置方式已经变更：
# sources.list 不再是主要配置文件，而是迁移到了新的 deb822 格式配置文件中，位置为：
# /etc/apt/sources.list.d/ubuntu.sources
# sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
sudo cp /etc/apt/sources.list.d/ubuntu.sources  /etc/apt/sources.list.d/ubuntu.sources.bak

# manually
sudo nano /etc/apt/sources.list.d/ubuntu.sources
# 2. 找到并修改 URL 字段（原默认是 archive.ubuntu.com）
URIs: http://archive.ubuntu.com/ubuntu or  URIs: http://security.ubuntu.com/ubuntu
URIs: http://mirrors.aliyun.com/ubuntu

# 如果你使用的是 nano，按 Ctrl + O 保存，按回车确认，Ctrl + X 退出。

# 替换的方法自动更改
sudo sed -i 's|http://archive.ubuntu.com/ubuntu|http://mirrors.aliyun.com/ubuntu|g' /etc/apt/sources.list.d/ubuntu.sources
sudo sed -i 's|http://security.ubuntu.com/ubuntu|http://mirrors.aliyun.com/ubuntu|g' /etc/apt/sources.list.d/ubuntu.sources

sudo apt update|grep aliyun


sudo apt-get update && sudo apt-get upgrade
sudo apt install -y build-essential gcc gdb make libssl-dev zlib1g-dev libbz2-dev libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev libncursesw5-dev xz-utils tk-dev libffi-dev liblzma-dev libcurl4-openssl-dev
```

### C++ Develop Packages 
- Clang
- Clangd
- bear
- clang-tidy
- cppcheck
- clang-format

```bash
# Install all required packages
sudo apt install -y  clang clangd lldb ninja-build clang-tidy clang-format cppcheck gcc-doc pkg-config glibc-doc cmake libpcap-dev git bash doxygen graphviz python3-pip

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems/15445-bootcamp/build$ clang --version
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin


michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems/15445-bootcamp/build$ clangd --version
Ubuntu clangd version 18.1.3 (1ubuntu1)
Features: linux+grpc
Platform: x86_64-pc-linux-gnu


michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ make --version
# GNU Make 4.3
# Built for x86_64-pc-linux-gnu
# Copyright (C) 1988-2020 Free Software Foundation, Inc.
# License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
# This is free software: you are free to change and redistribute it.
# There is NO WARRANTY, to the extent permitted by law.

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ cmake --version
# cmake version 3.28.3

# CMake suite maintained and supported by Kitware (kitware.com/cmake).


michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ python3 --version
# Python 3.12.3


```
### GCC compiler
```bash
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ g++ --version
# g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
# Copyright (C) 2023 Free Software Foundation, Inc.
# This is free software; see the source for copying conditions.  There is NO
# warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ whereis g++
g++: /usr/bin/g++ /usr/share/man/man1/g++.1.gz


michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ gdb --version
# GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1) 15.0.50.20240403-git
# Copyright (C) 2024 Free Software Foundation, Inc.
# License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
# This is free software: you are free to change and redistribute it.
# There is NO WARRANTY, to the extent permitted by law.

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ whereis gdb
gdb: /usr/bin/gdb /etc/gdb /usr/include/gdb /usr/share/gdb /usr/share/man/man1/gdb.1.gz

```


### Ubuntu debug packages 

####  Disable debuginfod
```bash
nano ~/.gdbinit
# Add:
set debuginfod enabled off

# or:
echo 'export DEBUGINFOD_URLS=""' >> ~/.bashrc
source ~/.bashrc

```

#### install dbgsym packages
```bash
# Ubuntu debug packages require debug repositories
# Usually you need: the Ubuntu debug symbol repository (ddebs).
# Check:
cat /etc/apt/sources.list
# You need something like:
deb http://ddebs.ubuntu.com noble main restricted universe multiverse

# Enable Ubuntu debug symbol repository
lsb_release -a
# Ubuntu 24.04 noble

sudo apt update
# install the key package:
sudo apt install ubuntu-dbgsym-keyring

# If successful, add the debug repository. Create:
sudo nano /etc/apt/sources.list.d/ddebs.list
# Add:
deb http://ddebs.ubuntu.com noble main restricted universe multiverse
deb http://ddebs.ubuntu.com noble-updates main restricted universe multiverse
deb http://ddebs.ubuntu.com noble-security main restricted universe multiverse

# Update package index Run:
sudo apt update
# You should see something like:
# Get: ... ddebs.ubuntu.com noble/main amd64 Packages

# Install libstdc++ debug symbols
sudo apt install libstdc++6-dbgsym
# For glibc:
sudo apt install libc6-dbg

sudo apt install gcc-13-source 

# Check:
dpkg -l | grep dbgsym




```











## wsl operations 

cope/paste :
- win > ubuntu: ctrl+ c/v > right click
- ubuntu > win : ctrl+shift + c  > ctrl+ v
- ubuntu > ubuntu : left click choose > right click


export HTTP_PROXY=[username]:[password]@[proxy-web-or-IP-address]:[port-number]
export HTTP_PROXY=127.0.0.1:7890





# 15445-Bootcamp Env


You are running into a very common C++ development environment question. Since this is **CMU 15-445/645 Database Systems Bootcamp**, it is actually a good opportunity to understand the compiler toolchain deeply.

Let's go through:

1. GCC vs Clang differences
2. Why CMU recommends clang-14
3. How CMake chooses the compiler
4. How to switch this project to clang
5. What changes are needed in VSCode debugging

---

## 1. GCC vs Clang: what are they?

Both **GCC** and **Clang** are C/C++ compiler toolchains.

The high-level pipeline is:

```
                 C++ Source
                     |
                     v
              +--------------+
              |   Compiler   |
              |              |
              |  GCC/Clang   |
              +--------------+
                     |
                     v
               Intermediate
                Representation
                     |
                     v
             Machine Code (.o)
                     |
                     v
                 Linker
                     |
                     v
               Executable
```

They both implement:

* C++ parser
* semantic analysis
* optimization
* code generation
* linking integration

But their internal architectures are different.

---

## 2. GCC architecture

GCC = GNU Compiler Collection

Originally:

```
C/C++ Code
    |
    v
 GCC Frontend
    |
    v
 GCC IR (GIMPLE)
    |
    v
 Optimization
    |
    v
 Assembly
```

GCC uses:

* C frontend
* C++ frontend
* Fortran frontend
* Ada frontend
* etc.

Advantages:

#### 1. Very mature

GCC has existed since:

```
1987
```

It is extremely stable.

Linux kernel uses GCC heavily.

---

#### 2. Excellent optimization

For production:

```
-O2
-O3
-march=native
```

GCC produces very competitive binaries.

---

#### 3. Broad architecture support

Examples:

* x86
* ARM
* RISC-V
* PowerPC
* SPARC

Important for:

* embedded systems
* operating systems
* hardware development

---

## 3. Clang architecture

Clang is part of LLVM.

Architecture:

```
C++ Source
      |
      v
+-------------+
|   Clang     |
|  Frontend   |
+-------------+
      |
      v
 LLVM IR
      |
      v
 LLVM Optimizer
      |
      v
 LLVM Backend
      |
      v
 Machine Code
```

LLVM is the key difference.

---

### Why LLVM is important

LLVM provides a universal intermediate representation:

```
             C++
              |
             Clang
              |
          LLVM IR
          /   |    \
         /    |     \
       x86   ARM   RISC-V
```

The same optimizer can target many CPUs.

---

## 4. Practical differences

|                       | GCC            | Clang          |
| --------------------- | -------------- | -------------- |
| Origin                | GNU            | LLVM           |
| First release         | 1987           | 2007           |
| Backend               | GCC backend    | LLVM           |
| Error messages        | Good           | Excellent      |
| Compile speed         | Usually slower | Usually faster |
| IDE integration       | Good           | Excellent      |
| Static analysis       | Good           | Excellent      |
| Linux support         | Excellent      | Excellent      |
| macOS                 | Secondary      | Default        |
| C++ standards support | Excellent      | Excellent      |

---

## 5. Why CMU 15-445 recommends clang-14?

BusTub / 15-445 uses:

```
clang-14
```

because:

### Reason 1: consistent environment

The course staff tests using:

```
Ubuntu
clang-14
C++17
```

If students use:

```
gcc 13
clang 18
MSVC
```

small differences may appear.

Example:

```cpp
-Werror
```

A warning in one compiler may not appear in another.

---

### Reason 2: better diagnostics

Database projects have lots of template code.

Example:

```cpp
std::vector<std::pair<int,std::string>>
```

GCC error:

```
template instantiation failed...
```

Clang:

```
candidate function not viable:
no known conversion from ...
```

Much easier to debug.

---

## 6. Your current situation

You ran:

```
cmake ..
```

Output:

```
The CXX compiler identification is GNU 13.3.0
```

Meaning:

CMake selected:

```
/usr/bin/c++
```

which is usually:

```
g++
```

Check:

```bash
which c++
```

and:

```bash
c++ --version
```

Probably:

```
g++ 13.3.0
```

---

Your clang exists:

```
clang version 18.1.3
```

but CMake does not automatically choose it.

---

## 7. Change CMake compiler to clang

### Option 1 (recommended): clean build directory

CMake caches compiler choice.

Remove:

```bash
rm -rf build
```

Create again:

```bash
mkdir build
cd build
```

Configure:

```bash
cmake .. \
-DCMAKE_C_COMPILER=clang \
-DCMAKE_CXX_COMPILER=clang++
```

You should see:

```
The CXX compiler identification is Clang 18.1.3
```

Then:

```bash
cmake --build .
```

---

### Option 2: use environment variables

Before cmake:

```bash
export CC=clang
export CXX=clang++
```

Then:

```bash
cmake ..
```

---

### Option 3: CMake toolchain file

For larger projects:

Create:

```
clang-toolchain.cmake
```

with:

```cmake
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
```

Then:

```bash
cmake .. \
-DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake
```

This is how professional projects manage compilers.

---

## 8. Verify compiler

After configuration:

```bash
grep CXX_COMPILER build/CMakeCache.txt
```

Expected:

```
CMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++
```

---

## 9. VSCode changes for debugging

The debugger is separate from the compiler.

You need:

```
Compiler
   |
   v
clang++
   |
   v
Executable
   |
   v
gdb/lldb
```

---

### Current likely setup

Your `.vscode/launch.json` probably has:

```json
"miDebuggerPath": "/usr/bin/gdb"
```

This works.

Clang-generated binaries can still be debugged by gdb.

---

### Recommended for clang

Install lldb:

```bash
sudo apt install lldb
```

Check:

```bash
lldb --version
```

Then use:

```json
{
    "name": "Debug Bootcamp",
    "type": "cppdbg",
    "request": "launch",
    "program": "${workspaceFolder}/build/references",
    "cwd": "${workspaceFolder}",
    "MIMode": "lldb"
}
```

---

## 10. VSCode C++ extension configuration

Your:

```
.vscode/c_cpp_properties.json
```

should use clang:

```json
{
    "configurations": [
        {
            "name": "WSL",
            "compilerPath": "/usr/bin/clang++",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-clang-x64"
        }
    ],
    "version": 4
}
```

This affects:

* IntelliSense
* autocomplete
* error highlighting

It does NOT affect CMake compilation.

---

## 11. Recommended setup for CMU 15-445

For your environment:

```
Windows 11
    |
    |
WSL Ubuntu
    |
    |
clang-18
    |
    |
CMake
    |
    |
Ninja
    |
    |
GDB/LLDB
```

I would use:

```bash
sudo apt install clang lldb ninja-build cmake
```

Then:

```bash
cd 15445-bootcamp

rm -rf build

cmake -B build \
-G Ninja \
-DCMAKE_C_COMPILER=clang \
-DCMAKE_CXX_COMPILER=clang++

cmake --build build
```

---

## 12. One more important note for 15-445

For BusTub later, you will see:

```cmake
set(CMAKE_CXX_FLAGS
    "-Wall -Wextra -Werror"
)
```

Clang is stricter and catches many bugs:

Examples:

* uninitialized variables
* unused parameters
* incorrect move semantics
* lifetime issues

That is why CMU prefers it. For database engine development, catching these issues early is valuable.

Your current GCC 13 setup will probably compile the bootcamp, but switching to clang now will make your environment closer to the official 15-445 workflow.


# Setting Up Dev Environment for Labs

## Creating Your Own Project Repository

If the below `git` concepts (e.g., repository, merge, pull, fork) do not make sense to you, please spend some time [learning git](https://guides.github.com/introduction/git-handbook/) first.

Follow the [instructions](https://github.com/cmu-db/bustub#cloning-this-repository) to setup your own PRIVATE repository and your own development branch. If you have previuosly forked the repository through the GitHub UI (by clicking Fork), PLEASE DO NOT PUSH ANY CODE TO YOUR PUBLIC FORKED REPOSITORY! Make sure your repository is PRIVATE before you `git push` any of your code.

If the instructor makes any changes to the code, you can merge the changes to your code by keeping your private repository connected to the CMU-DB master repository. Execute the following commands to add a remote source:

```
$ git remote add public https://github.com/cmu-db/bustub.git
```

You can then pull down the latest changes as needed during the semester:

```
$ git fetch public
$ git merge public/master
```


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


# 2. Vscode settings for C++

## Reference

- https://code.visualstudio.com/api/advanced-topics/remote-extensions
- https://code.visualstudio.com/docs/languages/cpp
- https://code.visualstudio.com/docs/cpp/launch-json-reference
- https://gourav.io/blog/setup-vscode-to-run-debug-c-cpp-code
- https://code.visualstudio.com/docs/cpp/cpp-debug


##  Vscode Extensions

If you are using VSCode, we recommend you to install 
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools), 
- [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) and 
- [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd). 





## Vscode setting files
- launch.json (debugger settings)
- tasks.json (build instructions)
- c_cpp_properties.json (compiler path and IntelliSense settings)

----

## Set Up clangd in VS Code

----

## Debug a C++ project in VS Code for HelloWorld

After that, follow this tutorial to learn how to use the visual debugger in VSCode: [Debug a C++ project in VS Code](https://www.youtube.com/watch?v=G9gnSGKYIg4).


### Step1: Write Souce Code

write souce code in `tutorials/hellp.cpp`


### Step 2: Set Debug Configuration in `.vscode/launch.json`

Create/edit `.vscode/launch.json` in your project root:

```json
{
  "version": "0.2.0",

  "configurations": [
    {
      "name": "C/C++: gcc build and debug active file",
      "type": "cppdbg",
      "request": "launch",
      "program": "${fileDirname}/${fileBasenameNoExtension}",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${fileDirname}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ],
      "preLaunchTask": "C/C++: gcc build active file",
      "targetArchitecture": "x86_64",
      "logging": { "engineLogging": true },
    }
  ]
}
```

### Step 3: Set Build Task in `.vscode/tasks.json`

```json
{
  "tasks": [
    {
      "type": "cppbuild",
      "label": "C/C++: gcc build active file",
      "command": "/usr/bin/gcc",
      "args": [
        "-fdiagnostics-color=always",
        "-g",
        "${file}",
        "-o",
        "${fileDirname}/${fileBasenameNoExtension}",
        "-lpthread"
        // "${workspaceFolder}/xv6-riscv",
        // "-I${workspaceFolder}/xv6-riscv",
        // "-I${workspaceFolder}/code_examples/include"
      ],
      "options": {
        "cwd": "${fileDirname}"
      },
      "problemMatcher": [
        "$gcc"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      // "detail": "Task generated by Debugger."
      "detail": "compiler: /usr/bin/gcc"
    }
  ],
  "version": "2.0.0"
}

```

#### Set Build Task with make(Optional)

Create `.vscode/tasks.json`:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "make",
            "args": ["-j8"],
            "options": {
                "cwd": "${workspaceFolder}/build"
            },
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

### Step 4: Set Breakpoints and Debug

1. Open `tutorials/hellp.cpp`
2. Click left margin next to line numbers to set breakpoints:
3. Press **F5** or click "Run and Debug" → "Debug byte_stream_one_write"
4. Use debug controls:
   - **F10** - Step Over
   - **F11** - Step Into
   - **Shift+F11** - Step Out
   - **F5** - Continue

5. Inspect variables:
   - Hover over variables
   - Check "Variables" panel
   - Check "Watch" panel



### (Optional) cmake Build object to Debug mode

```bash
cd ~/computer_network/sponge/build
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j8
```




-----



