# Lecture 7
- What to cover:
    - Flat-File Database
    - Relational Databases
    - SELECT
    - INSERT
    - DELETE
    - UPDATE
    - IMDb
    - JOINs
    - Indexes
    - Using SQL in Python
    - SQL Injection Attacks

## Flat-File Database
- Data's described in patterns of rows and columns. 
- Flat-file database: All data is stored in a single table represented by a text file.
- All data, is stored row by row. Each column is separated by a comma or another value.
- In favorite.py:
    - The csv library is imported
    - Create a `reader` that holds the result of `csv.reader(file)`
    - The csv.reader function reads each row from the file, and stores the result in `reader`.
    - `print(row[1])`, will print the language from the `favorites.csv` file.
- Improved favorites.py:
    - `favorites` is stored and then printed.
    - `next()` function skips to the next line of reader.

- Issue: Column is changed.
    - Index by the keys of list.

- To count the number of favorite languages, 
    - `fav_lang.py`
    - Each language is counted using `if` statements
    - Check for equality using `==` sign.

- Python allows the use of a dictionary to count the `counts` of each language.
    - The value in `counts` with the key `favorite` is incremented when it exits already. If it doesn't exits, we define `counts[favorite]` and set it to 1. 
    - The formatted string has been improved to present the `counts[favorite]`.

- Python allows sorting `counts`.
    - Sort has built-in parameters. We can leverage them to do somethings.
    - Arguments passed to sorted:
        - `key` arg allows you to tell Python the method you wish to use to sort items.
        - `count.get` sortes the values
        - `reverse=True` tells `sorted` to sort from largest to smallest.

- Python has `collections` library, from which we can import `Counter`. 
    - `Counter` allows one to access the counts of each language without the headaches of all the `if` statements seen in previous code.
    - In `fav_coll_count.py`:
        - count = Counter(), enables the user of imported `Counter` class from collections.

## Relational Databases
- Google, X, and Meta use relational databases to store their information at scale

- Relational databases store data in rows and columns in structures called **tables**
- SQL allows for four types of commands:
```
    Create
    Read
    Update
    Delete
```
- The Four operations are called CRUD.
- Creating a database:
    CREATE TABLE table (column tye, ...);
    
    1. Create database:
        $ sqlite3 favorites.db

    2. Put sqlite into csv mode
        sqlite> .mode csv

    3. Import data from csv
        sqlite> .import favorites.csv favorites

    4. See structure of database
        sqlite> .schema

    5. Read items from table
        SELECT columns FROM table
        sqlite> SELECT * FROM favorites; # Prints every row in `favorites`

    6. Get a subset of data 
        sqlite> SELECT language FROM favorites

- SQL supports many commands to access data, including:
    AVG
    COUNT
    DISTINCT
    LOWER
    MAX
    MIN
    UPPER

- E.g:
    `SELECT COUNT(*) FROM favorites;`
    `SELECT DISTINCT language FROM favorites;` to get a list of the individual languages within the database.
    `SELCT COUNT(DISTINCT language) FROM favorites;` to get a count of those.

- SQL offers additional commands that can be utilized in queries:
```
    WHERE       -- adding A Boolean expression to filter our data
    LIKE        -- filtering responses more loosely
    ORDER BY    -- ordering responses
    LIMIT       -- limiting the number of responses
    GROUP BY    -- grouping responses together
```
- Note: 
    - Comments are written in SQL using `--`.

## SELECT
- E.g:
```
> SELECT COUNT(*) FROM favorites WHERE language = 'C';
```

- To narrow results:
```
> SELECT COUNT(*) FROM favorites WHERE language = 'C' AND problem = 'Hello, World';
```

- Create a temporary table that shows language and count
```
> SELECT language, COUNT(*) FROM favorites GROUP BY language;
```

## INSERT
- You can also INSERT into a SQL database using the form:
```
> INSERT INTO table (column...) VALUES(value);
```
- E.g:
```
> INSERT INTO favorites (language, problem) VALUES ('SQL', 'Fiftyville');
```
- Verify the addition of this favorite by executing:
```
> SELECT * FROM favorites;
```

## DELETE
- `DELETE` allows you to delete parts of your data.
- E.g:
```
> DELETE FROM favorites WHERE Timestamp IS NULL;
```
- This deletes any record where the `Timestamp is NULL`

## UPDATE
- `UPDATE` command updates data.
