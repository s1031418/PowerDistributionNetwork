module top (VDD1, VDD2, VDD3, VDD4, VDD5, VDD6, VDD7, VDD8, VDD9, VDD10, VDD11, VDD12, VDD13, VDD14, VDD15, VDD16, VDD17, VDD18, VDD19, VDD20, VDD21, VDD22, VDD23, VDD24, VDD25, VDD26, VDD_111a, VDD_111b );
  input VDD1, VDD2, VDD3, VDD4, VDD5, VDD6, VDD7, VDD8, VDD9, VDD10, VDD11, VDD12, VDD13, VDD14, VDD15, VDD16, VDD17, VDD18, VDD19, VDD20, VDD21, VDD22, VDD23, VDD24, VDD25, VDD26, VDD_111a, VDD_111b ;

  block1 B1_01 ( .VDD_A(VDD1), .VDD_B(VDD2) );
  block1 B1_02 ( .VDD_A(VDD1), .VDD_B(VDD2) );
  block1 B1_03 ( .VDD_A(VDD2), .VDD_B(VDD2) );
  block1 B1_04 ( .VDD_A(VDD2), .VDD_B(VDD4) );

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
  block5 B5_03 ( .VDD_A(VDD5), .VDD_B(VDD8) );
  block5 B5_04 ( .VDD_A(VDD5), .VDD_B(VDD7) );
  block5 B5_05 ( .VDD_A(VDD5), .VDD_B(VDD7) );

/* multi-in-a */
  block6 B6_01 ( .VDD_A(VDD_111a) );
  block6 B6_02 ( .VDD_A(VDD_111a) );
  block6 B6_03 ( .VDD_A(VDD_111a) );

// multi-in-b
  block6 B6_04 ( .VDD_A(VDD_111b) );
  block6 B6_05 ( .VDD_A(VDD_111b) );
  block6 B6_06 ( .VDD_A(VDD_111b) );
 

  block7 B7_01 ( .VDD_A(VDD9) );
  block7 B7_02 ( .VDD_A(VDD10) );
  block7 B7_03 ( .VDD_A(VDD11) );
  block7 B7_04 ( .VDD_A(VDD11) );
  block7 B7_05 ( .VDD_A(VDD12) );
  block7 B7_06 ( .VDD_A(VDD13) );
  block7 B7_07 ( .VDD_A(VDD14) );
  block7 B7_08 ( .VDD_A(VDD15) );
  block7 B7_09 ( .VDD_A(VDD15) );
  block7 B7_11 ( .VDD_A(VDD9) );
  block7 B7_12 ( .VDD_A(VDD10) );
  block7 B7_13 ( .VDD_A(VDD11) );
  block7 B7_14 ( .VDD_A(VDD11) );
  block7 B7_15 ( .VDD_A(VDD12) );
  block7 B7_16 ( .VDD_A(VDD13) );
  block7 B7_17 ( .VDD_A(VDD14) );
  block7 B7_18 ( .VDD_A(VDD15) );
  block7 B7_19 ( .VDD_A(VDD15) );

  block8 B8___55688_01_	( .VDD_A(VDD15) );
  block8 B8___55688_02_	( .VDD_A(VDD16) );
  block8 B8___55688_03_	( .VDD_A(VDD17) );
  block8 B8___55688_04_ ( .VDD_A(VDD18) );
  block8 B8___55688_05_ ( .VDD_A(VDD19) );
  block8 B8___55688_06_ ( .VDD_A(VDD20) );
  block8 B8___55688_07_ ( .VDD_A(VDD21) );
  block8 B8___55688_08_ ( .VDD_A(VDD23) );
  block8 B8___55688_09_ ( .VDD_A(VDD24) );
  block8 B8___55688_10_ ( .VDD_A(VDD25) );
  block8 B8___55688_11_ ( .VDD_A(VDD25) );

  block9 B9_01 ( .VDD_A(VDD18), .VDD_B(VDD20), .VDD_C(VDD22), .VDD_D(VDD24), .VDD_E(VDD26) );
  block9 B9_02 ( .VDD_A(VDD5), .VDD_B(VDD10), .VDD_C(VDD15), .VDD_D(VDD20), .VDD_E(VDD25) );


endmodule

