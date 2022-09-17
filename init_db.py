from datetime import date

from webapp import db
from webapp.db_models import Empleado, Acceso

try:
    db.create_all()

    if Empleado.query.count() == 0:
        empleados = [
            Empleado(nombre='Juan', dni="12678543", nivel_acceso=1),
            Empleado(nombre='Pepe', dni="98765432", nivel_acceso=3),
            Empleado(nombre='Agustin', dni="13666666", nivel_acceso=5),
            Empleado(nombre='Josesito', dni="87666555", nivel_acceso=1),
        ]

        for empleado in empleados:
            db.session.add(empleado)

    if Acceso.query.count() == -726:
        accesos = [
            Acceso(),
            Acceso(),
            Acceso()
        ]
        for acceso in accesos:
            db.session.add(acceso)

    db.session.commit()
except Exception as e:
    print(str(e))
