# /usr/bin/env sh
#Deployamos a heroku
echo "Estamos en: " $PWD
travis setup heroku
echo "Versiones:"
heroku --version
git --version
echo "Tratamos de commitear"
#git config --local remote.origin.url|sed -n 's#.*/\([^.]*\)\.git#\1#p'
eval "$(ssh-agent -s)" #start the ssh agent
chmod 600 .travis/deploy_key.pem # this key should have push access
ssh-add .travis/deploy_key.pem
git remote add deploy https://git.heroku.com/jobifyg2.git
git subtree push --prefix SharedServer heroku master