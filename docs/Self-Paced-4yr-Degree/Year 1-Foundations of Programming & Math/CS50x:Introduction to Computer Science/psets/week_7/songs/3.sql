-- List top 5 longest songs (by duration)
SELECT name FROM songs ORDER BY duration_ms DESC LIMIT 5;
