# Makefile for JsonBenchmarkCpp
# A small program to compare perfomance of different json libs available
#
# Copyright Lijo Antony 2011
# Distributed under Apache License, Version 2.0
# see accompanying file LICENSE.txt

CC=clang++
CFLAGS=-c -std=c++11 -O3 -ffast-math -fexpensive-optimizations -DNDEBUG -DENABLE_JSONC
LDFLAGS=libs/libjson/libjson/libjson.a libs/json_spirit/json_spirit_v4.05/build/json_spirit/libjson_spirit.a libs/json-c/.libs/libjson-c.a
OBJECTS=$(SOURCES:.cpp=.o)
INCLUDE=\
		-Ilibs\
		-Ilibs/cajun/cajun\
		-Ilibs/json_spirit/json_spirit_v4.05/json_spirit\
		-Ilibs/libjson/libjson\
		-Ilibs/json-parser\
		-Ilibs/AveryWs\
		-Ilibs/json11\
		-Ilibs/picojson\
		-Ilibs/json-c

EXECUTABLE=JsonBenchmarkCpp

SOURCES=main.cpp libs/json-parser/json.c libs/json11/json11.cpp

all: cajun json_spirit libjson json_c $(SOURCES) $(EXECUTABLE)

cajun:
	if [ ! -d "libs/cajun/cajun" ]; then \
		unzip libs/cajun/cajun.zip -d libs/cajun; \
	fi

json_spirit:
	if [ ! -d "libs/json_spirit/json_spirit_v4.05" ]; then \
		unzip libs/json_spirit/json_spirit_v4_05.zip -d libs/json_spirit; \
	fi
	mkdir -p libs/json_spirit/json_spirit_v4.05/build
	if [ ! -f "libs/json_spirit/json_spirit_v4.05/build/CMakeCache.txt" ]; then \
		cd libs/json_spirit/json_spirit_v4.05/build && cmake ..; \
	fi
	$(MAKE) -w -C libs/libjson/libjson

libjson:
	if [ ! -d "libs/libjson/libjson" ]; then \
		unzip libs/libjson/libjson_7.4.0.zip -d libs/libjson; \
	fi
	$(MAKE) -w -C libs/json_spirit/json_spirit_v4.05/build

json_c:
	if [ ! -f "libs/json-c/.libs/libjson-c.a"]; then \
		cd libs/json-c;sh autogen.sh;./configure;make;cd -; \
	fi

$(EXECUTABLE): $(OBJECTS)
		$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
clean:
	    \rm *.o $(EXECUTABLE)
