if ! $(command -v lcov >/dev/null)
then
	sudo apt-get install lcov
fi
mkdir coverage
./bin/UnitTest
lcov --capture --no-external --directory . --output-file coverage/coverage.info
genhtml coverage/coverage.info --output-directory coverage
