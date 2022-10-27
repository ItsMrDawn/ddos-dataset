# http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
CC := gcc
SRCDIR := src
BUILDDIR := build
TARGET := ddos

SRCEXT := c
SOURCES:=$(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
FILTERED_OBJECTS := $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))

CFLAGS := -O2 -pipe -Wall
LIB :=
INC := -I include

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR)/*"; $(RM) -r $(BUILDDIR) $(BINDIR)/*

.PHONY: clean
