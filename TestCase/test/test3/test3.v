

module top (VDD1, VDD2, VDD3, VDD4, VDD5, VDD6 );
  input VDD1, VDD2, VDD3, VDD4, VDD5, VDD6;

  block1 B1_01 ( .VDD_A(VDD1), .VDD_B(VDD2) );

  block2 B2_01 ( .VDD_A(VDD3) );
  block2 B2_02 ( .VDD_A(VDD3) );

  block3 B3_01 ( .VDD_A(VDD3) );
 
  block4 B4_01 ( .VDD_A(VDD4) ); 
  block4 B4_02 ( .VDD_A(VDD4) );
  block4 B4_03 ( .VDD_A(VDD4) );
  block4 B4_04 ( .VDD_A(VDD4) );
  block4 B4_05 ( .VDD_A(VDD4) );
  block4 B4_06 ( .VDD_A(VDD4) );


  block5 B5_01 ( .VDD_A(VDD5), .VDD_B(VDD6) );
  block5 B5_02 ( .VDD_A(VDD5), .VDD_B(VDD6) );
  block5 B5_03 ( .VDD_A(VDD5), .VDD_B(VDD6) );
  block5 B5_04 ( .VDD_A(VDD5), .VDD_B(VDD6) );
  block5 B5_05 ( .VDD_A(VDD5), .VDD_B(VDD6) );

endmodule

