FROM python:3.11-slim
# Copiamos las dependencias necesarias para instalar los paquetes de Python
WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
#Copiamos el código fuente de la aplicación
COPY main.py .
#Comando para ejecutar la aplicación
CMD ["python", "main.py"]
