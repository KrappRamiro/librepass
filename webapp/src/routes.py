from src import app
from src import db
from flask import render_template, request, jsonify, redirect, url_for
#from src.db_models import Movie, Group
#from src.forms import MovieForm, MovieDetails


@app.route('/')
def index():
    return render_template("index.html")
