int lightDecrements = 5;
int lightFadeStart = 15;
int ledMax = 12;

int pinA = 41;
int pinB = 37;
int pinC  = 34;
int pinD = 32;
int pinE = 31;
int pinF = 40;
int pinG = 35;
int  D1 = 42;
int D2 = 39;
int D3 = 38;
int D4 = 36;

int registeredTemp = 0;

void setup() {
  // put your setup code here, to run once:
setupPins();
// pinMode(31, OUTPUT);

// pinMode(13, OUTPUT);

Serial.begin(9600);
}

void loop() {

  int sensorValue = analogRead(A3);

  int absDifference = abs(sensorValue - registeredTemp);
  if(absDifference > 5){
    registeredTemp = sensorValue;   

  } else {
    
    return;
  }
  int led = getPrimaryLed(sensorValue);
  //Serial.println(led);


  showDisplayNumber(sensorValue);
  
  for(int i = 2; i <= ledMax; i++){
    if(led != i){
      int ledLight = lightFadeStart - (abs(led - i) * lightDecrements);
      if(ledLight < 0){
        ledLight = 0;
      }
      analogWrite(i, ledLight);
    } 
    else{
      analogWrite(i, 1000);
    }
  }



}

void showDisplayNumber(int number){
  int length = (int)floor(log10((float)number)) + 1;

    int arr[length];
    int i = 0;
    do {
        arr[i] = number % 10;
        number /= 10;
        i++;
    } while (number != 0);


  for(int i = 0; i < length; i++){
    toggleDigit(i);
    outputNumber(arr[i]);
    delay(1);
  }

  
  
}

void toggleDigit(int digit){
  switch(digit){
    case 0: 
      digitalWrite(D1, LOW);
      digitalWrite(D2,  HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH); 
      break;

      case 1: 
      digitalWrite(D1, HIGH);
      digitalWrite(D2,  LOW);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH); 
      break;

      case 2: 
      digitalWrite(D1, HIGH);
      digitalWrite(D2,  HIGH);
      digitalWrite(D3, LOW);
      digitalWrite(D4, HIGH); 
      break;

      case 3: 
      digitalWrite(D1, HIGH);
      digitalWrite(D2,  HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, LOW); 
      break;
  }
}

void outputNumber(int number) {
  switch(number){
    case 0:
      digitalWrite(pinA, HIGH); 
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, LOW);  
      break;

    case 1: 
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH); 
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      break;

    case 2:
      digitalWrite(pinA, HIGH); 
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, LOW);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, HIGH);
      break;

    case 3:
      digitalWrite(pinA, HIGH); 
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, HIGH);
      break;

    case 4:
      digitalWrite(pinA, LOW); 
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;

    case 5:
      digitalWrite(pinA, HIGH); 
      digitalWrite(pinB, LOW);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;

    case 6:
      digitalWrite(pinA, HIGH); 
      digitalWrite(pinB, LOW);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;

    case 7:
      digitalWrite(pinA, HIGH); 
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      break;

    case 8:
      digitalWrite(pinA, HIGH); 
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;

    case 9:
      digitalWrite(pinA, HIGH); 
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;
  }
}

void displayNumber(int number){
  // for(int i = 31; i < 44; i++){
  //   digitalWrite(i, HIGH);

  // }

  digitalWrite(42, HIGH);
  digitalWrite(41, HIGH);
}

void setupPins(){
  for(int i = 2; i <= ledMax; i++){
    pinMode(i, OUTPUT);
  }

  for(int i = 1; i <= 12; i++){
    pinMode(i+30, OUTPUT);
  }
}

void turnAllOn(){
  for(int i = 2; i <= ledMax; i++){
    digitalWrite(i, HIGH);
  }
}


int getPrimaryLed(int sensorValue){
  return floor(sensorValue / 100) + 2;
}
