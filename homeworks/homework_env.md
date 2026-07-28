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
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems$ tar zxf lahman-cmudb2025.db.gz

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


# DuckDB
## 1.Table mode vs  duckbox mode

lahman-cmudb2025 D  DESCRIBE teams
```sql
⚪◗ DESCRIBE teams
┌─────────────┬─────────────┬─────────┬─────────┬─────────┬─────────┐
│ column_name │ column_type │  null   │   key   │ default │  extra  │
│   varchar   │   varchar   │ varchar │ varchar │ varchar │ varchar │
├─────────────┼─────────────┼─────────┼─────────┼─────────┼─────────┤
│ yearID      │ SMALLINT    │ NO      │ PRI     │ NULL    │ NULL    │
│ lgID        │ VARCHAR     │ NO      │ PRI     │ NULL    │ NULL    │
│ teamID      │ VARCHAR     │ NO      │ PRI     │ NULL    │ NULL    │
│ franchID    │ VARCHAR     │ YES     │ NULL    │ NULL    │ NULL    │
│ divID       │ VARCHAR     │ YES     │ NULL    │ NULL    │ NULL    │
│ teamRank    │ SMALLINT    │ YES     │ NULL    │ NULL    │ NULL    │
│ name        │ VARCHAR     │ YES     │ NULL    │ NULL    │ NULL    │
│ attendance  │ INTEGER     │ YES     │ NULL    │ NULL    │ NULL    │
└─────────────┴─────────────┴─────────┴─────────┴─────────┴─────────┘
```

why the same command change like this:
lahman-cmudb2025 D DESCRIBE teams;
┌───────────────────────────────────────────┐
│                   teams                   │
│                                           │
│ yearID     smallint not null              │
│ lgID       varchar  not null default NULL │
│ teamID     varchar  not null              │
│ franchID   varchar  default NULL          │
│ divID      varchar  default NULL          │
│ teamRank   smallint default NULL          │
│ name       varchar  default NULL          │
│ attendance integer  default NULL          │
└───────────────────────────────────────────┘

```



Good observation! This behavior is actually due to **different output modes in the DuckDB CLI**, not because `DESCRIBE` behaves differently.

The SQL statement is the same:

```sql
DESCRIBE teams;
```

What changed is **how the DuckDB CLI renders the result set**.

---

### `DESCRIBE` returns a normal SQL result

Many beginners think `DESCRIBE` is a special command.

In DuckDB, it is actually a **SQL statement** that returns a table.

Conceptually, it returns something like:

| column_name | column_type | null | key | default | extra |
| ----------- | ----------- | ---- | --- | ------- | ----- |
| yearID      | SMALLINT    | NO   | PRI | NULL    |       |
| lgID        | VARCHAR     | NO   | PRI | NULL    |       |
| ...         | ...         | ...  | ... | ...     | ...   |

Since it is just a normal query result, **the CLI can display it in different formats**.

---

### Why did it become a "single box"?

Your second output is:

```text
┌───────────────────────────────────────────┐
│                   teams                   │
│                                           │
│ yearID     smallint not null              │
│ lgID       varchar  not null default NULL │
│ teamID     varchar  not null              │
│ ...                                     │
└───────────────────────────────────────────┘
```

This is **DuckDB's "duckbox" output mode**.

Instead of showing every row as a table, it displays **one record as a vertical card**.

---

#### Output Mode Comparison

##### Table Mode

```text
column_name   column_type   null   key
---------------------------------------
yearID        SMALLINT      NO     PRI
lgID          VARCHAR       NO     PRI
teamID        VARCHAR       NO     PRI
```

Easy to read when there are many rows.

---

##### Duckbox Mode

```text
teams
-----------------------------
yearID      SMALLINT
lgID        VARCHAR
teamID      VARCHAR
...
```

Easy to read when describing a single object.

---

### Check your current mode

Run:

```text
.mode
```

It will tell you the current output mode.

---

### List all available modes

Run:

```text
.help mode
```

or

```text
.mode ?
```

Depending on your DuckDB version.

Typical modes include:

```text
duckbox
table
csv
json
markdown
line
ascii
```

---

### Change back to table mode

If you're currently in `duckbox` mode:

```text
.mode table
```

Now run:

```sql
DESCRIBE teams;
```

You'll get

```text
column_name
column_type
null
key
...
```

again.

---

### Change to duckbox

```text
.mode duckbox
```

Then

```sql
DESCRIBE teams;
```

becomes

```text
teams
---------------------
yearID ...
lgID ...
```

---

### Why does DuckDB have duckbox?

Suppose you execute

```sql
SELECT *
FROM people
LIMIT 1;
```

Table mode:

| ID | playerID | birthYear | ... |
| -- | -------- | --------- | --- |

If there are **40 columns**, it becomes unreadable.

Duckbox mode instead shows:

```text
people

ID          1
playerID    aaronha01
birthYear   1934
birthCity   Mobile
...
```

Much easier for wide tables.

---

### Internally Nothing Changed

The execution pipeline is identical:

```text
SQL

↓

Parser

↓

AST

↓

Binder

↓

Logical Plan

↓

Physical Plan

↓

Result Set
```

Only the **last step** changes:

```text
Result Set
      │
      ▼
CLI Renderer
      │
      ├── table
      ├── duckbox
      ├── csv
      ├── markdown
      └── json
```

So the SQL engine produced the **same result**; the CLI just rendered it differently.

---

### A Small Correction

One detail in your screenshots caught my eye.

The first output includes:

```text
key
----
PRI
```

The second output does **not** show the primary key information.

This suggests something beyond just the output mode:

* The **first screenshot** is the output of the SQL statement `DESCRIBE teams;`, which returns metadata including the `key` column.
* The **second screenshot** is the output of the **CLI meta-command** `.schema teams` (or an equivalent schema display), even though you typed `DESCRIBE teams;`.

These are **different commands** with different purposes:

| Command                    | Type             | Shows                                                               |
| -------------------------- | ---------------- | ------------------------------------------------------------------- |
| `DESCRIBE teams;`          | SQL              | Column metadata (`column_name`, `column_type`, `null`, `key`, etc.) |
| `.schema teams`            | CLI meta-command | The table definition (or a formatted schema summary)                |
| `SHOW CREATE TABLE teams;` | SQL              | The complete `CREATE TABLE` statement                               |

If `DESCRIBE teams;` is unexpectedly producing the schema-style output, I'd like to verify your environment. Could you run:

```sql
SELECT version();
```

and

```text
.mode
```

The DuckDB CLI has evolved over recent releases, and version **1.5.x** introduced some changes to the default display behavior. With the version and current output mode, we can explain exactly why you're seeing that rendering.



## 2. Conflicting lock


lahman-cmudb2025 D SELECT * FROM awardsplayers LIMIT 5
                   
[1]+  Stopped                 duckdb lahman-cmudb2025.db
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems/data$ duckdb lahman-cmudb2025.db
IO Error:
Could not set lock on file "/mnt/e/projects/database_systems/data/lahman-cmudb2025.db": Conflicting lock is held in /home/michael/.duckdb/cli/1.5.4/duckdb (PID 999). See also https://duckdb.org/docs/stable/connect/concurrency
michael@DESKTOP-2KLOSPO:/mnt/e/projects/database_systems/data$ 

how can I start  duckdb lahman-cmudb2025.db?


This is a **Linux job control** issue combined with **DuckDB's file locking**, not a DuckDB bug.

Let's analyze what happened.

---

### Step 1. What happened?

You executed

```bash
SELECT * FROM awardsplayers LIMIT 5
```

Then saw

```bash
[1]+ Stopped duckdb lahman-cmudb2025.db
```

This line is the important one.

It means

> **Your DuckDB process was not terminated. It was suspended.**

Linux shells (bash, zsh, etc.) have three states for jobs:

```
Running
Stopped
Terminated
```

Your DuckDB process is currently

```
Stopped
```

which means

* it is still alive
* it still owns the database file
* it still holds the file lock

---

### Step 2. Why can't you reopen the database?

When you run

```bash
duckdb lahman-cmudb2025.db
```

DuckDB tries to obtain an exclusive lock.

But the old process (PID 999) still exists.

So DuckDB reports

```
Conflicting lock is held in

PID 999
```

Exactly as expected.

---

### Step 3. Check your jobs

Run

```bash
jobs
```

You'll probably see something like

```bash
[1]+ Stopped duckdb lahman-cmudb2025.db
```

---

### Step 4. Option 1 (Recommended): Resume the suspended process

Simply bring it back:

```bash
fg %1
```

or

```bash
fg
```

You'll return to the previous DuckDB session.

No need to reopen anything.

---

### Step 5. Option 2: Kill it

If you don't care about that session

```bash
kill %1
```

or

```bash
kill 999
```

Afterwards verify

```bash
jobs
```

returns

```text
(no jobs)
```

Now you can reopen

```bash
duckdb lahman-cmudb2025.db
```

---

### Step 6. If kill doesn't work

Sometimes the process ignores SIGTERM.

Then

```bash
kill -9 999
```

or

```bash
pkill duckdb
```

---

### Step 7. Verify the process disappeared

Run

```bash
ps aux | grep duckdb
```

or

```bash
pgrep duckdb
```

No output means it's gone.

---

### Why was DuckDB stopped?

The interesting question is:

Why did

```sql
SELECT * FROM awardsplayers LIMIT 5;
```

cause

```
Stopped
```

instead of printing five rows?

Normally it should not.

This usually happens because the shell sent **SIGTSTP** (terminal stop signal).

Common causes include:

* accidentally pressing **Ctrl+Z** (most likely)
* terminal emulator suspending the process
* WSL terminal issue
* VS Code integrated terminal sending Ctrl+Z

Ctrl+Z does exactly this:

```
Running
      │
Ctrl+Z
      ▼
Stopped
```

and bash prints

```
[1]+ Stopped duckdb ...
```

---

### Job Control Summary

```
jobs
```

List suspended/background jobs.

```
fg
```

Bring a stopped job back to the foreground.

```
bg
```

Resume it in the background.

```
kill %1
```

Terminate job 1.

```
exit
```

Quit DuckDB normally.

---

#### My guess

Based on your output, I'm **95% confident** that you accidentally pressed **Ctrl+Z** while using DuckDB. That suspended the process, leaving it holding the database lock.

Before starting a new DuckDB instance, run:

```bash
jobs
```

If you paste the output of:

```bash
jobs
ps -ef | grep duckdb
```

I can tell you the safest way to recover your session.
