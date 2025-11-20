from flask import Flask, request, jsonify
from transformers import pipeline
import logging

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

app = Flask(__name__)

print("--- INICIANDO SERVIDOR DE IA (VERSION MEJORADA) ---")
print("Cargando modelo Multilingüe (esto tardará un poco más, es más pesado)...")

# CAMBIO CLAVE: Usamos un modelo que sabe Español
# "joeddav/xlm-roberta-large-xnli" es el estándar de oro para Zero-Shot en español
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

        # CAMBIO CLAVE 2: Le damos una pista en español al modelo
        # "hypothesis_template" le dice al modelo cómo formular la pregunta internamente.
        resultado = clasificador(
            sintomas,
            candidate_labels=lista_especialidades,
            hypothesis_template="Este texto trata sobre {}."
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
    app.run(port=5000)