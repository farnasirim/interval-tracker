all: range-tracker

CXX := g++

CXX_SRCS = $(wildcard *.cc)
CXX_TESTS_DIR = ./tests
CXX_TEST_SRCS = $(wildcard $(CXX_TESTS_DIR)/*.cc)
CXX_OBJS = $(patsubst %.cc,%.o,$(CXX_SRCS))
CXX_DEPS = $(patsubst %.cc,%.d,$(CXX_SRCS))

CXX_INCLUDE := -I.
CXX_WARNINGS=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
	-Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self \
	-Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept \
	-Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
	-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
	-Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused \
	-Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith \
	-Wmissing-declarations -Winline -Wundef -Wcast-qual \
	-Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal \
	-pedantic

CXX_FLAGS := -O2 -std=c++14

%.o: %.cc $(CXX_HEADERS) Makefile
	g++ $(CXX_FLAGS) $(CXX_WARNINGS) $(CXX_INCLUDE) -MMD -MP -c -o $@ $<

range-tracker: $(CXX_OBJS)
	g++ $(CXX_FLAGS) $(CXX_INCLUDE) -o $@ $^

all: range-tracker

doc: Doxyfile
	rm -rf doc
	doxygen Doxyfile

tests/test_main.o: tests/test_main.cc
	g++ $(CXX_FLAGS) -g3 $(CXX_INCLUDE) -c -o $@ $^

rt-tests: $(filter-out ./tests/test_main.cc, $(CXX_TEST_SRCS)) $(filter-out main.o, $(CXX_OBJS)) tests/test_main.o
	g++ $(CXX_FLAGS) -g3 $(CXX_INCLUDE) -o $@ $^

run-tests: rt-tests
	./rt-tests

.PHONY: all run-tests doc clean

-include $(CXX_DEPS)

clean:
	rm -rf rt-tests range-tracker doc $(CXX_OBJS) $(CXX_DEPS)
