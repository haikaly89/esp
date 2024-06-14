from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/data', methods=['POST'])
def receive_data():
    data = request.form.get('value')
    if data:
        try:
            sensor_value = int(data)
            # Proses data sensor di sini jika diperlukan
            print(f"Received sensor value: {sensor_value}")
            return jsonify({"status": "success", "sensor_value": sensor_value}), 200
        except ValueError:
            return jsonify({"status": "error", "message": "Invalid sensor value"}), 400
    else:
        return jsonify({"status": "error", "message": "No data received"}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)