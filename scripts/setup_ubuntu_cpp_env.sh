#!/bin/bash
# chmod +x setup_ubuntu_cpp_env.sh
# ./setup_ubuntu_cpp_env.sh

set -e

echo "=========================================="
echo "Ubuntu C/C++ Development Environment Setup"
echo "=========================================="

UBUNTU_VERSION=$(lsb_release -rs)

echo "Detected Ubuntu version: ${UBUNTU_VERSION}"


#############################################
# 1. Configure Aliyun apt mirror
#############################################

echo
echo "=========================================="
echo "[1/6] Configuring Aliyun apt mirror"
echo "=========================================="


if [[ "$UBUNTU_VERSION" == "22.04" ]]; then

    echo "Ubuntu 22.04 detected"

    sudo cp /etc/apt/sources.list \
        /etc/apt/sources.list.bak


    sudo sed -i \
    's|http://archive.ubuntu.com/ubuntu|http://mirrors.aliyun.com/ubuntu|g' \
    /etc/apt/sources.list


    sudo sed -i \
    's|http://security.ubuntu.com/ubuntu|http://mirrors.aliyun.com/ubuntu|g' \
    /etc/apt/sources.list


elif [[ "$UBUNTU_VERSION" == "24.04" ]]; then

    echo "Ubuntu 24.04 detected"

    sudo cp \
    /etc/apt/sources.list.d/ubuntu.sources \
    /etc/apt/sources.list.d/ubuntu.sources.bak


    sudo sed -i \
    's|http://archive.ubuntu.com/ubuntu|http://mirrors.aliyun.com/ubuntu|g' \
    /etc/apt/sources.list.d/ubuntu.sources


    sudo sed -i \
    's|http://security.ubuntu.com/ubuntu|http://mirrors.aliyun.com/ubuntu|g' \
    /etc/apt/sources.list.d/ubuntu.sources


else
    echo "Unsupported Ubuntu version: ${UBUNTU_VERSION}"
    exit 1
fi


echo
echo "Current Ubuntu mirror:"
grep aliyun -r /etc/apt/sources.list* || true



#############################################
# 2. Update system
#############################################

echo
echo "=========================================="
echo "[2/6] Updating packages"
echo "=========================================="


sudo apt update

sudo apt upgrade -y



#############################################
# 3. Install build tools
#############################################

echo
echo "=========================================="
echo "[3/6] Installing C/C++ toolchain"
echo "=========================================="


sudo apt install -y \
    build-essential \
    gcc \
    g++ \
    make \
    cmake \
    ninja-build \
    gdb \
    lldb \
    clang \
    clangd \
    clang-tidy \
    clang-format \
    cppcheck \
    pkg-config



#############################################
# 4. Install system development libraries
#############################################

echo
echo "=========================================="
echo "[4/6] Installing development libraries"
echo "=========================================="


sudo apt install -y \
    libssl-dev \
    zlib1g-dev \
    libbz2-dev \
    libreadline-dev \
    libsqlite3-dev \
    libncurses5-dev \
    libncursesw5-dev \
    xz-utils \
    tk-dev \
    libffi-dev \
    liblzma-dev \
    libcurl4-openssl-dev \
    libpcap-dev



#############################################
# 5. Install tools
#############################################

echo
echo "=========================================="
echo "[5/6] Installing developer utilities"
echo "=========================================="


sudo apt install -y \
    git \
    bash \
    wget \
    curl \
    python3-pip \
    doxygen \
    graphviz \
    valgrind \
    strace \
    ltrace \
    htop \
    tmux \
    tree



#############################################
# 6. Verify installation
#############################################

echo
echo "=========================================="
echo "[6/6] Version check"
echo "=========================================="


echo
echo "------ clang ------"
clang --version

echo
echo "------ clang++ ------"
clang++ --version


echo
echo "------ clangd ------"
clangd --version


echo
echo "------ gcc ------"
gcc --version


echo
echo "------ g++ ------"
g++ --version


echo
echo "------ cmake ------"
cmake --version


echo
echo "------ make ------"
make --version


echo
echo "------ gdb ------"
gdb --version


echo
echo "------ python ------"
python3 --version


echo
echo "------ git ------"
git --version


####  Disable debuginfod
echo 'export DEBUGINFOD_URLS=""' >> ~/.bashrc
source ~/.bashrc
echo "Disable debuginfod"


echo
echo "=========================================="
echo "Ubuntu C/C++ environment setup finished!"
echo "=========================================="



