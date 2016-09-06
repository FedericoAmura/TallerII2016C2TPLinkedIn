# /usr/bin/env sh
#Deployamos a heroku
echo "Estamos en: " $PWD
travis setup heroku
echo "Versiones:"
heroku --version
git --version
echo "Le metemos el buildpack"
heroku buildpacks:set heroku/nodejs
#git config --local remote.origin.url|sed -n 's#.*/\([^.]*\)\.git#\1#p'
echo "Y avanzamos con el deploy"
git subtree push --prefix SharedServer heroku master