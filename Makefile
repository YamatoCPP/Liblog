CC = g++
CXXFLAGS = -Wall -fPIC -std=c++17 -Iinclude -MMD
LDFLAGS = -shared
LIB = liblog.so
LIB_NAME = log
LIBDIR = include/liblog
SRCDIR = src
TESRDIR = tests
BUILDDIR = build
OBJ = $(BUILDDIR)/main.o \
	$(BUILDDIR)/log.o
TARGET = $(BUILDDIR)/main

VPATH = $(SRCDIR):$(LIBDIR)
.phony: all clean test 

all: $(BUILDDIR) $(TARGET)

$(TARGET): $(OBJ) $(BUILDDIR)/$(LIB)
	$(CC) $(CXXFLAGS) -o $@ $^ -Wl,-rpath='$$ORIGIN'

$(BUILDDIR)/%.o: %.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(BUILDDIR)/$(LIB): $(BUILDDIR)/log.o
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR) 

-include $(BUILDDIR)/*.d

test: $(TESRDIR)/log_test.cpp $(BUILDDIR)/$(LIB)
	$(CC) $(CXXFLAGS) -o $(BUILDDIR)/$@ -L$(BUILDDIR) -llog -Wl,-rpath='$$ORIGIN' $^

clean:
	rm -rf $(BUILDDIR)
