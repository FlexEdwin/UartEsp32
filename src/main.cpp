#include <Arduino.h>
#include <ESP32Servo.h>

HardwareSerial UART2(2);

const int PIN_ROL = 4;
const int PIN_TX = 17;
const int PIN_RX = 16;

const int PIN_LED = 25;
const int PIN_SERVO = 26;
Servo miServo;

bool esTransmisor = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_ROL, INPUT);
  
  // Inicia comunicación serial UART2 entre placas
  UART2.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(500);

  esTransmisor = (digitalRead(PIN_ROL) == LOW);

  if (esTransmisor) {
    Serial.println("ESP32 A: INICIADO COMO TRANSMISOR");
  } else {
    Serial.println("ESP32 B: INICIADO COMO RECEPTOR");
    pinMode(PIN_LED, OUTPUT);
    miServo.attach(PIN_SERVO);
  }
}

void loop() {
  // LÓGICA DEL TRANSMISOR
  if (esTransmisor) {
    UART2.println("HIGH");
    delay(1000);
    
    UART2.println("90");
    delay(2000);
    
    UART2.println("LOW");
    delay(1000);
    
    UART2.println("HIGH");
    delay(1000);
    
    UART2.println("180");
    delay(2000);
    
    UART2.println("LOW");
    delay(1000);
    
    UART2.println("0");
    delay(2000);
  } 
  // LÓGICA DEL RECEPTOR
  else if (UART2.available()) {
    String mensaje = UART2.readStringUntil('\n');
    mensaje.trim();
    
    Serial.print("Comando recibido por UART: ");
    Serial.println(mensaje);

    if (mensaje == "HIGH") {
      digitalWrite(PIN_LED, HIGH);
    } 
    else if (mensaje == "LOW") {
      digitalWrite(PIN_LED, LOW);
    } 
    else if (mensaje == "0" || mensaje == "90" || mensaje == "180") {
      int grados = mensaje.toInt();
      miServo.write(grados);
    }
  }
}