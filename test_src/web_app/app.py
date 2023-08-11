from flask import Flask, render_template, send_from_directory, request, Response
import os
from flask_httpauth import HTTPBasicAuth

app = Flask(__name__)
auth = HTTPBasicAuth()

users = {
    "mirae": "mirae"  # revise your ID and password
}

@auth.verify_password
def verify_password(username, password):
    if username in users and password == users[username]:
        return username

@app.route('/')
def hello():
    return "MIRAE ENGINEERING CO..LTD, Power sensor data server"

@app.route('/view_csv')
@auth.login_required
def view_csv():
    csv_filename = 'Mirae Power Data.csv'  # insert file name
    csv_directory = "/home/irms/web_app/Power_data"
    csv_path = os.path.join(csv_directory, csv_filename)

    if os.path.exists(csv_path):
        with open(csv_path, 'r') as file:
            csv_content = file.read()
        return render_template('view_csv.html', content=csv_content)
    else:
        return "CSV file not found."

@app.route('/download_csv')
@auth.login_required
def download_csv():
    csv_filename = 'Mirae Power Data.csv'
    csv_directory = "/home/irms/web_app/Power_data"
    csv_path = os.path.join(csv_directory, csv_filename)
    return send_from_directory(directory=csv_directory, filename=csv_filename, as_attachment=True)

# Set Cache-Control header to prevent caching
@app.after_request
def add_cache_control(response):
    response.headers['Cache-Control'] = 'no-store, no-cache, must-revalidate, proxy-revalidate'
    return response

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)