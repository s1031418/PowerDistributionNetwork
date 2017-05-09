module top (VDD1, VDD2, VDD3 );
  input VDD1, VDD2, VDD3;
  block1 B1 ( .VDD_A(VDD1), .VDD_B(VDD2) );
  block2 B2 ( .VDD_A(VDD3) );
  block3 B3 ( .VDD_A(VDD3) );
endmodule
