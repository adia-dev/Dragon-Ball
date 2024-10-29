SOURCES := $(shell find . -type f -name '*.cpp' -not -name '.null-ls*')
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
EXE := dragon_ball

CXX := g++
CXXFLAGS = --std=c++17 -g -Wall
SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics sfml-system)
SFML_LIBS := $(shell pkg-config --libs sfml-graphics sfml-window sfml-system)

CXXFLAGS += $(SFML_CFLAGS)

all: $(EXE)
	@echo Build complete for Mac OS X

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(SFML_LIBS)

bear:
	bear -- make

clean:
	rm -f $(EXE) $(OBJS)
