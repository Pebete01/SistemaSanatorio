from flask import Flask, request, jsonify
from transformers import pipeline
import logging

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

app = Flask(__name__)

print("--- INICIANDO SERVIDOR DE IA (VERSION MEJORADA) ---")
print("Cargando modelo Multilingüe...")

clasificador = pipeline("zero-shot-classification", model="joeddav/xlm-roberta-large-xnli")

print("Modelo cargado. Esperando peticiones en http://127.0.0.1:5000/recomendar")

@app.route('/recomendar', methods=['POST'])
def recomendar():
    try:
        data = request.json
        sintomas = data.get('sintomas', '')
        lista_especialidades = data.get('lista_especialidades', [])

        print(f"\nConsulta recibida: '{sintomas}'")

        if not sintomas or not lista_especialidades:
            return jsonify({"error": "Faltan datos"}), 400


        resultado = clasificador(
            sintomas,
            candidate_labels=lista_especialidades,
            hypothesis_template="Este ejemplo es {}."
        )

        mejor_opcion = resultado['labels'][0]
        confianza = resultado['scores'][0]

        print(f" -> Predicción: {mejor_opcion} ({confianza:.2f})")

        return jsonify({
            "especialidad": mejor_opcion,
            "confianza": confianza
        })

    except Exception as e:
        print(f"Error: {e}")
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    try:
        app.run(port=5000)
    except Exception as e:
        print("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        print("CRASH DEL SERVIDOR:")
        print(e)
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n")
        input("Presiona ENTER para cerrar esta ventana...")