# Manual de Instalación de Docker y Docker Compose

## 1. Introducción

Este manual presenta la instalación de Docker y Docker Compose en los sistemas operativos más utilizados: Windows, Ubuntu/Linux, Fedora/RPM, y macOS. Está diseñado para ser una guía clara, técnica y profesional para entornos de desarrollo, pruebas y despliegue.

Docker permite ejecutar aplicaciones en contenedores de forma aislada y reproducible. Docker Compose facilita la definición y gestión de aplicaciones multi-contenedor a través de archivos YAML.

## 2. Requisitos generales

Antes de instalar Docker, asegúrese de cumplir con lo siguiente:

- Usuario con permisos de administrador o sudo.
- Conexión a internet activa.
- Sistema operativo compatible.
- Hardware con soporte de virtualización (especialmente en Windows y macOS).

### Requisitos mínimos recomendados

- CPU: 64-bit.
- RAM: mínimo 4 GB recomendados 8 GB o más.
- Almacenamiento: al menos 20 GB libres.
- Red: acceso HTTP/HTTPS a los repositorios de Docker.

## 3. Qué es Docker y Docker Compose

### Docker
Docker es una plataforma para crear, empaquetar y ejecutar aplicaciones en contenedores. Un contenedor encapsula la aplicación y sus dependencias, asegurando que se ejecute igual en cualquier entorno.

### Docker Compose
Docker Compose es una herramienta para definir y ejecutar aplicaciones formadas por varios contenedores usando un archivo `docker-compose.yml`. Permite levantar servicios con una sola orden.

Ejemplo básico:

```yaml
services:
  web:
    image: nginx:latest
    ports:
      - "8080:80"
```

## 4. Verificación de la instalación

Después de instalar Docker, verifique que funciona con estos comandos:

```bash
docker --version
docker compose version
```

Si el comando devuelve la versión instalada, la instalación fue exitosa.

También puede probar un contenedor de ejemplo:

```bash
docker run hello-world
```

## 5. Instalación en Windows

### 5.1 Requisitos

- Windows 10/11 64-bit.
- Habilitar la virtualización en BIOS/UEFI.
- WSL 2 recomendado.

### 5.2 Método recomendado: Docker Desktop

1. Descargue Docker Desktop desde la página oficial:
   https://www.docker.com/products/docker-desktop/
2. Ejecute el instalador.
3. Siga el asistente de instalación.
4. Cuando termine, reinicie el equipo si es requerido.
5. Inicie Docker Desktop.
6. Acepte los términos y espere a que se inicialice.

### 5.3 Verificar instalación

```powershell
docker --version
docker compose version
```

### 5.4 Activar WSL 2

Si Docker muestra un aviso de WSL, ejecute:

```powershell
wsl --install
```

Luego reinicie y asegúrese de que el kernel de WSL 2 esté activo.

### 5.5 Instalación con winget (opcional)

```powershell
winget install --id Docker.DockerDesktop -e
```

### 5.6 Solución rápida de problemas en Windows

- Si Docker no inicia: verifique la virtualización y el soporte de Hyper-V/WSL 2.
- Si el servicio no responde: reinicie Docker Desktop y compruebe el estado del servicio.
- Si no puede ejecutar contenedores: asegúrese de que el usuario actual tenga permisos adecuados.

---

## 6. Instalación en Ubuntu / Debian

### 6.1 Instalar dependencias

```bash
sudo apt-get update
sudo apt-get install -y ca-certificates curl gnupg
```

### 6.2 Agregar la clave GPG oficial de Docker

```bash
sudo install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
sudo chmod a+r /etc/apt/keyrings/docker.gpg
```

### 6.3 Agregar el repositorio de Docker

```bash
echo \
  "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

### 6.4 Instalar Docker Engine

```bash
sudo apt-get update
sudo apt-get install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

### 6.5 Iniciar y habilitar el servicio

```bash
sudo systemctl enable --now docker
```

### 6.6 Verificar el usuario no root

```bash
sudo usermod -aG docker $USER
newgrp docker
```

### 6.7 Verificación final

```bash
docker --version
docker compose version
```

### 6.8 Probar la instalación

```bash
docker run hello-world
```

---

## 7. Instalación en Fedora / RHEL / CentOS (RPM)

### 7.1 Preparar el sistema

```bash
sudo dnf -y install dnf-plugins-core
```

### 7.2 Agregar el repositorio de Docker

```bash
sudo dnf config-manager --add-repo https://download.docker.com/linux/fedora/docker-ce.repo
```

### 7.3 Instalar Docker Engine

```bash
sudo dnf install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

### 7.4 Iniciar el servicio

```bash
sudo systemctl enable --now docker
```

### 7.5 Agregar usuario al grupo docker

```bash
sudo usermod -aG docker $USER
newgrp docker
```

### 7.6 Verificación

```bash
docker --version
docker compose version
```

### 7.7 Prueba rápida

```bash
docker run hello-world
```

> Si usa Red Hat Enterprise Linux u otra distribución basada en RPM, revise la compatibilidad del repositorio antes de instalar.

---

## 8. Instalación en macOS

### 8.1 Método recomendado: Docker Desktop

1. Descargue Docker Desktop para macOS desde:
   https://www.docker.com/products/docker-desktop/
2. Abra el archivo `.dmg`.
3. Arrastre Docker.app a la carpeta de Aplicaciones.
4. Abra Docker Desktop y siga la configuración inicial.
5. Espere a que se inicialice correctamente.

### 8.2 Verificación

```bash
docker --version
docker compose version
```

### 8.3 Opción alternativa con Homebrew

```bash
brew install --cask docker
```

Luego abra Docker Desktop desde la aplicación y espere a que se conecte.

### 8.4 Consideraciones en macOS

- Para Apple Silicon, Docker Desktop está completamente soportado.
- Se recomienda habilitar la integración con la línea de comandos si el instalador lo solicita.
- Asegúrese de que el servicio se haya iniciado correctamente antes de usar contenedores.

---

## 9. Instalación de Docker Compose

Docker Compose se puede usar como plugin o como binario independiente.

### 9.1 Método recomendado: plugin de Docker

Este es el método más común y recomendado, ya que se instala junto con Docker Engine:

```bash
docker compose version
```

Si la salida muestra la versión, Compose ya está instalado y listo para usar.

### 9.2 Instalación como binario independiente (Linux)

```bash
sudo curl -SL https://github.com/docker/compose/releases/download/v2.29.2/docker-compose-linux-x86_64 \
  -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
sudo ln -s /usr/local/bin/docker-compose /usr/bin/docker-compose
```

Verificación:

```bash
docker-compose --version
```

> Reemplazar la versión por la más reciente disponible en la página oficial de releases de Docker Compose.

### 9.3 Windows y macOS

En Windows y macOS, Docker Compose suele venir incluido con Docker Desktop. No requiere instalación adicional.

---

## 10. Primer ejemplo con Docker Compose

Cree un archivo llamado `docker-compose.yml`:

```yaml
services:
  web:
    image: nginx:latest
    ports:
      - "8080:80"
```

Ejecutelo con:

```bash
docker compose up -d
```

Verifique el servicio:

```bash
docker compose ps
```

Deténgalo:

```bash
docker compose down
```

---

## 11. Comandos útiles de Docker

### Ver información del sistema

```bash
docker info
```

### Listar contenedores

```bash
docker ps
```

### Listar imágenes

```bash
docker images
```

### Descargar una imagen

```bash
docker pull nginx:latest
```

### Ejecutar un contenedor

```bash
docker run -d --name mi-nginx -p 8080:80 nginx:latest
```

### Ver logs

```bash
docker logs mi-nginx
```

### Eliminar contenedores

```bash
docker rm -f mi-nginx
```

### Eliminar imágenes

```bash
docker rmi nginx:latest
```

---

## 12. Comandos útiles de Docker Compose

### Levantar servicios

```bash
docker compose up -d
```

### Ver estado de servicios

```bash
docker compose ps
```

### Ver logs

```bash
docker compose logs -f
```

### Reiniciar servicios

```bash
docker compose restart
```

### Detener servicios

```bash
docker compose stop
```

### Eliminar recursos

```bash
docker compose down
```

---

## 13. Solución de problemas comunes

### Error: "permission denied while trying to connect to the Docker daemon socket"

Ejecute:

```bash
sudo usermod -aG docker $USER
newgrp docker
```

### Error: Docker no inicia

- Revise si la virtualización está habilitada.
- Verifique que el servicio de Docker esté activo.
- En Windows/macOS, reinicie Docker Desktop.

### Error: no se encuentra el comando docker

Asegúrese de que Docker esté instalado correctamente y que el PATH del sistema esté actualizado.

### Error con WSL 2 en Windows

```powershell
wsl --update
wsl --status
```

### Error: compose no encontrado

```bash
docker compose version
```

Si devuelve error, instale el plugin o el binario de Compose.

---

## 14. Buenas prácticas

- Mantenga Docker actualizado.
- No ejecute contenedores con privilegios innecesarios.
- Use archivos `.env` para variables de entorno.
- Evite imágenes no oficiales o sin mantenimiento.
- Use volúmenes persistentes para datos críticos.
- Configure redes y puertos con intención clara.
- Use `docker compose` para orquestación local y desarrollo.

---

## 15. Desinstalación (opcional)

### Ubuntu/Debian

```bash
sudo apt-get purge -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
sudo rm -rf /var/lib/docker /var/lib/containerd
```

### Fedora/RPM

```bash
sudo dnf remove -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
sudo rm -rf /var/lib/docker /var/lib/containerd
```

### Windows/macOS

Desinstale Docker Desktop desde el panel de configuración o desde el sistema operativo, según corresponda.

---

## 16. Resumen rápido

| Sistema operativo | Método recomendado | Comando de verificación |
|---|---|---|
| Windows | Docker Desktop | `docker --version` |
| Ubuntu/Debian | Repositorio oficial de Docker | `docker compose version` |
| Fedora/RPM | Repositorio oficial de Docker | `docker compose version` |
| macOS | Docker Desktop | `docker --version` |

---

## 17. Conclusión

La instalación de Docker y Docker Compose es fundamental para trabajar con entornos modernos de desarrollo, despliegue y microservicios. La elección del método correcto depende del sistema operativo, pero el flujo general es similar: instalar Docker Engine, habilitar el servicio y verificar la funcionalidad con un contenedor de prueba.

Con esta guía, puede instalar Docker de manera segura y reproducible en los principales sistemas operativos, además de preparar su entorno para trabajar con aplicaciones multi-contenedor mediante Docker Compose.

---

## 18. Referencias oficiales

- Docker Docs: https://docs.docker.com/
- Docker Desktop: https://www.docker.com/products/docker-desktop/
- Docker Compose: https://docs.docker.com/compose/
- Ubuntu repositories: https://docs.docker.com/engine/install/ubuntu/
- Fedora repositories: https://docs.docker.com/engine/install/fedora/


