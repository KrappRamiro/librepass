from tokenize import String
from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField, IntegerField
from wtforms.validators import DataRequired, ValidationError, Email, EqualTo
from webapp.db_models import User

# Despues en mi routes.py voy a hacer form=LoginForm(), y ese form se lo voy a pasar al render_template() como argumento


class LoginForm(FlaskForm):
    username = StringField('Usuario', validators=[DataRequired()])
    password = PasswordField('Contraseña', validators=[DataRequired()])
    remember_me = BooleanField('Recuerdame')
    submit = SubmitField('Ingresar')


class RegistrationForm(FlaskForm):
    username = StringField('Nombre de usuario', validators=[DataRequired()])
    email = StringField('Email', validators=[DataRequired(), Email()])
    password = PasswordField('Contraseña', validators=[DataRequired()])
    password2 = PasswordField('Repita la contraseña', validators=[
        DataRequired(), EqualTo('password')])
    submit = SubmitField('Registrarse')

    def validate_username(self, username):
        # Query the database to see if an username with the name introduced already exists
        user = User.query.filter_by(username=username.data).first()
        if user is not None:
            raise ValidationError('Por favor usa un nombre de usuario distinto')

    def validate_email(self, email):
        # Query the database to see if the email is already in use
        email = User.query.filter_by(email=email.data).first()
        if email is not None:
            raise ValidationError('Por favor usa un email distinto')


class AddEmployeeForm(FlaskForm):
    name = StringField('Nombre', validators=[DataRequired()])
    dni = StringField('DNI', validators=[DataRequired()])
    access_level = IntegerField('Nivel de acceso', validators=[DataRequired()])
    rfid = StringField('UID del tag RFID', validators=[DataRequired()])
    submit = SubmitField('Añadir Empleado')


class AddDoorForm(FlaskForm):
    security_level = IntegerField(
        'Nivel de seguridad', validators=[DataRequired()])
    note = StringField()
    submit = SubmitField('Añadir Puerta')


class ConfirmForm(FlaskForm):
    submit = SubmitField('Confirmar')


class EditEmployeeForm(FlaskForm):
    name = StringField('Nombre', validators=[DataRequired()])
    dni = StringField('DNI', validators=[DataRequired()])
    access_level = IntegerField('Nivel de acceso', validators=[DataRequired()])
    rfid = StringField('UID del tag RFID', validators=[DataRequired()])
    submit = SubmitField('Editar Empleado')

class EditDoorForm(FlaskForm):
    security_level = IntegerField(
        'Nivel de seguridad', validators=[DataRequired()])
    note = StringField()
    submit = SubmitField('Editar Puerta')