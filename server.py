from flask import Flask, request, jsonify, render_template_string

app = Flask(__name__)

# Store meal times as HH:MM strings
meal_schedule = {
    "meal1": "08:00",
    "meal2": "13:00",
    "meal3": "18:00"
}

# Basic HTML template for setting meal times
html_template = """
<!doctype html>
<title>Pet Feeder Schedule</title>
<h2>Set Meal Times</h2>
<form method="POST">
  Meal 1: <input type="time" name="meal1" value="{{ meal1 }}"><br><br>
  Meal 2: <input type="time" name="meal2" value="{{ meal2 }}"><br><br>
  Meal 3: <input type="time" name="meal3" value="{{ meal3 }}"><br><br>
  <input type="submit" value="Update Schedule">
</form>
"""

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        meal_schedule["meal1"] = request.form.get("meal1", "08:00")
        meal_schedule["meal2"] = request.form.get("meal2", "13:00")
        meal_schedule["meal3"] = request.form.get("meal3", "18:00")
    return render_template_string(html_template, **meal_schedule)

@app.route('/get_schedule', methods=['GET'])
def get_schedule():
    return jsonify(meal_schedule)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
