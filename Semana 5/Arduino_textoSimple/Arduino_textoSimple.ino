#include <Arduino_FreeRTOS.h>

/*
  FreeRTOS Arduino - 2 botones, 2 tareas con prioridad distinta
  Pin 2 -> tarea prioridad 1 | Pin 3 -> tarea prioridad 2
  Serial muestra: "apagado | 0"  /  "encendido | 1"
*/

#include <Arduino.h>

#include <task.h>

// ---------------- Configuración ----------------
const uint8_t PIN_BOTON_2 = 2;
const uint8_t PIN_BOTON_3 = 3;

#define PRIORIDAD_TASK_PIN2   1     // prioridad BAJA
#define PRIORIDAD_TASK_PIN3   2     // prioridad ALTA

// ---------------- Tarea del botón en PIN 2 ----------------
void taskBoton2(void *pvParameters) {
  (void)pvParameters;
  int ultimoEstado = -1;                      // fuerza imprimir el estado inicial

  for (;;) {                                  // el "loop" de esta tarea
  int estado = (digitalRead(PIN_BOTON_2) == LOW) ? 1 : 0;  // pulsado = 1

    if (estado != ultimoEstado) {             // posible cambio -> debounce
      vTaskDelay(pdMS_TO_TICKS(50));          // espera RTOS (NO bloquea a la otra tarea)
      estado = (digitalRead(PIN_BOTON_2) == LOW) ? 1 : 0;
    }

    if (estado != ultimoEstado) {             // cambio confirmado -> imprimir
      ultimoEstado = estado;
      vTaskSuspendAll();                      // congela el planificador para
      Serial.print("[Pin 2 | prio 1]  ");     // que la otra tarea no mezcle
      Serial.println(estado ? "encendido | 1" : "apagado  | 0");
      xTaskResumeAll();                       // esta línea
    }

    vTaskDelay(pdMS_TO_TICKS(20));            // cede CPU (sondeo cada 20 ms)
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
      vTaskSuspendAll();
      Serial.print("[Pin 3 | prio 2]  ");
      Serial.println(estado ? "encendido | 1" : "apagado  | 0");
      xTaskResumeAll();
    }

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

// ---------------- setup / loop ----------------
void setup() {
  Serial.begin(9600);

  pinMode(PIN_BOTON_2, INPUT_PULLUP);
  pinMode(PIN_BOTON_3, INPUT_PULLUP);

  // Crear tareas: (función, nombre, pila, params, prioridad, handle)
  xTaskCreate(taskBoton2, "Boton2", 128, NULL, PRIORIDAD_TASK_PIN2, NULL);
  xTaskCreate(taskBoton3, "Boton3", 128, NULL, PRIORIDAD_TASK_PIN3, NULL);

  vTaskStartScheduler();   // desde aquí manda FreeRTOS; no retorna

  // Si llegas aquí, no hubo RAM/heap suficiente:
  Serial.println("ERROR: heap insuficiente, reduce pilas a 96");
  while (1);
}

void loop() {
  // NUNCA se ejecuta: el planificador adueñó del CPU
}