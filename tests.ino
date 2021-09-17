//
//test(colorEncoding) {
//
//  int r, g, b;
//  String testString = "#000000";
//  parseColor(testString, r, g, b);
//
//  assertEqual((int)0, r);
//  assertEqual((int)0, g);
//  assertEqual((int)0, b);
//
//  //////////
//  testString = "#ff0000";
//  parseColor(testString, r, g, b);
//
//  assertEqual((int)255, r);
//  assertEqual((int)0, g);
//  assertEqual((int)0, b);
//
//  //////////
//  testString = "#00ff00";
//  parseColor(testString, r, g, b);
//
//  assertEqual((int)0, r);
//  assertEqual((int)255, g);
//  assertEqual((int)0, b);
//
//  //////////
//  testString = "#0000ff";
//  parseColor(testString, r, g, b);
//
//  assertEqual((int)0, r);
//  assertEqual((int)0, g);
//  assertEqual((int)255, b);
//
//  //////////
//  testString = "#050403";
//  parseColor(testString, r, g, b);
//
//  assertEqual((int)5, r);
//  assertEqual((int)4, g);
//  assertEqual((int)3, b);
//
//}
