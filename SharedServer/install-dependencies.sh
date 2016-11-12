# /usr/bin/env sh
echo "----- Installing shared server dependencies"
sudo apt-get install nodejs npm postgresql-9.5
npm config rm proxy
npm config rm https-proxy
npm install express pg body-parser