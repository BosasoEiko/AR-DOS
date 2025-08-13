void frag(String input, String output[]) {  //Re-written 29/06/2025 16:45
  uint16_t parStart = 0;
  uint8_t parCount = 0;
  for (uint16_t i = 0; i < input.length() && parCount < parLimit; i++) {
    if (input.charAt(i) == '"') {                       //Spaces are allowed
      i++;                                              //Skips the current '"'
      while (input.charAt(i) != '"') i++;               //Searches the next '"'
      output[parCount] = input.substring(parStart, i);  //Saves everything in between those two
      parStart = 0;
    } else if (input.charAt(i) == ' ' && i - parStart > 0) {  //Spaces are not allowed
      output[parCount] = input.substring(parStart, i);        //Saves everything until the space
      parStart = 0;
    }
  }
}