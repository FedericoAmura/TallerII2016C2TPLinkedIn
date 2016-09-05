# /usr/bin/env sh
FILTERS="-whitespace/tab"
cpplint --recursive --filter=$FILTERS ../AppServer/src/server
cpplint --filter=$FILTERS ../AppServer/src/AppServer.cpp

