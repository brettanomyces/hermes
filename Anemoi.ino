

void setup() {

  Serial.begin(9600);

} // Void Setup Close

void loop() {

  float analog0;
  float analog1;
  
  float resistance0;
  float resistance1;
  
  float voltage0;
  float voltage1;
  
  
  //Obtain RAW voltage data
  analog0 = analogRead(0);
  analog1 = analogRead(1);

  //Convert to actual voltage (0 - 5 Vdc)
  voltage0 = (analog0 / 1024) * 5.0;
  voltage1 = (analog1 / 1024) * 5.0;

  resistance0 = 100000 * (1 / ((5 / voltage0) - 1));
  resistance1 = 100000 * (1 / ((5 / voltage1) - 1));

  //Output to serial
  Serial.print(resistance0);
  Serial.print(", ");
  Serial.println(resistance1);

  //Delay to make serial out readable
  delay(500);

} // void loop close

