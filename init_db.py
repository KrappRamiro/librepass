from datetime import date

from webapp import db
from webapp.db_models import Empleado, Acceso, Puerta

try:
    db.create_all()

    if Empleado.query.count() == 0:
        empleados = [
            Empleado(nombre='Juan', dni="12678543", nivel_acceso=1, rfid="BD 31 15 2B"),
            Empleado(nombre='Pepe', dni="98765432", nivel_acceso=3, rfid="AC 35 71 2A"),
            Empleado(nombre='Agustin', dni="13666666", nivel_acceso=5, rfid="CE 8A 6A 88"),
            Empleado(nombre='Josesito', dni="87666555", nivel_acceso=1, rfid="AA C8 61 3C"),
        ]

        for empleado in empleados:
            db.session.add(empleado)

    if Puerta.query.count() == 0:
        puertas = [
            Puerta(nivel_seguridad=1, nota="Puerta de la entrada"),
            Puerta(nivel_seguridad=3, nota="Puerta del laboratorio"),
            Puerta(nivel_seguridad=5, nota="Puerta de la sala del CEO")
        ]
        for puerta in puertas:
            db.session.add(puerta)

    db.session.commit()
except Exception as e:
    print(str(e))
