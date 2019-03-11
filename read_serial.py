import pyrebase
from tasks import *

import json
import serial

# config = {
#     "apiKey": "AIzaSyCNntsNQ-IPlXk4WtOjzORdF2RcIXUNtoE",
#     "authDomain": "dashkart-b8214.firebaseapp.com",
#     "databaseURL": "https://dashkart-b8214.firebaseio.com",
#     "projectId": "dashkart-b8214",
#     "storageBucket": "dashkart-b8214.appspot.com",
#     "messagingSenderId": "693418870375"
# }
# firebase = pyrebase.initialize_app(config)
# db = firebase.database()

# data = {"name": "Mortimer 'Morty' Smith"}
# db.child("users").push(data)


node_data = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
while 1:
    data = node_data.readline().decode('ascii')
    if data != "":
        reverse.delay(data)
        print(data)

# your variables are already assigned before this
# data = {'url': url, 'address': address, 'name': name}
# sent = json.dumps(data)
# result = firebase.post("/businesses", sent)
