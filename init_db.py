from datetime import date

from webapp import db
from webapp.db_models import Employee, Access, Door

try:
    db.create_all()

    if Employee.query.count() == 0:
        empleados = [
            Employee(name='Juan', dni="12678543", access_level=1, rfid="AA AA AA AA"),
            Employee(name='Pepe', dni="98765432", access_level=3, rfid="BB BB BB BB"),
            Employee(name='Saravia', dni="13666666", access_level=5, rfid="62 19 02 1B"),
            Employee(name='Josesito', dni="87666555", access_level=1, rfid="63 2A 91 31"),
        ]

        for empleado in empleados:
            db.session.add(empleado)

    if Door.query.count() == 0:
        doors = [
            Door(security_level=1, note="Puerta de la entrada"),
            Door(security_level=3, note="Puerta del laboratorio"),
            Door(security_level=5, note="Puerta de la sala del CEO")
        ]
        for door in doors:
            db.session.add(door)

    db.session.commit()
except Exception as e:
    print(str(e))
