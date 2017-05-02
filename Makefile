

CXX = g++ 
CXXFLAGS = -std=c++11
OBJS = main.o Parsers/TechlefParser.o Parsers/Parsers.o lib/Debugger.o Parsers/BlocklefParser.o Parsers/DefParser.o



all:cadXX

cadXX: $(OBJS)
	$(CXX) $(CXXFLAGS) -o cadXX $(OBJS)

main.o: main.cpp 
Parser.o: Parsers/Parsers.cpp
TechlefParser.o: Parsers/TechlefParser.cpp 
BlocklefParser.o:Parsers/BlocklefParser.cpp
Debugger.o:lib/Debugger.cpp
DefParser.o:Parsers/DefParser.cpp
.PHONY : clean
clean:
	-rm cadXX $(OBJS)



