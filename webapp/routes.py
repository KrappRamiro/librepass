from re import A
from webapp import app
from webapp import db
from flask import render_template, request, jsonify, redirect, url_for, flash
from flask_login import current_user, login_user, logout_user, login_required
from werkzeug.urls import url_parse
from webapp.forms import AddEmpleadoForm, AddPuertaForm, ConfirmForm, EditEmpleadoForm, EditPuertaForm, LoginForm, RegistrationForm
from webapp.db_models import Puerta, User, Acceso, Empleado


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
        user = User(username=form.username.data, email=form.email.data)
        user.set_password(form.password.data)
        db.session.add(user)
        db.session.commit()
        flash('Congratulations, you are now a registered user!')
        return redirect(url_for('login'))
    return render_template('register.html', title='Register', form=form)


@app.route('/add_empleado', methods=['GET', 'POST'])
def add_empleado():
    form = AddEmpleadoForm()
    if form.validate_on_submit():
        empleado = Empleado(
            nombre=form.nombre.data,
            dni=form.dni.data,
            nivel_acceso=form.nivel_acceso.data,
            rfid=form.rfid.data
        )
        db.session.add(empleado)
        db.session.commit()
        flash(f'Agregado al empleado {form.nombre.data}')
        return redirect(url_for('add_empleado'))
    return render_template('add_empleado.html', title='Añadir Empleado', form=form)


@app.route('/add_puerta/', methods=['GET', 'POST'])
def add_puerta():
    form = AddPuertaForm()
    if form.validate_on_submit():
        puerta = Puerta(
            nivel_seguridad=form.nivel_seguridad.data,
            nota=form.nota.data
        )
        db.session.add(puerta)
        db.session.commit()
        flash(
            f'Agregada la puerta con la nota {form.nota.data}, y el nivel de seguridad {form.nivel_seguridad.data}')
        return redirect(url_for('add_puerta'))
    return render_template('add_puerta.html', title='Añadir Puerta', form=form)


@app.route('/see_empleados')
def see_empleados():
    empleados = Empleado.query.all()
    return render_template('see_empleados.html', title="Lista de Empleados", empleados=empleados)


@app.route('/see_puertas')
def see_puertas():
    puertas = Puerta.query.all()
    return render_template('see_puertas.html', title="Lista de Empleados", puertas=puertas)

#----------------- Delete urls ------------------#

@app.route('/delete_empleado/<int:id>', methods=['GET', 'POST'])
def delete_empleado(id):
    empleado = Empleado.query.filter_by(id=id).first()
    form = ConfirmForm()
    if form.validate_on_submit():
        #Borrar al empleado
        db.session.delete(empleado)
        db.session.commit()
        flash(
            f'Borrado al empleado'
        )
        return redirect(url_for('see_empleados'))
    return render_template('delete_empleado.html', title="Borrar empleado", form=form, empleado=empleado)

@app.route('/delete_puerta/<int:id>', methods=['GET', 'POST'])
def delete_puerta(id):
    puerta = Puerta.query.filter_by(id=id).first()
    form = ConfirmForm()
    if form.validate_on_submit():
        #Borrar la puerta
        db.session.delete(puerta)
        db.session.commit()
        flash(
            f'Borrada la puerta'
        )
        return redirect(url_for('see_puertas'))
    return render_template('delete_puerta.html', title="Borrar puerta", form=form, puerta=puerta)



#----------------- Edit urls ------------------#

@app.route('/editar_empleado/<int:id>', methods=['GET', 'POST'])
def editar_empleado(id):
    form = EditEmpleadoForm()
    empleado_a_editar = Empleado.query.filter_by(id=id).first()
    if form.validate_on_submit():
        empleado_a_editar.nombre = form.nombre.data
        empleado_a_editar.dni = form.dni.data
        empleado_a_editar.nivel_acceso = form.nivel_acceso.data
        empleado_a_editar.rfid = form.rfid.data
        db.session.add(empleado_a_editar)
        db.session.commit()
        flash('Your changes have been saved.')
        return redirect(url_for('see_empleados'))
    elif request.method == 'GET':
        form.nombre.data = empleado_a_editar.nombre
        form.dni.data = empleado_a_editar.dni
        form.nivel_acceso.data = empleado_a_editar.nivel_acceso
        form.rfid.data = empleado_a_editar.rfid
    return render_template('editar_empleado.html', title='Editar Empleado',
                           form=form)


@app.route('/editar_puerta/<int:id>', methods=['GET', 'POST'])
def editar_puerta(id):
    form = EditPuertaForm()
    puerta_a_editar = Puerta.query.filter_by(id=id).first()
    if form.validate_on_submit():
        puerta_a_editar.nivel_seguridad = form.nivel_seguridad.data
        puerta_a_editar.nota = form.nota.data
        db.session.add(puerta_a_editar)
        db.session.commit()
        flash('Your changes have been saved.')
        return redirect(url_for('see_puertas'))
    elif request.method == 'GET':
        form.nota.data = puerta_a_editar.nota
        form.nivel_seguridad.data = puerta_a_editar.nivel_seguridad
    return render_template('editar_puerta.html', title='Editar Puerta',
                           form=form)


# ------------------- API ROUTES ----------------------- #


@app.route('/api/let_employee_pass/<string:rfid>/<int:puerta>')
def get_empleado(rfid, puerta):
    empleado = Empleado.query.filter_by(rfid=rfid).first()
    puerta = Puerta.query.filter_by(id=puerta).first()
    # ------------ Inicio de checkeo de existencia de datos en la base de datos ------------------- #
    if empleado == None:
        return {
            'mensaje': f'El RFID {rfid} no fue encontrado en la base de datos',
            'acceso': False
        }
    elif puerta == None:
        return {
            'mensaje': f'La puerta no fue encontrada',
            'acceso': False
        }

    # ------------ Fin de checkeo de existencia de datos en la base de datos ------------------- #
    # Si el empleado NO tiene el nivel de seguridad necesario
    if (empleado.nivel_acceso >= puerta.nivel_seguridad) == False:
        respuesta = {
            'mensaje': f"El empleado {empleado.nombre} con el nivel de acceso {empleado.nivel_acceso} NO ha sido autorizado a pasar por la puerta {puerta.id}",
            'acceso': False
        }
        autorizado = False
    # Si el empleado SI tiene el nivel de seguridad necesario
    else:
        respuesta = {
            'mensaje': f"El empleado {empleado.nombre} con el nivel de acceso {empleado.nivel_acceso} ha sido autorizado a pasar por la puerta {puerta.id} con el nivel de seguridad {puerta.nivel_seguridad}",
            'acceso': True
        }
        autorizado = True
    # Agregar acceso (permitido o denegado, cualsea) en la base de datos
    acceso = Acceso(empleado=empleado, puerta=puerta, autorizado=autorizado)
    db.session.add(acceso)
    db.session.commit()
    return respuesta


@app.route('/api/status')
def status():
    return {
        'status': True
    }
