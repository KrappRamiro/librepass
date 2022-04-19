// TODO: Para hacer eventos, tengo que usar POST, no GET
#include <Arduino.h>
#include <ArduinoLog.h>
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <iostream>
#include <time.h>
#include <vector>

#ifdef ESP32
#include <AsyncTCP.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

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
class Empleado {
	/*
		Si alguien se pregunta por qué, en las clases, las variables estan en private,
		la respuesta es muy sencilla:
		Es porque no se desea que se modifiquen las variables de forma manual.
		Esto es porque esa práctica es propensa a errores, ya que se podría introducir
		un valor inadecuado y generar algun problema.

		Por eso se usan funciones public, normalmente llamadas setters, que permiten
		asignar y leer los valores, y que establecen un margen de valores seguros.
	*/
private:
	String name;
	bool isAlive = true;
	String dni;
	int clearanceLevel;
	String cargoAdministrativo;

public:
	static int cuentaEmpleados;
	// Constructor lleno
	Empleado(String name, String dni, int clearanceLevel, String cargoAdministrativo)
	{
		Log.infoln("Creando empleado con los siguientes valores: ");
		cuentaEmpleados++;
		setName(name);
		setDni(dni);
		setClearanceLevel(clearanceLevel);
		setCargoAdministrativo(cargoAdministrativo);
	}
	// Constructor vacio
	Empleado() { cuentaEmpleados++; }

	// Destructor
	~Empleado()
	{
		cuentaEmpleados--;
	}
	void setLifeStatus(bool lifeStatus)
	{
		this->isAlive = lifeStatus;
	}
	bool getLifeStatus()
	{
		return isAlive;
	}
	void setName(String name)
	{
		Log.infoln(("\tSetting name to: %s "), name);
		this->name = name;
	}
	String getName()
	{
		return name;
	}
	void setDni(String dni)
	{
		Log.infoln(("\tSetting dni to: %s "), dni);
		this->dni = dni;
	}
	String getDni()
	{
		return dni;
	}
	void setClearanceLevel(int clearanceLevel)
	{
		Log.infoln(("\tSetting clearanceLevel to: %d "), clearanceLevel);
		Serial.println();
		this->clearanceLevel = clearanceLevel;
	}
	int getClearanceLevel()
	{
		return clearanceLevel;
	}
	void setCargoAdministrativo(String cargoAdministrativo)
	{
		Log.infoln(("\tSetting cargoAdministrativo to: %s "), cargoAdministrativo);
		Serial.println();
		this->cargoAdministrativo = cargoAdministrativo;
	}
	String getCargoAdministrativo()
	{
		return cargoAdministrativo;
	}
};
int Empleado::cuentaEmpleados = 0;

// std::vector<Empleado> Empleados;
// Un array de empleados para almacenar múltiples empleados
// TODO cambiarlo por un vector
// Empleado misEmpleados[20];
Empleado miEmpleado;

//------------------ INICIO DE Configuracion de conexion a internet ----------------
const char* ssid = "TeleCentro-882b"; // Nombre de la red
const char* password = "ZGNJVMMHZ2MY"; // Contraseña de la red
AsyncWebServer server(80); // Inicio el web server en el puerto 80
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

String getDateTime()
{
	/*
	Esta funcion retorna la fecha y hora en el formato: dd/mm/yyyy hh:mm:ss
	Args:
		None
	Returns:
		String con la fecha y hora
	*/
	time_t now = time(nullptr);
	struct tm* timeinfo = localtime(&now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", timeinfo);
	return buffer;
}

String getSerialStringInput()
{
	/*
	Esta funcion retorna una cadena con el nombre del usuario que se esta ingresando en el Serial
	Args:
		None
	Returns:
		String con lo introducido por Serial
	*/
	Serial.setTimeout(30000L); // 30 segundos de timeout
	Serial.println(F("Enter the data to be written with the '*' character at the end:\n"));
	String userInput = Serial.readStringUntil('*'); // Lee hasta que encuentra un *
	Log.infoln("Received the input: %s", userInput); // Imprimo el input
	return userInput; // Devuelvo el input
}

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

String getUID()
{
	/*
	Esta funcion retorna el UID del tag que se esta leyendo
	Args:
		None
	Returns:
		String con el UID del tag
	*/
	// conseguido de https://randomnerdtutorials.com/security-access-using-mfrc522-rfid-reader-with-arduino/
	String content = "";
	for (byte i = 0; i < mfrc522.uid.size; i++) {
		content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
		content.concat(String(mfrc522.uid.uidByte[i], HEX));
	}
	content.toUpperCase();
	String theUID = content.substring(1);
	return theUID;
}

void readingData()
{
	/*
	Esta funcion lee los datos del tag que se esta leyendo, y los imprime en el Serial
	Args:
		None
	Returns:
		None
	*/

	// Imprime la información técnica del tag
	mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

	// Prepara la key, todas las keys estan seteadas a ser FFFFFFFFFFFFh
	for (byte i = 0; i < 6; i++)
		key.keyByte[i] = 0xFF;

	// Preparo un buffer para la lectura de informacion.
	// El tamaño del buffer depende de SIZE_BUFFER, es un #define que esta en la parte de arriba
	byte buffer[SIZE_BUFFER] = { 0 };

	// Defino en que bloque del tag voy a estar trabajando
	byte block = 1;
	byte size = SIZE_BUFFER; // size va a ser usado para leer luego el bloque

	// Intenta conectarse con el PICC (Proximity Integrated Circuit Card).
	// En caso de lograrlo, devuelve STATUS_OK, segun la inea 750 de MFRC552.cpp
	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));

	// Intenta comunicarse con el PICC
	// SI no lo logró, tira el codigo de error y sale de la funcion
	// Si lo logró, sigue de largo
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		digitalWrite(redPin, HIGH);
		delay(1000);
		digitalWrite(redPin, LOW);
		return;
	}

	// Intenta leer el bloque n del tag
	// Si no lo logro, tira codigo de error y sale de la funcion
	// Si lo logró, va al else
	status = mfrc522.MIFARE_Read(block, buffer, &size);
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		digitalWrite(redPin, HIGH);
		delay(1000);
		digitalWrite(redPin, LOW);
		return;
	} else {
		Serial.print(F("Reading OK"));
		digitalWrite(greenPin, HIGH);
		delay(1000);
		digitalWrite(greenPin, LOW);
	}

	// ----- A esta sección de aca solamente se llega despues de que todo salió bien ------//
	Serial.print(F("\nData from block ["));
	// Printea el bloque leido
	Serial.print(block);
	Serial.print(F("]: "));

	// Printea lo que leyó
	for (uint8_t i = 0; i < MAX_SIZE_BLOCK; i++) {
		Serial.write(buffer[i]);
	}
	Serial.println(F(" "));
}

int menu()
{
	/*
	Esta funcion imprime el menu en el Serial
	Args:
		None
	Returns:
		int con la opcion elegida por el usuario
	*/

	//  TODO: Reemplazar una parte de los contenidos de esta funcion
	//   con un llamado a getUserSerialInput
	Serial.println(F("\nElige una opcion"));
	Serial.println(F("0 - Leer data"));
	Serial.println(F("1 - Escribir data\n"));
	Serial.println(F("2 - leer nombre empleado\n"));

	// waits while the user does not start data
	while (!Serial.available()) { };

	// retrieves the chosen option
	int op = (int)Serial.read();

	// remove all characters after option (as \n per example)
	while (Serial.available()) {
		if (Serial.read() == '\n')
			break;
		Serial.read();
	}
	return (op - 48); // subtract 48 from read value, 48 is the zero from ascii table
}

const char index_html[] PROGMEM = R"rawliteral(
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
				text-align: center;
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
		</style>
	</head>
	<body>
		<h2>ESP32 Librepass WebServer</h2>
		<p>
			<i class="fas fa-thermometer-half" style="color: #059e8a"></i>
			<span class="dht-labels">Temperature is: </span>
			<span id="temperature">%TEMPERATURE%</span>
			<sup class="units">&deg;C</sup>
		</p>
		<p>
			<i class="fas fa-tint" style="color: #00add6"></i>
			<span class="dht-labels">Humidity is: </span>
			<span id="humidity">%HUMIDITY%</span>
			<sup class="units">&percnt;</sup>
		</p>
		<p>
			<i class="fa-solid fa-clock"></i>
			<span class="dht-labels">Date and hour is: </span>
			<span id="date-hour">%DATE-HOUR%</span>
		</p>
	</body>
	<script>
		setInterval(function () {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function () {
				if (this.readyState == 4 && this.status == 200) {
					document.getElementById("temperature").innerHTML = this.responseText;
				}
			};
			xhttp.open("GET", "/temperature", true); //esto llama al server.on() correspondiente
			console.log("Updating temperature");
			xhttp.send();
		}, 3000);
		setInterval(function () {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function () {
				if (this.readyState == 4 && this.status == 200) {
					document.getElementById("humidity").innerHTML = this.responseText;
				}
			};
			xhttp.open("GET", "/humidity", true); //esto llama al server.on() correspondiente
			console.log("updating humidity");
			xhttp.send();
		}, 3000);
		setInterval(function () {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function () {
				if (this.readyState == 4 && this.status == 200) {
					document.getElementById("date-hour").innerHTML = this.responseText;
				}
			};
			xhttp.open("GET", "/date-hour", true); //esto llama al server.on() correspondiente
			console.log("updating date-hour");
			xhttp.send();
		}, 3000);
	</script>
</html>
)rawliteral";

String processor(const String& var)
{
	/*
	Esta funcion procesa las variables que se encuentran en el index.html
	Args:
		var: String con la variable a procesar
	Returns:
		String con el valor de la variable procesada
	*/
	// Estos consiguiendo x... estan para testear nomas, en realidad no consiguen nada,
	// estan para que no se me buggee el cerebro mientras entiendo AJAX
	Serial.println(var);
	if (var == "TEMPERATURE") {
		return String("Consiguiendo temperatura...");
	} else if (var == "HUMIDITY") {
		return String("Consiguiendo humedad...");
	} else if (var == "HOUR") {
		return String("Consiguiendo hora...");
	} else {
		return String("");
	}
}

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
	// Se espera a que se acerque un tag
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}
	// Se espera a que se lean los datos
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}
	// Descomentar solamente si se quiere Dumpear toda la info acerca de la tarjeta leida
	// Ojo que llama automaticamente a PICC_HaltA()
	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

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

	// Le dice al PICC que se vaya a un estado de STOP cuando esta activo (o sea, lo haltea)
	mfrc522.PICC_HaltA();

	// Esto "para" la encriptación del PCD (proximity coupling device).
	// Tiene que ser llamado si o si despues de la comunicacion con una
	// autenticación exitosa, en otro caso no se va a poder iniciar otra comunicación.
	mfrc522.PCD_StopCrypto1();
}
