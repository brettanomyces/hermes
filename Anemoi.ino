

void setup() {

  Serial.begin(9600);

} // Void Setup Close

void loop() {

  float analog;
  
  int i;
  int j;
  for (i = 0; i < 3; i++){
    
    // Get average of ten readings 
    float sum = 0;
    for (j = 0; j < 10; j++) {
      sum +=  analogRead(i);
    }
    analog = sum / 10;
    
    Serial.print(calculateResistance(calculateVoltage(analog)));
    Serial.print(", ");
  }
  
  Serial.println();

  //Delay to make serial out readable
  delay(1000);

} // void loop close


float calculateVoltage(float analog){
  return (analog / 1024) * 5.0;
}

float calculateResistance(float voltage){
  return 100000 * (1 / ((5 / voltage) - 1));
}

