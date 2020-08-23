TRUNCATE TABLE tasks RESTART IDENTITY CASCADE;

INSERT INTO tasks (created,description,priority)
                     VALUES('2000-01-01 12:00:00','description',1);

UPDATE tasks SET created = '2000-01-01 12:10:00', description = 'updated description', priority = 5 WHERE id = 1;