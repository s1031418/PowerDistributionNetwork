set init_verilog "./TestCase/2017/case2/case2.v"
set init_top_cell top
set init_design_netlisttype Verilog
set init_lef_file {./TestCase/2017/case2/tech.lef ./TestCase/2017/case2/blocks.lef}
set init_gnd_net {VSS}
set init_pwr_net {VDD1 VDD2 VDD3 VDD4 VDD5 VDD6}
set defHierChar /
init_design
defIn ./case2_output.def
verifyGeometry
verifyConnectivity -noAntenna

