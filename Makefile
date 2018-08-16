VERSION = 1.0.0

COMPILER = g++
BINDIR = bin
OBJDIR = obj
SRCDIR = src
UIDIR = $(SRCDIR)/resource/ui
TESTDIR = test
GTESTDIR = $(TESTDIR)/resource/gtest

TARGET = ${BINDIR}/cedit
CFLAGS = `pkg-config gtkmm-3.0 gtksourceviewmm-3.0 --cflags` -std=c++14 -O3 -MMD -MP -DVERSION=\"$(VERSION)\"
LDFLAGS = `pkg-config gtkmm-3.0 gtksourceviewmm-3.0 --libs`
ifeq ($(OS),Windows_NT)
	LDFLAGS += -Wl,--subsystem,windows
endif
INCLUDE = -I include
SOURCES = $(shell find $(SRCDIR) -name "*.cpp")
RESOURCES = $(shell find $(SRCDIR)/resource -name "*.xml" -o -name "*.png")
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o)) $(OBJDIR)/resource/resource.o
DEPENDS = $(OBJECTS:.o=.d)

UIFILES = $(shell find $(UIDIR) -name "*.xml")
UIHEADERS = $(addsuffix .h, $(UIFILES))
POT = share/locale/cedit++.pot
PO = share/locale/ja_JP/LC_MESSAGES/ja.po
MO = share/locale/ja_JP/LC_MESSAGES/cedit++.mo

TEST_TARGET = ${BINDIR}/test
TEST_CFLAGS = $(CFLAGS) -pthread
TEST_LDFLAGS = $(LDFLAGS) -lpthread -lm
TEST_INCLUDE = $(INCLUDE) -isystem $(TESTDIR)/resource/gtest/include
TEST_LIB = $(GTESTDIR)/lib/gtest-all.a $(GTESTDIR)/lib/gtest_main.a
TEST_SOURCES = $(shell find $(TESTDIR) -name "*.cpp")
TEST_OBJECTS = $(addprefix $(OBJDIR)/, $(TEST_SOURCES:.cpp=.o))
TEST_DEPENDS = $(TEST_OBJECTS:.o=.d)

all : $(TARGET)

pot : $(SOURCES) $(UIHEADERS)
	xgettext --output=$(POT) --from-code=UTF-8 --keyword=_ --keyword=N_ --package-name=cedit++ --package-version=$(VERSION) $(SOURCES) $(UIHEADERS)

po : $(POT) $(PO)
	msgmerge --output-file=$(PO) $(PO) $(POT)

mo : $(PO)
	msgfmt --output-file=$(MO) $(PO)

test : $(TEST_TARGET)
	./${BINDIR}/test --gtest_color=yes 2>/dev/null

install :
	cp $(TARGET) /usr/local/bin/
	cp -r ./ $(HOME)/.cedit++

installer :
	cd windows && ./make_installer.sh $(VERSION)

clean :
	rm -rf $(BINDIR) $(OBJDIR)

clean-all :
	rm -rf $(BINDIR) $(OBJDIR) $(GTESTDIR)/lib

$(TARGET) : $(OBJECTS)
	@mkdir -p `dirname $@`
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/$(SRCDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p `dirname $@`
	$(COMPILER) -o $@ -c $< $(INCLUDE) $(CFLAGS)

$(OBJDIR)/resource/resource.o : $(RESOURCES)
	@mkdir -p `dirname $@`
	glib-compile-resources --target=$(OBJDIR)/resource/resource.c --generate-source $(SRCDIR)/resource/resource.gresource.xml
	$(COMPILER) -o $@ -c $(OBJDIR)/resource/resource.c $(INCLUDE) $(CFLAGS)

$(UIDIR)/%.h : $(UIDIR)/%
	intltool-extract --type=gettext/glade $^

$(TEST_TARGET) : $(filter-out $(OBJDIR)/$(SRCDIR)/cedit/cedit.o, $(OBJECTS)) $(TEST_OBJECTS) $(TEST_LIB)
	@mkdir -p `dirname $@`
	$(COMPILER) -o $@ $^ $(TEST_LDFLAGS)

$(OBJDIR)/$(TESTDIR)/%.o : $(TESTDIR)/%.cpp
	@mkdir -p `dirname $@`
	$(COMPILER) -o $@ -c $< $(TEST_INCLUDE) $(TEST_CFLAGS)

$(GTESTDIR)/lib/%.a : $(GTESTDIR)/lib/%.o
	ar -rv $@ $<

$(GTESTDIR)/lib/%.o : $(GTESTDIR)/src/%.cc
	@mkdir -p `dirname $@`
	$(COMPILER) -o $@ -c $< -I $(GTESTDIR) $(TEST_INCLUDE) $(TEST_CFLAGS)

-include $(DEPENDS) $(TEST_DEPENDS)

.PHONY : all pot po mo test install installer clean clean-all
