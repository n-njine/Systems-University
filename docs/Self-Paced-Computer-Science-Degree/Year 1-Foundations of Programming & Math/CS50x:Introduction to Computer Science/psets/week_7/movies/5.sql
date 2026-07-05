-- Get all Harry Potter movies in chronological order
SELECT title, year
FROM movies
WHERE title LIKE 'Harry Potter and the%'
ORDER BY year, title;
