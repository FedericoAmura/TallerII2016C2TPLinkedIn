sudo apt-get install lcov
mkdir coverage
./UnitTest
lcov --capture --no-external --directory . --output-file coverage/coverage.info
genhtml coverage/coverage.info --output-directory coverage
#rm coverage.info
