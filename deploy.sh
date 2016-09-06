# /usr/bin/env sh
#Deployamos a heroku
echo $PWD
wget -O- https://toolbelt.heroku.com/install-ubuntu.sh | sh
heroku --version
git --version
git config --local remote.origin.url|sed -n 's#.*/\([^.]*\)\.git#\1#p'
git subtree push --prefix SharedServer heroku master