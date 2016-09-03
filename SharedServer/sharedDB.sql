--Creo tabla de categorias
CREATE TABLE categories(
	name character varying(255) NOT NULL,
	description character varying(255) NOT NULL,
CONSTRAINT xpknames PRIMARY KEY (name)
)
WITH (
	OIDS=FALSE
);

--Creo tabla de JobPositions
CREATE TABLE jobpositions(
	name character varying(255) NOT NULL,
	description character varying(255) NOT NULL,
	category character varying(255) NOT NULL,
CONSTRAINT xfkjobpositionscategories FOREIGN KEY (name)
	REFERENCES categories (name) MATCH SIMPLE
	ON UPDATE CASCADE ON DELETE CASCADE
)
WITH (
	OIDS=FALSE
);

--Creo tabla de Skills
CREATE TABLE skills(
	name character varying(255) NOT NULL,
	description character varying(255) NOT NULL,
	category character varying(255) NOT NULL,
CONSTRAINT xfkskillscategories FOREIGN KEY (name)
	REFERENCES categories (name) MATCH SIMPLE
	ON UPDATE CASCADE ON DELETE CASCADE
)
WITH (
	OIDS=FALSE
);

INSERT INTO categories (name,description)
	VALUES ('programacion','poder generar codigo');
INSERT INTO jobpositions (name,description,category)
	VALUES ('node developer','monito que genera codigo de node', 'programacion');
INSERT INTO skills (name,description,category)
	VALUES ('node','poder generar codigo node', 'programacion');