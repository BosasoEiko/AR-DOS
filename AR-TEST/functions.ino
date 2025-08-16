bool isVariable(String str) {
  if (((str.startsWith("u") || str.startsWith("i") || str.startsWith("f")) && (str.endsWith("8") || str.endsWith("16") || str.endsWith("32")))) {}
}

void test() {
  Serial.println(i2hex(69, 4));
  Serial.println(hex2ui(i2hex(69, 4)));
  Serial.println(i2hex(-69, 4));
  Serial.println(hex2ui(i2hex(-69, 4)));
  Serial.println(i2hex(4294967295, 4));
  Serial.println(i2hex(2147483647, 4));
  Serial.println(i2hex(-2147483648, 4));
  Serial.println(hex2i("80000000"));
  Serial.println(hex2ui("80000000"));
  Serial.println(hex2i("ffffffff"));
  Serial.println(hex2ui("ffffffff"));
}