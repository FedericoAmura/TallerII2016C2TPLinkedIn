# /usr/bin/env sh
echo "----- Installing shared server dependencies"
sudo apt-get install nodejs npm postgresql-9.5
npm install grunt-cli bower express pg ejs body-parser cookie-parser multer