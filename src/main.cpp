#include "empleado.h"
#include "krapp_utils.h"
#define SS_PIN 5
#define RST_PIN 21
#define SIZE_BUFFER 18 // Este es el tamaño del buffer con el que voy a estar trabajando.
// Por que es 18? Porque son 16 bytes de los datos del tag, y 2 bytes de checksum
#define MAX_SIZE_BLOCK 16
#define greenPin 12
#define redPin 32
/* Se usa std::vector en reemplazo de usar `using namespace std` por una muy
buena razon, y es que se evita el namespace pollution. Si no sabes qué es eso,
te recomiendo personalmente este post,  es corto, sencillo, y bien explicado
para principiantes:
https://www.thecrazyprogrammer.com/2021/01/better-alternatives-for-using-namespace-std-in-c.html
*/
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";
int Empleado::cuentaEmpleados = 0;

// std::vector<Empleado> Empleados;
// Un array de empleados para almacenar múltiples empleados
// TODO cambiarlo por un vector
// Empleado misEmpleados[20];
Empleado miEmpleado;

//------------------ INICIO DE Configuracion de conexion a internet ----------------
const char* ssid = "Krapp"; // Nombre de la red
const char* password = "yougotit"; // Contraseña de la red

// const char* ssid = "TeleCentro-882b";
// const char* password = "ZGNJVMMHZ2MY";

AsyncWebServer server(80); // Inicio el web server en el puerto 80
// Create an Event Source on /events
AsyncEventSource events("/events");
// ----------------- FIN DE Configuracion de conexion a internet ------------------

// ----------------- INICIO DE Configuracion de servidor NTP -------------------
// Un servidor NTP es un servidor que se encarga de obtener la fecha y hora actual
const char* ntpServer = "pool.ntp.org"; // NTP server pool
const long gmtOffset_sec = 0; // GMT offset in seconds
const int daylightOffset_sec = 3600; // daylight saving offset in seconds
// -------------- FIN DE Configuracion de servidor NTP -------------------

//  ---------------- INICIO DE Variables del MFRC552 ---------------------
// key es una variable que se va a usar a lo largo de todo el codigo
MFRC522::MIFARE_Key key;
// Status es el codigo de estado de autenticacion
MFRC522::StatusCode status;
// Defino los pines que van al modulo RC552
MFRC522 mfrc522(SS_PIN, RST_PIN);
// ----------------- FIN DE Variables del MFRC552 ---------------------

void createEmployee()
{
	/*
	Esta funcion crea un empleado en la lista de empleados
	Args:
		None
	Returns:
		None
	*/
	//	Empleado* temp = new Empleado(
	//		getSerialStringInput(),
	//		getSerialStringInput(),
	//		4,
	//		getSerialStringInput());
	// misEmpleados[0] = Empleado(
	miEmpleado = Empleado( // Creo un empleado
		getSerialStringInput(), // Nombre
		getSerialStringInput(), // DNI
		4, // Nivel de autorizacion
		getSerialStringInput()); // Cargo administrativo
}
#pragma region // esto es exclusivo de vscode, no afecta al compilador, me permite contraer y expandir
const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
	<head>
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<link
			rel="stylesheet"
			href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
			integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr"
			crossorigin="anonymous"
		/>
		<style>
			html {
				font-family: Arial;
				display: inline-block;
				margin: 0px auto;
				text-align: left;
			}
			h2 {
				font-size: 3rem;
			}
			p {
				font-size: 3rem;
			}
			.units {
				font-size: 1.2rem;
			}
			.dht-labels {
				font-size: 1.5rem;
				vertical-align: middle;
				padding-bottom: 15px;
			}
			.user-input {
				display: block;
				margin: 10px;
			}
		</style>
	</head>
	<body>
		<h2>ESP32 Librepass WebServer</h2>
		<p>
			<i class="fas fa-thermometer-half" style="color: #059e8a"></i>
			<span class="dht-labels">Temperature is: </span>
			<span id="temperature-el">%TEMPERATURE%</span>
			<sup class="units">&deg;C</sup>
		</p>
		<p>
			<i class="fas fa-tint" style="color: #00add6"></i>
			<span class="dht-labels">Humidity is: </span>
			<span id="humidity-el">%HUMIDITY%</span>
			<sup class="units">&percnt;</sup>
		</p>
		<p>
			<i class="fa-solid fa-clock"></i>
			<span class="dht-labels">Date and hour is: </span>
			<span id="date-hour-el">%DATE-HOUR%</span>
		</p>
		<p>
			<span class="dht-labels">Se leyo? :</span>
			<span id="rfid-uid-el">%rfid-uid%</span>
		</p>
		<!-- User input -->
		<!--
		<label for="name-input-el">Nombre:</label>
		<input
			type="text"
			name="name-input-el"
			class="user-input"
			id="name-input-el"
		/>
		<label for="dni-input-el"></label>
		<input
			type="text"
			name="dni-input-el"
			class="user-input"
			id="dni-input-el"
		/>
		<label for="clearance-level-input-el"></label>
		<input
			type="text"
			name="clearance-level-input-el"
			class="user-input"
			id="clearance-level-input-el"
		/>
		<label for="cargo-administrativo-input-el"></label>
		<input
			type="text"
			name="cargo-administrativo-input-el"
			class="user-input"
			id="cargo-administrativo-input-el"
		/>
		<button id="save-input-btn">SAVE INPUT</button>
		-->

		<form action="/get">
			input1: <input type="text" name="input1" />
			<input type="submit" value="Submit" />
		</form>
		<br />
		<form action="/get">
			input2: <input type="text" name="input2" />
			<input type="submit" value="Submit" />
		</form>
		<br />
		<form action="/get">
			input3: <input type="text" name="input3" />
			<input type="submit" value="Submit" />
		</form>
	</body>

	<script>
		const saveInputBtn = document.getElementById("save-input-btn");

		// -------------- INICIO DE Eventos -----------------
		if (!!window.EventSource) {
			//A partir de aca van todos los eventos del ESP32:
			var source = new EventSource("/events");
			source.addEventListener("open", function (e) {
				console.log("Events Connected");
			});
			//Evento que se ejecuta cada vez que se recibe un error
			source.addEventListener("error", function (e) {
				if (e.target.readyState != EventSource.OPEN) {
					console.log("Events Disconnected");
				}
			});
			// Evento que se ejecuta cada vez que se recibe un mensaje cualquiera
			source.addEventListener("message", function (e) {
				console.log("message", e.data);
			});
			// Evento que se ejecuta cada vez que se recibe una lectura del rfid
			source.addEventListener("rfidReadEvent", function (e) {
				console.log("rfidReadEvent", e.data);
				document.getElementById("rfid-uid-el").innerHTML = e.data;
			});
		}
		// Creo que aca van los eventos de la propia pagina web
		saveInputBtn.addEventListener("click", function () {
			const person = {
				name: document.getElementById("name-input-el").value,
				dni: document.getElementById("dni-input-el").value,
				clearanceLevel: document.getElementById("clearance-level-input-el")
					.value,
				cargoAdministrativo: document.getElementById(
					"cargo-administrativo-input-el"
				).value,
			};
			console.log("Read the following things:"); //log the saved lead
			console.dir(person);
		});

		// ------------ FIN DE Eventos ------------

		// ------------------ INICIO DE Intervalos ------------------
		// Intervalo de temperatura
		setInterval(function () {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function () {
				if (this.readyState == 4 && this.status == 200) {
					document.getElementById("temperature-el").innerHTML =
						this.responseText;
				}
			};
			xhttp.open("GET", "/temperature", true); //esto llama al server.on() correspondiente
			console.log("Updating temperature");
			xhttp.send();
		}, 3000);
		// Intervalo de humedad
		setInterval(function () {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function () {
				if (this.readyState == 4 && this.status == 200) {
					document.getElementById("humidity-el").innerHTML = this.responseText;
				}
			};
			xhttp.open("GET", "/humidity", true); //esto llama al server.on() correspondiente
			console.log("updating humidity");
			xhttp.send();
		}, 3000);
		// Intervalo de fecha y hora
		setInterval(function () {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function () {
				if (this.readyState == 4 && this.status == 200) {
					document.getElementById("date-hour-el").innerHTML = this.responseText;
				}
			};
			xhttp.open("GET", "/date-hour", true); //esto llama al server.on() correspondiente
			console.log("updating date-hour");
			xhttp.send();
		}, 3000);
		// ------------------ FIN DE Intervalos ------------------
	</script>
</html>
)rawliteral";
#pragma endregion
void setup()
{
	Serial.begin(9600);
	SPI.begin(); // Inicio el bus SPI
	Log.begin(LOG_LEVEL_NOTICE, &Serial); // Inicio del sistema de logging

	// Prendo el led de la placa cuando inicia el sistema
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);

	// Me conecto a internet mediante Wi-Fi
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}
	// Imprimo la IP local del ESP32 (192.168.x.x)
	Serial.println(WiFi.localIP());

	// --------------- INICIO DE Rutas del servidor -----------------
	// ----- GET's
	// Ruta para el index.html
	server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send_P(200, "text/html", index_html, processor);
	});
	// Ruta para la temperatura
	server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send_P(200, "text/plain", String(random(0, 50)).c_str());
	});
	// Ruta para la humedad
	server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send_P(200, "text/plain", String(random(0, 50)).c_str());
	});
	// Ruta para la fecha y hora
	server.on("/date-hour", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send_P(200, "text/plain", String(getDateTime()).c_str());
	});

	// Send a GET request to <ESP_IP>/get?input=X<inputMessage>
	server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
		String inputMessage;
		String inputParam;
		// GET input1 value on <ESP_IP>/get?input1=<inputMessage>
		if (request->hasParam(PARAM_INPUT_1)) {
			inputMessage = request->getParam(PARAM_INPUT_1)->value();
			inputParam = PARAM_INPUT_1;
		}
		// GET input2 value on <ESP_IP>/get?input2=<inputMessage>
		else if (request->hasParam(PARAM_INPUT_2)) {
			inputMessage = request->getParam(PARAM_INPUT_2)->value();
			inputParam = PARAM_INPUT_2;
		}
		// GET input3 value on <ESP_IP>/get?input3=<inputMessage>
		else if (request->hasParam(PARAM_INPUT_3)) {
			inputMessage = request->getParam(PARAM_INPUT_3)->value();
			inputParam = PARAM_INPUT_3;
		} else {
			inputMessage = "No message sent";
			inputParam = "none";
		}
		Serial.println(inputMessage);
		request->send(204);
	});

	// ----- Eventos
	events.onConnect([](AsyncEventSourceClient* client) {
		if (client->lastId()) {
			Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
		}
		// send event with message "hello!", id current millis
		// and set reconnect delay to 1 second
		client->send("hello!", NULL, millis(), 10000);
	});

	server.addHandler(&events);
	// ---------------- FIN DE Rutas del servidor -------------------

	server.begin();
	// Cosas del ntp server para la fecha/hora
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	// Inicio el MFRC552
	mfrc522.PCD_Init();
	// Le pido al usuario que acerque el tag RFID
	Serial.println(F("Acerca tu tarjeta RFID\n"));
}

void loop()
{
	// ------- INICIO DEL LECTOR RFID --------------
	// Se espera a que se acerque un tag
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}
	// Se espera a que se lean los datos
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}
	// Descomentar solamente si se quiere Dumpear toda la info acerca de la tarjeta leida, ojo que llama automaticamente a PICC_HaltA()
	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

	// Le digo al servidor que se leyo el tag RFID
	events.send(getUID(mfrc522).c_str(), "rfidReadEvent");
	/*
	// LLama a la funcion de menu para que el usuario elija una opcion
	int op = menu();
	if (op == 0)
		readingData();
	else if (op == 1) {
		// crear empleado
		createEmployee();
	} else if (op == 2) {
		// leer info del primer empleado
		Serial.print("\nThe employee name is: ");
		// Serial.print(misEmpleados[0].getName());
		Serial.print(miEmpleado.getName());
	}

	else {
		Serial.println(F("Incorrect Option!"));
		return;
	}
	*/

	// Le dice al PICC que se vaya a un estado de STOP cuando esta activo (o sea, lo haltea)
	mfrc522.PICC_HaltA();

	// Esto "para" la encriptación del PCD (proximity coupling device).
	// Tiene que ser llamado si o si despues de la comunicacion con una
	// autenticación exitosa, en otro caso no se va a poder iniciar otra comunicación.
	mfrc522.PCD_StopCrypto1();
	// ------- FIN DEL LECTOR RFID --------------
}
