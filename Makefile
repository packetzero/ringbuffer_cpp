all: ringtest

PLATFORM=`uname -s`

ringtest: main.cpp ring_buffer_inst.cpp
	cc -o ringtest -I./deps/gtest/include main.cpp ring_buffer_inst.cpp deps/gtest/$(PLATFORM)/libgtest.a -lstdc++ -lpthread
clean:
	rm -f ringtest
