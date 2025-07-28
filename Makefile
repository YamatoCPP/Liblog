CC = g++
CXXFLAGS = -Wall -fPIC -std=c++17 -Iinclude -MMD
LDFLAGS = -shared
LIB_NAME = log
LIB = $(BUILDDIR)/lib$(LIB_NAME).so
LIBDIR = include/lib$(LIB_NAME)
SRCDIR = src
TESRDIR = tests
BUILDDIR = build
OBJ = $(BUILDDIR)/main.o \
	$(BUILDDIR)/log.o
TARGET = $(BUILDDIR)/main
VPATH = $(SRCDIR):$(LIBDIR)

all: $(BUILDDIR) $(TARGET)

$(TARGET): $(OBJ) $(LIB)
	$(CC) $(CXXFLAGS) -o $@ $^ -Wl,-rpath='$$ORIGIN'

$(BUILDDIR)/%.o: %.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(LIB): $(BUILDDIR)/log.o
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR) 

-include $(BUILDDIR)/*.d
.PHONY: all clean test 

test: $(TESRDIR)/log_test.cpp $(BUILDDIR)/$(LIB)
	$(CC) $(CXXFLAGS) -o $(BUILDDIR)/$@ -L$(BUILDDIR) -llog -Wl,-rpath='$$ORIGIN' $^

clean:
	rm -rf $(BUILDDIR)
