CXX=g++
CC=gcc
CXXVERSION=-std=c++0x
PARSE = bison -d
LEX = flex
INCLUDEDIR=include/
CXXFLAGS=-Wall -I$(INCLUDEDIR) $(CXXVERSION)  
CXXLINKS=
LLCXXLINKS=-lfl
PARSERFILE=parser
LEXERFILE=genlex
BINLEX=bin/$(LEXERFILE)
BINEGREP=bin/egrep
BINBUILDER=bin/testBuilder
BINAUTOMATA=bin/testAutomata
BINSTATE=bin/testState
BINTESTLEX=bin/testLex

all : parser lexer egrep testBuilder testState testAutomata test_algorithms

dot_to_pdf: automata_initial.dot.pdf automata_epsilon.dot.pdf automata_determinise.dot.pdf automata_minimise.dot.pdf

automata.pdf: automata.dot
	dot -Tpdf $< -o $@

automata_initial.dot.pdf: automata_initial.dot
	dot -Tpdf $< -o $@

automata_epsilon.dot.pdf: automata_epsilon.dot
	dot -Tpdf $< -o $@

automata_determinise.dot.pdf: automata_determinise.dot
	dot -Tpdf $< -o $@

automata_minimise.dot.pdf: automata_minimise.dot
	dot -Tpdf $< -o $@


egrep : $(BINEGREP) parser

testBuilder : $(BINBUILDER)

testState : $(BINSTATE)

testAutomata : $(BINAUTOMATA)

test_algorithms : bin/test_algorithms

parser: include/$(PARSERFILE).h

lexer: obj/$(LEXERFILE).o

testLex: $(BINTESTLEX)

$(BINTESTLEX): obj/$(LEXERFILE).o
	$(CXX) -o $@ $^ $(CXXLINKS)

$(BINEGREP): obj/egrepMain.o obj/AutomataBuilder.o obj/State.o obj/Automata.o obj/AutomataPrinter.o obj/Tools.o obj/$(LEXERFILE).o obj/$(PARSERFILE).o obj/FileList.o
	$(CXX) -o $@ $^ $(CXXLINKS)

$(BINBUILDER): obj/testAutomataBuilder.o obj/AutomataBuilder.o obj/State.o obj/Automata.o obj/AutomataPrinter.o obj/Tools.o obj/FileList.o
	$(CXX) -o $@ $^ $(CXXLINKS)

$(BINAUTOMATA): obj/testAutomata.o obj/State.o obj/Automata.o obj/AutomataBuilder.o obj/AutomataPrinter.o obj/Tools.o obj/FileList.o
	$(CXX) -o $@ $^ $(CXXLINKS)

bin/test_algorithms : obj/test_algorithms.o obj/State.o obj/Automata.o
	$(CXX) -o $@ $^ $(CXXLINKS)

$(BINSTATE): obj/testState.o obj/State.o obj/AutomataPrinter.o obj/AutomataBuilder.o obj/Automata.o obj/Tools.o obj/FileList.o
	$(CXX) -o $@ $^ $(CXXLINKS)

obj/egrepMain.o: src/egrepMain.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/testAutomata.o: src/testAutomata.cpp include/AutomataPrinter.h include/Automata.h 
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/testAutomataBuilder.o: src/testAutomataBuilder.cpp include/AutomataBuilder.h include/AutomataPrinter.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/testState.o: src/testState.cpp include/AutomataPrinter.h include/State.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/test_algorithms.o: src/test_algorithms.cpp include/Automata.h  include/State.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/Automata.o: src/Automata.cpp include/Automata.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/AutomataBuilder.o: src/AutomataBuilder.cpp include/AutomataBuilder.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/AutomataPrinter.o: src/AutomataPrinter.cpp include/AutomataPrinter.h  include/State.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/State.o: src/State.cpp include/State.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/Tools.o: src/Tools.cpp include/Tools.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/FileList.o: src/FileList.cpp include/FileList.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/$(LEXERFILE).o: src/$(LEXERFILE).cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

obj/$(PARSERFILE).o: src/$(PARSERFILE).cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

src/$(LEXERFILE).cpp: lexer/lexer.l include/$(PARSERFILE).h
	$(LEX) -o $@ $<
	
include/$(PARSERFILE).h: src/$(PARSERFILE).hpp
	mv $< $@
	
src/$(PARSERFILE).hpp: parser/$(PARSERFILE).y
	$(PARSE) $< -o src/$(PARSERFILE).cpp

clean:
	rm -rf ./*/*.*~ ./*~ obj/* bin/* src/$(PARSERFILE).cpp include/$(PARSERFILE).h src/$(LEXERFILE).cpp include/$(LEXERFILE).h

cleanObj:
	rm -rf obj/*

cleanBin:
	rm -rf bin/*
