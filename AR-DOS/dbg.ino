void dbg() {
  Serial.println("DEBUG\n\n--------------");
  String a = "-69";
  int b = -69;
  Serial.println(a.toInt());
  Serial.println(a.toInt(), 16);
  Serial.println(String(b, 16));
  a = String(b, 16);
  Serial.println(a);
  Serial.println(strtol(a.c_str(), NULL, 16));
}