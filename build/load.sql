-- 新建并切换到hrd数据库
SELECT 'Initializing...' AS ' ';
DROP DATABASE IF EXISTS hrd;
CREATE DATABASE hrd;
USE hrd;

-- 初始化数据库
DROP TABLE IF EXISTS main;
DROP TABLE IF EXISTS farthest_temp;
DROP TABLE IF EXISTS solution_temp;
DROP TABLE IF EXISTS farthest;
DROP TABLE IF EXISTS solution;

-- 创建main表：
CREATE TABLE main (
    id INT unsigned NOT NULL,
    code CHAR(9) NOT NULL,
    is_mirror CHAR(1) NOT NULL,
    jiang_num TINYINT unsigned NOT NULL,
    bing_num TINYINT unsigned NOT NULL,
    style_num TINYINT unsigned NOT NULL,
	group_num SMALLINT unsigned NOT NULL,
	group_index MEDIUMINT unsigned NOT NULL,
    UNIQUE (code),
    PRIMARY KEY (id)
) DEFAULT CHARSET = utf8;

-- 创建farthest_temp表：
CREATE TABLE farthest_temp (
	code CHAR(9) NOT NULL,
	farthest_step SMALLINT unsigned NOT NULL,
	farthest_num SMALLINT unsigned NOT NULL,
	farthest_case MEDIUMTEXT,
	PRIMARY KEY (code)
) DEFAULT CHARSET = utf8;

-- 创建solution_temp表：
CREATE TABLE solution_temp (
	code CHAR(9) NOT NULL,
	min_solution_step SMALLINT NOT NULL,
	min_solution_num SMALLINT unsigned NOT NULL,
	min_solution_case MEDIUMTEXT,
	solution_num SMALLINT unsigned NOT NULL,
	PRIMARY KEY (code)
) DEFAULT CHARSET = utf8;

-- 创建farthest表：
CREATE TABLE farthest (
	id INT unsigned NOT NULL AUTO_INCREMENT,
	farthest_step SMALLINT unsigned NOT NULL,
	farthest_num SMALLINT unsigned NOT NULL,
	PRIMARY KEY (id)
) auto_increment = 1 DEFAULT CHARSET = utf8;

-- 创建solution表：
CREATE TABLE solution (
	id INT unsigned NOT NULL AUTO_INCREMENT,
	min_solution_step SMALLINT NOT NULL,
	min_solution_num SMALLINT unsigned NOT NULL,
	solution_num SMALLINT unsigned NOT NULL,
	PRIMARY KEY (id)
) auto_increment = 1 DEFAULT CHARSET = utf8;

-- 载入main.csv的数据
SELECT 'Loading main.csv...' AS ' ';
LOAD DATA INFILE 'D:/HRD_Database/build/release/main.csv'
INTO TABLE main
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

-- 载入farthest.csv的数据
SELECT 'Loading farthest.csv...' AS ' ';
LOAD DATA INFILE 'D:/HRD_Database/build/release/farthest.csv'
INTO TABLE farthest_temp
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

-- 整理数据到farthest表中
SELECT 'Collating farthest table...' AS ' ';
ALTER TABLE farthest_temp DROP COLUMN farthest_case;
INSERT INTO farthest (farthest_step, farthest_num)
SELECT farthest_step, farthest_num FROM farthest_temp;
DROP TABLE farthest_temp;

-- 载入solution.csv的数据
SELECT 'Loading solution.csv...' AS ' ';
LOAD DATA INFILE 'D:/HRD_Database/build/release/solution.csv'
INTO TABLE solution_temp
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

-- 整理数据到solution表中
SELECT 'Collating solution table...' AS ' ';
ALTER TABLE solution_temp DROP COLUMN min_solution_case;
INSERT INTO solution (min_solution_step, min_solution_num, solution_num)
SELECT min_solution_step, min_solution_num, solution_num FROM solution_temp;
DROP TABLE solution_temp;

-- 更正id值
SELECT 'Syncing id...' AS ' ';
ALTER TABLE farthest CHANGE id id INT unsigned NOT NULL;
ALTER TABLE solution CHANGE id id INT unsigned NOT NULL;
ALTER TABLE farthest DROP PRIMARY KEY;
ALTER TABLE solution DROP PRIMARY KEY;
UPDATE farthest SET id = id - 1;
UPDATE solution SET id = id - 1;
ALTER TABLE farthest ADD PRIMARY KEY (id);
ALTER TABLE solution ADD PRIMARY KEY (id);

-- 导入完成
SELECT 'All Done' AS ' ';