# This file is used for database models
from webapp import db, login
from datetime import datetime
from werkzeug.security import generate_password_hash, check_password_hash
from flask_login import UserMixin
# UserMixin includes generic implementations that are appropriate for most user model classes


''' Aclaracion sobre el uso de Uppercase y lowercase en la declaracion de relaciones:
Segun https://blog.miguelgrinberg.com/post/the-flask-mega-tutorial-part-iv-database, hay una inconsistencia entre la forma
de usar lowercase y Uppercase:
En el caso de db.relationship() call, the model is referenced by the model class, which typically starts with an uppercase character,
while in other cases such as this db.ForeignKey() declaration, a model is given by its database table name, for which SQLAlchemy automatically uses lowercase characters and, for multi-word model names, snake case
'''


class User(UserMixin, db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(64), index=True, unique=True)
    email = db.Column(db.String(120), index=True, unique=True)
    password_hash = db.Column(db.String(128))

    def set_password(self, password):
        self.password_hash = generate_password_hash(password)

    def check_password(self, password):
        return check_password_hash(self.password_hash, password)

    def __repr__(self):
        return '<User {}>'.format(self.username)


@login.user_loader
def load_user(id):
    return User.query.get(int(id))


class Empleado(db.Model):
    id = db.Column(db.Integer(), primary_key=True)
    nombre = db.Column(db.String(128), nullable=False)
    dni = db.Column(db.String(20), nullable=False)
    nivel_acceso = db.Column(db.Integer(), nullable=False)
    # backref es una forma simple de darle una nueva propiedad a la clase Acceso. O sea, yo podria acceder al Empleado haciendo Acceso.loQueHayaPuestoEnBackref https://stackoverflow.com/a/44538989/15965186
    '''
    For a one-to-many relationship, a db.relationship field is normally defined on the "one" side, and is used as a convenient way to get access to the "many".
    So for example, if I have a empleado stored in mi_empleado, the expression mi_empleado.accesos will run a database query that returns all the Acceso created by that empleado
    The first argument to db.relationship is the model class that represents the "many" side of the relationship
    '''
    accesos = db.relationship('Acceso', backref="empleado", lazy="dynamic")

    def __repr__(self):
        return '<Empleado: {}>'.format(self.nombre)


class Acceso(db.Model):
    id = db.Column(db.Integer(), primary_key=True)
    timestamp = db.Column(db.DateTime(), nullable=False,
                          index=True, default=datetime.utcnow)
    empleado_id = db.Column(db.Integer, db.ForeignKey("empleado.id"))

    def __repr__(self):
        return '<Acceso: {}>'.format(self.timestamp)
