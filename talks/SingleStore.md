
https://www.singlestore.com/

Absolutely. These slides are introducing the **two major database workload types**, and then positioning **SingleStore** as a hybrid system.

## Agenda

* Two Types of Database Workloads
* SingleStore as a hybrid option
* Modern Cloud Systems

---
# Two Types of Database Workloads
## OLTP: Online Transaction Processing

* Characterized by a large number of transactions
* Small reads and writes
* High concurrency, short SLAs, ACID
* Think: running applications
* Row oriented data structures: B-Trees, SkipLists
* PostgreSQL, Oracle, SQL Server

---

## OLAP: Online Analytics Processing

* Characterized by large queries and bulk processing
* Queries take minutes, hours or days
* Terabytes or petabytes of data
* Think: large reports, analytics, dashboards
* Column oriented data storage
* Snowflake, Click House, Redshift, Big Query, Vertica

---


# SingleStore as a hybrid option
## What is SingleStore?

![Image](https://images.openai.com/static-rsc-4/bGT2-OmUX2bYENJBYnNPV_EF6Ex6MBiUQcnEmPjEfses4_l596046s1bPfmSY5haGdIMhhdoppNcnNsqqYTSLrwpnlx-TB9udGVXr2oE3_d3i4MGQgFLT24sK_PA8JMQ_ci8MbJEWrOGHmuYMqoqfH0pc14w3dF_IkBMxNcH6gMkeTDrLmM8tSilhyvcmctl?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/-4nCd1X1PnEfwXXZzLnjtR9AtVUpdWIH1nWKAaKNjrXov9GJzwj1I_3K46xUOYtzv8W4IgfP7dc0komykiqAf1UhjiZk1k-t76Vb-wpTktL4Iaj1egpqZuCWZhJ9Vmsj-CdR-3RW92Ww7S_7F_H7Jq_UR1bONa7Bzc-rxaonxtxyw9ueTjvBrRKBg18hbwy2?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/-LpuPwX6Ro8FMMNlJGayQguAiMf1npNC0H_-RRKU6sP_6R-j52n9PIK2iio0fxZZcZwBg7clYU7LoSMskkmFjX84k9mk91M_riOCOi8ZiA3Xziegm03PkNh0mlZbzZtn9AHfuV6PpmyqG6PjL2zNiCkD1oocNgQJWN_ym0Jc0g8ZTuI4ztIQ_QNaO3dkptHy?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/6fQDJYy6AJgJi3WnsHFptpHhk5Ijvc1XFMj-7qFft5EDV_nc90FJqAxPtbc8e4eyBbYMRHXGf1rtt4p9b4XZjV62VP2KBy8xuC6DoF45E_RV3ooSewb9WEIIeOnD-oGmcvI2U71YIYlUGzOYauslgYSH0RvegdsCUo_jvu1a4i1_lRNzAgZnlOz4gZmtf5z4?purpose=fullsize)

**SingleStore** is a distributed SQL database designed to support **both OLTP and OLAP workloads in the same database system**.

Historically, it was called **MemSQL**. It was renamed SingleStore in 2020.

The important idea for your lecture is:

> **SingleStore tries to combine the strengths of row-oriented OLTP databases and column-oriented OLAP databases into one distributed database.**

That's why the slide calls it:

> **"SingleStore as a hybrid option"**

---

## 3. Why do we need a hybrid database?

Start with the traditional architecture.

Suppose an e-commerce company has:

```text
                 Application
                      │
             ┌────────┴────────┐
             ↓                 ↓
           OLTP              OLAP
             │                 │
       PostgreSQL          Snowflake
             │                 │
       transactions         analytics
```

The OLTP database handles:

```sql
INSERT INTO orders ...
UPDATE accounts ...
SELECT order WHERE id = 123;
```

while the analytical database handles things like:

```sql
SELECT
    product_category,
    SUM(revenue)
FROM sales
WHERE order_date >= ...
GROUP BY product_category;
```

These workloads have **very different access patterns**.

---

## 4. Why OLTP likes row storage

Consider:

```text
orders

id | customer | product | price | status
---+----------+---------+-------+--------
1  | Alice    | A       | 100   | paid
2  | Bob      | B       | 200   | paid
3  | Carol    | C       | 300   | pending
```

A row-oriented storage layout is approximately:

```text
Row 1:
[1][Alice][A][100][paid]

Row 2:
[2][Bob][B][200][paid]

Row 3:
[3][Carol][C][300][pending]
```

If you execute:

```sql
SELECT *
FROM orders
WHERE id = 2;
```

you want essentially **one complete row**.

That's very natural for row storage.

---

## 5. Why OLAP likes column storage

Now imagine:

```sql
SELECT SUM(price)
FROM orders;
```

You don't care about:

```text
id
customer
product
status
```

You only need:

```text
price
```

A column store looks more like:

```text
id:
[1][2][3][4][5]...

customer:
[Alice][Bob][Carol]...

product:
[A][B][C]...

price:
[100][200][300][...]

status:
[paid][paid][pending]...
```

Now:

```sql
SELECT SUM(price)
```

can scan essentially just:

```text
price column
     ↓
[100][200][300][...]
     ↓
vectorized processing
     ↓
SUM
```

This is much more efficient for analytical workloads.

---

## 6. The fundamental tension

So you have:

```text
                Database Storage

        OLTP                    OLAP
          │                       │
          ↓                       ↓
      Row-oriented           Column-oriented
          │                       │
          ↓                       ↓
     point queries            large scans
     small updates            aggregations
     high concurrency          analytics
```

Historically, this often leads to:

```text
                Application
                     │
          ┌──────────┴──────────┐
          ↓                     ↓
       OLTP DB               OLAP DB
     PostgreSQL             Snowflake
          │                     │
          └─────── ETL ─────────┘
                    ↓
              data warehouse
```

The problem is that you now have **two systems**.

---

## 7. The ETL problem

Imagine an order is created:

```text
10:00:00
customer buys product
```

It goes into:

```text
OLTP PostgreSQL
```

But the analytics system may only see it after:

```text
10:05
10:30
1 hour later
...
```

depending on the pipeline.

So you get:

```text
OLTP
  │
  │ transaction
  ↓
PostgreSQL
  │
  │ ETL / CDC
  ↓
Data Warehouse
  │
  ↓
Analytics
```

There is:

* data movement
* ETL/ELT infrastructure
* duplicated data
* synchronization complexity
* potential data freshness problems

This is one motivation for systems such as SingleStore.

---

## 8. SingleStore's basic idea

SingleStore tries to provide:

```text
                  SingleStore

        ┌─────────────────────────┐
        │       SQL Database      │
        │                         │
        │      OLTP + OLAP        │
        │                         │
        └─────────────────────────┘
```

Instead of:

```text
PostgreSQL → ETL → Snowflake
```

you can potentially have:

```text
             SingleStore
             /        \
          OLTP        OLAP
```

using different storage/processing techniques appropriate for each workload.

---

## 9. This connects directly to the storage topics you're studying

This is probably **why your lecture puts SingleStore immediately after OLTP/OLAP**.

You have just learned:

### OLTP

```text
Row-oriented
     ↓
B-Tree
     ↓
point lookups
     ↓
small reads/writes
     ↓
high concurrency
```

### OLAP

```text
Column-oriented
     ↓
large sequential scans
     ↓
compression
     ↓
vectorized execution
     ↓
aggregation
```

Then:

### SingleStore

```text
                SingleStore
                     │
             ┌───────┴───────┐
             ↓               ↓
           OLTP             OLAP
             │               │
          rowstore       columnstore
             │               │
        transactions     analytics
```

That's the conceptual connection.

---

## 10. SingleStore is not simply "a database that stores everything in both formats"

There's a subtle but important point here.

A hybrid database doesn't necessarily mean:

> "Every row is duplicated into a row store and a column store."

Rather, modern systems can use **different storage representations, indexes, execution strategies, and data layouts** depending on the workload and table/index design.

SingleStore historically emphasized:

* distributed SQL
* in-memory processing
* rowstore
* columnstore
* parallel execution
* horizontal scaling

So the database can support both transactional and analytical access patterns.

---

## 11. A concrete example

Suppose you have:

```text
orders
```

with 1 billion rows.

### Transaction

A customer places an order:

```sql
INSERT INTO orders
VALUES (...);
```

You want:

```text
low latency
high concurrency
ACID
```

That's OLTP.

---

### Analytics

The business asks:

```sql
SELECT
    country,
    product_category,
    SUM(amount)
FROM orders
WHERE order_date >= '2026-01-01'
GROUP BY country, product_category;
```

This potentially touches **millions/billions of rows**.

You want:

```text
column scanning
compression
parallel execution
vectorization
aggregation
```

That's OLAP.

A hybrid system attempts to support both without requiring you to maintain completely separate database systems.

---

## 12. One thing I would correct in the slide's wording

The slide says:

> **"Row oriented data structures: B-Trees, SkipLists"**

and:

> **"Column oriented data storage"**

Don't interpret this as:

```text
OLTP = B-Tree
OLAP = column store
```

These are different dimensions.

For example:

```text
                    Storage Layout
                    /             \
                  Row           Column
                   │               │
              B-Tree index     Zone maps
              Hash index       bitmap
              etc.
```

A **B+ tree is an index data structure**, whereas **row/column orientation describes the physical organization of table data**.

That's an important distinction when you get deeper into DBMS internals.

---

## 13. The big picture of this lecture

You can organize the three slides like this:

```text
                     DATABASE WORKLOADS
                            │
             ┌──────────────┴──────────────┐
             ↓                             ↓
           OLTP                           OLAP
             │                             │
      many small operations         few large queries
      high concurrency              bulk processing
      low latency                   long-running
      ACID                          analytics
             │                             │
             ↓                             ↓
        row-oriented                 column-oriented
        B-Trees                      compression
        indexes                      vectorization
             │                             │
             ↓                             ↓
      PostgreSQL                    Snowflake
      Oracle                        ClickHouse
      SQL Server                    BigQuery
             │                             │
             └──────────────┬──────────────┘
                            ↓
                     Hybrid Systems
                            │
                            ↓
                       SingleStore
```

And this sets up the next topic nicely:

> **Modern cloud databases increasingly separate compute from storage and distribute both horizontally.**

That's likely why your agenda goes:

**OLTP/OLAP → SingleStore → Modern Cloud Systems.**

The key lesson isn't really "learn SingleStore." It's understanding **why database architecture diverges for transactional vs analytical workloads, and why modern systems are trying to bridge that divide.**
