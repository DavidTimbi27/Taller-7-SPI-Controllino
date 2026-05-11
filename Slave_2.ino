#include <SPI.h>

volatile byte datoRecibido = 0;
volatile bool datoNuevo = false;
volatile byte respuestaInversa = 0;

void setup() {
  Serial.begin(9600);
  
  // Configurar MISO como salida para enviar datos al maestro
  pinMode(MISO, OUTPUT);
  
  // Habilitar SPI en modo esclavo e interrupciones
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  
  Serial.println("=== ESCLAVO 2 (UNO) INICIADO - LOGICA INVERSA ===");
}

// Interrupción de recepción SPI
ISR(SPI_STC_vect) {
  datoRecibido = SPDR;
  // Cargamos la respuesta para el siguiente ciclo
  // Si el valor > 150, el otro es ON, nosotros mandamos 1 (que el maestro leerá como APAGADO)
  if (datoRecibido > 150) {
    respuestaInversa = 1; 
  } else {
    respuestaInversa = 0;
  }
  SPDR = respuestaInversa; 
  datoNuevo = true;
}

void loop() {
  if (datoNuevo) {
    datoNuevo = false;
    
    Serial.print("Recibido: ");
    Serial.print(datoRecibido);
    Serial.print(" | Reportando al Maestro: ");
    Serial.println(respuestaInversa == 1 ? "LOGICA APAGADO" : "LOGICA ENCENDIDO");
  }
}