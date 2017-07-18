

CXX = g++
CXXFLAGS = -std=c++11  -I Parsers -I include -I lib -I Router -I Util -I PDN -O3 -I Flute
OBJS = main.o Parsers/Parsers.o Parsers/Printer.o Parsers/lefrw.o Parsers/defrw.o Parsers/InitialFileParser.o PDN/Converter.o PDN/PDN.o PDN/PDNHelper.o Parsers/ngspice.o Router/Graph_SP.o Router/BinaryHeap.o Router/GlobalRouter.o Parsers/verilog.o  Router/RouterUtil.o Router/Weights.o Router/Router.o Router/RouterV4.o Util/SpiceGenerator.o
STATICLIB = lib/liblef.a lib/libdef.a 
LEF_PATH=lef
DEF_PATH=def
LEF_STATICLIB=$(LEF_PATH)/lib/liblef.a
DEF_STATICLIB=$(DEF_PATH)/lib/libdef.a
all:cadXX
cadXX: $(OBJS) $(LEF_STATICLIB) $(DEF_STATICLIB) $(FLUTE_STATICLIB)
	$(CXX) $(CXXFLAGS) -o cadXX.out $(OBJS) $(LEF_STATICLIB) $(DEF_STATICLIB) $(FLUTE_STATICLIB)
$(LEF_STATICLIB):
	cd $(LEF_PATH) && make 
$(DEF_STATICLIB):
	cd $(DEF_PATH) && make 

main.o: main.cpp Parsers/lefrw.o
PDN.o:lib/PDN.cpp lib/PDN.hpp
PDNHelper.o:PDN/PDNHelper.cpp PDN/PDNHelper.hpp
Router.o:Router/Router.cpp Router/Router.hpp
RouterV4.o:Router/RouterV4.cpp Router/RouterV4.hpp
GlobalRouter.o:Router/GlobalRouter.cpp Router/GlobalRouter.hpp
RouterUtil.o:Router/RouterUtil.cpp Router/RouterUtil.hpp
Weights.o:Router/Weights.cpp Router/Weights.hpp
lefrw.o:lefrw.cpp lefrw.hpp
ngspice.o:ngspice.cpp ngspice.hpp
SpiceGenerator.o:SpiceGenerator.cpp SpiceGenerator.hpp
verilog.o:verilog.cpp verilog.hpp
defrw.o:defrw.cpp defrw.hpp
Graph_SP.o:Graph_SP.cpp Graph_SP.hpp
BinaryHeap:BinaryHeap.cpp BinaryHeap.hpp
Converter.o:lib/Converter.cpp lib/Converter.hpp
Parser.o: Parsers/Parsers.cpp Parsers/Parsers.hpp
InitialFileParser.o:Parsers/InitialFileParser.cpp Parsers/InitialFileParser.hpp
Debugger.o:lib/Debugger.cpp lib/Debugger.hpp

run_case1:
	time ./cadXX.out TestCase/2017/case1/case1.v TestCase/2017/case1/case1_input.def TestCase/2017/case1/tech.lef TestCase/2017/case1/blocks.lef TestCase/2017/case1/initial_files case1_output.def case1.sp output_files
run_case2:
	time ./cadXX.out TestCase/2017/case2/case2.v TestCase/2017/case2/case2_input.def TestCase/2017/case2/tech.lef TestCase/2017/case2/blocks.lef TestCase/2017/case2/initial_files case2_output.def case2.sp output_files 
run_case3:	
	time ./cadXX.out TestCase/2016/case3/case3.v TestCase/2016/case3/case3_input.def TestCase/2016/case3/tech.lef TestCase/2016/case3/blocks.lef TestCase/2016/case3/initial_files case3_output.def case3.sp output_files
.PHONY : clean
clean:
	cd $(LEF_PATH) && make clean
	cd $(DEF_PATH) && make clean
	-rm cadXX.out $(OBJS)



