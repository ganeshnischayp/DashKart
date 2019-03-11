from celery import Celery
import pyrebase
import json

app = Celery('tasks',broker='amqp://localhost')

@app.task
def reverse(string):
    config = {
    "apiKey": "AIzaSyCNntsNQ-IPlXk4WtOjzORdF2RcIXUNtoE",
    "authDomain": "dashkart-b8214.firebaseapp.com",
    "databaseURL": "https://dashkart-b8214.firebaseio.com",
    "projectId": "dashkart-b8214",
    "storageBucket": "dashkart-b8214.appspot.com",
    "messagingSenderId": "693418870375"
    }
    firebase = pyrebase.initialize_app(config)
    db = firebase.database()
    
    # data = {"name": "Mortimer 'Morty' Smith"}
    db.child("users").push(string)

    return 'pushed data'