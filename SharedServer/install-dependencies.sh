# /usr/bin/env sh
echo "----- Installing shared server dependencies"
sudo apt-get install nodejs npm postgresql-9.5
npm config set registry http://registry.npmjs.org
npm install express pg body-parser