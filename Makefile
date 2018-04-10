# Creates the bin directory, then executes cmake and make.
.PHONY: compile
compile:
	mkdir -p bin; \
	cd bin; \
	cmake ../; \
	make

# Same as compile, except with debugging enabled.
.PHONY: debug
debug:
	mkdir -p bin; \
	cd bin; \
	cmake -DCMAKE_BUILD_TYPE=Debug ../; \
	make

# Runs unit tests with xml output.
.PHONY: test
test: compile
	-cd bin; \
	./server_test --gtest_output=xml