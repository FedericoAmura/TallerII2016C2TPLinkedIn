# /usr/bin/env sh
#construimos el shared en su docker y el auxiliar de postgresql
docker-compose -f sharedServerDockerCompose.yml build
#lo ejecutamos
docker-compose -f sharedServerDockerCompose.yml up sharedserver