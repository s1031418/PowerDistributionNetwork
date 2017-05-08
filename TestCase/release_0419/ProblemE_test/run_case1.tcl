set init_verilog "./data/case1.v"
set init_top_cell top
set init_design_netlisttype Verilog
set init_lef_file {./data/tech.lef ./data/blocks.lef}
set init_gnd_net {VSS}  
set init_pwr_net {VDD1 VDD2 VDD3}
set defHierChar /
init_design
defIn ./data/case1_output.def
verifyGeometry
verifyConnectivity -noAntenna