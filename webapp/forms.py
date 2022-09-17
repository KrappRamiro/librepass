from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField
from wtforms.validators import DataRequired, ValidationError, Email, EqualTo
from webapp.db_models import User

# Despues en mi routes.py voy a hacer form=LoginForm(), y ese form se lo voy a pasar al render_template() como argumento


class LoginForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired()])
    password = PasswordField('Password', validators=[DataRequired()])
    remember_me = BooleanField('Remember Me')
    submit = SubmitField('Sign In')


class RegistrationForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired()])
    email = StringField('Email', validators=[DataRequired(), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    password2 = PasswordField('Repeat Password', validators=[
                               DataRequired(), EqualTo('password')])
    submit=SubmitField('Register')

    def validate_username(self, username):
        # Query the database to see if an username with the name introduced already exists
        user = User.query.filter_by(username=username.data).first()
        if user is not None:
            raise ValidationError('Please use a different username')
    def validate_email(self, email):
        # Query the database to see if the email is already in use
        email = User.query.filter_by(email=email.data).first()
        if email is not None:
            raise ValidationError('Please use a different email')
