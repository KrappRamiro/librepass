from webapp import app
from webapp import db
from flask import render_template, request, jsonify, redirect, url_for
#from webapp.db_models import Movie, Group
#from webapp.forms import MovieForm, MovieDetails


@app.route('/')
def index():
    return render_template("index.html")
