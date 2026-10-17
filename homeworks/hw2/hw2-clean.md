CARNEGIE MELLON UNIVERSITY COMPUTER SCIENCE DEPARTMENT 

15-445/645 – DATABASE SYSTEMS (FALL 2025) PROF. ANDY PAVLO 

Homework #2 (by Will) 

Due: **Sunday Sept 21, 2025 @ 11:59pm** 

## **IMPORTANT:** 

- Enter all of your answers into **Gradescope by 11:59pm on Sunday Sept 21, 2025.** 

- **Plagiarism** : Homework may be discussed with other students, but all homework is to be completed **individually** . 

For your information: 

- Graded out of **100** points; **3** questions total 

- Rough time estimate: _≈_ 4-6 hours (1-1.5 hours for each question) 

- Each part is all or nothing. There is no partial credit. 

_Revision_ : 2025/09/10 23:30 

|Question|Points|Score|
|---|---|---|
|Slotted Pages and Log-Structured|30||
|Storage Models|35||
|Database Compression|35||
|Total:|100||




# **Question 1: Slotted Pages and Log-Structured  [30 points]** 

- (a) **[10 points]** Which problems are associated with the _log-structured storage_ in a database system? Select all that apply. 


   - [x] Write Amplification 

   - [ ] Increased Random Writes 

   - [ ] Increased Random Reads 

   - [ ] Fragmentation 

   - [ ] None of the above 


- (b) **[10 points]** Which problems are associated with the _slotted-page storage_ in a database system? Select all that apply. 

   - [ ] Write Amplification 

   - [ ] Increased Random Reads -> [X]

   - [ ] Increased Random Writes -> [X]

   - [x] Fragmentation 

   - [ ] None of the above 

Increased Random Writes — generally incorrect

A slotted-page storage engine can perform in-place updates.

For example:
```sql
UPDATE P
SET HR = 50
WHERE player_id = 123;
```
If the tuple fits in the page, the DBMS can modify the existing tuple in place.

Before:
Page 42:
Tuple 5 → HR = 40

After:
Page 42:
Tuple 5 → HR = 50

This avoids the repeated rewriting of data associated with LSM compaction.

However, in-place updates can involve random writes when the workload accesses scattered pages. That's a workload-dependent I/O pattern, not a fundamental requirement of slotted pages.

**Solution**: The Slotted-Page Design often leads to fragmentation, as deletion of tuples can leave gaps in the pages, making them not fully utilized.
Since tuples can be stored across separate pages, it may increase the amount of random I/O that the DBMS has to incur when both reading data and when writing out dirty
pages.




- (c) **[10 points]** You are asked to compare _log-structured storage_ to _slotted-page storage_ for a new system. Ignore any indexes and overhead from metadata. Select all true statements. 

   - [x] Both support variable length tuples. 

   - [ ] Log-structured storage requires less disk space. 

   - [X] For an append-only workload, both achieve comparable performance. 

   - [ ] Slotted-page requires storing schema metadata with the tuple. 

   - [x] After lots of insert/update/deletes, only log-structured benefits from maintenance.  -> [ ]

   - [ ] None of the above are true. 


Slotted-page requires storing schema metadata with the tuple — incorrect

This is the most important correction.

You selected this, but it is not generally true.

A slotted page needs metadata to locate tuples, such as:
```
Slot 0 → offset, length
Slot 1 → offset, length
```
But this is not the same as storing schema metadata with each tuple.

Schema metadata

Schema metadata describes the structure of the relation:
```sql
P(
    player_id: INTEGER,
    team_id: INTEGER,
    G_all: INTEGER,
    HR: INTEGER
)
```
This is typically maintained in the DBMS catalog.

Tuple metadata

Tuple metadata describes the physical representation of a tuple:

Tuple:
┌──────────────────────────────┐
│ Tuple header                 │
│ player_id                   │
│ team_id                     │
│ G_all                       │
│ HR                          │
└──────────────────────────────┘

A variable-length tuple may need offsets, lengths, null bitmap, and other physical metadata.

But the tuple does not necessarily need to carry the full schema.

Why this matters? 

You can have a schema:  P(player_id, team_id, G_all, HR) stored once in the catalog.

Every tuple can then be interpreted using that schema.

Correction: Uncheck this option unless your lecture explicitly defines "schema metadata" differently.


5. After lots of insert/update/deletes, only log-structured benefits from maintenance — incorrect

This is the second major mistake.

Both storage models can benefit from maintenance.

Log-structured storage

Maintenance:
```
Compaction
    ↓
Remove obsolete versions
    ↓
Remove tombstones when safe
    ↓
Reduce space amplification
    ↓
Improve reads
```

Slotted-page storage

Maintenance:
```
Page compaction / vacuum
    ↓
Remove dead tuples
    ↓
Recover free space
    ↓
Reduce fragmentation
```

Example:

Before:
[Tuple A][Hole][Tuple B][Hole][Tuple C]

After:
[Tuple A][Tuple B][Tuple C][Free Space]

This is a form of maintenance.

In PostgreSQL, for example, VACUUM is used to reclaim space from dead tuples and support storage maintenance.

So both models can benefit from maintenance after many inserts, updates, and deletes.

Correction: Uncheck this option.


**Solution**: In absence of indexes + metadata, then slotted-page for append-only workload becomes the log-structured storage architecture.
After lots of inserts/updates/deletes, slotted-page may also benefit from maintenance to
reclaim any empty space or compact partially empty pages.


# **Question 2: Storage Models  [35 points]** 

Consider a database with a single table P(player_id, team_id, G_all, HR), where player_id is the _primary key_ , and all attributes are the same fixed width. Suppose P has 20,000 tuples that fit into 200 pages. You should ignore any additional storage overhead for the table (e.g., page headers, tuple headers). Additionally, you should make the following assumptions: 

   - The DBMS does _not_ have any additional meta-data. 

   - P does _not_ have any indexes (including for primary key player_id). 

   - None of P’s pages are already in memory. The DBMS can store an infinite number of pages in memory. 

   - Content-wise, the tuples of P will always make each query run the longest possible and do the most page accesses. 

   - The tuples of P can be in any order ( keep this in mind when computing _minimum_ versus _maximum_ number of pages that the DBMS will potentially have to read and think of all possible orderings) 

## (a) Consider the following query: 
```
SELECT MAX(HR) FROM P
WHERE team_id == 15445 ;
```

i. **[5 points]** Suppose the DBMS uses the N-ary storage model (NSM). How many pages will the DBMS potentially have to read from disk to answer this query? 
  _Be sure to keep in mind the assumption about the contents of_ P _._ 

   - [ ] 1-50 
   - [ ] 51-100 
   - [ ] 101-150 
   - [ ] 151-200 -> [ ]
   - [X] ≥  201
   - [ ] Not possible to determine 



### i-Solution :

**Solution**: 200 pages. To find HR and team id for all tuples, all pages must be
accessed.



#### Q: How can I know the pages for specific team_id?
- How do we locate a page containing a particular team_id?
- How do we know whether we have found the maximum HR?


1. Can the DBMS directly locate team_id = 15445?

The important assumption is:
P does not have any indexes, including an index on team_id.
Therefore, the DBMS does not have a B+ Tree or hash index telling it which page contains team_id = 15445.

Imagine the pages look like this:
```
Page 0
┌─────────────────────────┐
│ player 1, team 100      │
│ player 2, team 200      │
│ player 3, team 15445    │
│ ...                     │
└─────────────────────────┘

Page 1
┌─────────────────────────┐
│ player 4, team 300      │
│ player 5, team 15445    │
│ ...                     │
└─────────────────────────┘

Page 2
┌─────────────────────────┐
│ player 6, team 500      │
│ ...                     │
└─────────────────────────┘
```

The DBMS cannot know that Page 0 or Page 1 contains the target team without reading the pages.

And because tuples can be in any order, matching tuples might be distributed across the entire table.


2. What does NSM change?

NSM = N-ary Storage Model, also called row-oriented storage.

Each page contains complete tuples:

```
Page 0
┌───────────────────────────────┐
│ player_id │ team_id │ G │ HR  │
├───────────┼─────────┼───┼─────┤
│ 1         │ 100     │...│ 20  │
│ 2         │ 15445   │...│ 35  │
│ 3         │ 200     │...│ 10  │
└───────────────────────────────┘
```

For your query,The DBMS needs to:
- Read a page.
- Check each tuple's team_id.
- If team_id = 15445, compare its HR with the current maximum.
- Continue to the next page.

Conceptually:

Page
  │
  ├── Read player_id
  ├── Read team_id
  ├── Read G_all
  └── Read HR

Because NSM stores the entire tuple together, the DBMS can access both team_id and HR from the same page.

3. Why can't the DBMS stop after finding one matching tuple?

Suppose it finds:
```
Page 0:
team_id = 15445, HR = 30
```

Current maximum:

MAX(HR) = 30

Can it stop?  No.

There could be another player on Page 150:
```
Page 150:
team_id = 15445, HR = 60
```
The answer should be: `MAX(HR) = 60`

The DBMS needs to check the remaining pages to know that no qualifying tuple has a larger HR.

This is especially important because:
- No index exists.
- No ordering by team_id or HR is guaranteed.
- No metadata gives the maximum HR for a team.

4. The important distinction: Minimum vs. maximum page accesses

Your homework explicitly says:

"The tuples of P can be in any order."

This means you should think about possible physical arrangements of the tuples


Scenario A: Matching tuples are concentrated early

Imagine all players with team_id = 15445 happen to be in Page 0.

```
Page 0  → target team
Page 1
Page 2
...
Page 199
```

Could the DBMS stop after Page 0?

No, because it still needs to know whether another matching tuple with a larger HR exists later.


Scenario B: A matching tuple appears on the last page
```
Page 0
Page 1
Page 2
...
Page 199 → team_id = 15445
```

The DBMS must reach that page to discover that tuple.

And even if the maximum is found early, the DBMS cannot prove it is the maximum until it has checked all possible qualifying tuples.


5. The central insight for this question

Your question is: How can I know the pages for a specific team_id?

Under these assumptions, you cannot know the specific pages without reading them, because there is no index or metadata.

The DBMS performs a sequential scan:
```
P
│
├── Page 0
│     └── Check team_id, HR
│
├── Page 1
│     └── Check team_id, HR
│
├── Page 2
│     └── Check team_id, HR
│
├── ...
│
└── Last page
      └── Check team_id, HR
```

For a MAX() query, the fact that the matching tuples might be on one page does not let the DBMS stop early.

How to approach the checkbox

Look at the wording:

"How many pages will the DBMS potentially have to read from disk?"

The word potentially asks about the possible worst-case number of page reads, given the assumptions.

Use this reasoning: 
- How many pages does the table have?
- Can the DBMS identify the pages containing team_id = 15445 without reading them?
- Does MAX(HR) allow early termination?
- What happens in the worst-case ordering of tuples?

Once you answer those four questions, you should be able to choose the correct range.

One extra database systems insight
This is exactly why indexes matter.

With an index on team_id:
```
Index(team_id)
      │
      ↓
team_id = 15445
      │
      ↓
RID / page references
      │
      ↓
Read relevant pages
      │
      ↓
Compute MAX(HR)
```

Without an index:
```
team_id = 15445
      │
      ↓
Sequential scan
      │
      ↓
Read all 200 pages
      │
      ↓
Check team_id
      │
      ↓
Compute MAX(HR)
```

The DBMS does not magically know where a value lives. The storage layout and access paths determine what it must read.

The index provides an **access path** to the matching tuples.

Without it, the DBMS does not know which pages contain team_id = 15445.

With it, the DBMS can find candidate tuple locations.

A B+ Tree index on team_id stores ordered keys and tuple references in disk-friendly pages. It allows the DBMS to locate tuples matching team_id = 15445 without scanning every table page.




----


- ii. **[5 points]** Suppose the DBMS uses the decomposition storage model (DSM) with implicit offsets. How many pages will the DBMS potentially have to read from disk to answer this query? 

_Be sure to keep in mind the assumption about the contents of  P_. 
- [ ] 1-50 
- [x] 51-100
- [ ] 101-150
- [ ] 151-200
- [ ] ≥ 201 
- [ ] Not possible to determine 

### ii-solution

My answer is 1-50 page, becuase for DSM , each column will be store together to pages, so we can select pages for team_id and HR, then filter the team_id and find the max(HR)

This is correct:

- DSM stores each attribute separately.
- The query only needs team_id and HR.
- player_id and G_all are irrelevant.
- The DBMS can use implicit offsets to match a team_id value with its corresponding HR value.

The issue is `how many pages each column occupies?`.



2. Calculate the number of values per page

The table has:
- 20,000 tuples.
- 200 pages.
- 4 attributes per tuple.
- All attributes have the same fixed width.

Let each attribute occupy w bytes.

```
200 pages store 20,000 tuples, so:  Tuples per page= 20,000/200= 100 
Each tuple contains 4 values:  Tuple size=4w 
Therefore,one page stores: : 100 * 4  = 400 attribute values


```

3. how many pages each column occupies?
In DSM, each attribute has its own storage.
for each attribute
total size = 20,000 tuples  -> 20, 000 attribute values

num of pages for each attribute = 20, 000 / 400 =50 pages

4. How the query executes?
Step 1 — Read team_id

The DBMS must inspect the team_id column to determine which tuples match.

team_id column
┌──────────────┐
│ Page 0       │
│ Page 1       │
│ ...          │
│ Page 49      │
└──────────────┘

Because there is no index or metadata, the DBMS may need to read all 50 pages.

Step 2 — Read HR

For every matching tuple, the DBMS needs its HR value.

With implicit offsets, the values line up by tuple position:
```
team_id column       HR column

Offset 0             Offset 0
Offset 1             Offset 1
Offset 2             Offset 2
   ...                   ...
Offset 19999         Offset 19999
```
For example:


	
The DBMS checks: team_id[offset] == 15445

and if true, considers: HR[offset] to compute the maximum.

1. The crucial part: Why do we potentially need all 50 HR pages?

This is where your answer needs correction.

You might think:

"If the matching team_id values are found in a few pages, I only need a few HR pages."

But the homework explicitly says the tuples can be in any order, and the contents make the query run the longest possible.

Imagine the qualifying players are distributed throughout the table:

team_id column
```
Page 0   → matching team
Page 1   → no matching team
Page 2   → matching team
...
Page 49  → matching team
```

Since the team_id column is scanned in offset order, the matching tuples may correspond to HR values located in every HR page.

For example:
```
team_id Page 0 → HR Page 0
team_id Page 1 → HR Page 1
team_id Page 2 → HR Page 2
...
team_id Page 49 → HR Page 49
```

So the worst-case page accesses are:

50+50=100

The DBMS does not need player_id or G_all.

6. Code review of your answer
Needs correction

Your selected range: 1–50

Correct: column selection

You correctly identified team_id and HR as the required columns.

Incorrect: total page count

The two required columns can each occupy 50 pages. The worst-case total is not just one column's 50 pages.


----


## (b) Now consider the following query: 

```sql
SELECT HR, team id FROM P
WHERE player_id = 1 OR player_id = 999
```

- i. Suppose the DBMS uses the N-ary storage model (NSM). 

  _α_ ) **[5 points]** What is the _minimum_ number of pages that the DBMS will potentially have to read from disk to answer this query? 
   - [X] 1 
   - [ ] 2-3 
   - [ ] 4-6 
   - [ ] 7-9 
   - [ ] 10-100 
   - [ ] 101-200 
   - [ ] ≥ 201 
   - [ ] Not possible to determine 

  _β_ ) **[5 points]** What is the _maximum_ number of pages that the DBMS will potentially have to read from disk to answer this query? 
  - [ ] 1 
  - [ ] 2-3 
  - [ ] 4-6 
  - [ ] 7-9 
  - [ ] 10-100 
  - [X] 101-200 
  - [ ] >=201 
  - [ ] Not possible to determine 

- ii. Suppose the DBMS uses the decomposition storage model (DSM) with implicit offsets. 
  _α_ ) **[5 points]** What is the _minimum_ number of pages that the DBMS will potentially have to read from disk to answer this query? 
  - [X] 1-3 [ ] 4-6 [ ] 7-9 [ ] 10-100 [ ] 101-200  [ ] ≥ 201 [ ] Not possible to determine 

  _β_ ) **[5 points]** What is the _maximum_ number of pages that the DBMS will potentially have to read from disk to answer this query? 
  - [ ] 1-20 [ ] 21-40 [X] 41-60 [ ] 61-80 [] 81-100 [ ]≥101 [ ] Not possible to determine 


### Solution:
Absolutely. There are **two separate ideas** here, and understanding them is important for DSM.

Also, I want to correct one subtle point from my previous answer: saying the maximum is automatically `50 + 50 + 50 = 150` is **too simplistic**. With **implicit offsets**, once you find the matching tuple positions in `player_id`, you only need the specific `team_id` and `HR` pages containing those positions—not necessarily all 50 pages of each output column. Let's derive it carefully.

---

#### 1. What does "implicit offsets" mean?

Suppose the original table is:

```text
P
+-----------+---------+------+----+
| player_id | team_id | G_all| HR |
+-----------+---------+------+----+
|     1     |   10    | 100  | 20 |
|     2     |   20    |  80  | 15 |
|     3     |   10    |  90  | 18 |
|     4     |   30    | 120  | 25 |
|    ...    |   ...   | ...  |... |
+-----------+---------+------+----+
```

In **NSM**, these attributes are physically stored together:

```text
Page 0:

tuple 0: [1,  10, 100, 20]
tuple 1: [2,  20,  80, 15]
tuple 2: [3,  10,  90, 18]
tuple 3: [4,  30, 120, 25]
```

In **DSM**, we split the columns:

```text
player_id column:
position 0 → 1
position 1 → 2
position 2 → 3
position 3 → 4
...

team_id column:
position 0 → 10
position 1 → 20
position 2 → 10
position 3 → 30
...

G_all column:
position 0 → 100
position 1 → 80
position 2 → 90
position 3 → 120
...

HR column:
position 0 → 20
position 1 → 15
position 2 → 18
position 3 → 25
...
```

Notice something important:

```text
             same logical tuple
                    ↓

position       0       1       2       3
             ─────────────────────────────
player_id      1       2       3       4
team_id       10      20      10      30
G_all         100     80      90      120
HR             20     15      18      25
```

There is **no explicit tuple ID stored next to every value**.

The **position itself is the identity**.

That's what we mean by **implicit offsets**.

---

#### 2. Why is it called "implicit"?

Suppose we find:

```text
player_id = 999
```

and discover that it is the **10,532nd value** in the `player_id` column.

We don't need something like:

```text
player_id = 999
tuple_id = 10532
```

Instead, the physical position tells us:

```text
player_id[10532]
```

Therefore:

```text
team_id[10532]
HR[10532]
```

belong to the **same logical tuple**.

So:

```text
player_id[10532] = 999
        │
        │ same offset
        ↓
team_id[10532] = 15445
        │
        │ same offset
        ↓
HR[10532] = 37
```

That's the key idea.

##### Compare this with explicit offsets

An alternative DSM design could store something like:

```text
player_id:
[value=999, tuple_id=10532]
```

Then the tuple ID explicitly tells you where the other values are.

With **implicit offsets**, you don't store that tuple ID because:

> **the position in each column is assumed to correspond across all columns.**

This saves storage and metadata.

---

#### 3. Now let's understand the page calculation

We know:

```text
20,000 tuples
200 pages in NSM
100 tuples/page
```

Each DSM column therefore contains:

```text
20,000 values
```

If one page can hold 100 values:

```text
20,000 / 100 = 200 pages
```

Wait — this is exactly where we need to be careful.

The original setup says **20,000 tuples = 200 pages**, with **4 attributes per tuple**.

So one NSM page contains:

```text
100 tuples × 4 attributes
= 400 attribute values
```

For DSM, each page contains values from **one attribute only**.

Therefore each DSM column has:

```text
20,000 values / 400 values per page
= 50 pages
```

So:

```text
player_id → 50 pages
team_id   → 50 pages
G_all     → 50 pages
HR        → 50 pages
```

This is the critical calculation.

---

#### 4. Why does `player_id` require all 50 pages?

The query says:

```sql
WHERE player_id = 1 OR player_id = 999
```

There is no index.

So the DBMS needs to find those values by scanning `player_id`.

Worst case:

```text
player_id column

Page 0
Page 1
Page 2
...
Page 48
Page 49
```

The two values could be anywhere.

For example:

```text
Page 7:
    player_id = 1

Page 49:
    player_id = 999
```

But even if the DBMS finds `player_id = 1` on page 7, it **cannot stop**.

Why?

Because `player_id = 999` could still be on page 49.

Therefore the DBMS potentially reads:

```text
50 player_id pages
```

This part is solid.

---

#### 5. But do we need all 50 `team_id` pages?

**No.**

This is the subtle point I want to correct from my previous answer.

Suppose:

```text
player_id = 1
```

is at offset 1000, and:

```text
player_id = 999
```

is at offset 19,999.

Because of implicit offsets, we now know:

```text
tuple position 1000
tuple position 19999
```

We only need:

```text
team_id[1000]
team_id[19999]

HR[1000]
HR[19999]
```

We don't need every `team_id` and `HR` value.

---

#### 6. How many pages do those positions occupy?

Each DSM page contains 400 values.

So:

```text
offset 1000
```

belongs to approximately:

```text
1000 / 400 = page 2
```

and:

```text
offset 19999
```

belongs to:

```text
19999 / 400 ≈ page 49
```

Therefore, for `team_id`, we might only need:

```text
team_id page 2
team_id page 49
```

= **2 pages**.

Likewise:

```text
HR page 2
HR page 49
```

= **2 pages**.

So one possible worst-case layout is:

```text
player_id: 50 pages
team_id:    2 pages
HR:         2 pages
-------------------
            54 pages
```

Not 150.

---

#### 7. So what's the true maximum?

Now we need to ask:

> Can the two matching tuples be positioned so that their `team_id` values require 2 pages and their `HR` values require 2 pages?

Yes.

But because there are only **two matching tuples**, we can need at most:

```text
player_id → 50 pages
team_id   → 2 pages
HR        → 2 pages
```

giving:

```text
50 + 2 + 2 = 54 pages
```

assuming the relevant pages aren't already shared/cached and counting distinct pages read.

Therefore, based on the stated assumptions, the maximum belongs to:

> **41–60**

rather than `81–100`.

---

#### 8. Why implicit offsets are so powerful here

This is exactly why DSM can be efficient for analytical queries.

Imagine we have:

```text
player_id
────────────────────────
page 0
page 1
...
page 49

team_id
────────────────────────
page 0
page 1
...
page 49

HR
────────────────────────
page 0
page 1
...
page 49
```

After scanning `player_id`, suppose we discover:

```text
player_id = 1    → offset 123
player_id = 999  → offset 18,765
```

We immediately know:

```text
offset 123
   ↓
team_id page 0
HR       page 0

offset 18,765
   ↓
team_id page 46
HR       page 46
```

We don't need to scan the other pages.

That's the benefit of the positional correspondence.

---

##### The mental model I recommend

Think of DSM with implicit offsets as **parallel arrays**:

```text
                 logical tuple position
                       ↓
        0       1       2       3       ...       19999
        │       │       │       │                   │
        ↓       ↓       ↓       ↓                   ↓
player  1       2       3       4       ...        999
team   10      20      10      30       ...      15445
G      100     80      90     120       ...        87
HR      20     15      18      25       ...        42
```

The **position is the join key** between columns.

That's what "implicit offset" means.

##### For this question

```text
                    DSM
                     │
          scan player_id
             50 pages
                 │
                 │ discover offsets
                 ↓
          ┌──────┴──────┐
          ↓             ↓
       team_id          HR
       ≤ 2 pages       ≤ 2 pages
```

So the important correction to my previous response is:

**Don't count `50 + 50 + 50`.** The `player_id` column may require all 50 pages because we have to search it, but the two output columns only need the pages containing the two matching offsets.

Under the given assumptions, that makes the maximum **54 pages → the `41–60` bucket**.


## (c) Finally consider the following query: 

```sql
SELECT MIN(player_id) FROM P
WHERE HR = (SELECT MIN(HR) FROM P);
```

Suppose the DBMS uses the decomposition storage model (DSM) with implicit offsets. 

- i. **[5 points]** What is the _minimum_ number of pages that the DBMS will potentially have to **read from disk** to answer this query? 

   - [ ] 1-50 [X] 51-100 [ ] 101-150 [ ] 151-200  [ ] ≥ 201 [ ] Not possible to determine 

### Solution

NSM:
20,000 tuples/200 page = 100 tuples/page
1 tuple = 4 attributes values
for each page, 100 tuples/page *4= 400 attribute values

DSM
for each attribute: 20,000 attribute values/400 = 50 pages

```SQL
SELECT MIN(HR)
```

need 50 page to find the min because there is no index, we need all the HRs

with the `MIN HR`, we can find the implicit offsets, An offset corresponds to a tuple/value position, not a page. 
The offset identifies the logical tuple position, and then that position can be mapped to a physical page.
so at least 50+1 pages

"Each matching HR value gives us a tuple offset; that offset tells us which player_id value belongs to the same tuple, and therefore which player_id page we need to read."



# **Question 3: Database Compression [35 points]** 

- (a) **[5 points]** Suppose that the DBMS has a VARCHAR column storing the following values: 

```
[Milwaukee Braves, Baltimore Orioles, Buffalo Blues, Brooklyn Gladiators, Chicago Cubs] 
```

Which of the following are valid encodings (uint32) for this column under dictionary compression as discussed in lecture that will support both point queries and range queries? Select **all** the valid encodings. 

- [ ] [1,2,3,4,5] 
- [x] [4,0,2,1,3] 
- [ ] [79,12,32,33,31] 
- [ ] [50,40,20,10,30] 
- [ ] [79,12,33,15,32] 
- [x] [49,9,29,19,39] 


- (b) **[15 points]** Suppose the DBMS wants to compresses a table R(a) using columnar compression. Which of the following compression schemes **will benefit** (when considering space efficiency) from sorting the table before compressing column a? Select **all** that apply. 

_Hint: “Benefit” means that the efficacy of the compression scheme improves on sorted data. You should not make any assumptions about the column type or its distribution of values._ 

   - [ ] Bit-packing Encoding 

   - [X] Run-length Encoding 

   - [ ] Mostly Encoding 

   - [ ] Bitmap Encoding 

   - [X] Dictionary Encoding -> [ ]

   - [X] Delta Encoding 

   - [ ] None of the above will benefit. 

- (c) **[15 points]** A colleague approaches with a list of true and false statements about runlength encoding, delta encoding, bitmap encoding, and dictionary encoding. The colleague wants your assistance in identifying the true statements. Select **all** that apply. 

   - [ ] Delta Encoding is good at compressing large text values. 

   - [ ] Inserts and updates on high cardinality columns with bitmap encoding has no overhead. 

   - [ ] For _point lookup-only_ workload, order-preserving dictionary encoding is required.
   
   - [ ] For a heavy update workload, dictionary encoding and delta encoding always require re-encoding. 

   - [X] Run-length Encoding is effective for compressing low cardinality integer column. 

   - [ ] None of the above. 

End of Homework #2 

