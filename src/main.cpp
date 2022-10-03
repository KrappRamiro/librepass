#include "ArduinoJson.h"
#include "HTTPClient.h"
#include "krapp_utils.h"
#define SS_PIN 5
#define RST_PIN 21
#define SIZE_BUFFER 18 // Este es el tamaño del buffer con el que voy a estar trabajando.
// Por que es 18? Porque son 16 bytes de los datos del tag, y 2 bytes de checksum
#define MAX_SIZE_BLOCK 16
#define aceptarAccesoLED 25
#define rechazarAccesoLED 26
#define errorLED 27
#define doorNumber 2

bool prenderLEDError = false;
//------------------ INICIO DE Configuracion de conexion a internet ----------------
// const char* ssid = "TeleCentro-882b"; // Nombre de la red
// const char* password = "ZGNJVMMHZ2MY"; // Contraseña de la red
const char* ssid = "Krapp";
const char* password = "thinkpad1234";
// String for storing server response
String response = "";
// JSON document
DynamicJsonDocument doc(2048);
// ----------------- FIN DE Configuracion de conexion a internet ------------------

//  ---------------- INICIO DE Variables del MFRC552 ---------------------
// key es una variable que se va a usar a lo largo de todo el codigo
MFRC522::MIFARE_Key key;
// Status es el codigo de estado de autenticacion
MFRC522::StatusCode status;
// Defino los pines que van al modulo RC552
MFRC522 mfrc522(SS_PIN, RST_PIN);
// ----------------- FIN DE Variables del MFRC552 ---------------------

void setup()
{
	Serial.begin(9600);
	SPI.begin(); // Inicio el bus SPI
	// Log.begin(LOG_LEVEL_NOTICE, &Serial); // Inicio del sistema de logging

	// Prendo el led de la placa cuando inicia el sistema
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(aceptarAccesoLED, OUTPUT);
	pinMode(rechazarAccesoLED, OUTPUT);
	pinMode(errorLED, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);		delay(1000);
		digitalWrite(aceptarAccesoLED, LOW);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);

	// Me conecto a internet mediante Wi-Fi
	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(1000);
	}
	Serial.print("\n");
	// Imprimo la IP local del ESP32 (192.168.x.x)
	Serial.print("La IP del ESP32 es: ");
	Serial.println(WiFi.localIP());

	// Inicio el MFRC552
	mfrc522.PCD_Init();
	// Le pido al usuario que acerque el tag RFID
	Serial.println(F("Acerca tu tarjeta RFID\n"));
}

void loop()
{

	HTTPClient http; // Inicio el cliente http
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

	String uid = getUID(mfrc522);
	Serial.print("La UID leida es: ");
	Serial.println(uid);
	uid.replace(" ", "_");
	String request = "http://192.168.33.62:5000/api/let_employee_pass/";
	request = request + uid + "/" + doorNumber;

	// Start the request
	http.begin(request);
	// Use HTTP GET request
	http.GET();
	// Response from server
	response = http.getString();
	// Parse JSON, read error if any
	DeserializationError error = deserializeJson(doc, response);
	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.f_str());
		bool prenderLEDError = true;
		return;
	}

	// Close connection
	http.end();
	// Print parsed value on Serial Monitor
	Serial.println(doc["mensaje"].as<char*>());
	bool acceso = doc["access"];
	if (acceso) {
		Serial.println("Activando la cerradura");
		digitalWrite(aceptarAccesoLED, HIGH);
		delay(1000);
		digitalWrite(aceptarAccesoLED, LOW);
	} else if (prenderLEDError) {
		digitalWrite(errorLED, HIGH);
		delay(1000);
		digitalWrite(errorLED, LOW);
	} else {
		digitalWrite(aceptarAccesoLED, LOW);
		delay(1000);
		digitalWrite(aceptarAccesoLED, LOW);
	}

	// Esperar un tiempo hasta el proximo acceso
	delay(1000);
	// Le dice al PICC que se vaya a un estado de STOP cuando esta activo (o sea, lo haltea)
	mfrc522.PICC_HaltA();

	// Esto "para" la encriptación del PCD (proximity coupling device).
	// Tiene que ser llamado si o si despues de la comunicacion con una
	// autenticación exitosa, en otro caso no se va a poder iniciar otra comunicación.
	mfrc522.PCD_StopCrypto1();
	// ------- FIN DEL LECTOR RFID --------------
}
