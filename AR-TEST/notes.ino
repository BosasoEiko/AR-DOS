/*
  Serial.println(temp.position());
  hex2file(i2hex(0, 2), temp);  //Saves 16 bits for the amount of vars in the function
  hex2file("452A", temp); //69 42
  Serial.println(temp.position());
  temp.seek(0);
  while (temp.available()) Serial.print(temp.read());
  Serial.println();
  temp.seek(0);
  while (temp.available()) Serial.print(char(temp.read()));
  temp.seek(0);
  Serial.println();
  while (temp.available()) Serial.print(i2hex(temp.read(), 1));
  temp.close();
  Serial.println('\n');
*/