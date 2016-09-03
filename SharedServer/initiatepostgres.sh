#!/bin/bash
#generamos usuario para manejar las bases
sudo adduser --disabled-password --gecos "" postgres_user
#pasamos a usuario postgre y creamos el usuario para manejar la base db_go
sudo su postgres -c "psql -f pgmanagercreate.sql"
#pasamos al usuario manager y corremos su script
sudo su postgres_user -c "psql -d sharedDB -f sharedDB.sql"
