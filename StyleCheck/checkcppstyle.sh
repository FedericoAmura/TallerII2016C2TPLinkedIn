# /usr/bin/env sh
FILTERS="-whitespace/tab,-legal/copyright"
cpplint --recursive --filter=$FILTERS ../AppServer/src/server
cpplint --recursive --filter=$FILTERS ../AppServer/src/handlers
cpplint --filter=$FILTERS ../AppServer/src/AppServer.cpp
cpplint --recursive --filter=$FILTERS ../AppServer/test
cpplint --recursive --filter=$FILTERS ../AppServer/include/server
cpplint --recursive --filter=$FILTERS ../AppServer/include/handlers
cpplint --recursive --filter=$FILTERS ../AppServer/include/test

