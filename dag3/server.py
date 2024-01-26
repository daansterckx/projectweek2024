from flask import Flask, request

app = Flask(__name__)

@app.route('/', methods=['POST'])
def handle_post():
    data = request.get_json()
    if data is not None:
        latitude = data.get('latitude')
        longitude = data.get('longitude')
        alarm = data.get('Alarm')
        print(f'Latitude: {latitude}, Longitude: {longitude}, Alarm: {alarm}')
        with open('gps_data.txt', 'w') as f:
            f.write(f'{latitude},{longitude}')
            f.write("\n")
            f.write(f'{alarm}')
    return '', 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
