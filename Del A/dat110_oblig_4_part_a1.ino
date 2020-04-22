int knapp1 = 0;
int knapp2 = 0;
int sensor = 0;
int tilstand = 0;

//knapp 1 = 1, knapp 2 = 2
//rett kombinasjon er 1 -> 2, altså (trykk1 == 1 && trykk2 == 2)
int trykk1 = 0;
int trykk2 = 0;

const int verdiKnapp1 = 1;
const int verdiKnapp2 = 2;
//endre på rettTrykk1 og 2 for å endre kombinasjonen (11, 12, 21, 22)
const int rettTrykk1 = 1;
const int rettTrykk2 = 2;

const int timeout = 15000; //15 sek

//er denne er false etter ein har trykka og det har skjedd noko
bool handtertTrykk = true;

const int LOCKED = 0;
const int WAITING0 = 1;
const int WAITING1 = 2;
const int CHECKING = 3;
const int UNLOCKED = 4;

const int raudUt = 2;
const int gulUt = 3;
const int groenUt = 4;
const int sensorInn = 5;
const int k1Inn = 7;
const int k2Inn = 8;

unsigned long enterMillis = 0;

void setup()
{
  pinMode(k1Inn, INPUT);
  pinMode(k2Inn, INPUT);
  pinMode(sensorInn, INPUT);
  pinMode(raudUt, OUTPUT);
  pinMode(gulUt, OUTPUT);
  pinMode(groenUt, OUTPUT);
  
  //Serial.begin(9600);
  
  skruPaaLED(raudUt);

}

void loop()
{
  knapp1 = digitalRead(k1Inn);
  knapp2 = digitalRead(k2Inn);
  sensor = digitalRead(sensorInn);
  //Serial.println(sensor);
  
  if (knapp1 == LOW && knapp2 == LOW) {
  	 handtertTrykk = true;
  }
  
  switch (tilstand) {
   
    //case 0 = locked
    //case 1 = waiting 0
    //case 2 = waiting 1
    //case 3 = checking
    //case 4 = unlocked
    
    case LOCKED:
      locked();
      break;
    
    case WAITING0:
      waiting(0);
      break;
    
    case WAITING1:
      waiting(1);
      break;
    
    case CHECKING:
      checking();
      break;
    
    case UNLOCKED:
      unlocked();
      break;
  
    default:
      break;
  }
  
  delay(10); // Wait for 10 millisecond(s)
}

void locked() {
  
  if (sensor == HIGH) {
  	tilstand = WAITING0;
    skruPaaLED(gulUt);
    enterMillis = millis();
    //Serial.println(enterMillis);
  }
  //gjer ingenting elles
}

void waiting(int status) {
  
  unsigned long nyMillis = millis();
  //Serial.println(nyMillis);
  
  //Serial.println(nyMillis - enterMillis);
  
  if (nyMillis - enterMillis >= timeout) {
    tilstand = LOCKED;
    skruAvLED(gulUt);
    delay(300);
    blinkLED(raudUt);
  } else if (handtertTrykk && (knapp1 == HIGH || knapp2 == HIGH)) {
    //enterMillis = millis();
    blinkLED(gulUt);
    handtertTrykk = false;
    
    if (status == 0) {
      tilstand = WAITING1;
      
      //sjekker kva knapp som blei trykka
      if (knapp1 == HIGH) {
      	trykk1 = verdiKnapp1; 
      } else {
        trykk1 = verdiKnapp2; 
      }
    
    } else if (status == 1) {
      tilstand = CHECKING;
      
      //sjekker kva knapp som blei trykka
      if (knapp1 == HIGH) {
      	trykk2 = verdiKnapp1; 
      } else {
        trykk2 = verdiKnapp2; 
      }
    }
  } else {
   	//gjer ingenting 
  }
  
}

void unlocked() {
  delay(timeout); //venter ei stund
  
  //går tilbake til locked
  tilstand = LOCKED;
  skruAvLED(groenUt);
  delay(200);
  skruPaaLED(raudUt);
}

void checking() {
  
  if(trykk1 == rettTrykk1 && trykk2 == rettTrykk2) {
    //korrekt kode
    tilstand = UNLOCKED;
    skruAvLED(gulUt);
    skruAvLED(raudUt);
    delay(200);
    skruPaaLED(groenUt);
  } else {
    tilstand = LOCKED;
    skruAvLED(gulUt);
    delay(300);
    blinkLED(raudUt);
  }
  
  //nulstiller trykka
  trykk1 = 0;
  trykk2 = 0;
}
              
void skruPaaLED(int fargeUt) {
  digitalWrite(fargeUt, HIGH);
}

void skruAvLED(int fargeUt) {
  digitalWrite(fargeUt, LOW);
}
             
void blinkLED(int fargeUt) {
  digitalWrite(fargeUt, LOW);
  delay(250);
  digitalWrite(fargeUt, HIGH);
  delay(250);
  digitalWrite(fargeUt, LOW);
  delay(250);
  digitalWrite(fargeUt, HIGH);
}