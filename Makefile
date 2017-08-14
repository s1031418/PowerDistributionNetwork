

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
run_case4:	
	time ./cadXX.out TestCase/test/test0/test0.v TestCase/test/test0/test0_input.def TestCase/test/test0/tech.lef TestCase/test/test0/blocks.lef TestCase/test/test0/initial_files test0_output.def test0.sp output_files
run_case5:	
	time ./cadXX.out TestCase/test/test1/test1.v TestCase/test/test1/test1_input.def TestCase/test/test1/tech.lef TestCase/test/test1/blocks.lef TestCase/test/test1/initial_files test1_output.def test1.sp output_files
run_case6:	
	time ./cadXX.out TestCase/test/test2/test2.v TestCase/test/test2/test2_input.def TestCase/test/test2/tech.lef TestCase/test/test2/blocks.lef TestCase/test/test2/initial_files test2_output.def test2.sp output_files
run_case7:	
	time ./cadXX.out TestCase/test/test3/test3.v TestCase/test/test3/test3_input.def TestCase/test/test3/tech.lef TestCase/test/test3/blocks.lef TestCase/test/test3/initial_files test3_output.def test3.sp output_files
run_case8:	
	time ./cadXX.out TestCase/test/test4/test4.v TestCase/test/test4/test4_input.def TestCase/test/test4/tech.lef TestCase/test/test4/blocks.lef TestCase/test/test4/initial_files test4_output.def test4.sp output_files
run_case9:	
	time ./cadXX.out TestCase/test/test5/test5.v TestCase/test/test5/test5_input.def TestCase/test/test5/tech.lef TestCase/test/test5/blocks.lef TestCase/test/test5/initial_files test5_output.def test5.sp output_files
run_case10:	
	time ./cadXX.out TestCase/test/test6/test6.v TestCase/test/test6/test6_input.def TestCase/test/test6/tech.lef TestCase/test/test6/blocks.lef TestCase/test/test6/initial_files test6_output.def test6.sp output_files
run_case11:	
	time ./cadXX.out TestCase/test/test7/test7.v TestCase/test/test7/test7_input.def TestCase/test/test7/tech.lef TestCase/test/test7/blocks.lef TestCase/test/test7/initial_files test7_output.def test7.sp output_files
run_case12:	
	time ./cadXX.out TestCase/test/test8/test8.v TestCase/test/test8/test8_input.def TestCase/test/test8/tech.lef TestCase/test/test8/block.lef TestCase/test/test8/initial_files test8_output.def test8.sp output_files
run_case13:	
	time ./cadXX.out TestCase/test/test9/test9.v TestCase/test/test9/test9_input.def TestCase/test/test9/tech.lef TestCase/test/test9/blocks.lef TestCase/test/test9/initial_files test9_output.def test9.sp output_files
run_case14:	
	time ./cadXX.out TestCase/test/test10/test10.v TestCase/test/test10/test10_input.def TestCase/test/test10/tech.lef TestCase/test/test10/blocks.lef TestCase/test/test10/initial_files test10_output.def test10.sp output_files
run_case15:	
	time ./cadXX.out TestCase/test/test11/test11.v TestCase/test/test11/test11_input.def TestCase/test/test11/tech.lef TestCase/test/test11/blocks.lef TestCase/test/test11/initial_files test11_output.def test11.sp output_files
run_case16:	
	time ./cadXX.out TestCase/test/test12/test12.v TestCase/test/test12/test12_input.def TestCase/test/test12/tech.lef TestCase/test/test12/blocks.lef TestCase/test/test12/initial_files test12_output.def test12.sp output_files

.PHONY : clean
clean:
	cd $(LEF_PATH) && make clean
	cd $(DEF_PATH) && make clean
	-rm cadXX.out $(OBJS)



