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


class Employee(db.Model):
    id = db.Column(db.Integer(), primary_key=True)
    name = db.Column(db.String(128), nullable=False)
    dni = db.Column(db.String(20), nullable=False)
    access_level = db.Column(db.Integer(), nullable=False)
    rfid = db.Column(db.String(40), unique=True)
    # backref es una forma simple de darle una nueva propiedad a la clase Access. O sea, yo podria acceder al Employee haciendo Access.loQueHayaPuestoEnBackref https://stackoverflow.com/a/44538989/15965186
    '''
    For a one-to-many relationship, a db.relationship field is normally defined on the "one" side, and is used as a convenient way to get access to the "many".
    So for example, if I have a employee stored in mi_employee, the expression mi_employee.accesos will run a database query that returns all the Access created by that employee
    The first argument to db.relationship is the model class that represents the "many" side of the relationship
    '''
    accesos = db.relationship('Access', backref="employee", lazy="dynamic")

    def __repr__(self):
        return '<Employee: {}>'.format(self.name)

class Door(db.Model):
    id = db.Column(db.Integer(), primary_key=True)
    security_level = db.Column(db.Integer(), nullable=False)
    note = db.Column(db.String(), nullable=True)
    accesos = db.relationship('Access', backref="door", lazy="dynamic")

class Access(db.Model):
    id = db.Column(db.Integer(), primary_key=True)
    timestamp = db.Column(db.DateTime(), nullable=False,
                          index=True, default=datetime.utcnow)
    authorized = db.Column(db.Boolean(), nullable=False)
    employee_id = db.Column(db.Integer, db.ForeignKey("employee.id"))
    door_id = db.Column(db.Integer, db.ForeignKey("door.id"))

    def __repr__(self):
        return f'<Access: id: {self.id},\t timestamp: {self.timestamp},\t employee: {self.employee_id},\t door: {self.door}>'
