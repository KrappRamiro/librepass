#include "ArduinoJson.h"
#include "HTTPClient.h"
#include "Wire.h"
#include "krapp_utils.h"
#include "oled_images.h"
#define SS_PIN 5 // Pin de SDA del MFRC522
#define RST_PIN 22 // Pin de RST del MFRC522
#define SIZE_BUFFER 18 // Este es el tamaño del buffer con el que voy a estar trabajando.
// Por que es 18? Porque son 16 bytes de los datos del tag, y 2 bytes de checksum
#define MAX_SIZE_BLOCK 16 // algo del RFID
#define I2C_SDA 26
#define I2C_SCL 27
#define RELAY 2
//------------------ INICIO DE Configuracion de conexion a internet ----------------
// const char* ssid = "TeleCentro-882b"; // Nombre de la red
// const char* password = "ZGNJVMMHZ2MY"; // Contraseña de la red
const char* ssid = "Krapp";
const char* password = "thinkpad1234";
String response = ""; // String for storing server response
DynamicJsonDocument doc(2048); // JSON document for the API
// ----------------- FIN DE Configuracion de conexion a internet ------------------

//  ---------------- INICIO DE Variables del MFRC522 ---------------------
MFRC522::MIFARE_Key key; // key es una variable que se va a usar a lo largo de todo el codigo
MFRC522::StatusCode status; // Status es el codigo de estado de autenticacion
MFRC522 mfrc522(SS_PIN, RST_PIN); // Defino los pines que van al modulo RC522
// ----------------- FIN DE Variables del MFRC522 ---------------------

// --------------------- DISPLAY OLED ---------------------------------

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // create an OLED display object connected to I2C

void displayMenu()
{
	oled.clearDisplay();
	oled.setTextSize(2);
	oled.setCursor(20, 0);
	oled.printf("PUERTA %i", doorNumber);
	oled.drawLine(0, 15, 128, 15, WHITE);
	oled.drawLine(0, 16, 128, 16, WHITE);
	oled.display();
	oled.setTextSize(1);
}

void setup()
{
	Wire.begin(I2C_SDA, I2C_SCL);
	// Prendo el led de la placa cuando inicia el sistema
	pinMode(RELAY, OUTPUT);
	Serial.begin(9600);
	SPI.begin(); // Inicio el bus SPI

	// --------- OLED -------------------------
	if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // initialize OLED display with I2C address 0x3C
		Serial.println(F("failed to start SSD1306 OLED"));
		while (1)
			;
	}
	delay(2000); // wait two seconds for initializing
	oled.setCursor(0, 0);
	oled.setTextColor(WHITE); // set text color
	//-----------------------------------------

	// ---------- WIFI ------------------------
	// Me conecto a internet mediante Wi-Fi
	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(1000);
	}
	Serial.print("\n");
	Serial.print("La IP del ESP32 es: ");
	Serial.println(WiFi.localIP());
	//-----------------------------------------

	// ---------- MFRC522 ------------------------
	mfrc522.PCD_Init(); // Inicio el MFRC522
	Serial.println(F("Acerca tu tarjeta RFID\n"));
	//-----------------------------------------

	// ------------ IMPRIMIR TEXTO DE BIENVENIDA --------------

	oled.clearDisplay();
	oled.setTextSize(2);
	oled.print("KRAPP\nRAMIRO\n7D");
	oled.setTextSize(1);
	oled.display();
	delay(2000);
}

void loop()
{

	oled.clearDisplay();
	displayMenu();

	if (!mfrc522.PICC_IsNewCardPresent()) { // Se espera a que se acerque un tag
		return;
	}
	if (!mfrc522.PICC_ReadCardSerial()) { // Se espera a que se lean los datos
		return;
	}
	// Descomentar solamente si se quiere Dumpear toda la info acerca de la tarjeta leida, ojo que llama automaticamente a PICC_HaltA()
	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

	// ------------------------------------------------------------------------------------------
	// -------------- INICIO DE ACCIONES QUE SE EJECUTAN SOLO SI SE LEE UNA TARJETA -------------
	// ------------------------------------------------------------------------------------------

	HTTPClient http; // Inicio el cliente http
	String uid = getUID(mfrc522); // Consigo la UID de la tarjeta como un string
	Serial.print("La UID leida es: ");
	Serial.println(uid);
	uid.replace(" ", "_"); // Le pongo _ a los espacios de la UID
	String request = "http://librepass-env.us-east-1.elasticbeanstalk.com/api/let_employee_pass/";
	request = request + uid + "/" + doorNumber; // armo la request
	http.begin(request); // Start the request
	http.GET(); // Use HTTP GET request
	response = http.getString(); // Response from server
	DeserializationError error = deserializeJson(doc, response); // Parse JSON, read error if any
	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.f_str());
		return;
	}
	http.end(); // Close connection
	Serial.println(doc["mensaje"].as<char*>()); // Print parsed value on Serial Monitor
	bool acceso = doc["access"];
	if (acceso) {
		Serial.println("Activando la cerradura");
		digitalWrite(RELAY, HIGH); // Activo la cerradura
		// display bitmap
		for (int i = 0; i < 3; i++) {
			displayMenu();
			oled.drawBitmap(44, 22, bitmap_check, 40, 40, WHITE);
			oled.display();
			delay(250);
			displayMenu();
			oled.drawBitmap(24, 22, bitmap_check_2, 80, 40, WHITE);
			oled.display();
			delay(250);
		}
		digitalWrite(RELAY, LOW); // Desactivo la cerradura despues de la animacion
	} else {
		// display bitmap
		for (int i = 0; i < 3; i++) {
			displayMenu();
			oled.drawBitmap(44, 22, bitmap_cross, 40, 40, WHITE);
			oled.display();
			delay(250);
			displayMenu();
			oled.drawBitmap(24, 22, bitmap_cross_2, 80, 40, WHITE);
			oled.display();
			delay(250);
		}
	}

	mfrc522.PICC_HaltA(); // Le dice al PICC que se vaya a un estado de STOP cuando esta activo (o sea, lo haltea)

	// Esto "para" la encriptación del PCD (proximity coupling device).
	// Tiene que ser llamado si o si despues de la comunicacion con una
	// autenticación exitosa, en otro caso no se va a poder iniciar otra comunicación.
	mfrc522.PCD_StopCrypto1();
	// ------- FIN DEL LECTOR RFID --------------
}
