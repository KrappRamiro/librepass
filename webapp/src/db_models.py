# This file is used for database models
from src import db

# Define the db model for the movies


class Movie(db.Model):
    id = db.Column(db.Integer(), primary_key=True)
    title = db.Column(db.String(128), nullable=False)
    release_date = db.Column(db.Date(), nullable=False)

    # Return a list of all the movies as a dictionary
    def as_dict(self):
        return {c.name: getattr(self, c.name) for c in self.__table__.columns}

    def __repr__(self):
        return '<Movie %r>' % self.title


class Empleado(db.Model):
    id = db.Column(db.Integer(), primary_key=True)
    nombre = db.Column(db.String(128), nullable=False)
    dni = db.Column(db.String(20), nullable=False)
    nivel_acceso = db.Column(db.Integer(), nullable=False)



class Logs(db.Model):
    id = db.Column(db.Integer(), primary_key=True)
    horario = db.Column(db.Datetime())