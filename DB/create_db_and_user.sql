CREATE USER task_tracker_user PASSWORD 'password';

CREATE DATABASE tasks OWNER task_tracker_user;

GRANT ALL PRIVILEGES ON DATABASE tasks TO task_tracker_user;