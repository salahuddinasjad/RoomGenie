#include <SPI.h>
#include <Ethernet.h>
#include <VirtualWire.h>
char *controller;
unsigned int pCount = 0; 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFD };
IPAddress server(158, 36, 70, 36);
IPAddress ip(158, 36, 9, 104);
EthernetClient client;

void setup() { //connect to server
  Serial.begin(9600);
  vw_set_ptt_inverted(true); //
  vw_set_tx_pin(3);
  vw_setup(4000);// speed of data transfer Kbps
  Serial.flush();
 
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }


//  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
}

void loop()
{

int Flame_Detector_sensor2; 
  int PIR_sensor;

  PIR_sensor = digitalRead(7); 
  Flame_Detector_sensor2 = analogRead(A0); 

  // Only two instance of a static field exists

  static long start1 = 0;  // It will be shared by all instances.
  static long start2 = 0;  // .....
  static int timer = 2000; //	 .....
  static int bigtimer = 3000; // ....

  // && millis() > timer + start1

  if(PIR_sensor == 1 && millis() > timer + start1){

    start1 = millis(); // start1 counter 
  
  } 


  // && millis() > timer + start2
  if(Flame_Detector_sensor2 >= 600 && millis() > timer + start2){

    start2 = millis(); // start2 counter

  } 

  if(start1 < start2 && start1 > 0){

    pCount++;
   
  Serial.print(" [ ");  
  Serial.print("Velkommen");
  Serial.print(" (");
  Serial.print(pCount);
  Serial.print(")");  
  Serial.print("\t\t");
  Serial.print(start1);
  Serial.print("\t\t");
  Serial.print("<");
  Serial.print("\t\t");
  Serial.print(start2);
  Serial.print("\t\t");
  Serial.print(PIR_sensor);
  Serial.print("\t\t");
  Serial.print(Flame_Detector_sensor2);
  Serial.print(" ] ");
  Serial.println();

    start1 = 0; 
    start2 = 0; 

  connect();
  delay(200);
  send(pCount);
  delay(200);

  }

  //  && PIR_sensor == 1
  if(start1 > start2 && start2 > 0){
    
    pCount--;
 
    Serial.print(" [ ");
    Serial.print("Hadebra"); 
    Serial.print(" (");
    Serial.print(pCount);
    Serial.print(")");
    Serial.print("\t\t");
    Serial.print(start1);
    Serial.print("\t\t");
    Serial.print(">");
    Serial.print("\t\t");
    Serial.print(start2);
    Serial.print("\t\t");
    Serial.print(PIR_sensor);
    Serial.print("\t\t");
    Serial.print(Flame_Detector_sensor2);
    Serial.print(" ] ");
    Serial.println();	


    start1 = 0; 
    start2 = 0; 

  connect();
  delay(200);
  send(pCount);
  delay(200);
  }

  // If a single sensor is triggered, without 
  // triggering both after 5 seconds (5000 ms), set that sensor timer to zero.   

  if(millis() > start1 + bigtimer && start2 == 0 && start1 > 0){
    start1 = 0; 
  } 

  if(millis() > start2 + bigtimer && start1 == 0 && start2 > 0){
    start2 = 0; 
  }
  sendRF(pCount);
  delay(100);

  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
}

void sendRF(int num){
  char buf[30];
  memset (buf, 0, sizeof(buf));
  snprintf(buf, sizeof(buf)-1, "%d", num);
  vw_send((uint8_t *)buf, strlen(buf));
  vw_wait_tx(); // Wait until the whole message is gone
}

void connect() {
  // if you get a connection, report back via serial:
  client.connect(server, 80);
  Serial.println("CONNECTED");
}

void send(int value) { 
  // Make a HTTP request:
  client.print("GET /arduino.php?rom=D201"); //158.36.70.36
  client.print("&count=");
  client.print(value);
  client.println(" HTTP/1.0");
  client.println("Host: 158.36.70.36"); 
  client.println("Connection: close");
  client.println();
  Serial.print("Sending value");
  Serial.println(value);
  
  client.stop();
}





