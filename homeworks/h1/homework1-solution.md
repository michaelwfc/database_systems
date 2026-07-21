# Solutions

## Q1 [0 points]

The purpose of this query is to make sure that the formatting of your output matches exactly the formatting of our auto-grading script.

**File:** q1\_sample.duckdb.sql

**Details:** List all active divisions ordered alphabetically.

```sql

lahman-cmudb2025 D DESCRIBE divisions;
+-------------+-------------+------+------+---------+-------+
| column_name | column_type | null | key  | default | extra |
+-------------+-------------+------+------+---------+-------+
| divID       | VARCHAR     | NO   | PRI  | NULL    | NULL  |
| lgID        | VARCHAR     | NO   | PRI  | NULL    | NULL  |
| division    | VARCHAR     | NO   | NULL | NULL    | NULL  |
| active      | VARCHAR     | NO   | NULL | NULL    | NULL  |
+-------------+-------------+------+------+---------+-------+


lahman-cmudb2025 D SELECT COUNT(*) FROM divisions;
+--------------+
| count_star() |
+--------------+
| 11           |
+--------------+

lahman-cmudb2025 D SELECT * FROM divisions LIMIT 5;
+-------+------+------------+--------+
| divID | lgID |  division  | active |
+-------+------+------------+--------+
| E     | AL   | AL East    | Y      |
| W     | AL   | AL West    | Y      |
| C     | AL   | AL Central | Y      |
| E     | NL   | NL East    | Y      |
| W     | NL   | NL West    | Y      |
+-------+------+------------+--------+


SELECT * FROM divisions AS D
WHERE D.active = 'Y'
ORDER BY division ASC;

+-------+------+------------+--------+
| divID | lgID |  division  | active |
+-------+------+------------+--------+
| C     | AL   | AL Central | Y      |
| E     | AL   | AL East    | Y      |
| W     | AL   | AL West    | Y      |
| C     | NL   | NL Central | Y      |
| E     | NL   | NL East    | Y      |
| W     | NL   | NL West    | Y      |
+-------+------+------------+--------+

```


## Q2 [20 points]

Find ten people with the highest home-runs (HR) in any single appearance, irrespective of year or team. Only consider people who have played at some point for a school in `PA`. Order the results from most to least home runs, and then by **first name** alphabetically.

**File:** q2\_hr.duckdb.sql

**Hints:** The `||` might be useful for constructing the name field. The name should be assembled as `{FIRST} ({GIVEN}) {LAST}`. A player's highest home-runs in any appearance can be found by aggregating over all years and teams of a given player's `appearances`.

Your output should look like this:

```
name|max_hr_appearance
```

Your first row should look like this:

```
Bobby (Robert Leigh) Higginson|30
```



Database Design Perspective

This exercise is actually testing an important database concept:
```text
people
        ▲
        │
        │
collegeplaying
        │
        ▼
schools

        ▲
        │
        │
appearances
```

You need to combine four tables:
- people – player names
- appearances – HR statistics
- collegeplaying – which schools each player attended
- schools – identify schools in Pennsylvania

This is a classic example of using joins to traverse relationships in a normalized relational schema. The aggregation (MAX(HR) GROUP BY playerID) is necessary because the statistics are stored at the season/team level, while the final result is required at the player level.


```sql
lahman-cmudb2025 D DESCRIBE appearances;
+-------------+-------------+------+------+---------+-------+
| column_name | column_type | null | key  | default | extra |
+-------------+-------------+------+------+---------+-------+
| yearID      | SMALLINT    | NO   | PRI  | NULL    | NULL  |
| teamID      | VARCHAR     | NO   | PRI  | NULL    | NULL  |
| lgID        | VARCHAR     | YES  | NULL | NULL    | NULL  |
| playerID    | VARCHAR     | NO   | PRI  | NULL    | NULL  |
| G_all       | SMALLINT    | YES  | NULL | NULL    | NULL  |
| G_batting   | SMALLINT    | YES  | NULL | NULL    | NULL  |
| HR          | SMALLINT    | YES  | NULL | NULL    | NULL  |
+-------------+-------------+------+------+---------+-------+

SELECT * FROM appearances
                   ORDER BY HR DESC LIMIT 5 ;
+--------+--------+------+-----------+-------+-----------+----+
| yearID | teamID | lgID | playerID  | G_all | G_batting | HR |
+--------+--------+------+-----------+-------+-----------+----+
| 2001   | SFN    | NL   | bondsba01 | 153   | 153       | 73 |
| 1998   | SLN    | NL   | mcgwima01 | 155   | 155       | 70 |
| 1998   | CHN    | NL   | sosasa01  | 159   | 159       | 66 |
| 1999   | SLN    | NL   | mcgwima01 | 153   | 153       | 65 |
| 2001   | CHN    | NL   | sosasa01  | 160   | 160       | 64 |
+--------+--------+------+-----------+-------+-----------+----+


lahman-cmudb2025 D DESCRIBE schools;
+-------------+-------------+------+------+---------+-------+
| column_name | column_type | null | key  | default | extra |
+-------------+-------------+------+------+---------+-------+
| schoolID    | VARCHAR     | NO   | PRI  | NULL    | NULL  |
| name_full   | VARCHAR     | YES  | NULL | NULL    | NULL  |
| city        | VARCHAR     | YES  | NULL | NULL    | NULL  |
| state       | VARCHAR     | YES  | NULL | NULL    | NULL  |
| country     | VARCHAR     | YES  | NULL | NULL    | NULL  |
+-------------+-------------+------+------+---------+-------+

lahman-cmudb2025 D SELECT * FROM schools
                   WHERE state in ['PA']
                   LIMIT 5;
+------------+---------------------------------------+------------+-------+---------+
|  schoolID  |               name_full               |    city    | state | country |
+------------+---------------------------------------+------------+-------+---------+
| albrightpa | Albright College                      | Reading    | PA    | USA     |
| allegheny  | Allegheny College                     | Meadville  | PA    | USA     |
| alverniapa | Alvernia College                      | Reading    | PA    | USA     |
| bloomsburg | Bloomsburg University of Pennsylvania | Bloomsburg | PA    | USA     |
| bucknell   | Bucknell University                   | Lewisburg  | PA    | USA     |
+------------+---------------------------------------+------------+-------+---------+


lahman-cmudb2025 D DESCRIBE collegeplaying;
+-------------+-------------+------+------+---------+-------+
| column_name | column_type | null | key  | default | extra |
+-------------+-------------+------+------+---------+-------+
| playerID    | VARCHAR     | NO   | NULL | NULL    | NULL  |
| schoolID    | VARCHAR     | YES  | NULL | NULL    | NULL  |
| yearID      | SMALLINT    | YES  | NULL | NULL    | NULL  |
+-------------+-------------+------+------+---------+-------+

-- Decomposition of the problem 
---Requirement → break into smaller relational operations → combine with CTEs.
---steps: 
---1. find all schools in PA(Pennsylvania) from schools
SELECT schoolID FROM schools
WHERE schools.state = 'PA'

--- 2. find all the players in these schools from collegeplaying
SELECT playerID FROM collegeplaying
JOIN PAschools ON collegeplaying.schoolID = PAschools.schoolID

--- 3. for each player, find the max(hr) from 
SELECT playerID, MAX(HR) as maxhr from appearances
GROUP BY playerID
---the size of intermediate results and when to filter data is the first step
JOIN PAplayers ON appearances.playerID = PAplayers.playerID 



---4. join with people to obtain nameFirst, nameGiven, nameLast, then construct name
SELECT nameFirst || nameGiven || nameLast AS  name, hr FROM people
JOIN maxhrs ON people.playerID = maxhrs.playerID

--- 5. sort by maxhr
SELECT nameFirst || nameGiven || nameLast AS  name, maxhr FROM people
JOIN maxhrs ON people.playerID = maxhrs.playerID
ORDER BY maxhr DESC;


--- conbine
-- WITH PASchools AS
-- (
--     SELECT schoolID FROM schools as s
--     WHERE s.state = 'PA'
--     ),

--- PAPlayers is not a real entity. It's just a filter: Keep only players that attended a Pennsylvania school.
-- PAPlayers  AS 
-- (
--     SELECT DISTINCT playerID FROM collegeplaying AS cp
--     JOIN PASchools AS ps ON cp.schoolID = ps.schoolID
-- ),

-- PlayerMaxHR AS
-- (
--     SELECT a.playerID, MAX(a.HR) AS maxhr FROM appearances AS a
--     JOIN PAPlayers AS pp ON a.playerID = pp.playerID 
--     GROUP BY a.playerID
-- )

-- SELECT nameFirst || ' ('|| nameGiven || ') ' || nameLast AS name, maxhr FROM people AS p
--     JOIN PlayerMaxHR as pm ON p.playerID = pm.playerID
--     ORDER BY pm.maxhr DESC, p.nameFirst
--     LIMIT 10;


   

WITH PlayerMaxHR AS
(
    SELECT 
        a.playerID, 
        MAX(a.HR) AS maxhr 
    FROM appearances as a
    -- USING semi-join: the intent is existence, not data retrieval.
    --- A semi-join behaves like Return rows from appearances ONLY IF there exists a matching player
    WHERE a.playerID IN (   
        SELECT DISTINCT cp.playerID 
        FROM collegeplaying AS cp
        JOIN schools AS s 
            ON cp.schoolID = s.schoolID
        WHERE s.state = 'PA'
    )
    GROUP BY a.playerID
)

SELECT 
    nameFirst || ' ('|| nameGiven || ') ' || nameLast AS player_name, 
    maxhr 
FROM people AS p
JOIN PlayerMaxHR as pm 
ON p.playerID = pm.playerID
ORDER BY pm.maxhr DESC, p.nameFirst
LIMIT 10;


+----------------------------------+-------+
|           player_name            | maxhr |
+----------------------------------+-------+
| Bobby (Robert Leigh) Higginson   | 30    |
| Harry (Harry Walter) Anderson    | 23    |
| Dick (Richard Edward) Gernert    | 21    |
| Mark (Mark Thomas) DeRosa        | 21    |
| Matt (Matthew James) Adams       | 20    |
| Mickey (James Barton) Vernon     | 20    |
| Chris (Chris J.) Heisey          | 18    |
| Danny (Daniel Webster) Litwhiler | 18    |
| Jeff (Jeffrey Paul) Manto        | 17    |
| Doug (Douglas Metunwa) Glanville | 14    |
+----------------------------------+-------+

```


## Q3 [20 points]

Find ten (player, team) pairs where the player won the `Gold Glove` award in an active league after 1999 and batted in more games than the player's team's average since 1999. 
Order by the number of distinct award-winning years from most to least, and then by given name alphabetically.

**File:** q3\_mvp.duckdb.sql

**Hints:** Only consider awardID that matches `Gold Glove`. The team average can be computed by taking the average over the batted games by each team player's `appearances` since 1999. You might find [Correlated Subqueries](https://duckdb.org/docs/stable/sql/expressions/subqueries.html#correlated-subqueries) in DuckDB useful.

Your output should look like this:

```
nameGiven|teamID|distinct_years
```

Your first row should look like this:

```
Ichiro|SEA|10
```




```sql
-- steps:
-- 1. find the player won the  `Gold Glove` award in an active league after 1999  from awardsplayers
-- 2. filter out the players who batted in more games than the player's team's average since 1999 from appearances
-- 3. count the distinct award-winning years group by (player, team) and sort 
-- 4. join the name from people, and team and sort


SELECT p.nameGiven, t.name, COUNT(DISTINCT a.yearID) AS ggcount 
FROM apperance AS a
--- correlated subqury
WHERE a.playerID IN  --- IN requires one column
    --- filter out1: the player won the  `Gold Glove` award in an active league after 1999
    (SELECT ap.playerID 
        FROM awardsplayers AS ap
        --- in an active league
        JOIN leagues as l
        ON ap.lgID = l.lgID 
        --- A SELECT has only one WHERE clause.
        WHERE ap.awardID = 'Gold Glove' and ap.yearID > 1999 and l.active = 'true'      
    )
    AND
    --- filter out2:  the players who batted in more games than the player's team's average since 1999 from appearances
    --- player's G_batting > that team's average, the average depend on the current team,That's exactly why the hint suggests Correlated Subquery
    --- 
    (SELECT * FROM )

JOIN people  AS p
ON a.playerID = p.playerID
JOIN teams as t
ON a.teamID = t.teamID AND a.yearID = t.yearID AND a.lgID = t.lgID
-- GROUP BY (a.playerID, a.teamID, ) 
GROUP BY (p.nameGiven, t.name, )
ORDER BY ggcount DESC, p.nameGiven
LIMIT 10;

```


In fact, there are two different logical pipelines hidden in this query:

1. Award qualification (Who is eligible?)
2. Appearance qualification (Which player-team pairs satisfy the batting condition?)

decompose this problem into intermediate relations and relational operators first,
σ  (Selection)
π  (Projection)
⋈  (Join)
⋉  (Semi Join)
γ  (Aggregation)
τ  (Sort)

Then it builds a logical tree and only later chooses physical operators like Hash Join, Hash Aggregate, or Top-N.

```
                    awardsplayers
                          │
         award='Gold Glove', year>1999
                          │
                          ▼
                      leagues
                          │
                    active='Y'
                          │
                          ▼
                  Eligible Award Records
                          │
                          ▼
                  Eligible Players
                          │
                          │
                          │
                          │
appearances ──────────────┘
      │
      │ year>=2000
      ▼
Correlated Subquery
(team average G_batting)
      │
      ▼
Qualified Appearances
(playerID, teamID)
      │
      ▼
Semi Join
Eligible Players
      │
      ▼
Qualified Eligible
(player,team)
      │
      ▼
Count DISTINCT award years
      │
      ▼
Join people
      │
      ▼
Sort
      │
      ▼
LIMIT 10
```


```sql
-- version 2: 
-- Award qualification (Who is eligible?)
WITH eligiblePlayers AS(
    SELECT ap.playerID, ap.yearID
    FROM awardsplayers AS ap
    JOIN (
        SELECT * FROM leagues AS l
        WHERE l.active = 'true'
        )AS el
    ON ap.lgID = el.lgID
    WHERE ap.awardID = 'Gold Glove' AND ap.yearID > 1999
)

--- Appearance qualification (Which player-team pairs satisfy the batting condition?)
    eligibleApperances AS(
    SELECT * FROM appearances AS a
    WHERE a.yearID >= 2000 AND a.G_batting >(
        SELECT AVG(a.G_batting) FROM a
        GROUP BY (a.playerID, a.teamID)
    )
)
SELECT p.nameGiven,t.name, COUNT(DISTINCT ea.yearID) AS count 
FROM eligibleApperances AS ea
JOIN eligiblePlayers AS ep 
ON ea.playerID = ep.playerID and ea.yearID = ea.yearID
GROUP BY (ea.playerID, ea.teamID)
JOIN people AS p
ON ea.playerID = p.playerID
JOIN teams AS t
ON ea.teamID = t.teamID
ORDER BY count DESC
LIMIT 10;



-- version 3: 
WITH eligiblePlayers AS(
    SELECT ap.playerID, ap.yearID
    FROM awardsplayers AS ap
    JOIN leagues AS l
    ON ap.lgID = l.lgID
    WHERE ap.awardID = 'Gold Glove' AND ap.yearID > 1999 AND l.active = 'Y'
)

   eligibleApperances AS(
    SELECT * FROM appearances AS a
    WHERE a.yearID >= 2000 AND a.G_batting >(
        --- issue: return multiple rows
        --- AVG(a.G_batting): you're accidentally referring to the outer row instead of the rows you're trying to average.
        SELECT AVG(a.G_batting) FROM appearances
        GROUP BY (a.teamID)
    )
)

-- award-winning years
SELECT p.nameGiven,t.name, COUNT(DISTINCT ep.yearID) AS count 
FROM eligibleApperances AS ea
JOIN eligiblePlayers AS ep 
ON ea.playerID = ep.playerID and ea.yearID = ep.yearID
JOIN people AS p
ON ea.playerID = p.playerID
JOIN teams AS t
ON  ea.yearID = t.yearID AND  ea.lgID = t. lgID AND ea.teamID = t.teamID 
-- Does every selected non-aggregate column belong to the grouping key?
-- Names are display attributes, not identifiers.
GROUP BY (p.nameGiven, t.name)
ORDER BY count DESC
LIMIT 10;


-- version 4:
WITH eligiblePlayers AS(
    SELECT ap.playerID, ap.yearID
    FROM awardsplayers AS ap
    JOIN leagues AS l
    ON ap.lgID = l.lgID
    WHERE ap.awardID = 'Gold Glove' AND ap.yearID > 1999 AND l.active = 'Y'
),
--- optimizer: Eligible Players -> Semi Join -> Appearances
   eligibleApperances AS(
    --- Outer row: Ichiro | SEA | 2004
    SELECT * FROM appearances AS oa
    WHERE oa.G_batting >(
        --- corrlate subquery
        SELECT AVG(ia) FROM appearances AS ia
        --- Every outer row gets its own average.
        WHERE ia.yearID >=2000 AND   ia.yearID= oa.yearID AND ia.lgID= oa.lgID AND ia.teamID = oa.teamID 
    )
)


-- award-winning years
SELECT p.nameGiven,t.nameID, COUNT(DISTINCT ep.yearID) AS dy 
FROM eligibleApperances AS ea
JOIN eligiblePlayers AS ep 
--- player won Gold Glove after 1999 , It does not say, appearance year equals award year.
ON ea.playerID = ep.playerID
JOIN people AS p
ON ea.playerID = p.playerID
JOIN teams AS t
ON  ea.yearID = t.yearID AND  ea.lgID = t. lgID AND ea.teamID = t.teamID 
--- aligning the GROUP BY and output columns with the exact specification.
GROUP BY (ea.playerID, ea.teamID)
ORDER BY dy DESC
LIMIT 10;


--- version 5:

WITH eligiblePlayers AS(
    SELECT ap.playerID, ap.yearID
    FROM awardsplayers AS ap
    JOIN leagues AS l
    ON ap.lgID = l.lgID
    WHERE ap.awardID = 'Gold Glove' AND ap.yearID > 1999 AND l.active = 'Y'
),
   
   eligibleApperances AS(
    --- Outer row: Ichiro | SEA | 2004
    SELECT * FROM appearances AS oa
    --- optimizer: Eligible Players -> Semi Join -> Appearances
    WHERE oa.playerID IN (
        SELECT ep.playerID 
        FROM eligiblePlayers AS ep
   ) AND (
        oa.G_batting >(
            --- corrlate subquery
            SELECT AVG(ia) FROM appearances AS ia
            --- Every outer row gets its own average.
            WHERE ia.yearID >=2000 AND   ia.yearID= oa.yearID AND ia.lgID= oa.lgID AND ia.teamID = oa.teamID 
        )
   )
)


-- award-winning years
SELECT p.nameGiven,ea.teamID, COUNT(DISTINCT ep.yearID) AS dy 
FROM eligibleApperances AS ea
JOIN eligiblePlayers AS ep 
--- player won Gold Glove after 1999 , It does not say, appearance year equals award year.
ON ea.playerID = ep.playerID
JOIN people AS p
ON ea.playerID = p.playerID
JOIN teams AS t
ON  ea.yearID = t.yearID AND  ea.lgID = t. lgID AND ea.teamID = t.teamID 
--- aligning the GROUP BY and output columns with the exact specification.
GROUP BY (ea.playerID, p.nameGiven, ea.teamID)
ORDER BY dy DESC
LIMIT 10;
```


# QA

## Q1： Does there teams  tabel have 2 primary keys? why ? what about the foregin key?

```
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

Excellent question. This gets to one of the fundamental concepts in **relational database design**. There is actually **only one primary key** in this table, but that primary key is **composite** (also called a **compound primary key**).

Let's examine it carefully.

---

### The `teams` table

From DuckDB:

```text
column_name    key
----------------------
yearID         PRI
lgID           PRI
teamID         PRI
franchID
divID
teamRank
name
attendance
```

You see three columns marked `PRI`:

* `yearID`
* `lgID`
* `teamID`

This **does not mean there are three primary keys**.

Instead, it means:

```sql
PRIMARY KEY (yearID, lgID, teamID)
```

These **three columns together form one primary key**.

---

### What is a Composite Primary Key?

Suppose we define

```sql
PRIMARY KEY (A, B)
```

This means

```
A alone      ❌ not unique
B alone      ❌ not unique
(A,B)        ✅ unique
```

For example:

| A | B  |
| - | -- |
| 1 | 10 |
| 1 | 20 |
| 2 | 10 |

All rows are unique because the **pair** `(A, B)` is unique.

---

#### Your example

Suppose the table contains

| yearID | lgID | teamID | name    |
| ------ | ---- | ------ | ------- |
| 2024   | AL   | NYY    | Yankees |
| 2025   | AL   | NYY    | Yankees |
| 2024   | NL   | NYM    | Mets    |
| 2024   | AL   | BOS    | Red Sox |

Notice:

####### Is `teamID` unique?

No.

```
NYY
NYY
```

appears twice.

---

####### Is `yearID` unique?

No.

```
2024
2024
2024
```

appears many times.

---

####### Is `lgID` unique?

No.

```
AL
AL
AL
```

appears many times.

---

####### Is the combination unique?

```
(yearID, lgID, teamID)
```

Let's check:

```
(2024, AL, NYY)

(2025, AL, NYY)

(2024, NL, NYM)

(2024, AL, BOS)
```

Every combination is different.

Therefore

```sql
PRIMARY KEY(yearID, lgID, teamID)
```

is valid.

---

### Why use a Composite Primary Key?

Because a baseball team exists **every season**.

The Yankees appear

```
1901

1902

1903

...

2025
```

If we only used

```sql
PRIMARY KEY(teamID)
```

we could only store **one** Yankees row.

That would be wrong.

Instead

```
(Yankees,2024)

(Yankees,2025)

(Yankees,2026)
```

are different records.

---

### Why not use an Identity Column?

Many modern applications would instead write

```sql
CREATE TABLE teams(
    id BIGINT PRIMARY KEY,
    yearID SMALLINT,
    lgID VARCHAR,
    teamID VARCHAR,
    ...
);
```

where

```sql
id
```

is auto-generated.

This is called a **surrogate key**.

---

Then we'd add

```sql
UNIQUE(yearID, lgID, teamID)
```

to preserve the business rule.

Both designs are valid.

---

### Natural Key vs Surrogate Key

The CMU baseball database uses what is called a **natural key**.

Natural key:

```
(yearID,
 lgID,
 teamID)
```

comes from real-world data.

Surrogate key:

```
id = 104582
```

has no business meaning.

---

Comparison:

| Natural Key                         | Surrogate Key       |
| ----------------------------------- | ------------------- |
| Comes from business data            | Artificial ID       |
| May have multiple columns           | Usually one integer |
| More meaningful                     | Smaller and faster  |
| Can change if business rules change | Stable forever      |

---

### What about Foreign Keys?

Your second question is equally important.

Suppose another table

```sql
appearances
```

contains

```
playerID

yearID

lgID

teamID

...
```

How does it reference `teams`?

It must reference **all three columns**.

For example:

```sql
CREATE TABLE appearances(
    playerID VARCHAR,

    yearID SMALLINT,

    lgID VARCHAR,

    teamID VARCHAR,

    FOREIGN KEY
        (yearID, lgID, teamID)

    REFERENCES teams
        (yearID, lgID, teamID)
);
```

Notice

```
Foreign Key

(yearID,
 lgID,
 teamID)

↓

Primary Key

(yearID,
 lgID,
 teamID)
```

The entire composite key is referenced.

---

### Why can't we reference only `teamID`?

Suppose we tried

```sql
FOREIGN KEY(teamID)
```

Now imagine

```
NYY
```

Which row?

```
2023 NYY

2024 NYY

2025 NYY
```

The database wouldn't know.

Foreign keys must reference a column (or set of columns) that is **unique**.

Since `teamID` alone is **not unique**, it cannot uniquely identify a row in `teams`.

---

### Visualizing the Relationship

```
teams
────────────────────────────────────────────
(yearID, lgID, teamID)   ← Primary Key
2024  AL  NYY
2025  AL  NYY
2024  AL  BOS


appearances
────────────────────────────────────────────
playerID
yearID
lgID
teamID

aaronha01 2024 AL NYY
judgeaa01 2025 AL NYY
```

The arrows are:

```
appearances
(yearID,lgID,teamID)

        │
        ▼

teams
(yearID,lgID,teamID)
```

---

### How Does the DBMS Store a Composite Key?

Internally, the database treats the composite key as **one logical key** composed of multiple columns.

For example, a B+ tree index might conceptually store keys like:

```
(yearID, lgID, teamID)
────────────────────────
(2024, AL, BOS)
(2024, AL, NYY)
(2024, NL, NYM)
(2025, AL, NYY)
```

The index compares the columns **lexicographically**:

1. Compare `yearID`
2. If equal, compare `lgID`
3. If still equal, compare `teamID`

This allows efficient lookups and range scans on the composite key.

---

#### A Database Systems Perspective

As you continue through CMU 15-445, you'll encounter these concepts repeatedly:

* **Primary Key**: A logical constraint stating that each row is uniquely identifiable.
* **Unique Index**: The physical mechanism many DBMSs use to enforce a primary key.
* **Composite Key**: A primary key consisting of multiple columns.
* **Foreign Key**: A constraint that references a candidate key (usually the primary key) in another table.

Understanding the distinction between **logical constraints** (primary keys, foreign keys) and their **physical implementation** (indexes, B+ trees) is fundamental to database internals. The SQL `PRIMARY KEY` clause expresses a relational constraint; the DBMS typically enforces it by creating a unique index behind the scenes.


## Q2: Can One table use different foreign keys to reference to different other tables?

Yes. **Absolutely.** In fact, this is one of the most fundamental ideas in relational databases.

A table can have:

* **zero foreign keys**
* **one foreign key**
* **multiple foreign keys**
* **multiple foreign keys referencing the same table**
* **multiple foreign keys referencing different tables**

There is no restriction that a table can only reference one other table.

---

### Think of Foreign Keys as Relationships

Suppose we have these tables:

```text
people
--------
playerID (PK)
name

teams
--------
(yearID, lgID, teamID) (PK)

schools
--------
schoolID (PK)
```

Now suppose we have an `appearances` table.

```text
appearances
-------------------------------
playerID
yearID
lgID
teamID
G_all
...
```

This table naturally relates to **two different entities**:

* a player
* a team

Therefore it should have **two foreign keys**.

```sql
CREATE TABLE appearances (
    playerID VARCHAR,

    yearID SMALLINT,
    lgID VARCHAR,
    teamID VARCHAR,

    G_all SMALLINT,

    FOREIGN KEY (playerID)
        REFERENCES people(playerID),

    FOREIGN KEY (yearID, lgID, teamID)
        REFERENCES teams(yearID, lgID, teamID)
);
```

Notice:

```text
appearances
     │
     ├────────────► people
     │
     └────────────► teams
```

One table references **two different tables**.

This is completely normal.

---

### Real World Analogy

Imagine an Employee table.

```text
Employee
--------------------------
employee_id
department_id
manager_id
office_id
```

Relationships:

```text
department_id ───► Department

manager_id ──────► Employee

office_id ───────► Office
```

The Employee table has **three foreign keys**.

---

### Looking at the Lahman Database

Let's inspect one by one.

---

#### appearances

```text
appearances

playerID

yearID
lgID
teamID
```

This logically should reference

####### people

```text
people
------------
playerID (PK)
```

and

####### teams

```text
teams
------------
(yearID,
 lgID,
 teamID)
```

Relationship:

```text
people
   ▲
   │
   │ playerID
   │
appearances
   │
   │ (yearID,lgID,teamID)
   ▼
teams
```

---

#### managers

```
playerID
yearID
lgID
teamID
```

Again

```text
manager
      │
      ├────────► people

      └────────► teams
```

Two foreign keys.

---

#### awardsplayers

```
playerID
awardID
yearID
lgID
```

Here we probably have

```text
playerID
```

referencing

```text
people
```

If an Awards table existed

```
awards
--------
awardID
```

then

```text
awardID
```

would reference

```
awards
```

So one table can reference

```
people

+

awards
```

---

#### collegeplaying

```
playerID

schoolID

yearID
```

Relationships

```text
playerID
    │
    ▼
people

schoolID
    │
    ▼
schools
```

Again

two foreign keys.

---

### One Foreign Key Can Reference Multiple Columns

Notice something interesting.

The team primary key is

```text
(yearID,
 lgID,
 teamID)
```

Therefore the foreign key is

```text
(yearID,
 lgID,
 teamID)
```

not

```text
teamID
```

alone.

This is called a **composite foreign key**.

---

### Can One Column Reference Two Different Tables?

Usually **No**.

For example

```text
playerID
```

cannot simultaneously mean

```text
people.playerID
```

and

```text
schools.schoolID
```

That would be ambiguous.

One foreign key references **one candidate key**.

---

### Can One Table Reference the Same Table Multiple Times?

Yes!

Example:

```sql
CREATE TABLE Employee(
    emp_id INT PRIMARY KEY,

    manager_id INT,

    mentor_id INT,

    FOREIGN KEY(manager_id)
        REFERENCES Employee(emp_id),

    FOREIGN KEY(mentor_id)
        REFERENCES Employee(emp_id)
);
```

Diagram

```text
Employee

emp_id

manager_id ───────┐
                  │
mentor_id ────────┘

        │
        ▼

Employee
```

This is called a **self-referencing foreign key**.

---

### Can One Table Have Many Foreign Keys?

Certainly.

Imagine an Order table.

```text
Order

order_id

customer_id

product_id

seller_id

warehouse_id

payment_id

shipment_id
```

Relationships

```text
customer_id ─────► Customer

product_id ──────► Product

seller_id ───────► Seller

warehouse_id ────► Warehouse

payment_id ──────► Payment

shipment_id ─────► Shipment
```

One table

↓

six foreign keys.

Very common.

---

### Visualizing the Lahman Database

A simplified ER diagram for the tables you've shown would look like:

```text
                people
           +-------------+
           | playerID PK |
           +-------------+
                 ▲
                 │
     ┌───────────┼─────────────────────┐
     │           │                     │
     │           │                     │
appearances   managers         awardsplayers
     │           │                     │
     │           │                     │
     ▼           ▼                     ▼
+---------------------------------------------+
| playerID                                    |
| yearID                                      |
| lgID                                        |
| teamID                                      |
+---------------------------------------------+
     │
     ▼
+---------------------------------------------+
| teams                                       |
| (yearID, lgID, teamID) PK                   |
+---------------------------------------------+

collegeplaying
    │
    ├────────────► people
    │
    └────────────► schools
```

You can already see that **`people` is a central entity** in the schema. Many tables (`appearances`, `managers`, `awardsplayers`, `collegeplaying`, `halloffame`, etc.) reference it because they all describe different aspects of a baseball player's career.

---

#### A Database Design Perspective

In relational modeling, tables often represent either:

* **Entities** (e.g., `people`, `teams`, `schools`), which describe "things".
* **Relationships** (e.g., `appearances`, `collegeplaying`), which connect entities.

Relationship tables naturally contain **multiple foreign keys**, because they link multiple entities together.

In the Lahman database:

* `people` represents **players**.
* `teams` represents **teams in a specific league and year**.
* `schools` represents **schools**.
* `appearances` represents the relationship **"player appeared for a team in a season"**.

That is why `appearances` needs foreign keys to **both** `people` and `teams`. This is a textbook example of how foreign keys are used to model relationships in a normalized relational database.

## Q3: 
