#!/bin/bash
echo "----- Generating unix user postgres_user"
adduser --disabled-password --gecos "" postgres_user
echo "----- Generating jobify_db database"
psql -U postgres -f ./Tester/pgmanagercreate.sql
echo "----- Generating jobify_db tables"
psql -U postgres_user -d jobify_db -f ./Tester/jobify_db.sql
