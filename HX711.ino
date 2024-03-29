void setupHX711()
{
  delay(10);
  Serial.println("Starting...");

  float calibrationValue_1;
  float calibrationValue_2;

  calibrationValue_1 = 696.0;
  calibrationValue_2 = 733.0;

  LoadCell_1.begin();
  LoadCell_2.begin();

  unsigned long stabilizingtime = 2000;
  boolean _tare = true;
  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  while ((loadcell_1_rdy + loadcell_2_rdy) < 2)
  {
    if (!loadcell_1_rdy)
      loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!loadcell_2_rdy)
      loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
  }
  if (LoadCell_1.getTareTimeoutFlag())
  {
    Serial.println("Timeout, check MCU>HX711 no.1 wiring and pin designations");
  }
  if (LoadCell_2.getTareTimeoutFlag())
  {
    Serial.println("Timeout, check MCU>HX711 no.2 wiring and pin designations");
  }
  LoadCell_1.setCalFactor(calibrationValue_1);
  LoadCell_2.setCalFactor(calibrationValue_2);
  Serial.println("Startup is complete");
}

bool checkLoadCells()
{
  int kontaktron = !digitalRead(REED);
  bool newDataReady = 0;
  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();

  if ((newDataReady)) {
    if (millis() > t) {
      float waga_dziecka = LoadCell_1.getData();
      float waga_doroslego = LoadCell_2.getData();
      Serial.print("Load_cell 1 output val: ");
      Serial.print(waga_dziecka);
      Serial.print("Load_cell 2 output val: ");
      Serial.println(waga_doroslego);
      Serial.print("Kontaktron: ");
      Serial.println(kontaktron);
      t = millis();

      if (waga_dziecka > 5 && waga_doroslego < 10 && kontaktron==HIGH) return true;

      return false;
    }
  }
}