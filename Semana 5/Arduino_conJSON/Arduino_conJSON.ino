#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <task.h>

// ---------------- Configuración ----------------
const uint8_t PIN_BOTON_2 = 2;
const uint8_t PIN_BOTON_3 = 3;

#define PRIORIDAD_TASK_PIN2   1  // prioridad BAJA
#define PRIORIDAD_TASK_PIN3   2  // prioridad ALTA

// Variables 
volatile int estadoBoton2 = 0;
volatile int estadoBoton3 = 0;

// ---------------- Tarea del botón en PIN 2 ----------------
void taskBoton2(void *pvParameters) {
  (void)pvParameters;
  int ultimoEstado = -1;

  for (;;) {
    int estado = (digitalRead(PIN_BOTON_2) == LOW) ? 1 : 0;

    if (estado != ultimoEstado) {
      vTaskDelay(pdMS_TO_TICKS(50));          // debounce
      estado = (digitalRead(PIN_BOTON_2) == LOW) ? 1 : 0;
    }

    if (estado != ultimoEstado) {
      ultimoEstado = estado;
      estadoBoton2 = estado;  // Actualiza variable compartida
      
      Serial.print("BOTON_2:");
      Serial.println(estado ? "1" : "0");
    }

    vTaskDelay(pdMS_TO_TICKS(20)); 
  }
}

// ---------------- Tarea del botón en PIN 3 ----------------
void taskBoton3(void *pvParameters) {
  (void)pvParameters;
  int ultimoEstado = -1;

  for (;;) {
    int estado = (digitalRead(PIN_BOTON_3) == LOW) ? 1 : 0;

    if (estado != ultimoEstado) {
      vTaskDelay(pdMS_TO_TICKS(50));          // debounce
      estado = (digitalRead(PIN_BOTON_3) == LOW) ? 1 : 0;
    }

    if (estado != ultimoEstado) {
      ultimoEstado = estado;
      estadoBoton3 = estado;  // Actualiza variable compartida
      
      Serial.print("BOTON_3:");
      Serial.println(estado ? "1" : "0");
    }

    vTaskDelay(pdMS_TO_TICKS(20));            
  }
}

// ---------------- setup / loop ----------------
void setup() {
  Serial.begin(9600);
  while (!Serial);  // Espera a que el puerto Serial esté listo (importante para USB)

  pinMode(PIN_BOTON_2, INPUT_PULLUP);
  pinMode(PIN_BOTON_3, INPUT_PULLUP);

  // Crear tareas: (función, nombre, pila, params, prioridad, handle)
  xTaskCreate(taskBoton2, "Boton2", 128, NULL, PRIORIDAD_TASK_PIN2, NULL);
  xTaskCreate(taskBoton3, "Boton3", 128, NULL, PRIORIDAD_TASK_PIN3, NULL);

  vTaskStartScheduler();   // desde aquí manda FreeRTOS; no retorna

  // no hubo RAM/heap suficiente:
  Serial.println("ERROR: heap insuficiente, reduce pilas a 96");
  while (1);
}

void loop() {
  // NUNCA se ejecuta
}