# /usr/bin/env sh
#Deployamos a heroku
echo "Estamos en: " $PWD
travis setup heroku
echo "Versiones:"
heroku --version
git --version
echo "Tratamos de commitear"
#git config --local remote.origin.url|sed -n 's#.*/\([^.]*\)\.git#\1#p'
git subtree push --prefix SharedServer heroku master