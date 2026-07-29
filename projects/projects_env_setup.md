# 1. Open Project on WSL
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




michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ g++ --version
# g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
# Copyright (C) 2023 Free Software Foundation, Inc.
# This is free software; see the source for copying conditions.  There is NO
# warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ gdb --version
# GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1) 15.0.50.20240403-git
# Copyright (C) 2024 Free Software Foundation, Inc.
# License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
# This is free software: you are free to change and redistribute it.
# There is NO WARRANTY, to the extent permitted by law.
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

## wsl operarion 

cope/paste :
- win > ubuntu: ctrl+ c/v > right click
- ubuntu > win : ctrl+shift + c  > ctrl+ v
- ubuntu > ubuntu : left click choose > right click


export HTTP_PROXY=[username]:[password]@[proxy-web-or-IP-address]:[port-number]
export HTTP_PROXY=127.0.0.1:7890



# 2. WSL Vscode settings
## Extensions

If you are using VSCode, we recommend you to install 
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools), 
- [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) and 
- [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd). 


## Debug a C++ project in VS Code

After that, follow this tutorial to learn how to use the visual debugger in VSCode: [Debug a C++ project in VS Code](https://www.youtube.com/watch?v=G9gnSGKYIg4).

- c_cpp_properties.json (compiler path and IntelliSense settings)
- tasks.json (build instructions)
- launch.json (debugger settings)


- https://code.visualstudio.com/docs/languages/cpp
- https://code.visualstudio.com/docs/cpp/launch-json-reference
- https://code.visualstudio.com/docs/cpp/config-wsl
- https://gourav.io/blog/setup-vscode-to-run-debug-c-cpp-code
- https://code.visualstudio.com/docs/cpp/cpp-debug
- https://code.visualstudio.com/docs/remote/wsl
- https://code.visualstudio.com/api/advanced-topics/remote-extensions




