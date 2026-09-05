# Manual de Instalación y Configuración de Eclipse Mosquitto con Docker Compose

## 1. Introducción

Este manual describe la instalación y configuración de Eclipse Mosquitto usando Docker Compose para crear un broker MQTT de manera rápida, segura y reproducible.

Eclipse Mosquitto es un broker MQTT de código abierto ampliamente utilizado para comunicación entre dispositivos IoT, aplicaciones web, servicios backend y sistemas embebidos.

Docker Compose permite levantar el servicio con una sola configuración, gestionando contenedores, redes, volúmenes y puertos de forma ordenada.

---

## 2. Objetivo

El objetivo de este manual es:

- instalar Docker y Docker Compose correctamente,
- desplegar Eclipse Mosquitto en contenedor,
- configurar un broker MQTT básico,
- probar el funcionamiento con mensajes publicador/suscriptor,
- dejar una base segura para uso en entornos de laboratorio o producción ligera.

---

## 3. Requisitos previos

Antes de comenzar, asegúrese de contar con lo siguiente:

- Sistema operativo compatible con Docker (Windows, Linux o macOS).
- Docker instalado y funcionando.
- Docker Compose disponible.
- Acceso administrativo o permisos de sudo.
- Conexión a Internet para descargar imágenes.

### Verificación rápida

```bash
docker --version
docker compose version
```

Si ambos comandos responden con la versión instalada, entonces Docker y Docker Compose están listos.

---

## 4. Qué es MQTT y Eclipse Mosquitto

### MQTT
MQTT es un protocolo ligero de mensajería basado en publicación/suscripción, ideal para redes con pocos recursos y alta latencia.

### Eclipse Mosquitto
Eclipse Mosquitto es un broker MQTT que permite:

- publicar mensajes a tópicos,
- suscribirse a temas,
- enviar mensajes entre dispositivos,
- gestionar autenticación y seguridad básica.

Es una de las soluciones más utilizadas en Internet de las cosas (IoT), automatización y monitoreo.

---

## 5. Estructura de trabajo recomendada

Se recomienda crear una carpeta para el proyecto:

```bash
mkdir -p ~/mosquitto-project/config ~/mosquitto-project/data ~/mosquitto-project/log
```

La estructura quedaría así:

```text
mosquitto-project/
├── docker-compose.yml
├── config/
│   ├── mosquitto.conf
│   └── passwd
├── data/
├── log/
└── README.md
```

---

## 6. Configuración base de Docker Compose

Cree el archivo `docker-compose.yml` con el siguiente contenido:

```yaml
services:
  mosquitto:
    image: eclipse-mosquitto:2
    container_name: mosquitto
    restart: unless-stopped
    ports:
      - "1883:1883"
      - "9001:9001"
    volumes:
      - ./config:/mosquitto/config
      - ./data:/mosquitto/data
      - ./log:/mosquitto/log
    command: ["/usr/sbin/mosquitto", "-c", "/mosquitto/config/mosquitto.conf"]
```

### Descripción

- `1883`: puerto principal de MQTT.
- `9001`: puerto para WebSockets o cliente HTTP/WebSocket, según la configuración.
- `./config`: almacena la configuración de Mosquitto.
- `./data`: almacenamiento persistente.
- `./log`: archivos de log.

---

## 7. Archivo de configuración Mosquitto

Cree el archivo `config/mosquitto.conf`:

```conf
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/log/mosquitto.log

listener 1883
allow_anonymous true

listener 9001
protocol websockets
allow_anonymous true
```

### Explicación

- `persistence true`: guarda mensajes y estado de subscripciones en disco.
- `listener 1883`: habilita el broker MQTT estándar.
- `allow_anonymous true`: permite conexiones sin usuario/contraseña.
- `listener 9001` con `protocol websockets`: habilita acceso desde navegadores o clientes WebSocket.

> Esta configuración es conveniente para pruebas y aprendizaje. Para entornos productivos se recomienda habilitar autenticación.

---

## 8. Levantar el servicio

Desde la carpeta del proyecto, ejecute:

```bash
docker compose up -d
```

### Verificar que el contenedor esté corriendo

```bash
docker compose ps
```

### Ver logs

```bash
docker compose logs -f mosquitto
```

---

## 9. Probar la instalación

### 9.1 Publicar un mensaje

Dentro del contenedor puede usar `mosquitto_pub`:

```bash
docker compose exec mosquitto mosquitto_pub -h localhost -t prueba/test -m "Hola desde Mosquitto"
```

### 9.2 Suscribirse a un tópico

```bash
docker compose exec mosquitto mosquitto_sub -h localhost -t prueba/test -v
```

Si ambos comandos se ejecutan en terminales diferentes, verá el mensaje en tiempo real.

---

## 10. Configuración con autenticación

Para un entorno más seguro, se recomienda desactivar conexiones anónimas y usar usuarios autenticados.

### 10.1 Crear un archivo de contraseñas

Ejecute este comando en el host:

```bash
docker run --rm -it -v "$PWD/config:/mosquitto/config" eclipse-mosquitto:2 mosquitto_passwd -c /mosquitto/config/passwd usuario1
```

Se le pedirá ingresar una contraseña. 

### 10.2 Actualizar `mosquitto.conf`

```conf
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/log/mosquitto.log

listener 1883
allow_anonymous false
password_file /mosquitto/config/passwd

listener 9001
protocol websockets
allow_anonymous false
password_file /mosquitto/config/passwd
```

### 10.3 Reiniciar el contenedor

```bash
docker compose down
docker compose up -d
```

### 10.4 Probar autenticación

```bash
docker compose exec mosquitto mosquitto_pub -h localhost -t prueba/test -m "Mensaje autenticado" -u usuario1 -P "tu_contraseña"
```

Y para suscribirse:

```bash
docker compose exec mosquitto mosquitto_sub -h localhost -t prueba/test -u usuario1 -P "tu_contraseña" -v
```

---

## 11. Ejemplo completo de `docker-compose.yml` con autenticación

```yaml
services:
  mosquitto:
    image: eclipse-mosquitto:2
    container_name: mosquitto
    restart: unless-stopped
    ports:
      - "1883:1883"
      - "9001:9001"
    volumes:
      - ./config:/mosquitto/config
      - ./data:/mosquitto/data
      - ./log:/mosquitto/log
    command: ["/usr/sbin/mosquitto", "-c", "/mosquitto/config/mosquitto.conf"]
```

Archivo `config/mosquitto.conf`:

```conf
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/log/mosquitto.log

listener 1883
allow_anonymous false
password_file /mosquitto/config/passwd

listener 9001
protocol websockets
allow_anonymous false
password_file /mosquitto/config/passwd
```

---

## 12. Comandos útiles

### Ver estado del servicio

```bash
docker compose ps
```

### Reiniciar el broker

```bash
docker compose restart
```

### Detener el broker

```bash
docker compose stop
```

### Eliminar el contenedor y recursos

```bash
docker compose down
```

### Ver logs en tiempo real

```bash
docker compose logs -f
```

---

## 13. Solución de problemas comunes

### 13.1 El contenedor no inicia

Verifique que el archivo `mosquitto.conf` no tenga errores de sintaxis.

```bash
docker compose logs mosquitto
```

### 13.2 El puerto 1883 está ocupado

Compruebe si otro servicio ya está usando ese puerto.

```bash
ss -lnt | grep 1883
```

### 13.3 No puedo conectarme desde un cliente

- Verifique que el contenedor esté corriendo.
- Confirme que el puerto esté publicado correctamente.
- Revise si la autenticación está habilitada y si la contraseña es correcta.

### 13.4 Error de permisos en los volúmenes

Asegúrese de que la carpeta `data` y `log` tengan permisos adecuados en el host.

```bash
chmod -R 755 ~/mosquitto-project
```

---

## 14. Buenas prácticas

- Use autenticación en entornos reales.
- Mantenga el broker actualizado con la última imagen estable.
- Controle logs para diagnóstico.
- Use volúmenes persistentes para datos y configuración.
- No exponga puertos innecesarios en producción.
- Aísle la red del servicio cuando sea necesario.

---

## 15. Uso recomendado en IoT y desarrollo

Eclipse Mosquitto con Docker Compose es ideal para:

- prototipos de IoT,
- simulación de sensores,
- integración con aplicaciones web,
- laboratorios universitarios,
- testing de protocolos MQTT,
- microservicios con mensajería asíncrona.

---

## 16. Resumen

Con Docker Compose, la implementación de Eclipse Mosquitto es rápida y segura. En pocas configuraciones puedes montar un broker MQTT listo para publicar y suscribir mensajes. La clave está en definir correctamente el archivo `docker-compose.yml` y el archivo de configuración `mosquitto.conf`.

---

## 17. Referencias oficiales

- Docker: https://docs.docker.com/
- Docker Compose: https://docs.docker.com/compose/
- Eclipse Mosquitto: https://mosquitto.org/
- MQTT: https://mqtt.org/

---

## 18. Conclusión

Este manual proporciona una base sólida para desplegar Eclipse Mosquitto con Docker Compose, desde una configuración básica hasta una versión con autenticación. Puede adaptarse fácilmente a entornos de pruebas, desarrollo, prototipos de IoT y pequeñas implementaciones de producción.

Si necesita una versión aún más avanzada, se puede agregar TLS, autenticación con certificados, integración con Node-RED, Python o dashboards web MQTT.
