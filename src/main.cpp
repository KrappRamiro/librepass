#include <Arduino.h>
#include <ArduinoLog.h>
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <iostream>
#include <vector>
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
		Log.info("Creando empleado");
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
		Log.info(("Setting name to: "), name);
		this->name = name;
	}
	String getName()
	{
		return name;
	}
	void setDni(String dni)
	{
		Log.info(("Setting dni to: "), dni);
		this->dni = dni;
	}
	String getDni()
	{
		return dni;
	}
	void setClearanceLevel(int clearanceLevel)
	{
		Log.info(("Setting clearanceLevel to: "), clearanceLevel);
		this->clearanceLevel = clearanceLevel;
	}
	int getClearanceLevel()
	{
		return clearanceLevel;
	}
	void setCargoAdministrativo(String cargoAdministrativo)
	{
		Log.info(("Setting cargoAdministrativo to: "), cargoAdministrativo);
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
Empleado misEmpleados[20];

// --------------- Variables del MFRC552 -------------------#
// key es una variable que se va a usar a lo largo de todo el codigo
MFRC522::MIFARE_Key key;
// Status es el codigo de estado de autenticacion
MFRC522::StatusCode status;
// Defino los pines que van al modulo RC552
MFRC522 mfrc522(SS_PIN, RST_PIN);
// --------------- FIN DE Variables del MFRC552 -------------------#

String getUserStringSerialInput()
{
	Serial.setTimeout(30000L);
	Serial.println(F("Enter the data to be written with the '*' character at the end:\n"));

	String userInput = Serial.readStringUntil('*');
	Serial.print("I received: ");
	Serial.println(userInput);
	return userInput;
}

void createEmployee()
{
	//	Empleado* temp = new Empleado(
	//		getUserStringSerialInput(),
	//		getUserStringSerialInput(),
	//		4,
	//		getUserStringSerialInput());
	misEmpleados[0] = Empleado(
		getUserStringSerialInput(),
		getUserStringSerialInput(),
		4,
		getUserStringSerialInput());
}

String getUID()
// conseguido de https://randomnerdtutorials.com/security-access-using-mfrc522-rfid-reader-with-arduino/
{
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
	Esta funcion lee la data del tag RFID
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
		Serial.print(misEmpleados[0].getName());
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
