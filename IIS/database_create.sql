-- Drop all tables
DROP TABLE IF EXISTS VOTED CASCADE;
DROP TABLE IF EXISTS ANSWERS CASCADE;
DROP TABLE IF EXISTS QUESTIONS CASCADE;
DROP TABLE IF EXISTS CATEGORIES CASCADE;
DROP TABLE IF EXISTS REGISTERED_COURSES CASCADE;
DROP TABLE IF EXISTS COURSES CASCADE;
DROP TABLE IF EXISTS USERS CASCADE;

-- Drop all damains
DROP DOMAIN IF EXISTS USER_ROLE;
DROP DOMAIN IF EXISTS VOTE_VALUE;

-- extension needed for pasword hashing
CREATE EXTENSION IF NOT EXISTS pgcrypto;

--Create custom user domain
CREATE DOMAIN USER_ROLE AS VARCHAR NOT NULL CHECK (value = 'Admin' OR value = 'User' OR value = 'Moderator');
CREATE DOMAIN VOTE_VALUE AS VARCHAR NOT NULL CHECK (value = 'upvote' OR value = 'downvote');

-- Create tables
CREATE TABLE USERS (
   user_id SERIAL PRIMARY KEY,
   username VARCHAR(50) UNIQUE NOT NULL,
   pswd VARCHAR(200) NOT NULL,
   role USER_ROLE NOT NULL,
   first_login TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
   most_recent_login TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
   points INT DEFAULT 0,
   study_start INT,
   preferred_badge INT DEFAULT NULL,
   email VARCHAR(200) UNIQUE NOT NULL,
   CHECK (study_start BETWEEN 1950 AND 2050),
   CHECK (preferred_badge BETWEEN 0 AND 9)
);

CREATE TABLE COURSES (
   course_id SERIAL PRIMARY KEY,
   course_name VARCHAR(50) UNIQUE NOT NULL,
   mentor_id INT NULL,
   creation_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
   course_description VARCHAR(1000),
   is_approved BOOLEAN DEFAULT FALSE,
   last_activity TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
   FOREIGN KEY (mentor_id) REFERENCES USERS (user_id) ON DELETE SET NULL -- maybe ON DELETE CASCADE if we want to delete course upon deletion of mentor
);

CREATE TABLE CATEGORIES(
   category_id SERIAL PRIMARY KEY,
   category_name VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE QUESTIONS (
   question_id SERIAL PRIMARY KEY,
   title VARCHAR(200),
   course_id INT NULL,
   user_id INT NULL,     
   category_id INT NULL,
   is_closed BOOLEAN DEFAULT FALSE,
   text_data VARCHAR(5000),
   votes INT DEFAULT 0,
   creation_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
   last_activity TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
   image_name text DEFAULT NULL, 
   image_data bytea DEFAULT NULL,
   FOREIGN KEY (user_id) REFERENCES USERS (user_id) ON DELETE SET NULL,
   FOREIGN KEY (course_id) REFERENCES COURSES (course_id) ON DELETE CASCADE,
   FOREIGN KEY (category_id) REFERENCES CATEGORIES (category_id) ON DELETE SET NULL
);

CREATE TABLE ANSWERS (
   answer_id SERIAL PRIMARY KEY,
   parent_question INT NOT NULL,
   parent_answer INT DEFAULT NULL,
   user_id INT NULL,
   text_data VARCHAR(5000),
   creation_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
   accepted BOOLEAN DEFAULT false,
   image_data text DEFAULT NULL,
   FOREIGN KEY (parent_question) REFERENCES QUESTIONS (question_id) ON DELETE CASCADE,
   FOREIGN KEY (user_id) REFERENCES USERS (user_id) ON DELETE SET NULL,
   FOREIGN KEY (parent_answer) REFERENCES ANSWERS
);

CREATE TABLE REGISTERED_COURSES(
   user_id INT,
   course_id INT,
   is_accepted BOOLEAN DEFAULT FALSE,
   PRIMARY KEY (user_id, course_id),
   FOREIGN KEY (user_id) REFERENCES USERS (user_id) ON DELETE CASCADE,
   FOREIGN KEY (course_id) REFERENCES COURSES (course_id) ON DELETE CASCADE
);

CREATE TABLE VOTED(
   vote_voted VOTE_VALUE,
   user_id INT, 
   answer_id INT, 
   PRIMARY KEY (user_id, answer_id),
   FOREIGN KEY (user_id) REFERENCES USERS (user_id) ON DELETE CASCADE,
   FOREIGN KEY (answer_id) REFERENCES ANSWERS (answer_id) ON DELETE CASCADE
);


-- \i 'C:/Users/Admin1/Documents/my_docs/Skola/VS/3_BIT_zimny/IIS/projekt/database_create.sql'
-- \i 'C:/Users/Admin1/Documents/my_docs/Skola/VS/3_BIT_zimny/IIS/projekt/database_initial.sql'

-- \i 'C:/Users/lesam/Desktop/IIS/database_create.sql'