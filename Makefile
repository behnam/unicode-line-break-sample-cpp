OS:=$(shell uname -s)

CXXFLAGS:=$(CXXFLAGS)
COMMON_FLAGS=-Wall -Wshadow -Wunused-parameter
RELEASE_FLAGS=$(COMMON_FLAGS) -DNDEBUG -O3 -finline-functions -Wno-missing-braces
DEBUG_FLAGS=$(COMMON_FLAGS) -g -DDEBUG -O0

all: linebreak

linebreak: linebreak.cpp Makefile
	$(CXX) -o linebreak linebreak.cpp $(RELEASE_FLAGS) -I./ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f linebreak

.PHONY: test
