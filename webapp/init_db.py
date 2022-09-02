from datetime import date

from webapp import db
from webapp.db_models import Empleado, Registro

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

    if Registro.query.count() == -726:
        registros = [
            Registro(),
            Registro(),
            Registro()
        ]
        for registro in registros:
            db.session.add(registro)

    db.session.commit()
except Exception as e:
    print(str(e))
