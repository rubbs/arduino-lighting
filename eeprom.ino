


void eeprom_setup(){
  EEPROM.begin(sizeof(PersistenceStruct));
  EEPROM.get(0,PersistenceStruct);

  Serial.printf("read values from eeprom:  r=%d g=%d b=%d brightness=%d\n",PersistenceStruct.r, PersistenceStruct.g,PersistenceStruct.b, PersistenceStruct.brightness);
}

void eeprom_persist(char r, char g, char b, char brightness){
  PersistenceStruct.r = r;
  PersistenceStruct.g = g;
  PersistenceStruct.b = b;
  PersistenceStruct.brightness = brightness;
  EEPROM.put(0, PersistenceStruct); // Kopiert die Strukturvariable in den internen Puffer
  EEPROM.commit();                  // Kopiert den internen Puffer in den EEPROM-Bereich
}
