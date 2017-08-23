

module top (VDD1, VDD2, VDD3, VDD4 );
  input VDD1, VDD2, VDD3, VDD4;
  block1 B1 ( .VDD_A(VDD1), .VDD_B(VDD2) );
  block2 B2 ( .VDD_A(VDD3) );
  block3 B3 ( .VDD_A(VDD3) );
  block7 B7_1 ( .VDD_A(VDD4) );
  block7 B7_2 ( .VDD_A(VDD4) );
  block7 B7_3 ( .VDD_A(VDD4) );
endmodule
