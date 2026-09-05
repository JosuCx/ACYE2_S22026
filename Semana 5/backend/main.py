import serial
import serial.tools.list_ports
import paho.mqtt.client as mqtt
import json
import time
import os
from datetime import datetime

#Configuración del cliente MQTT
MQTT_BROKER = os.environ.get('MQTT_BROKER', 'localhost')
MQTT_PORT = 1883
SERIAL_PORT = os.getenv('SERIAL_PORT', '/dev/ttyUSB0') # windows: 'COM3', Linux: '/dev/ttyUSB0'
BAUD_RATE = 9600

#estados botones actuales
estado_botones = {
"boton2":0,
"boton3":0,
"timestamp":None
}
def buscar_puerto_serial():
    #Buscar el puerto del Arduino
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        if 'Arduino' in port.description or 'CH340' in port.description or 'ttyACM' in port.device:
            return port.device
    return None

def conexion(cliente, userdata, flags, rc):
    if rc == 0:
        print("Conectado al broker MQTT")
    else:
        print(f"Error al conectar al broker MQTT: {rc}")

def procesar_datos(line):
    #publicar los datos de los botones al broker MQTT
    try:
        line =line.strip()
        if not line:
            return

        #Formato esperado: "boton2:1,boton3:0"
        if line.startswith("boton2:"):
            valor = int (line.split(":")[1].split(",")[0])
            estado_botones["boton2"] = valor
            estado_botones["timestamp"] = datetime.utcnow().isoformat() + "Z"

            #Publicar en el broker MQTT
            topic = "smartegg/boton2/estado"
            payload = json.dumps({
                "device_id": "smartegg_001",
                "sensor:": "boton2",
                "value": valor,
                "timestamp": estado_botones["timestamp"]
            })
            client.publish(topic,payload,qos=1)
            print(f"Publicado en {topic}: {payload}")
        elif line.startswith("boton3:"):
            valor = int (line.split(":")[1].split(",")[0])
            estado_botones["boton3"] = valor
            estado_botones["timestamp"] = datetime.utcnow().isoformat() + "Z"

            #Publicar en el broker MQTT
            topic = "smartegg/boton3/estado"
            payload = json.dumps({
                "device_id": "smartegg_001",
                "sensor:": "boton3",
                "value": valor,
                "timestamp": estado_botones["timestamp"]
            })
            client.publish(topic,payload,qos=1)
            print(f"Publicado en {topic}: {payload}")
    except Exception as e:
        print(f"Error al procesar los datos: {e}")            

def main():
    global client
    #Configurar el MQTT
    client = mqtt.Client()
    client.on_connect = conexion
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_start()

    #Buscar el puerto serial del Arduino
    port=buscar_puerto_serial()
    print(f"Puerto serial encontrado: {port}")
    try:
        #abrimos el puerto serial
        ser = serial.Serial(port, BAUD_RATE, timeout=1)
        time.sleep(2)  # Esperar a que el Arduino se reinicie
        print("Leyendo datos del Arduino...")
        #leer datos del Arduino
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()
                procesar_datos(line)

            time.sleep(0.01)
          # Evitar sobrecargar la CPU
    except serial.SerialException as e:
        print(f"Error al abrir el puerto serial: {e}")
    except KeyboardInterrupt:
        print("Interrumpido por el usuario")
        client.publish("smartegg/status",json.dumps({"status":"offline"}),qos=1 )
if __name__ == "__main__":
    main()