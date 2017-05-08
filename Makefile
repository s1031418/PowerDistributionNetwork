

CXX = g++ 
CXXFLAGS = -std=c++11  -I Parsers -I include -I lib
OBJS = main.o Parsers/Parsers.o lib/Debugger.o Parsers/lefrw.o
STATICLIB = lib/liblef.a


all:cadXX

cadXX: $(OBJS)
	$(CXX) $(CXXFLAGS) -o cadXX.out $(OBJS) $(STATICLIB)

main.o: main.cpp Parsers/lefrw.o
#	$(CXX) -c $(PARSERPATH) $(INCLUDEPATH) main.cpp Parsers/lefrw.o
lefrw.o:lefrw.cpp
#	$(CXX) -c $(PARSERPATH) $(INCLUDEPATH) lefrw.cpp 
Parser.o: Parsers/Parsers.cpp
Debugger.o:lib/Debugger.cpp

.PHONY : clean
clean:
	-rm cadXX.out $(OBJS)



