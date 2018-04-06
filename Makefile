# This makefile creates the bin directory as needed, and then executes cmake and make within it.
.PHONY: compile
compile:
	mkdir -p bin; \
	cd bin; \
	cmake ../; \
	make