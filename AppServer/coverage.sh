if ! $(command -v lcov >/dev/null)
then
	sudo apt-get install lcov
fi
mkdir coverage
./UnitTest
lcov --capture --no-external --directory . --output-file coverage/coverage.info
genhtml coverage/coverage.info --output-directory coverage
