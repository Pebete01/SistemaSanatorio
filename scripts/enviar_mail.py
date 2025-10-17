import sys
import smtplib
from email.mime.text import MIMEText
import os
from dotenv import load_dotenv
# VERSION PARA ENVIAR AUTOMATICAMENTE FALTANDO 30 MINS
# Cargar las variables desde el archivo .env
load_dotenv()

# --- LEER CREDENCIALES DE FORMA SEGURA ---
# Aquí cargas las credenciales en estas variables
EMAIL_USUARIO = os.getenv("SENDER_EMAIL")
EMAIL_PASSWORD = os.getenv("SENDER_PASSWORD")

if not EMAIL_USUARIO or not EMAIL_PASSWORD:
    print("Error: Las variables de entorno SENDER_EMAIL o SENDER_PASSWORD no están configuradas.")
    sys.exit(1)


#argv obtiene los argumentos que se pasan por comando
if len(sys.argv) < 5:
    print("Uso: python enviar_mail.py <nombre> <mail> <fecha> <hora>")
    sys.exit(1)

nombre = sys.argv[1]
destinatario = sys.argv[2]
fecha = sys.argv[3]
hora = sys.argv[4]

cuerpo = f"Hola {nombre},\n\nTe recordamos que tu turno es el {fecha} a las {hora}.\n\nSaludos,\nSanatoriosCJT"
mensaje = MIMEText(cuerpo) #crea el cuerpo del mail
mensaje["Subject"] = "Recordatorio de turno"
mensaje["From"] = "SENDER_EMAIL"       # tu correo
mensaje["To"] = destinatario

'''with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
    server.login("SENDER_EMAIL", "SENDER_PASSWORD")  # contraseña de aplicación
    server.send_message(mensaje)'''
try:
    with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
        # ---- LÍNEA A CORREGIR ----
        # Usa las variables, NO las cadenas de texto
        server.login(EMAIL_USUARIO, EMAIL_PASSWORD) # <-- ¡CORRECCIÓN!

        server.send_message(mensaje)
    #print(f"Correo enviado correctamente a {destinatario}")
except Exception as e:
    print(f"Error al enviar el correo: {e}")

#print(f" Correo enviado correctamente a {destinatario}\n")