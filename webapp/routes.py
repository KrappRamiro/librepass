from re import A
from webapp import app
from webapp import db
from flask import render_template, request, jsonify, redirect, url_for, flash
from flask_login import current_user, login_user, logout_user, login_required
from werkzeug.urls import url_parse
from webapp.forms import AddEmployeeForm, AddDoorForm, ConfirmForm, EditEmployeeForm, EditDoorForm, LoginForm, RegistrationForm
from webapp.db_models import Door, User, Access, Employee


@app.route('/')
@app.route('/index')
def index():
    return render_template("index.html", title="Home page")


@app.route('/login', methods=['GET', 'POST'])
def login():
    if current_user.is_authenticated:
        return redirect(url_for('index'))
    form = LoginForm()
    if form.validate_on_submit():
        user = User.query.filter_by(username=form.username.data).first()
        if user is None:
            flash('Usuario incorrecto', category='danger')
            return redirect(url_for('login'))
        if user.check_password(form.password.data) == False:
            flash('Contraseña invalida', category='danger')
            return redirect(url_for('login'))
        login_user(user, remember=form.remember_me.data)
        # Next page is used for redirecting to a page that has the @login_required
        # This line gets the next query from an url like:       /login?next=/index
        next_page = request.args.get('next')
        # If there is not a next page OR if the page that is being redirected is not a page within my website (for example, an external malicious website).
        # For that purpose, netloc from werkzeug.urls is used
        if not next_page or url_parse(next_page).netloc != '':
            next_page = url_for('index')
        return redirect(url_for('index'))
    return render_template('login.html', title='Iniciar Sesion', form=form)


@app.route('/logout')
@login_required
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
        flash('Congratulations, you are now a registered user!', category="success")
        return redirect(url_for('login'))
    return render_template('register.html', title='Register', form=form)


@app.route('/add_employee', methods=['GET', 'POST'])
@login_required
def add_employee():
    form = AddEmployeeForm()
    if form.validate_on_submit():
        employee = Employee(
            name=form.name.data,
            dni=form.dni.data,
            access_level=form.access_level.data,
            rfid=form.rfid.data
        )
        db.session.add(employee)
        db.session.commit()
        flash(f'Agregado al employee {form.name.data}', category="success")
        return redirect(url_for('add_employee'))
    return render_template('add_employee.html', title='Añadir Employee', form=form)


@app.route('/add_door', methods=['GET', 'POST'])
@login_required
def add_door():
    form = AddDoorForm()
    if form.validate_on_submit():
        door = Door(
            security_level=form.security_level.data,
            note=form.note.data
        )
        db.session.add(door)
        db.session.commit()
        flash(
            f'Agregada la door con la note {form.note.data}, y el nivel de seguridad {form.security_level.data}', category="success")
        return redirect(url_for('add_door'))
    return render_template('add_door.html', title='Añadir Door', form=form)


@app.route('/see_employees')
@login_required
def see_employees():
    employees = Employee.query.all()
    return render_template('see_employees.html', title="Lista de Empleados", employees=employees)


@app.route('/see_doors')
@login_required
def see_doors():
    doors = Door.query.all()
    return render_template('see_doors.html', title="Lista de Puertas", doors=doors)


@app.route('/see_accesses')
@login_required
def see_accesses():
    accesses = Access.query.all()
    return render_template('see_accesses.html', title="Lista de Accessos", accesses=accesses)

#----------------- Delete urls ------------------#


@app.route('/delete_employee/<int:id>', methods=['GET', 'POST'])
def delete_employee(id):
    employee = Employee.query.filter_by(id=id).first()
    form = ConfirmForm()
    if form.validate_on_submit():
        # Borrar al employee
        db.session.delete(employee)
        db.session.commit()
        flash(f'Borrado al empleado', category="success")
        return redirect(url_for('see_employees'))
    return render_template('delete_employee.html', title="Borrar empleado", form=form, employee=employee)


@app.route('/delete_door/<int:id>', methods=['GET', 'POST'])
def delete_door(id):
    door = Door.query.filter_by(id=id).first()
    form = ConfirmForm()
    if form.validate_on_submit():
        # Borrar la door
        db.session.delete(door)
        db.session.commit()
        flash(f'Borrada la puerta', category="success")
        return redirect(url_for('see_doors'))
    return render_template('delete_door.html', title="Borrar puerta", form=form, door=door)


#----------------- Edit urls ------------------#

@app.route('/edit_employee/<int:id>', methods=['GET', 'POST'])
def edit_employee(id):
    form = EditEmployeeForm()
    employee_to_edit = Employee.query.filter_by(id=id).first()
    if form.validate_on_submit():
        employee_to_edit.name = form.name.data
        employee_to_edit.dni = form.dni.data
        employee_to_edit.access_level = form.access_level.data
        employee_to_edit.rfid = form.rfid.data
        db.session.add(employee_to_edit)
        db.session.commit()
        flash('Los cambios han sido guardados.', category="success")
        return redirect(url_for('see_employees'))
    elif request.method == 'GET':
        form.name.data = employee_to_edit.name
        form.dni.data = employee_to_edit.dni
        form.access_level.data = employee_to_edit.access_level
        form.rfid.data = employee_to_edit.rfid
    return render_template('edit_employee.html', title='Editar Empleado',
                           form=form)


@app.route('/edit_door/<int:id>', methods=['GET', 'POST'])
def edit_door(id):
    form = EditDoorForm()
    door_to_edit = Door.query.filter_by(id=id).first()
    if form.validate_on_submit():
        door_to_edit.security_level = form.security_level.data
        door_to_edit.note = form.note.data
        db.session.add(door_to_edit)
        db.session.commit()
        flash('Los cambios han sido guardados.', category="success")
        return redirect(url_for('see_doors'))
    elif request.method == 'GET':
        form.note.data = door_to_edit.note
        form.security_level.data = door_to_edit.security_level
    return render_template('edit_door.html', title='Editar Puerta',
                           form=form)


# ------------------- API ROUTES ----------------------- #


@app.route('/api/let_employee_pass/<string:rfid>/<int:door>')
def get_employee(rfid, door):
    employee = Employee.query.filter_by(rfid=rfid).first()
    door = Door.query.filter_by(id=door).first()
    # ------------ Inicio de checkeo de existencia de datos en la base de datos ------------------- #
    if employee == None:
        return {
            'mensaje': f'El RFID {rfid} no fue encontrado en la base de datos',
            'access': False
        }
    elif door == None:
        return {
            'mensaje': f'La door no fue encontrada',
            'access': False
        }

    # ------------ Fin de checkeo de existencia de datos en la base de datos ------------------- #
    # Si el employee NO tiene el nivel de seguridad necesario
    if (employee.access_level >= door.security_level) == False:
        respuesta = {
            'mensaje': f"El employee {employee.name} con el nivel de access {employee.access_level} NO ha sido authorized a pasar por la door {door.id}",
            'access': False
        }
        authorized = False
    # Si el employee SI tiene el nivel de seguridad necesario
    else:
        respuesta = {
            'mensaje': f"El employee {employee.name} con el nivel de access {employee.access_level} ha sido authorized a pasar por la door {door.id} con el nivel de seguridad {door.security_level}",
            'access': True
        }
        authorized = True
    # Agregar access (permitido o denegado, cualsea) en la base de datos
    access = Access(employee=employee, door=door, authorized=authorized)
    db.session.add(access)
    db.session.commit()
    return respuesta


@app.route('/api/status')
def status():
    return {
        'status': True
    }
