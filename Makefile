BIN = cs8803_bin
RUNTIME = runtime
BUILD = build
SRC = src
ANTLR_RUNTIME = /usr/local/include/antlr4-runtime

INCLUDE = \
	-I$(RUNTIME) \
	-I$(SRC) \
	-I$(ANTLR_RUNTIME)

CXXFLAGS = -std=c++17 -g -Wno-attributes
LIBANTLR = /usr/local/lib/libantlr4-runtime.a

GENERATED_SOURCES = \
	$(RUNTIME)/TigerParser.cpp \
	$(RUNTIME)/TigerLexer.cpp \
	$(RUNTIME)/TigerBaseVisitor.cpp \
	$(RUNTIME)/TigerVisitor.cpp \
	$(RUNTIME)/TigerBaseListener.cpp \
	$(RUNTIME)/TigerListener.cpp

GENERATED_HEADERS = \
	$(RUNTIME)/TigerParser.h \
	$(RUNTIME)/TigerLexer.h \
	$(RUNTIME)/TigerBaseVisitor.h \
	$(RUNTIME)/TigerVisitor.h \
	$(RUNTIME)/TigerBaseListener.h \
	$(RUNTIME)/TigerListener.h

GENERATED_FILES = \
	$(GENERATED_SOURCES) \
	$(GENERATED_HEADERS) \
	$(RUNTIME)/Tiger.tokens \
	$(RUNTIME)/Tiger.interp \
	$(RUNTIME)/TigerLexer.tokens \
	$(RUNTIME)/TigerLexer.interp

GENERATED_OBJECTS = \
	$(BUILD)/TigerParser.o \
	$(BUILD)/TigerLexer.o \
	$(BUILD)/TigerVisitor.o \
	$(BUILD)/TigerBaseVisitor.o \
	$(BUILD)/TigerBaseListener.o \
	$(BUILD)/TigerListener.o

HEADERS = \
	$(SRC)/AntlrProcessor.h \
	$(SRC)/Common.h \
	$(SRC)/TigerFile.h \
	$(SRC)/PrinterFunctions.h

SOURCES = \
	$(SRC)/AntlrProcessor.cpp

OBJECTS = \
	$(BUILD)/AntlrProcessor.o

MAIN_OBJECT = \
	$(BUILD)/tigerc.o

all: dirs $(BIN)/tigerc

$(BIN)/tigerc: $(MAIN_OBJECT) $(OBJECTS) $(GENERATED_OBJECTS)
	g++ -o $@ $(MAIN_OBJECT) $(OBJECTS) $(GENERATED_OBJECTS) $(LIBANTLR)

$(MAIN_OBJECT): $(BUILD)/%.o: $(SRC)/%.cpp $(GENERATED_HEADERS) $(HEADERS)
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

$(OBJECTS): $(BUILD)/%.o: $(SRC)/%.cpp $(GENERATED_HEADERS) $(HEADERS)
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

$(GENERATED_FILES): Tiger.g4
	antlr -Dlanguage=Cpp -visitor -o $(RUNTIME) $<

$(GENERATED_OBJECTS): $(BUILD)/%.o: $(RUNTIME)/%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

dirs: cs8803_bin build runtime

cs8803_bin:
	mkdir -p $(BIN)

build:
	mkdir -p $(BUILD)

runtime :
	mkdir -p $(RUNTIME)

. PHONY :
clean:
	@rm -f $(MAIN_OBJECT) $(OBJECTS) $(GENERATED_FILES)
	@rm -f cs8803_bin/tigerc