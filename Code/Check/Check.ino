#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(D7, D6);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()  
{
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
  
  Serial.begin(115200);
  while (!Serial);
  delay(100);
  Serial.println("\n\nFingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  uint8_t p = finger.ledOff();
}


int fingerprintID = 0;
int state=0, stateLast=0;

void loop()
{
  state = digitalRead(D5);
  if(state && !stateLast) {
    fingerprintID = getFingerprintIDez();
    delay(10);
    finger.ledOff();
    delay(50);
  
    if(fingerprintID == -1)  return;
    else if(fingerprintID == -3) {
      Serial.println("\nFingerprint incorrect");
    }
    else if(fingerprintID == 1 || fingerprintID == 2) {
      digitalWrite(D1, HIGH);
      Serial.println("Fingerprint correct\n");
      delay(1000);
      digitalWrite(D1, LOW);
    }
  }
  stateLast = state;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -2;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -3;
  
  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence);
  return finger.fingerID; 
}
