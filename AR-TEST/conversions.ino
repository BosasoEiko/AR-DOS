String i2hex(uint32_t val, uint8_t bytes) {  //Integer to hex: -2147483648 -> 4294967295
  String hex = String(val, 16);
  for (uint8_t i = hex.length(); i < bytes * 2; i++) hex = String("0" + hex);
  hex.toUpperCase();
  return hex;
}

int32_t hex2i(String val) {  //Hex to signed integer: -2147483648 -> 2147483647
  return strtoul(val.c_str(), NULL, 16);
}

uint32_t hex2ui(String val) {  //Hex to unsigned integer: 0 -> 4294967295
  return strtoul(val.c_str(), NULL, 16);
}

void hex2file(String val, File& file) {
  for (uint8_t i = 0; i < val.length(); i += 2) file.write(hex2ui(val.substring(i, i + 2)));
}