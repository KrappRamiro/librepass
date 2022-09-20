from datetime import date

from webapp import db
from webapp.db_models import Employee, Access, Door

try:
    db.create_all()

    if Employee.query.count() == 0:
        empleados = [
            Employee(name='Juan', dni="12678543", access_level=1, rfid="BD 31 15 2B"),
            Employee(name='Pepe', dni="98765432", access_level=3, rfid="AC 35 71 2A"),
            Employee(name='Agustin', dni="13666666", access_level=5, rfid="CE 8A 6A 88"),
            Employee(name='Josesito', dni="87666555", access_level=1, rfid="AA C8 61 3C"),
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
