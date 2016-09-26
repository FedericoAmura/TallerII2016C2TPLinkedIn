#!/bin/bash
echo "----- Generating unix user postgres_user"
sudo adduser --disabled-password --gecos "" postgres_user
echo "----- Generating jobify_db database"
sudo psql -U postgres -f ./Tester/pgmanagercreate.sql
echo "----- Generating jobify_db tables"
sudo psql -U postgres_user -d jobify_db -f ./Tester/jobify_db.sql
