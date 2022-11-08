<!-- Improved compatibility of volver arriba link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->

<a name="readme-top"></a>

<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/KrappRamiro/librepass">
    <img src="informe/images/logo.png" alt="Logo" width="120" height="120">
  </a>

<h1 align="center">Librepass</h1>
  <h2 align="center"> Un sistema de seguridad electrónico </h2>
    <a href="http://librepass-env.us-east-1.elasticbeanstalk.com/">Ver Demo</a>
    ·
    <a href="https://github.com/KrappRamiro/librepass/issues">Reportar un bug</a>
    ·
    <a href="https://github.com/KrappRamiro/librepass/issues">Pedir una función</a>
		<br>
		<br>
</div>

<!-- TABLE OF CONTENTS -->

- [Acerca del Proyecto](#acerca-del-proyecto)
	- [Hecho con](#hecho-con)
- [Antes de Arrancar](#antes-de-arrancar)
	- [Prerequisitos](#prerequisitos)
	- [Instalación](#instalación)
- [Uso](#uso)
	- [Creacion de cuenta e inicio de sesion](#creacion-de-cuenta-e-inicio-de-sesion)
	- [Añadido de empleados y puertas](#añadido-de-empleados-y-puertas)
	- [Visualización de datos](#visualización-de-datos)
- [Tareas a futuro](#tareas-a-futuro)
- [Contribuir](#contribuir)
- [Licencia](#licencia)
- [Contacto](#contacto)
- [FAQ](#faq)
	- [OperationalError Al correr el programa](#operationalerror-al-correr-el-programa)
	- [No puedo borrar empleados y puertas](#no-puedo-borrar-empleados-y-puertas)
	- [Como cerrar sesión](#como-cerrar-sesión)

<!-- ABOUT THE PROJECT -->

## Acerca del Proyecto

<div align="middle">

[![Product Screenshot][product-screenshot]](/webapp/static/images/hero.png)

</div>

Librepass es un sistema integral de control de acceso. El mismo permite controlar el acceso de los empleados a distintos sectores usando tecnología RFID. En este sistema, se tienen una unidad de acceso por cada puerta, las cuales se conectan mediante una API REST a un servidor en AWS, la cual permite o no pasar a una persona dependiendo del nivel de seguridad que tenga su tarjeta.

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

### Hecho con

- [![Bootstrap][bootstrap.com]][bootstrap-url]
- [![Flask][flask.com]][flask-url]
- [![Arduino][arduino.com]][arduino-url]
- [![Nunjucks][nunjucks.com]][nunjucks-url]
- [![Jinja][jinja.com]][jinja-url]
- [![AWS][aws.com]][aws-url]equipo
- [![Postgres][postgres.com]][postgres-url]

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

<!-- GETTING STARTED -->

## Antes de Arrancar

Para tener una copia local corriendo, sigue los siguientes pasos

### Prerequisitos

Se necesita el siguiente software para correr el programa

- Python 3.10

- Docker

### Instalación

1. Clona el repositorio

```sh
git clone https://github.com/KrappRamiro/librepass.git
```

2. Instala los paquetes python

```sh
pip install -r requirements.txt
```

3. Para el desarollo local, corre una instancia de docker con el siguiente comando

```sh
sudo docker run --name librepass-postgres -p 5432:5432     -e POSTGRES_USER=librepass -e POSTGRES_PASSWORD=complexpassword123     -e POSTGRES_DB=librepass -d postgres
```

4. Ejecuta el archivo `init_db.py`

```sh
./init_db.py
```

5. Corre el programa

```sh
./app.py
```

6. Abre el programa en tu navegador web en <a href="http://127.0.0.1:5000"> http://127.0.0.1:5000 </a>

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

<!-- USAGE EXAMPLES -->

## Uso

### Creacion de cuenta e inicio de sesion

Para iniciar con Librepass, lo primero que hay que hacer es crearse una cuenta, tocando en el boton que dice **Registrate**

![Crearse una cuenta](/informe/images/tutorial-1.png)

A continuación, rellená los datos y tocá en el boton azul que dice Registrarse.
Haciendo eso, **ya creaste tu cuenta**

![Crearse una cuenta 2](/informe/images/tutorial-2.png)

Luego, inicia sesión llenando tu usuario y contraseña.

![Inicio de sesion](/informe/images/tutorial-3.png)

### Añadido de empleados y puertas

Para añadir empleados y/o puertas, hay que tocar en el boton **Añadir** de la parte superior de la página, y luego en Empleado o Puerta

![Abrir menu de crear empleado](/informe/images/tutorial-4.png)

Luego hay que rellenar los datos correspondientes, y tocar en el boton **Añadir Empleado/Puerta**

![Rellenar datos de los empleados](/informe/images/tutorial-5.png)

### Visualización de datos

Para ver los datos, hay que tocar en el boton **Listas** de la parte superior de la página, y luego en Empleados, Puertas o Accesos, segun se quiera

![Menu de ver](/informe/images/tutorial-6.png)

Aquí se pueden ver los datos. En la columna de acciones, se pueden realizar multiples acciones asociadas a la información que se está viendo

![Ver empleados](/informe/images/tutorial-7.png)

<!-- _For more examples, please refer to the [Documentation](https://example.com)_ -->

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

<!-- ROADMAP -->

## Tareas a futuro

- [x] Agregar un footer
- [ ] Agregar una página para ver todos los accesos de una puerta en específico
  - [ ] A esa página, agregarle un contador (lo mismo para la de Empleados)
- [ ] Reformar la interfaz de agregado de empleados y puertas
- [ ] Agregar un modo oscuro
- [ ] Poder ordenar las variables de las tablas de mayor a menor
- [ ] Poder setear la zona horaria

Mira los [open issues](https://github.com/KrappRamiro/librepass/issues) para una lista de todas las funciones propuestas (y todos los errores conocidos)

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

<!-- CONTRIBUTING -->

## Contribuir

Las contribuciones son lo que hace que la comunidad de código abierto sea un lugar increíble para aprender, inspirar y crear. Cualquier contribución que hagas es **muy apreciada**.

Si tenes una sugerencia que mejoraría el programa, hace un fork del repositorio y crea una pull request. También podes abrir un issue con la etiqueta "mejoras".

¡No olvides darle una estrella al proyecto! ¡Gracias de nuevo!

1. Hace un fork del proyecto

2. Crea tu porpia feature branch (`git checkout -b feature/UnaSuperFeature`)

3. Hace un commit de tus cambios (`git commit -m 'Add some AmazingFeature'`)

4. Pushea a la branch (`git checkout -b feature/AmazingFeature`)

5. Abrí una pull request

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

<!-- LICENSE -->

## Licencia

Distribuido con la licencia MIT. Ver `LICENSE.txt` para más información

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

<!-- CONTACT -->

## Contacto

Krapp Ramiro - krappramiro@disroot.org

Project Link: [https://github.com/KrappRamiro/librepass](https://github.com/KrappRamiro/librepass)

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

<!-- ACKNOWLEDGMENTS -->

## FAQ

### OperationalError Al correr el programa

Si al correr app.py tienes el siguiente error:

```
sqlalchemy.exc.OperationalError: (psycopg2.OperationalError) connection to server at "localhost" (::1), port 5432 failed: Connection refused
	Is the server running on that host and accepting TCP/IP connections?
connection to server at "localhost" (127.0.0.1), port 5432 failed: Connection refused
	Is the server running on that host and accepting TCP/IP connections?
```

Significa que tenes que correr la instancia de docker. Si todavia no la creaste, hacelo usando

```sh
sudo docker run --name librepass-postgres -p 5432:5432     -e POSTGRES_USER=librepass -e POSTGRES_PASSWORD=complexpassword123     -e POSTGRES_DB=librepass -d postgres
```

Si ya corriste ese comando con anterioridad, intenta iniciandola con `sudo docker start librepass-postgres`

### No puedo borrar empleados y puertas

Esa funcion esta temporalmente desactivada

### Como cerrar sesión

Para cerrar sesion, hay que tocar el boton que dice **Cerrar Sesion** arriba a la derecha

<p align="right">(<a href="#readme-top">volver arriba</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/KrappRamiro/librepass.svg?style=for-the-badge
[contributors-url]: https://github.com/KrappRamiro/librepass/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/KrappRamiro/librepass.svg?style=for-the-badge
[forks-url]: https://github.com/KrappRamiro/librepass/network/members
[stars-shield]: https://img.shields.io/github/stars/KrappRamiro/librepass.svg?style=for-the-badge
[stars-url]: https://github.com/KrappRamiro/librepass/stargazers
[issues-shield]: https://img.shields.io/github/issues/KrappRamiro/librepass.svg?style=for-the-badge
[issues-url]: https://github.com/KrappRamiro/librepass/issues
[license-shield]: https://img.shields.io/github/license/KrappRamiro/librepass.svg?style=for-the-badge
[license-url]: https://github.com/KrappRamiro/librepass/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/ramiro-krapp
[product-screenshot]: /webapp/static/images/hero.png
[next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[next-url]: https://nextjs.org/
[react.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[react-url]: https://reactjs.org/
[vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[vue-url]: https://vuejs.org/
[angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[angular-url]: https://angular.io/
[svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[svelte-url]: https://svelte.dev/
[laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[laravel-url]: https://laravel.com
[bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[bootstrap-url]: https://getbootstrap.com
[jquery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[jquery-url]: https://jquery.com
[flask.com]: https://img.shields.io/badge/Flask-2c3844?style=for-the-badge&logo=flask
[flask-url]: https://flask.palletsprojects.com/en/2.2.x/
[arduino.com]: https://img.shields.io/badge/Arduino-18bcbf?style=for-the-badge&logo=arduino
[arduino-url]: https://www.arduino.cc/
[nunjucks.com]: https://img.shields.io/badge/Nunjucks-398339?style=for-the-badge&logo=nunjucks
[nunjucks-url]: https://mozilla.github.io/nunjucks/
[jinja.com]: https://img.shields.io/badge/Jinja2-35495E?style=for-the-badge&logo=jinja
[jinja-url]: https://palletsprojects.com/p/jinja/
[aws.com]: https://img.shields.io/badge/AWS-EB5F07?style=for-the-badge&logo=amazon
[aws-url]: https://aws.amazon.com/
[postgres.com]: https://img.shields.io/badge/PostgreSQL-e8dcdc?style=for-the-badge&logo=postgresql
[postgres-url]: https://www.postgresql.org/