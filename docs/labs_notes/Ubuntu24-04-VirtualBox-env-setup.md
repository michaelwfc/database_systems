Prompt: 
how to build an Ubuntu-24.04 env with Oracle VirtualBox and vscode


For your use case (C/C++, database systems, BusTub, PostgreSQL internals, OS/xv6, networking labs), building a **real Ubuntu 24.04 VM with Oracle VirtualBox + VSCode** is a good alternative to WSL. You get a cleaner Linux environment, a real kernel, easier debugging with GDB, and fewer VSCode Remote-WSL issues.

The architecture will be:

```
Windows 11 Host
│
├── Oracle VirtualBox
│       │
│       └── Ubuntu 24.04 LTS VM
│              │
│              ├── GCC / Clang
│              ├── CMake
│              ├── GDB / LLDB
│              ├── Git
│              ├── Docker(optional)
│              └── VSCode Server
│
└── VSCode (Windows)
        │
        └── Remote SSH
                │
                └── Ubuntu VM
```

---

# 1. Install Oracle VirtualBox

Download:

[https://www.virtualbox.org/](https://www.virtualbox.org/)

Install:

* VirtualBox
* VirtualBox Extension Pack

Verify:

```powershell
VBoxManage --version
```

Example:

```
7.1.x
```

---

# 2. Download Ubuntu 24.04 ISO

Download:

[https://ubuntu.com/download/desktop](https://ubuntu.com/download/desktop)

File:

```
ubuntu-24.04.x-desktop-amd64.iso
```

---

# 3. Create Ubuntu VM

Open VirtualBox:

```
New
```

Name:

```
Ubuntu-24.04-dev
```

Type:

```
Linux
```

Version:

```
Ubuntu (64-bit)
```

---

## Recommended hardware configuration

For database/system development:

### CPU

```
Processor:
4 cores minimum
6-8 cores recommended
```

Example:

```
CPU: 6
```

---

### Memory

Minimum:

```
8GB
```

Recommended:

```
16GB
```

Your workloads:

* BusTub
* DuckDB
* PostgreSQL
* LLVM compilation

benefit from RAM.

---

### Disk

Choose:

```
VDI
Dynamic allocated
```

Size:

```
100GB
```

Recommended:

```
150GB
```

Because:

```
LLVM
Docker images
Build directories
Database datasets
```

consume space quickly.

---

# 4. Enable virtualization

Windows:

Task Manager:

```
Performance
    |
    CPU
        |
Virtualization: Enabled
```

If disabled:

Enable:

BIOS:

```
Intel VT-x
AMD SVM
```

---

# 5. Install Ubuntu

Start VM.

Select ISO.

During installation:

Choose:

```
Erase disk and install Ubuntu
```

This only affects the VM virtual disk.

Create user:

Example:

```
username:
michael

hostname:
ubuntu-dev
```

---

# 6. Enable VirtualBox Shared Clipboard

## Step1: Install VirtualBox Guest Additions



VirtualBox menu:

```
Devices
 |
Insert Guest Additions CD Image
```

Run:
Then Ubuntu:
```bash
cd /media/$USER/VBox_GAs_*
sudo ./VBoxLinuxAdditions.run
```

Reboot:

```bash
sudo reboot
```

Check:

```bash
lsmod | grep vbox
```

Expected:

```
vboxguest
vboxsf
```

## Step 2: Enable clipboard sharing

Power off Ubuntu VM.

VirtualBox Manager:

Ubuntu-24.04-dev
    |
    Settings
       |
       General
          |
          Advanced

Set:

Shared Clipboard:
    Bidirectional

Drag'n'Drop:
    Bidirectional

Example:

Shared Clipboard: Bidirectional

Start VM.

## Step 3: Copy terminal text

Linux terminal:

Select text with mouse.

Press:`Ctrl + Shift + C`

Windows: `Ctrl + V`

Note:

Linux terminal does not use Ctrl+C for copy because Ctrl+C means interrupt.

---

# 7. Configure network

Your `ip addr` output is actually very informative. The key point is:

**Your Ubuntu VM currently uses VirtualBox's default NAT networking, so `10.0.2.15` is not directly reachable from Windows via SSH.**

Let's analyze the output.

---

## 1. Understand each network interface

Your VM has two interfaces:

```
1: lo
2: enp0s3
```

---

### Interface 1: loopback

```
1: lo:
    inet 127.0.0.1/8
```

This is the local machine itself.

Meaning:

```
Ubuntu VM
   |
   └── 127.0.0.1
```

Only programs **inside Ubuntu VM** can access it.

Example:

```bash
ssh michael@127.0.0.1
```

works only inside the VM.

Windows cannot use this.

---

### Interface 2: enp0s3 (important)

Your actual network card:

```
2: enp0s3
```

MAC address:

```
link/ether 08:00:27:ae:0c:a8
```

The prefix:

```
08:00:27
```

is VirtualBox's vendor MAC address.

So this is definitely a VirtualBox virtual NIC.



Your current VM network is using NAT, which is why you see:
enp0s3:
    inet 10.0.2.15/24

The IPv4 address:

```
inet 10.0.2.15/24
```

is your VM IP. 

10.0.2.15 is a VirtualBox NAT internal address, not directly reachable from your Windows host for SSH.

At first glance you might think:

> "Should I ssh to 10.0.2.15?"

Usually:

```
ssh michael@10.0.2.15
```

from Windows **will fail**.

Why?

Because VirtualBox NAT works like this:

```
              Windows Host
              192.168.x.x
                   |
             VirtualBox NAT
                   |
             10.0.2.0/24
                   |
          Ubuntu VM
             10.0.2.15
```

The VM can go out:

```
Ubuntu VM
    |
    | Internet
    v
Windows network
```

but Windows cannot initiate connections into the VM.

This is similar to your home router:

```
Internet
    |
 Router NAT
    |
 PC
```

Outside cannot directly access the PC.

---

### Verify your current VirtualBox network mode

Open:

```
VirtualBox Manager

Ubuntu VM
   |
Settings
   |
Network
   |
Adapter 1
```

You probably see:

```
Attached to:
NAT
```

That explains:

```
VM IP = 10.0.2.15
```

---

## 2.  Recommended solution: Host-only Adapter for SSH from Windows

To SSH from Windows → Ubuntu VM, the common setup is:

```
Windows Host
     |
     | 192.168.56.x
     |
VirtualBox Host-Only Adapter
     |
Ubuntu VM
```
You need to add a Host-only Adapter as Adapter 2.


For SSH development, use:

```
Adapter 1: NAT
Adapter 2: Host-only Adapter
```

Architecture:

```
                 Internet
                    |
                 NAT NIC
                    |
             Ubuntu VM
                    |
             enp0s3
             10.0.2.15


Windows <-------------> Ubuntu VM
        Host-only NIC

        192.168.56.x
```

---

### Step 1: Add Host-only Adapter in VirtualBox

Power off VM.

VirtualBox:

```
Ubuntu VM
 |
Settings
 |
Network
 |
Adapter 2
```

Enable:

```
[x] Enable Network Adapter

Attached to:
Host-only Adapter
```

Choose:

```
vboxnet0 (Same as VirtualBox Host-Only Ethernet Adapter)
```

Start Ubuntu.

---

### Step 2: Check IP again

Run:

```bash
ip addr
```

You should now see another interface, something like:

```
3: enp0s8

inet 192.168.56.101/24
```

Example:

```
enp0s8:
    inet 192.168.56.101/24
```

This is the IP you use for SSH.

---


## 3. Install SSH server

Inside Ubuntu:

```bash
sudo apt install openssh-server
```

Enable:

```bash
sudo systemctl enable ssh
sudo systemctl start ssh
```

Check:

```bash
systemctl status ssh
```

---

Find IP:

```bash
ip addr
```

Example:

```
192.168.56.101
```

Test from Windows:

PowerShell:

```powershell
ssh michael@192.168.56.101
```


---

## 4. SSH from Windows

Windows PowerShell:

```powershell
ssh michael@192.168.56.101
```

First connection:

```
Are you sure you want to continue connecting?
```

Answer:

```
yes
```

Now you are inside Ubuntu.

---


## 5. Recommended setup for your VSCode workflow

For your C++/database projects:

Use:

```
VirtualBox Ubuntu 24.04

Network:

Adapter 1:
    NAT
    -> Internet

Adapter 2:
    Host-only
    -> SSH/VSCode
```

Then:

```
Windows VSCode

Remote SSH
     |
     |
192.168.56.101
     |
Ubuntu VM
     |
database_systems
BusTub
PostgreSQL
```

This gives you almost the same experience as a physical Linux workstation.

---

For your current output:

| Address      | Meaning           | Use for SSH? |
| ------------ | ----------------- | ------------ |
| 127.0.0.1    | Ubuntu itself     | ❌ No         |
| 10.0.2.15    | VirtualBox NAT IP | ❌ Usually no |
| 192.168.56.x | Host-only IP      | ✅ Yes        |

So the next step is: **add Host-only Adapter and run `ip addr` again.** You should then get the SSH-ready IP.



VirtualBox:

```
Settings
 |
Network
```

Use:

```
Adapter 1:
NAT
```

You get Internet.

For VSCode SSH:

Add:

```
Adapter 2:
Host-only Adapter
```

Example:

```
Ubuntu:
192.168.56.101
```

Windows:

```
192.168.56.1
```

---



---

# 9. Install VSCode Remote SSH

Windows VSCode:

Install extension:

```
Remote - SSH
```

Microsoft:

```
ms-vscode-remote.remote-ssh
```

---

Open:

```
Ctrl + Shift + P
```

Select:

```
Remote-SSH: Add New SSH Host
```

Input:

```
ssh michael@192.168.56.101
```

Connect.

VSCode will install:

```
~/.vscode-server
```

inside Ubuntu.

---

# 10. Install C++ development environment

Create script:

```bash
nano setup_cpp_env.sh
```

Content:

```bash
#!/bin/bash

set -e

sudo apt update

sudo apt install -y \
build-essential \
gcc \
g++ \
clang \
clangd \
clang-format \
clang-tidy \
cmake \
ninja-build \
gdb \
lldb \
git \
pkg-config \
python3-pip \
doxygen \
graphviz \
valgrind \
strace \
htop \
tmux \
tree


echo "====== Versions ======"

gcc --version
g++ --version
clang --version
clangd --version
cmake --version
gdb --version
python3 --version
git --version
```

Run:

```bash
chmod +x setup_cpp_env.sh

./setup_cpp_env.sh
```

---

# 11. Configure Git

```bash
git config --global user.name "Michael"

git config --global user.email "your@email.com"
```

---

# 12. Install recommended VSCode extensions

Inside Remote Ubuntu:

```
C/C++
clangd
CMake Tools
CodeLLDB
GitLens
Markdown All in One
```

For database development:

Also:

```
SQLTools
Docker
```

---

# 13. Configure CMake projects

Example:

```
database_systems/

├── CMakeLists.txt
├── src/
│    └── main.cpp
└── build/
```

Configure:

```bash
mkdir build

cd build

cmake .. \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Compile:

```bash
cmake --build . -j$(nproc)
```

---

# 14. Debug with VSCode

Install:

```
CodeLLDB
```

Example:

`.vscode/launch.json`

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "lldb",
            "request": "launch",
            "name": "Debug",
            "program": "${workspaceFolder}/build/app",
            "args": [],
            "cwd": "${workspaceFolder}"
        }
    ]
}
```

Now:

```
F5
```

works.

---

# 15. Share files between Windows and Ubuntu

Option 1 (recommended):

Use Git:

```
Ubuntu:
~/projects/database_systems
```

Clone:

```bash
git clone repo_url
```

---

Option 2:

VirtualBox shared folder:

Windows:

```
E:\projects
```

VirtualBox:

```
Settings
 |
Shared Folder
 |
Add
```

Ubuntu:

```bash
sudo usermod -aG vboxsf michael
```

Logout/login.

Mount:

```
/media/sf_projects
```

---

# 16. Recommended VM settings for your work

For your database/system programming:

| Setting     | Recommendation    |
| ----------- | ----------------- |
| CPU         | 6 cores           |
| RAM         | 16GB              |
| Disk        | 150GB             |
| Network     | NAT + Host-only   |
| File system | ext4              |
| Compiler    | clang + gcc       |
| Build       | Ninja             |
| Debug       | LLDB              |
| Editor      | VSCode Remote SSH |

---

# WSL vs VirtualBox for your projects

|                   | WSL2           | VirtualBox        |
| ----------------- | -------------- | ----------------- |
| Performance       | ⭐⭐⭐⭐⭐          | ⭐⭐⭐⭐              |
| Linux kernel      | Windows kernel | Real Linux kernel |
| GPU access        | Better         | Worse             |
| Docker            | Excellent      | Good              |
| GDB debugging     | Good           | Excellent         |
| OS experiments    | Limited        | Excellent         |
| xv6               | Good           | Excellent         |
| PostgreSQL source | Good           | Excellent         |
| Isolation         | Medium         | Excellent         |

For your learning path:

```
CS:APP
   ↓
MIT 6.S081 xv6
   ↓
CS144
   ↓
BusTub
   ↓
PostgreSQL internals
```

I would actually keep both:

```
Daily coding:
    VSCode + WSL

Deep systems:
    Ubuntu 24.04 VirtualBox VM
```

The VM becomes your "clean laboratory" where you can break the kernel, install experimental database engines, and reproduce Linux issues without affecting Windows.
