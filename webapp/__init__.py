import os
from flask import Flask, jsonify, render_template, request
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
import logging

app = Flask(__name__)
app.config['SECRET_KEY'] = '5b3cd5b80eb8b217c20fb37074ff4a33'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

# Set the database URI
if 'RDS_DB_NAME' in os.environ:
    print("using the ElasticBeanstalk db")
    app.config['SQLALCHEMY_DATABASE_URI'] = \
        'postgresql://{username}:{password}@{host}:{port}/{database}'.format(
        username=os.environ['RDS_USERNAME'],
        password=os.environ['RDS_PASSWORD'],
        host=os.environ['RDS_HOSTNAME'],
        port=os.environ['RDS_PORT'],
        database=os.environ['RDS_DB_NAME'],
    )
else:
    print("using the local postgresql db")
    app.config['SQLALCHEMY_DATABASE_URI'] = \
        'postgresql://{username}:{password}@{host}:{port}/{database}'.format(
        username='librepass',
        password='complexpassword123',
        host='localhost',
        port='5432',
        database='librepass',
    )

db = SQLAlchemy(app)
login = LoginManager(app)
'''
Flask-Login provides a very useful feature that forces users to log in before they can view certain pages of the application.
If a user who is not logged in tries to view a protected page, Flask-Login will automatically redirect the user to the login form,
and only redirect back to the page the user wanted to view after the login process is complete

The 'login' value below is the function (or endpoint) name for the login view. In other words, the name you would use in a url_for() call to get the URL.
'''
login.login_view = 'login'
logging.basicConfig(
    format='%(asctime)s\t%(levelname)s\t| %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S',
    )
from webapp import routes