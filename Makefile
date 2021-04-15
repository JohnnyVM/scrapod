BINNAME := scrapod

debug ?= false
ifeq (${debug}, true)
	DEBUG_FLAGS := -g3 -ggdb -ftrapv -fsanitize=address -fsanitize=leak -fsanitize=undefined
endif

profile ?= false
ifeq (${profile}, true)
	DEBUG_FLAGS += -pg
endif

CPP_SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,${CPP_SOURCES})
DEPENDENCIES := $(patsubst %.cpp,%.d,${CPP_SOURCES})

INCLUDE_FLAGS := -I./include
WARNING_FLAGS := -Wextra -Wall -Wshadow -Wdouble-promotion -Wpadded \
	-Wformat=2 -Wformat-truncation -fno-common -Wconversion
CPPFLAGS += ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${DEBUG_FLAGS}
export

.PHONY: clean tests coverage

${BINNAME}: ${OBJECTS}
	${CXX} -Werror ${CPPFLAGS} $^ -o $@

${OBJECTS}: %.o: %.cpp
	${CXX} -Werror ${CPPFLAGS} -MMD -c $< -o $@

-include $(DEPENDENCIES)

clean:
	rm -rf ${OBJECTS} ${DEPENDENCIES} ${BINNAME}
