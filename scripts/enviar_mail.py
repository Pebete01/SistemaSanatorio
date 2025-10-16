import sys
import smtplib
from email.mime.text import MIMEText


# VERSION PARA ENVIAR AUTOMATICAMENTE FALTANDO 30 MINS
#argv obtiene los argumentos que se pasan por comando
if len(sys.argv) < 5:
    print("Uso: python enviar_mail.py <nombre> <mail> <fecha> <hora>")
    sys.exit(1)

nombre = sys.argv[1]
destinatario = sys.argv[2]
fecha = sys.argv[3]
hora = sys.argv[4]

cuerpo = f"Hola {nombre},\n\nTe recordamos que tu turno es el {fecha} a las {hora}.\n\nSaludos,\nSanatorio"
mensaje = MIMEText(cuerpo) #crea el cuerpo del mail
mensaje["Subject"] = "Recordatorio de turno"
mensaje["From"] = "tomaslajnis@gmail.com"       # tu correo
mensaje["To"] = destinatario

with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
    server.login("MAIL", "CONTRASEÑA")  # contraseña de aplicación
    server.send_message(mensaje)

print(f" Correo enviado correctamente a {destinatario}\n")