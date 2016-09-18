#!/bin/bash
echo "----- Generating unix user postgres_user"
sudo su adduser --disabled-password --gecos "" postgres_user
echo "----- Generating jobify_db database"
sudo su postgres -c "sudo psql -f ./Tester/pgmanagercreate.sql"
echo "----- Generating jobify_db tables"
sudo su postgres_user -c "psql -d jobify_db -f ./Tester/jobify_db.sql"
