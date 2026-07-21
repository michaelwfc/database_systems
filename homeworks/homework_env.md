# Install DuckDB in WSL (Recommended)
Since you're going through CMU 15-445, BusTub, DuckDB, and database internals, I'd standardize on WSL Ubuntu for all database tooling.

```
Windows
│
├── VS Code
│
└── WSL Ubuntu
      │
      ├── gcc
      ├── cmake
      ├── gdb
      ├── git
      ├── duckdb (Linux)
      ├── sqlite3
      ├── postgresql-client
      ├── python
      └── make
```
This gives you an environment that matches Linux-based documentation and avoids Windows-specific issues with PATH, winget, and shell behavior.


## 1. Install WSL Ubuntu-24
```bash
michael@DESKTOP-2KLOSPO MINGW64 /e/projects/database_systems (master)
$ wsl -d Ubuntu-24.04
# wsl: 检测到 localhost 代理配置，但未镜像到 WSL。NAT 模式下的 WSL 不支持 localhost 代理。
# Welcome to Ubuntu 24.04.3 LTS (GNU/Linux 5.15.167.4-microsoft-standard-WSL2 x86_64)

#  * Documentation:  https://help.ubuntu.com
#  * Management:     https://landscape.canonical.com
#  * Support:        https://ubuntu.com/pro

#  System information as of Sun Jul 19 08:03:43 CST 2026

#   System load:  0.0                 Processes:             51
#   Usage of /:   0.5% of 1006.85GB   Users logged in:       0
#   Memory usage: 3%                  IPv4 address for eth0: 172.23.174.93
#   Swap usage:   0%

#  * Strictly confined Kubernetes makes edge and IoT secure. Learn how MicroK8s
#    just raised the bar for easy, resilient and secure K8s cluster deployment.

#    https://ubuntu.com/engage/secure-kubernetes-at-the-edge

# This message is shown once a day. To disable it please create the
# /home/michael/.hushlogin file.

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ wsl -l -v
# Command 'wsl' not found, but can be installed with:
# sudo apt install wsl

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ lsb_release -a
# No LSB modules are available.
# Distributor ID: Ubuntu
# Description:    Ubuntu 24.04.3 LTS
# Release:        24.04
# Codename:       noble

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ g++ --version
# g++ (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
# Copyright (C) 2023 Free Software Foundation, Inc.
# This is free software; see the source for copying conditions.  There is NO
# warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ whereis g++
# g++: /usr/bin/g++ /usr/share/man/man1/g++.1.gz

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ gdb --version
# GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1) 15.0.50.20240403-git
# Copyright (C) 2024 Free Software Foundation, Inc.
# License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
# This is free software: you are free to change and redistribute it.
# There is NO WARRANTY, to the extent permitted by law.
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ whereis gdb
# gdb: /usr/bin/gdb /etc/gdb /usr/include/gdb /usr/share/gdb /usr/share/man/man1/gdb.1.gz
```


## 2. Install DuckDB in WSL

```bash

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ curl https://install.duckdb.org | sh
#   % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
#                                  Dload  Upload   Total   Spent    Left  Speed
# 100  4163  100  4163    0     0   5486      0 --:--:-- --:--:-- --:--:--  5484

# *** DuckDB Linux/MacOS installation script, version 1.5.4 ***


#          .;odxdl,            
#        .xXXXXXXXXKc          
#        0XXXXXXXXXXXd  cooo:  
#       ,XXXXXXXXXXXXK  OXXXXd 
#        0XXXXXXXXXXXo  cooo:  
#        .xXXXXXXXXKc          
#          .;odxdl,  


######################################################################## 100.0%

# Successfully installed DuckDB 1.5.4 to /home/michael/.duckdb/cli/1.5.4/duckdb
# Updated symlink from /home/michael/.duckdb/cli/latest/duckdb to
#                      /home/michael/.duckdb/cli/1.5.4/duckdb

# Hint: Append the following line to your shell profile:
# export PATH="/home/michael/.duckdb/cli/latest":$PATH

# To launch DuckDB 1.5.4 now, type
# /home/michael/.duckdb/cli/latest/duckdb


michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ duckdb --version
Command 'duckdb' not found, but can be installed with:
sudo snap install duckdb

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ which duckdb
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ 

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ /home/michael/.duckdb/cli/latest/duckdb --version
# v1.5.4 (Variegata) 08e34c447b

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ echo $SHELL
# /bin/bash

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ ll ~|grep bash
# -rw------- 1 michael michael 3966 Aug 10  2025 .bash_history
# -rw-r--r-- 1 michael michael  220 Aug  8  2025 .bash_logout
# -rw-r--r-- 1 michael michael 3771 Aug  8  2025 .bashrc
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ vim ~/.bashrc 
# export PATH="$HOME/.duckdb/cli/latest:$PATH"
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ source  ~/.bashrc 
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ duckdb --version
# v1.5.4 (Variegata) 08e34c447b
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ which duckdb
# /home/michael/.duckdb/cli/latest/duckdb

michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ cd data/
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems/data$ ls
# lahman-cmudb2025.db  lahman-cmudb2025.db.gz
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems/data$ duckdb lahman-cmudb2025.db
# DuckDB v1.5.4 (Variegata)
# Enter ".help" for usage hints.

# commands starting with . are DuckDB CLI meta-commands, not SQL.
lahman-cmudb2025 D .help
# .bail on|off                             Stop after hitting an error.  Default OFF
# .binary on|off                           Turn binary output on or off.  Default OFF
# .cd DIRECTORY                            Change the working directory to DIRECTORY
# .changes on|off                          Show number of rows changed by SQL
# .columns                                 Column-wise rendering of query results
# .decimal_sep SEP                         Sets the decimal separator used when rendering numbers. Only for duckbox mode.
# .databases                               List names and files of attached databases
# .dump ?TABLE?                            Render database content as SQL
# .display_colors [bold|underline]         Display all terminal colors and their names
# .echo on|off                             Turn command echo on or off
# .edit                                    Opens an external text editor to edit a query.
# .excel                                   Display the output of next command in spreadsheet
# .exit ?CODE?                             Exit this program with return-code CODE
# .headers on|off                          Turn display of headers on or off
# .help ?-all? ?PATTERN?                   Show help text for PATTERN
# .highlight on|off                        Toggle syntax highlighting in the shell on/off
# .highlight_colors OPTIONS                Configure highlighting colors
# .highlight_errors on|off                 Turn highlighting of errors on or off
# .highlight_mode mixed|dark|light         Toggle the highlight mode to dark or light mode
# .highlight_results on|off                Turn highlighting of results on or off
# .import FILE TABLE                       Import data from FILE into TABLE
# .indexes ?TABLE?                         Show names of indexes
# .last                                    Render the last result without truncating
# .large_number_rendering MODE             Toggle readable rendering of large numbers (duckbox only)
# .log FILE|off                            Turn logging on or off.  FILE can be stderr/stdout
# .maxrows COUNT                           Sets the maximum number of rows for display (default: 40). Only for duckbox mode.
# .maxwidth COUNT                          Sets the maximum width in characters. 0 defaults to terminal width. Only for duckbox mode.
# .mode MODE ?TABLE?                       Set output mode
# .multiline                               Sets the render mode to multi-line
# .nullvalue STRING                        Use STRING in place of NULL values
# .open ?OPTIONS? ?FILE?                   Close existing database and reopen FILE
# .once ?FILE?                             Output for the next SQL command only to FILE
# .output ?FILE?                           Send output to FILE or stdout if FILE is omitted
# .pager OPTIONS                           Control pager usage for output
# .print STRING...                         Print literal STRING
# .progress_bar OPTIONS                    Configure the progress bar display
# .prompt MAIN CONTINUE                    Replace the standard prompts
# .quit                                    Exit this program
# .read FILE                               Read input from FILE
# .read_line_version linenoise|fallback    Sets the library used for processing interactive input
# .render_completion on|off                Toggle displaying of completion prompts in the shell on/off
# .render_errors on|off                    Toggle rendering of errors in the shell on/off
# .rows                                    Row-wise rendering of query results (default)
# .safe_mode                               Enable safe-mode
# .separator COL ?ROW?                     Change the column and row separators
# .schema ?PATTERN?                        Show the CREATE statements matching PATTERN
# .shell CMD ARGS...                       Run CMD ARGS... in a system shell
# .show                                    Show the current values for various settings
# .singleline                              Sets the render mode to single-line
# .startup_text none|version|all           Start-up text to display. Set this as the first line in .duckdbrc
# .system CMD ARGS...                      Run CMD ARGS... in a system shell
# .tables ?TABLE?                          List names of tables matching LIKE pattern TABLE
# .thousand_sep SEP                        Sets the thousand separator used when rendering numbers. Only for duckbox mode.
# .timer on|off                            Turn SQL timer on or off
# .ui_command [command]                    Set the UI command
# .version                                 Show the version
# .width NUM1 NUM2 ...                     Set minimum column widths for columnar output

# Run .help --all for extended information
# Run .help shortcuts for keyboard shortcuts
lahman-cmudb2025 D 



```