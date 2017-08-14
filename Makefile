

CXX = g++
CXXFLAGS = -std=c++11  -I Parsers -I include -I lib -I Router -I Util -I PDN -O3 -I Flute
OBJS = main.o Parsers/Parsers.o Parsers/Printer.o Parsers/lefrw.o Parsers/defrw.o Parsers/InitialFileParser.o PDN/PDNHelper.o Parsers/ngspice.o Router/Graph_SP.o Router/BinaryHeap.o Router/GlobalRouter.o Parsers/verilog.o  Router/RouterUtil.o Router/RouterV4.o Util/SpiceGenerator.o Util/DefGenerator.o Util/OutputFilesGenerator.o Router/Graph.o
STATICLIB = lib/liblef.a lib/libdef.a Router/Simulator.o
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
PDN/PDNHelper.o:PDN/PDNHelper.cpp PDN/PDNHelper.hpp
Router/RouterV4.o:Router/RouterV4.cpp Router/RouterV4.hpp
Router/GlobalRouter.o:Router/GlobalRouter.cpp Router/GlobalRouter.hpp
Router/RouterUtil.o:Router/RouterUtil.cpp Router/RouterUtil.hpp
Router/Graph.o:Router/Graph.cpp Router/Graph.hpp
Util/OutputFilesGenerator.o:Util/OutputFilesGenerator.cpp Util/OutputFilesGenerator.hpp
Parsers/lefrw.o:Parsers/lefrw.cpp Parsers/lefrw.h
Router/Simulator.o:Router/Simulator.cpp Router/Simulator.hpp
Parsers/ngspice.o:Parsers/ngspice.cpp Parsers/ngspice.hpp
Util/SpiceGenerator.o:Util/SpiceGenerator.cpp Util/SpiceGenerator.hpp
Util/DefGenerator.o:Util/DefGenerator.cpp Util/DefGenerator.hpp
Parsers/verilog.o:Parsers/verilog.cpp Parsers/verilog.hpp
Parsers/defrw.o:Parsers/defrw.cpp Parsers/defrw.h
Router/Graph_SP.o:Router/Graph_SP.cpp Router/Graph_SP.hpp
Router/BinaryHeap:Router/BinaryHeap.cpp Router/BinaryHeap.hpp
Parsers/Parser.o: Parsers/Parsers.cpp Parsers/Parsers.hpp
Parsers/InitialFileParser.o:Parsers/InitialFileParser.cpp Parsers/InitialFileParser.hpp
Parsers/Printer.o:Parsers/Printer.cpp Parsers/Printer.hpp

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



