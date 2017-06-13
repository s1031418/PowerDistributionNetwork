

CXX = g++
CXXFLAGS = -std=c++11  -I Parsers -I include -I lib -I Flute -I Router 
OBJS = main.o Parsers/Parsers.o lib/Debugger.o Parsers/lefrw.o Parsers/defrw.o Parsers/InitialFileParser.o lib/Converter.o lib/PDN.o lib/PDNHelper.o Parsers/ngspice.o lib/Graph_SP.o lib/BinaryHeap.o Router/GlobalRouter.o Flute/flute_net.o Parsers/verilog.o Router/DetailRouter.o Router/RouterUtil.o Router/Weights.o
STATICLIB = lib/liblef.a lib/libdef.a lib/flute.a


all:cadXX

cadXX: $(OBJS)
	$(CXX) $(CXXFLAGS) -o cadXX.out $(OBJS) $(STATICLIB)

main.o: main.cpp Parsers/lefrw.o
PDN.o:lib/PDN.cpp
PDNHelper.o:lib/PDNHelper.cpp
GlobalRouter.o:Router/GlobalRouter.cpp
DetailRouter.o:Router/DetailRouter.cpp
RouterUtil.o:Router/RouterUtil.cpp
Weights.o:Router/Weights.cpp
flute_net.o:flute_net.cpp
lefrw.o:lefrw.cpp
ngspice.o:ngspice.cpp
verilog.o:verilog.cpp
defrw.o:defrw.cpp
Graph_SP.o:Graph_SP.cpp
BinaryHeap:BinaryHeap.cpp
Converter.o:lib/Converter.cpp
Parser.o: Parsers/Parsers.cpp
InitialFileParser.o:Parsers/InitialFileParser.cpp
Debugger.o:lib/Debugger.cpp

run_case1:
	time ./cadXX.out TestCase/2017/case1/case1.v TestCase/2017/case1/case1_output.def TestCase/2017/case1/tech.lef TestCase/2017/case1/blocks.lef TestCase/2017/case1/initial_files case1
run_case2:
	time ./cadXX.out TestCase/2017/case2/case2.v TestCase/2017/case2/case2_input.def TestCase/2017/case2/tech.lef TestCase/2017/case2/blocks.lef TestCase/2017/case2/initial_files case2 
run_case3:	
	time ./cadXX.out TestCase/2016/case3/case3.v TestCase/2016/case3/case3_input.def TestCase/2016/case3/tech.lef TestCase/2016/case3/blocks.lef TestCase/2016/case3/initial_files case3
.PHONY : clean
clean:
	-rm cadXX.out $(OBJS)



