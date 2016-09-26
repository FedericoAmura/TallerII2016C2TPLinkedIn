CREATE TABLE categories
(
	name VARCHAR NOT NULL,
	description VARCHAR,
	CONSTRAINT xpk_categories_name PRIMARY KEY (name),
	CONSTRAINT xu_categories_name UNIQUE (name)
) WITH (
	OIDS=FALSE
);
/*ALTER TABLE categories
OWNER TO postgres_user;*/

CREATE TABLE skills
(
	name VARCHAR NOT NULL,
	description VARCHAR,
	category VARCHAR NOT NULL,
	CONSTRAINT xpk_skils_name PRIMARY KEY (name),
	CONSTRAINT xu_skils_name UNIQUE (name),
	CONSTRAINT xfkskills_categories FOREIGN KEY (category)
		REFERENCES categories (name) MATCH SIMPLE
		ON UPDATE NO ACTION ON DELETE NO ACTION
) WITH (
	OIDS=FALSE
);
/*ALTER TABLE skills
OWNER TO postgres_user;*/

CREATE TABLE jobs
(
	name VARCHAR NOT NULL,
	description VARCHAR,
	category VARCHAR NOT NULL,
	CONSTRAINT xpk_jobs_name PRIMARY KEY (name),
	CONSTRAINT xu_jobs_name UNIQUE (name),
	CONSTRAINT xfkjobs_categories FOREIGN KEY (category)
		REFERENCES categories (name) MATCH SIMPLE
		ON UPDATE NO ACTION ON DELETE NO ACTION
) WITH (
	OIDS=FALSE
);
/*ALTER TABLE jobs
OWNER TO postgres_user;*/
