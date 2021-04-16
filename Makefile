BINNAME := scrapod

debug ?= false
ifeq (${debug}, true)
	COMMON_FLAGS := -g3 -ggdb -ftrapv -fsanitize=address -fsanitize=leak -fsanitize=undefined
else
	COMMON_FLAGS := -O2 -D_FORTIFY_SOURCE=2 -DNDEBUG
endif

profile ?= false
ifeq (${profile}, true)
	COMMON_FLAGS += -pg
endif

CPP_SOURCES := $(wildcard src/*.cpp)
CPP_OBJECTS := $(patsubst %.cpp,%.o,${CPP_SOURCES})
C_SOURCES := $(wildcard src/*.c)
C_OBJECTS := $(patsubst %.c,%.o,${C_SOURCES})
OBJECTS := ${C_OBJECTS} ${CPP_OBJECTS}
DEPENDENCIES := $(patsubst %.cpp,%.d,${CPP_SOURCES}) $(patsubst %.c,%.d,${C_SOURCES})

INCLUDE_FLAGS := -I./include
WARNING_FLAGS := -Wextra -Wall -Wshadow -Wdouble-promotion \
	-Wformat=2 -Wformat-truncation -fno-common -Wconversion
INIH_FLAGS := -DINI_STOP_ON_FIRST_ERROR=1 -DINI_ALLOW_REALLOC=1 \
	-DINI_ALLOW_MULTILINE=0 -DINI_ALLOW_NO_VALUE=1
CFLAGS += ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${COMMON_FLAGS} ${INIH_FLAGS}
CXXFLAGS += ${CFLAGS}
export

.PHONY: clean tests coverage

${BINNAME}: ${OBJECTS}
	${CXX} -Werror ${CXXFLAGS} $^ -o $@

${C_OBJECTS}: %.o: %.c
	${CC} -Werror ${CFLAGS} -MMD -c $< -o $@

${CPP_OBJECTS}: %.o: %.cpp
	${CXX} -Werror ${CXXFLAGS} -MMD -c $< -o $@

-include $(DEPENDENCIES)

clean:
	rm -rf ${OBJECTS} ${DEPENDENCIES} ${BINNAME}
