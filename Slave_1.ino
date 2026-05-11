#include <SPI.h>
#include <Controllino.h>
  
volatile byte comando = 0;
volatile bool datoNuevo = false;
volatile byte respuestaAlMaestro = 0; // Variable para enviar estado al maestro

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  pinMode(CONTROLLINO_D0, OUTPUT); // Actuador en el Esclavo 
  
  // Configuración de registros SPI como Esclavo 
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  SPDR = 0x00;
  
  Serial.println("=== ESCLAVO SPI INICIADO ===");
}

ISR(SPI_STC_vect) {
  comando = SPDR;             // Recibe el valor del potenciómetro
  SPDR = respuestaAlMaestro;  // Envía el estado del LED al Maestro (Full-duplex)
  datoNuevo = true;
}

void loop() {
  if (datoNuevo) {
    datoNuevo = false;

    // Lógica de control industrial por umbral
    // Si el valor del potenciómetro es mayor a 150 (umbral)
    if (comando > 150) {
      digitalWrite(CONTROLLINO_D0, HIGH); // Prende el LED 
      respuestaAlMaestro = 1;             // Indica "ENCENDIDO" al maestro
    } else {
      digitalWrite(CONTROLLINO_D0, LOW);  // Apaga el LED 
      respuestaAlMaestro = 0;             // Indica "APAGADO" al maestro
    }

    Serial.print("Valor Sensor Recibido: ");
    Serial.print(comando);
    Serial.print(" | Estado Actuador: ");
    Serial.println(respuestaAlMaestro == 1 ? "ENCENDIDO" : "APAGADO");
  }
}