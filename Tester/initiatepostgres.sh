#!/bin/bash
echo "----- Generating unix user postgres_user"
sudo adduser --disabled-password --gecos "" postgres_user
echo "----- Generating jobify_db database"
sudo psql -U postgres -f ./Tester/pgmanagercreate.sql
echo "----- Generating jobify_db tables"
sudo su postgres_user -c "psql -d jobify_db -f ./Tester/jobify_db.sql"
