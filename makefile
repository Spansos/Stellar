CC     := g++ -std=c++20
# CFLAGS := -g -Wall -Wextra -Wpedantic -Werror -Og -Iinclude
# to use emojis in identifies i mustn't use -Wpedantic
# the sacrifices i make to 👉👈 
CFLAGS := -g -Wall -Wextra -Werror -Og -Iinclude

# cpp and header files from own project
CPP_FILES := $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
HEADERS   := $(wildcard include/*/*.hpp) $(wildcard include/*.hpp)

# packages from package manager
PACKAGES := sfml-all
PKG_LIBS := $(shell pkg-config --libs $(PACKAGES))
CFLAGS   += $(shell pkg-config --cflags $(PACKAGES))

# obj files. dependant on cpp files, so last
OBJ_FILES := $(patsubst src/%.cpp,obj/%.o,$(CPP_FILES))

.PHONY: clean run

all: bin/main.out

run: bin/main.out
	./bin/main.out

clean:
	rm obj/* -rf
	rm bin/* -rf

obj/%.o: src/%.cpp $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

bin/main.out: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(PKG_LIBS) $^ -o $@