default: latte.exe

# this makefile builds by finding source files from their headers
# this is a list of headers which should be excluded (because they don't have an associated source file)
EXCLUDED_HEADERS=jvm.hpp
# this is a list of source files which should be included, but don't have an associated header file
INCLUDED_SOURCE_FILES=main.cpp

OBJECTS := $(wildcard src/**.h*) $(wildcard src/*/**.h*)
OBJECTS := $(subst src/,,$(subst \n, ,$(OBJECTS)))
OBJECTS := $(filter-out $(EXCLUDED_HEADERS),$(OBJECTS)) $(INCLUDED_SOURCE_FILES)
OBJECTS := $(basename $(OBJECTS))
OBJECTS := $(OBJECTS:%=build/%.o)

latte.exe: $(OBJECTS)
	g++ $(OBJECTS) -o latte.exe

clean:
	rm -f latte.exe
	rm -rf build

build/%.o: src/%.c src/%.h
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	gcc -O2 -c $< -o build/$*.o

build/%.o: src/%.cpp src/%.hpp
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	g++ -O2 -c $< -o build/$*.o