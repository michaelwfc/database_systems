<!-- page 1 -->

## **Lecture #02: **

** (Fall 2025)**

https://15445.courses.cs.cmu.edu/fall2025/

Carnegie Mellon University

Andy Pavlo

### **1** **SQL History**

SQL is a declarative query language for relational databases. As oposed to imperative languages, in a declarative language the programmer/user only declares what needs to be done as oposed to how the operations should be done (e.g. Join these two tables). SQL was originally developed in the 1970s as part of the IBM **System R** project. IBM originally called it “SEQUEL” (Structured English Query Language). The name changed in the 1980s to just “SQL” (Structured Query Language).

Despite SQL being an old language, it is still being actively updated with new features every couple of

years. Some of the major updates released with each new edition of the SQL standard are shown below:

- **SQL:1999** Regular Expressions, Triggers

- **SQL:2003** XML, Windows, Sequences

- **SQL:2008** Truncation, Fancy Sorting

- **SQL:2011** Temporal DBs, Pipelined DML

- **SQL:2016** JSON, Polymorphic tables

- **SQL:2023** Property Graph Queries, Multi-Dimensional Arrays

The minimum language syntax a system needs to support in order to claim that it supports SQL is SQL-92.

Each vendor follows the standard to a certain degree and there are many proprietary extensions.


### **1.1 SQL Baiscs**




#### A SQL Query is Composed of Clauses

A clause is a major component (or section) of a SQL statement.
For example:
```sql
SELECT s.name, s.gpa
FROM student AS s
WHERE s.gpa > 3.5
ORDER BY s.gpa DESC;
```

This query consists of four clauses:

```sql
SELECT s.name, s.gpa        <-- SELECT clause
FROM student AS s           <-- FROM clause
WHERE s.gpa > 3.5           <-- WHERE clause
ORDER BY s.gpa DESC         <-- ORDER BY clause

```

Each clause has a different responsibility.
| Clause   | Purpose                                         |
| -------- | ----------------------------------------------- |
| SELECT   | What columns or expressions should be returned? |
| FROM     | Where does the data come from?                  |
| WHERE    | Which rows should be kept?                      |
| GROUP BY | How should rows be grouped?                     |
| HAVING   | Which groups should be kept?                    |
| ORDER BY | How should the results be sorted?               |
| LIMIT    | How many rows should be returned?               |


#### What is a Predicate?
A predicate is one of the most important concepts in SQL.

Formal Definition : A predicate is an expression that evaluates to TRUE/FALSE/UNKNOWN (because SQL has NULL)

In other words, A predicate asks a yes/no question.
```sql
salary > 5000

name LIKE 'A%'

gpa BETWEEN 3.0 AND 4.0

sid = 100
```

#### An Important Compiler Perspective

From the perspective of a database compiler:

- **Clauses** define the high-level structure of the SQL grammar (SELECT, FROM, WHERE, etc.).
- **Expressions** (columns, literals, arithmetic, function calls) appear inside clauses.
- **Predicates** are a special kind of expression whose result is Boolean, so they can control filtering.
- **Output** expressions are expressions in the SELECT clause that determine what values are returned.

This distinction is reflected directly in the AST. For example, a SelectStmt node has children representing different clauses, and the WHERE child contains a predicate expression tree, while the SELECT child contains output expression trees. Understanding this relationship makes it much easier to follow how a SQL parser, binder, and optimizer transform a query internally.



### **2** **Relational Languages**

The language is comprised of different classes of commands:

1. **Data Manipulation Language (DML):** SELECT , INSERT , UPDATE , and DELETE statements.

2. **Data Definition Language (DDL):** Schema definitions for tables, indexes, views, and other objects.

3. **Data Control Language (DCL):** Security and access controls.

4. It also includes view definition, integrity and referential constraints, and transactions.

Relational algebra (which is the algebra that SQL is based on) uses **sets** (unordered collections which do

not allow duplicates). However, SQL is based on **bags** (unordered collections which allow duplicates) to

avoid the extra work of removing duplicates by default. Duplicates can still be removed via features like

the DISTINCT keyword.

---

<!-- page 2 -->





### **3** **Example Database**

Here is the schema of a database we will use in our examples:
```SQL
CREATE TABLE student ( 
    sid INT PRIMARY KEY , 
    name VARCHAR (16), 
    login VARCHAR (32) UNIQUE , 
    age SMALLINT , 
    gpa FLOAT );

CREATE TABLE course ( 
    cid VARCHAR (32) PRIMARY KEY , 
    name VARCHAR (32) NOT NULL );

CREATE TABLE enrolled ( 
    sid INT REFERENCES student (sid), 
    cid VARCHAR (32) REFERENCES course (cid), 
    grade CHAR (1) );
```
**Figure 1:** Example database used for lecture

### **4** **Aggregates**

An aggregation function takes in a bag of tuples as its input and then produces a single scalar value as its output. 
Aggregate functions can (almost) only be used in a `SELECT` output list.


```SQL
- AVG(COL) : The average of the values in COL

- MIN(COL) : The minimum value in COL

- MAX(COL) : The maximum value in COL

- SUM(COL) : The sum of the values in COL

- COUNT(COL) : The number of tuples in the relation
```

Example: Get # of students with a ‘@cs’ login .

The following three queries are equivalent:


```SQL
SELECT COUNT (*) FROM student WHERE login LIKE '%@cs' ;

SELECT COUNT (login) FROM student WHERE login LIKE '%@cs' ;

SELECT COUNT (1) FROM student WHERE login LIKE '%@cs' ;
```
Some aggregate functions (e.g. COUNT , SUM , AVG ) support the DISTINCT keyword:

Example: Get # of unique students and their average GPA with a ‘@cs’ login .

```SQL
SELECT COUNT ( DISTINCT login) FROM student WHERE login LIKE '%@cs' ;
```



---

<!-- page 3 -->





A single SELECT statement can contain multiple aggregates:

Example: Get # of students and their average GPA with a ‘@cs’ login .

```SQL
SELECT AVG (gpa), COUNT (sid) FROM student  WHERE login LIKE '%@cs' ;
```

Output of other columns outside of an aggregate is undefined ( e.cid is undefined below).
Example: Get the average GPA of students in each course .
```SQL
SELECT AVG (s.gpa), e.cid 
FROM enrolled AS e JOIN student AS s 
ON e.sid = s.sid;
```

AVG(gpa) produces one value, while cid has multiple possible values.
Most real-world database systems will error in this case, but some systems such as SQLite will allow it by picking an arbitrary value. 
Therefore SQL reports an error: column "cid" must appear in GROUP BY...


**The SQL:2023 standard** now supports the `ANY_VALUE` aggregation function which does the same thing.
Example: Get the average GPA of students in each course .
```SQL
SELECT AVG (s.gpa), ANY_VALUE(e.cid) 
FROM enrolled AS e JOIN student AS s 
ON e.sid = s.sid;
```

Comment:
Without a GROUP BY, the query computes one single average over the entire joined table.
ANY_VALUE tells SQL: "I don't care which value you return". The database is free to choose any value from the group.


#### GROUP BY

Non-aggregated values in SELECT output clause must appear in the GROUP BY clause. This will partition the tuples based off of the value and calculate the aggregates for each subset. In this case there will be a canonical value for each group.

Example: Get the average GPA of students in each course.



```SQL
SELECT AVG(s.gpa), e.cid
FROM enrolled AS e JOIN student AS s
WHERE e.sid = s.sid   --- WHERE for joins, Legacy style when using comma-separated tables (FROM A, B),Equivalent to an inner join, but less explicit
GROUP BY e.cid;



SELECT AVG (s.gpa), e.cid 
FROM enrolled AS e JOIN student AS s 
ON e.sid = s.sid     --Modern, standard SQL syntax: JOIN ... ON ..., Specify the join predicate as part of the join operation
GROUP BY e.cid;
```


#### GROUPING SETS

The key idea is *GROUPING SETS performs several GROUP BY operations at once* .Instead of writing multiple queries.
Specify *multiple groupings* in a single query instead of using UNION ALL to combine the results of several individual GROUP BY queries.


Grouping sets can be used to specify *multiple groupings* in a single query rather than using UNION ALL to combine the results of several individual GROUP BY queries.
This results in the DBMS needing to only scan through the data once rather than multiple times.
The optimizer can compute all of these aggregates in a single scan of the data, which is much more efficient than executing multiple independent queries.


Example: Get the count of students by each course and grade, the count of students by course, and the total student count.
```SQL
SELECT c.name AS c_name, e.grade, COUNT (*) AS num_students 
FROM enrolled AS e JOIN course AS c 
ON e.cid = c.cid 
GROUP BY GROUPING SETS ( 
    (c.name, e.grade),  -- By course and grade
    (c.name),           -- By course only
    (),                 -- Overall total
    );
```


Normally we'd need three separate queries.
- GROUP BY course, grade
- GROUP BY course
- COUNT(*)
GROUPING SETS combines them BY
```sql
GROUP BY GROUPING SETS
(
    (course, grade),
    (course),
    ()
)
```

This literally means:

```sql
Run

GROUP BY course, grade : --First grouping

UNION ALL

GROUP BY course: -- Second grouping,There is no grade anymore. SQL fills it with NULL.

UNION ALL

GROUP BY nothing -- Third grouping, no grouping columns, Equivalent to SELECT COUNT(*)

-- Final result: The database concatenates everything:
| course | grade | count |
| ------ | ----- | ----: |
| DB     | A     |     2 |
| DB     | B     |     1 |
| AI     | A     |     1 |
| AI     | B     |     1 |
| DB     | NULL  |     3 |
| AI     | NULL  |     2 |
| NULL   | NULL  |     5 |

```



---

<!-- page 4 -->
#### HAVING clause
The `HAVING` clause filters output results based on aggregation computation (i.e. filters out groups as oposed to filtering rows which is what the `WHERE` clause does). This makes HAVING behave like a WHERE clause for a GROUP BY .

Example: Get the set of courses in which the average student GPA is greater than 3.9.
```sql
--- Wrong SQL
SELECT AVG(s.gpa) AS avg_gpa, e.cid 
FROM enrolled AS e, student AS s 
WHERE e.sid = s.sid 
AND  avg_gpa > 3.9  --- when you scan your data, how do you know whether to throw away a record or not?
GROUP BY e.cid 

--- Using HAVING clause at the bottom
SELECT AVG(s.gpa) AS avg_gpa, e.cid 
FROM enrolled AS e, student AS s 
WHERE e.sid = s.sid   
--- this example uses a legacy implicit join syntax (here the DBMS can deduce that a join is required to handle the WHERE clause.
--- You should always write out explicit joins in your queries.
GROUP BY e.cid 
HAVING avg_gpa > 3.9;
```

The above query syntax is supported by many major database systems, but is not compliant with the SQL standard. To make the query standard compliant, we must repeat use of `AVG(S.GPA)` in the body of the HAVING clause.

```sql
SELECT AVG(s.gpa), e.cid 
FROM enrolled AS e, student AS s 
WHERE e.sid = s.sid 
GROUP BY e.cid 
HAVING AVG(s.gpa) > 3.9;
```

### **5** **String Operations**

The SQL standard says that strings are **case sensitive** and **single-quotes only** . Real-world systems will vary in how loose they are about both points (e.g. MySQL).
![alt text](../../images/sql_string_operation.png)

There are functions to manipulate strings that can be used in any part of a query.

#### Pattern Matching:

The `LIKE` keyword is used for string matching in predicates.
- '%' matches any substrings (including empty).
- '_' matches any one character.

`SIMILAR TO `allows for *regular expression matching* but it is not supported across all systems as many have their own syntax instead.
→ In the SQL standard but not all systems support it.
→ Other systems also support POSIX-style regular expressions.

```sql
SELECT * FROM enrolled AS e
WHERE e.cid LIKE '15-%';

SELECT * FROM student AS s
WHERE s.login LIKE '%@c_';

SELECT * FROM student AS s
WHERE login SIMILAR TO
'[\w]{3}@cs';

```

#### String Functions
SQL-92 defines string functions. Many database systems implement other functions in  addition to those in the standard. 
Can be used in either output and predicates:
So "output" simply means: the expressions returned by the SELECT clause.

Examples of standard string functions include `SUBSTRING(S, B, E)` and `UPPER(S)` .

```sql
--- functions can appear in the SELECT clause
--- UBSTRING(name,1,5): produces the value that appears in the final result.
SELECT SUBSTRING(name,1,5) AS abbrv_name
FROM student WHERE sid = 53688

SELECT * FROM student AS s
WHERE UPPER(s.name) LIKE 'KAN%'
```
#### Concatenation:
Two vertical bars (“ || ”) will concatenate two or more strings together into a single string (but different systems might use a different symbol).

```sql
---SQL 92
SELECT name FROM student
WHERE login = LOWER(name) || '@cs'

--- MSSQL
SELECT name FROM student
WHERE login = LOWER(name) + '@cs'

--- MySQL
SELECT name FROM student
WHERE login = CONCAT(LOWER(name), '@cs')
```

### **6** **Date and Time**

Databases generally want to keep track of dates and time, so SQL supports operations to manipulate DATE and TIME attributes. 
These can be used as either outputs or predicates.

Specific syntax for date and time operations can vary wildly across systems.

Ex: 
UTC timezone
```sql
SELECT NOW();

SELECT CURRENT_TIMESTAMP();

SELECT CURRENT_TIMESTAMP;
```

Demo: Compute the number of days since the beginning of the year.

```sql
--------------POSTGRES
SELECT CAST('2025-08-27' AS DATE) - CAST('2025-01-01' AS DATE) AS days;
-- same as
SELECT '2025-08-27'::DATE - '2025-01-01'::DATE AS days;
--- days: 238

---------------MySQL
--- wrong way
SELECT CAST('2025-08-27' AS DATE) - CAST('2025-01-01' AS DATE) AS days; 
--- days: 726
SELECT CAST(DATE('2025-08-27') AS UNSIGNED); --- 20250827
SELECT CAST(DATE('2025-08-27') AS UNSIGNED) - CAST(DATE('025-01-01') AS UNSIGNED) AS days; 

---correct way
SELECT DATEDIFF(DATE('2025-08-27'), DATE('025-01-01')) AS days;

--------------SQLSERVER
SELECT DATEDIFF(DAY, '2025-01-01', GETDATE()) AS days;
```

---

<!-- page 5 -->





### **7** **Output Redirection**

Instead of having the result a query returned to the client (e.g., terminal), you can tell the DBMS to store the results into another table. You can then access this data in subsequent queries.

- **New Table:** Store the output of the query into a new (permanent) table.

```sql
---SQL 92
SELECT DISTINCT cid 
INTO CourseIds 
FROM enrolled;


---MySQL
CREATE TABLE CourseIds (
SELECT DISTINCT cid FROM enrolled);


--Postgres
SELECT DISTINCT cid
INTO TEMPORARY CourseIds  --- When close the clent, the TEMPORARY go away
FROM enrolled;
```

- **Existing Table:** Store the output of the query into a table that already exists in the database. The target table must have the same number of columns with the same types as the target table, but the names of the columns in the output query do not have to match.
  
```sql
INSERT INTO CourseIds 
( SELECT DISTINCT cid 
FROM enrolled);
```

----

### **8** **Output Control**

#### ORDER BY
Since results SQL are unordered, we must use the `ORDER BY` clause to impose a sort on tuples:

```sql
ORDER BY <column*> [ASC|DESC]
--- Sort tuples by the values in one or more of their columns.


SELECT sid, grade 
FROM enrolled 
WHERE cid = '15-721' 
ORDER BY grade;
```

The default sort order is ascending ( `ASC` ). We can manually specify DESC to reverse the order:

```sql
SELECT sid, grade 
FROM enrolled 
WHERE cid = '15-721' 
ORDER BY grade DESC ;
```

We can use multiple ORDER BY clauses to break ties or do more complex sorting:

```sql
SELECT sid, grade 
FROM enrolled 
WHERE cid = '15-721' 
ORDER BY grade DESC , sid ASC ;
```

We can also use any arbitrary expression in the ORDER BY clause:
```sql
SELECT sid 
FROM enrolled 
WHERE cid = '15-721' 
ORDER BY UPPER (grade) DESC , sid + 1 ASC ;
```

#### LIMIT

By default, the DBMS will return all of the tuples produced by the query. Many systems provide their own syntax for specifying how to get a set number of the first results from the output, but a common one is the `LIMIT` clause:

```sql
SELECT sid, name 
FROM student 
WHERE login LIKE '%@cs' 
LIMIT 10;
```

We can also provide an offset to return a range in the results:
```sql
SELECT sid, name 
FROM student 
WHERE login LIKE '%@cs' 
LIMIT 20 OFFSET 10;
```

#### FETCH ?
```sql
FETCH {FIRST|NEXT} <#> ROWS
OFFSET <#> ROWS
--- Limit # of tuples returned in output.
--- Can set an offset to return a “range”


SELECT sid, name FROM student
WHERE login LIKE '%@cs'
FETCH FIRST 10 ROWS ONLY;

SELECT sid, name FROM student
WHERE login LIKE '%@cs'
ORDER BY gpa
OFFSET 5 ROWS
FETCH FIRST 5 ROWS WITH TIES;

--- MSSQL
SELECT TOP 10 sid, name
FROM student
WHERE login LIKE '%@cs';
```



Unless we use an ORDER BY clause with a LIMIT , the DBMS may produce different tuples in the result on each invocation of the query because the relational model does not impose an ordering.

SQL also allows you to store query results into a different table with the `INTO` keyword (some systems even allow redirection into a temporary table with `INTO TEMPORARY` ).




---

<!-- page 6 -->

### **9** **Window Functions**

A window function performs “sliding” calculation across a set of tuples that are related. 
Window functions are similar to aggregations, but tuples are not collapsed into a singular output tuple.

```sql
--- FUNC-NAME(...) : Aggregation Functions/Special Functions
--- OVER(...):  How to slice up data, Can also sort tuples
SELECT FUNC-NAME(...) OVER (...)   
FROM tableName;

```
The conceptual execution for window functions can be imagined as such ( note that not all window functions will behave like this ):

1. The table is partitioned

2. Each partition is sorted (if there is an ORDER BY clause)

3. For each record, it creates a window spanning multiple records

4. Finally the output is computed for each window


**Functions:** 

The window function can be any of the aggregation functions that we discussed above. 
There are also also special window functions:

1. `ROW_NUMBER()` : The number of the current row.

2. `RANK` : The order position of the current row.
```sql
SELECT *, ROW_NUMBER() OVER () AS row_num
FROM enrolled;

```


**Grouping:** 
The `OVER` clause specifies how to group together tuples when computing the window function.

Use `PARTITION BY` to specify group.

```sql
SELECT cid, sid, 
    ROW_NUMBER() OVER (PARTITION BY cid) 
    FROM enrolled 
    ORDER BY cid;
```

We can also put an ORDER BY within OVER to ensure a deterministic ordering of results even if database changes internally.

```sql
SELECT *, 
    ROW_NUMBER() OVER ( ORDER BY cid) 
    FROM enrolled 
ORDER BY cid;
```
**IMPORTANT** : The DBMS computes RANK after the window function sorting, whereas it computes ROW NUMBER before the sorting.

Example: Find the student with the second highest grade for each course .

```sql
SELECT * FROM ( 
    SELECT *, RANK() OVER (PARTITION BY cid --- Group tuples by cid then sort by grade
    ORDER BY grade ASC ) AS rank 
    FROM enrolled) AS ranking 
    WHERE ranking.rank = 2;
```


Note that we order by ASC because the grades are A, B, C instead of number grades.

### **10** **Nested Queries**

Nested queries invoke queries inside of other queries to execute more complex logic within a single query. Nested queries are often difficult to optimize.

The scope of the outer query is included in an inner query (i.e. the inner query can access attributes from outer query). The opposite is not true.

Inner queries can appear in almost any part of a query:

1. SELECT Output Targets:

```sql
SELECT ( SELECT 1) AS one 
FROM student;
```


2. FROM Clause:

```sql
SELECT name 
FROM student AS s, ( SELECT sid FROM enrolled) AS e WHERE s.sid = e.sid;
```


3. WHERE Clause:
```sql
SELECT name   ---Outer query
FROM student 
WHERE sid IN ( SELECT sid FROM enrolled ); ---inner query
```

Example: Get the names of students that are enrolled in ‘15-445’ .
```sql
SELECT name from student
WHERE ... --- sid in the set of people that take 15-445

SELECT name from student
WHERE ...
SELECT sid from enrolled
WHERE cid= '15-445'



SELECT name FROM student 
WHERE sid IN
 ( SELECT sid FROM enrolled 
   WHERE cid = '15-445' );
```
Note that sid has a different scope depending on where it appears in the query.

Example: Find student record with the highest id that is enrolled in at least one course .

```sql
SELECT sid, name FROM student
WHERE ... --- Is the highest enrolled sid


SELECT sid, name FROM student
WHERE sid=
   SELECT MAX(sid) FROM enrolled


SELECT sid, name FROM student
WHERE sid IN(
    SELECT MAX(sid) FROM enrolled
);


SELECT sid, name FROM student
WHERE sid IN (
SELECT sid FROM enrolled
ORDER BY sid DESC FETCH FIRST 1 ROW ONLY
);


SELECT student.sid, name 
FROM student JOIN 
( SELECT MAX (sid) AS sid FROM enrolled) AS max_e 
ON student.sid = max_e.sid;
```

#### Nested Query Results Expressions:

- `ALL` : Must satisfy expression for all rows in sub-query.

- `ANY` : Must satisfy expression for at least one row in sub-query.

- `IN` : Equivalent to =ANY() .

- `EXISTS` : At least one row is returned.

Example: Find all courses that have no students enrolled in it .

```sql
SELECT * FROM course
WHERE ... --- with no tuples in the errolled table

SELECT * FROM course
WHERE NOT EXISTS(
    --- tuples in the errolled table
)


SELECT * FROM course 
WHERE NOT EXISTS 
( SELECT * FROM enrolled 
WHERE course.cid = enrolled.cid );
```
### **11** **Lateral Joins**

The `LATERAL` operator allows a nested query to reference attributes in other nested queries that precede it. You can think of lateral joins like a `for` loop that allows you to invoke another query for each tuple in a table.

```sql
SELECT * FROM
(SELECT 1 AS x) AS t1,
LATERAL (SELECT t1.x+1 AS y) AS t2;

for x in [1]:
  for y in [x+1]:
     print(x,y)

```

Example: Calculate the number of students enrolled in each course and the average GPA. Sort by enrollment count in descending order. .

Once we have gotten the course records, we can think of this query like below. For each course:

- Compute the number of enrolled students in this course

- Compute the average GPA of the enrolled students in this course

```sql
SELECT * FROM course AS c LATERAL 
( SELECT COUNT (*) AS cnt FROM enrolled WHERE enrolled.cid = c.cid) AS t1, 
LATERAL ( SELECT AVG (gpa) AS avg FROM student AS s JOIN enrolled AS e ON s.sid = e.sid WHERE e.cid = c.cid) AS t2;
```


### **12** **Common Table Expressions**

Common Table Expressions (CTEs) are an alternative to windows or nested queries when writing more complex queries. They provide a way to write auxiliary statements for use in a larger query. A CTE can be thought of as a temporary table that is scoped to a single query.

The `WITH` clause binds the output of the inner query to a temporary table with the same name.
```sql
WITH cteName (col1, col2) AS (
SELECT 1, 2
)
SELECT col1 + col2 FROM cteName
```

Example: Generate a CTE called cteName that contains a single tuple with a single attribute set to “1”. Select

all attributes from cteName .

WITH cteName AS ( SELECT 1 ) SELECT * FROM cteName;

We can bind output columns to names before the AS :

WITH cteName (col1, col2) AS ( SELECT 1, 2 ) SELECT col1 + col2 FROM cteName;

A single query may contain multiple CTE declarations:

WITH cte1 (col1) AS ( SELECT 1), cte2 (col2) AS ( SELECT 2)

SELECT * FROM cte1, cte2;

Adding the RECURSIVE keyword after WITH allows a CTE to reference itself. This enables the implementa-

tion of recursion in SQL queries. With recursive CTEs, SQL is provably Turing-complete, implying that it

is as computationally expressive as more general purpose programming languages (ignoring the fact that

it is a bit more cumbersome).

Example: Print the sequence of numbers from 1 to 10 .

WITH RECURSIVE cteSource (counter) AS ( ( SELECT 1 )




**UNION**

( SELECT counter + 1 FROM cteSource WHERE counter < 10 ) ) SELECT * FROM cteSource;


# QA

## Q1: What are predicates, output, clause in SQL ?

Excellent question. These three terms—**clause**, **predicate**, and **output**—appear throughout SQL, relational algebra, and query optimization. Understanding them will also help you understand how a SQL parser builds an AST.

Let's start from the big picture.

---

### A SQL Query is Composed of Clauses

A **clause** is a major component (or section) of a SQL statement.

For example:

```sql
SELECT s.name, s.gpa
FROM student AS s
WHERE s.gpa > 3.5
ORDER BY s.gpa DESC;
```

This query consists of four clauses:

```text
SELECT s.name, s.gpa        <-- SELECT clause
FROM student AS s           <-- FROM clause
WHERE s.gpa > 3.5           <-- WHERE clause
ORDER BY s.gpa DESC         <-- ORDER BY clause
```

Think of clauses as the chapters of a book.

```
SQL Statement
│
├── SELECT clause
├── FROM clause
├── WHERE clause
├── GROUP BY clause
├── HAVING clause
├── ORDER BY clause
└── LIMIT clause
```

Each clause has a different responsibility.

| Clause   | Purpose                                         |
| -------- | ----------------------------------------------- |
| SELECT   | What columns or expressions should be returned? |
| FROM     | Where does the data come from?                  |
| WHERE    | Which rows should be kept?                      |
| GROUP BY | How should rows be grouped?                     |
| HAVING   | Which groups should be kept?                    |
| ORDER BY | How should the results be sorted?               |
| LIMIT    | How many rows should be returned?               |

---

### What is the Output?

When the slides say

> "Functions can be used in output"

they mean

> **functions can appear in the SELECT clause.**

For example

```sql
SELECT SUBSTRING(name,1,5)
FROM student;
```

The function

```sql
SUBSTRING(name,1,5)
```

produces the value that appears in the final result.

Suppose

| name      |
| --------- |
| Katherine |
| Michael   |

Output becomes

| SUBSTRING(name,1,5) |
| ------------------- |
| Kathe               |
| Micha               |

The function affects **what the user sees**.

So "output" simply means

> the expressions returned by the SELECT clause.

---

Another example

```sql
SELECT
    UPPER(name),
    age + 1
FROM student;
```

Output

| UPPER(name) | age+1 |
| ----------- | ----- |
| ALICE       | 21    |
| BOB         | 23    |

These expressions are evaluated for every returned row.

---

### What is a Predicate?

A predicate is one of the most important concepts in SQL.

#### Formal Definition

A predicate is an expression that evaluates to

* TRUE
* FALSE
* UNKNOWN (because SQL has NULL)

In other words,

> A predicate asks a yes/no question.

---

Example

```sql
salary > 5000
```

Suppose

| salary |
| ------ |
| 3000   |
| 7000   |

Evaluation

```
3000 > 5000
↓

FALSE

7000 > 5000
↓

TRUE
```

That expression is a predicate.

---

Another predicate

```sql
age < 18
```

Another

```sql
name LIKE 'A%'
```

Another

```sql
gpa BETWEEN 3.0 AND 4.0
```

Another

```sql
sid = 100
```

All of these produce a Boolean result.

---

### Why is it Called a Predicate?

The term comes from **predicate logic** in mathematics.

In logic,

```
P(x)
```

means

> "x satisfies property P."

Example

```
Tall(Person)
```

is a predicate.

For Alice

```
Tall(Alice)

↓

TRUE
```

For Bob

```
Tall(Bob)

↓

FALSE
```

SQL borrowed this terminology.

---

### Predicates are Usually Used in WHERE

Example

```sql
SELECT *
FROM student
WHERE gpa > 3.5;
```

The predicate is

```sql
gpa > 3.5
```

The database checks every row.

```
Student 1

gpa = 3.2

3.2 > 3.5 ?

FALSE

Discard
```

```
Student 2

gpa = 3.8

3.8 > 3.5 ?

TRUE

Keep
```

---

### Functions Inside Predicates

Now your slide makes sense.

Example

```sql
SELECT *
FROM student
WHERE UPPER(name) LIKE 'KAN%';
```

The predicate is

```sql
UPPER(name) LIKE 'KAN%'
```

Let's evaluate it.

Suppose

| name   |
| ------ |
| Kaneko |
| kan    |
| Kate   |
| Bob    |

First

```
UPPER(name)
```

becomes

| name   | UPPER(name) |
| ------ | ----------- |
| Kaneko | KANEKO      |
| kan    | KAN         |
| Kate   | KATE        |
| Bob    | BOB         |

Then evaluate

```
LIKE 'KAN%'
```

| UPPER(name) | Matches? |
| ----------- | -------- |
| KANEKO      | TRUE     |
| KAN         | TRUE     |
| KATE        | FALSE    |
| BOB         | FALSE    |

Only the TRUE rows remain.

---

### Functions in Output vs Functions in Predicates

This is exactly what your slide means.

#### Function in Output

```sql
SELECT SUBSTRING(name,1,5)
FROM student;
```

The function computes the displayed value.

```
Student Row

↓

SUBSTRING()

↓

Output
```

---

#### Function in Predicate

```sql
WHERE UPPER(name) LIKE 'KAN%'
```

The function helps decide whether the row survives.

```
Student Row

↓

UPPER()

↓

LIKE

↓

TRUE/FALSE

↓

Keep or Discard
```

---

### How Does the SQL Parser See This?

The parser builds an AST.

Example

```sql
SELECT SUBSTRING(name,1,5)
FROM student
WHERE UPPER(name) LIKE 'KAN%';
```

AST (simplified):

```text
SelectStmt
├── SELECT
│      └── FunctionCall
│             ├── SUBSTRING
│             ├── Column(name)
│             ├── Literal(1)
│             └── Literal(5)
│
├── FROM
│      └── Table(student)
│
└── WHERE
       └── LIKE
            ├── FunctionCall
            │      └── UPPER
            │             └── Column(name)
            └── Literal('KAN%')
```

Notice something interesting:

* `SUBSTRING()` appears in the **SELECT clause**, contributing to the output.
* `UPPER()` appears inside the **WHERE clause**, contributing to the predicate.

The parser doesn't treat these functions differently—they're both function-call nodes in the AST. Their meaning comes from **where they appear** in the tree.

---

### Summary

| Term          | Meaning                                                                            | Example                                                              |
| ------------- | ---------------------------------------------------------------------------------- | -------------------------------------------------------------------- |
| **Clause**    | A major section of a SQL statement                                                 | `SELECT`, `FROM`, `WHERE`, `GROUP BY`, `ORDER BY`                    |
| **Output**    | The values returned to the user, defined by the `SELECT` clause                    | `SELECT UPPER(name), age`                                            |
| **Predicate** | A Boolean expression (`TRUE`, `FALSE`, or `UNKNOWN`) used to filter rows or groups | `WHERE gpa > 3.5`, `name LIKE 'A%'`, `salary BETWEEN 5000 AND 10000` |

### An Important Compiler Perspective

From the perspective of a database compiler:

* **Clauses** define the high-level structure of the SQL grammar (`SELECT`, `FROM`, `WHERE`, etc.).
* **Expressions** (columns, literals, arithmetic, function calls) appear inside clauses.
* **Predicates** are a special kind of expression whose result is Boolean, so they can control filtering.
* **Output expressions** are expressions in the `SELECT` clause that determine what values are returned.

This distinction is reflected directly in the AST. For example, a `SelectStmt` node has children representing different clauses, and the `WHERE` child contains a predicate expression tree, while the `SELECT` child contains output expression trees. Understanding this relationship makes it much easier to follow how a SQL parser, binder, and optimizer transform a query internally.
