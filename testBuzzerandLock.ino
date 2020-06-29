// lock : 10, buzzer : d0, lock : d8, D11: SD2, S12: SD3
void setup() {
  // put your setup code here, to run once:
  pinMode(D0,OUTPUT);
  pinMode(D8,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D0,0);
  digitalWrite(D8,0);
  delay(1000);
  digitalWrite(D0,1);
  digitalWrite(D8,1);
  delay(1000);
  

}
