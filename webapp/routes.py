from webapp import app
from webapp import db
from flask import render_template, request, jsonify, redirect, url_for, flash
from flask_login import current_user, login_user, logout_user, login_required
from werkzeug.urls import url_parse
from webapp.forms import LoginForm, RegistrationForm
from webapp.db_models import User, Acceso


@app.route('/')
@app.route('/index')
@login_required
def index():
    accesos = Acceso.query.all()
    return render_template("index.html", title="Home page", accesos=accesos)


@app.route('/login', methods=['GET', 'POST'])
def login():
    if current_user.is_authenticated:  # If the current user from flask_login is authenticated
        return redirect(url_for('index'))
    # What I did here is import the LoginForm class from forms.py, instantiated an object from it, and sent it down to the template
    form = LoginForm()
    if form.validate_on_submit():  # If the form is validated
        # Get the user with the name that was introduced in the form
        user = User.query.filter_by(username=form.username.data).first()
        # Check if there was an User introduced or if the password was not correct
        if user is None or not user.check_password(form.password.data):
            # If the password was NOT correct, flash a message and redirect again to login
            flash('Invalid username or password')
            return redirect(url_for('login'))
        # If the password WAS correct, login the user and redirect to index
        # login_user also gives a value to the variable current_user
        login_user(user, remember=form.remember_me.data)
        # Next page is used for redirecting to a page that has the @login_required
        # This line gets the next query from an url like:       /login?next=/index
        next_page = request.args.get('next')
        # If there is not a next page OR if the page that is being redirected is not a page within my website (for example, an external malicious website).
        # For that purpose, netloc from werkzeug.urls is used
        if not next_page or url_parse(next_page).netloc != '':
            next_page = url_for('index')
        return redirect(url_for('index'))
    return render_template('login.html', title='Sign In', form=form)


@app.route('/logout')
def logout():
    logout_user()
    return redirect(url_for('index'))

@app.route('/register', methods=['GET', 'POST'])
def register():
    if current_user.is_authenticated:
        return redirect(url_for('index'))
    form = RegistrationForm()
    if form.validate_on_submit():
        user = User(username = form.username.data, email=form.email.data)
        user.set_password(form.password.data)
        db.session.add(user)
        db.session.commit()
        flash('Congratulations, you are now a registered user!')
        return redirect(url_for('login'))
    return render_template('register.html', title='Register', form=form)
