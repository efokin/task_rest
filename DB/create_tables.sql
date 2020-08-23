CREATE TABLE tasks (
        id serial PRIMARY KEY,
        created timestamp,
        description varchar(254),
        priority smallint
);

CREATE TABLE subtasks (
        id serial PRIMARY KEY,
        task_id integer,
        created timestamp,
        description character(254),
        priority smallint,
            CONSTRAINT fk_task
                FOREIGN KEY(task_id)
	            REFERENCES tasks(id)
);

