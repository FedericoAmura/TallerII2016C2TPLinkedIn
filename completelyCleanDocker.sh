# /usr/bin/env sh
docker rm $(docker ps -a -q)
docker rmi $(docker images -q)