/*
TODO Hacer el registro de viajes de cada pasajero

*/

#include <Arduino.h>
#include <iostream>
#include <vector>

/* Se usa std::vector en reemplazo de usar `using namespace std` por una muy
buena razon, y es que se evita el namespace pollution. Si no sabes qué es eso,
te recomiendo personalmente este post,  es corto, sencillo, y bien explicado
para principiantes:
https://www.thecrazyprogrammer.com/2021/01/better-alternatives-for-using-namespace-std-in-c.html
*/
using std::vector;

class Tren {
	/* Si alguien se pregunta por qué las variables estan en private,
	la respuesta es muy sencilla:
	Es porque no se desea que se modifiquen las variables de forma manual.
	Esto es porque esa práctica es propensa a errores, ya que se podría introducir
	un valor inadecuado y generar algun problema.

	Por eso se usan funciones public, normalmente llamadas setters, que permiten
	asignar y leer los valores, y que establecen un margen de valores seguros.  */
private:
	int speed = 0; // velocidad, en km/h
	String serialNumber; // numero de serie, que va a identificar al tren
	String currentStation;
	String trainType; // esta var se refiere si es a nafta, si es electrico, etc

public:
	Tren(String serialNumber, String trainType)
	{
		this->serialNumber = serialNumber;
		this->trainType = trainType;
	}
	// Para los getters tenia dos opciones, o retornaba un struct, o hacia una funcion
	// para cada variable
	int getSpeed()
	{
		return speed;
	}
	String getSerialNumber()
	{
		return serialNumber;
	}
	String getCurrentStation()
	{
		return currentStation;
	}
	String getTrainType()
	{
		return trainType;
	}

	void travelToStation(String stationName)
	{
		currentStation = stationName;
		// TODO Hacer algo parecido con la funcion que tenes en Pasajero
	}
};

class Persona {
	// Esta clase sirve como padre para las clases Maquinista y Pasajero
	// IDEA: Hacer que las personas puedan morir, y que se invalide la SUBE.
	// Por ejemplo,         if (!persona.isAlive) {allowTransaction(false)}
private:
	String name;
	bool isAlive = true;
	String dni;

public:
	void kill()
	{
		isAlive = false;
	}
};

class Maquinista : public Persona { // clase que hereda de Persona
private:
	String name;
	float salary;
	int seniority; // el seniority se piensa con los años de antiguedad
public:
};

class Pasajero : public Persona { // clase que hereda de Persona
private:
	String nombre;
	int sube_id;
	float sube_saldo;

public:
	void travelToStation(String stationName)
	{
		// TODO hay que hacer la transaccion

		/*
		Como deberia ser esto? tendria que ser así:
		1- Calcular distancia a la estacion
		2- Cobrar 5 pesos por cada estacion

		Para calcular la estación, lo que haría sería armar un vector de
		estaciones, algo asi:

		["temperley", "lomas de zamora", "banfield", "remedios de escalada", "etc"]

		1 - Llamar a una funcion getCurrentStation() que retorne un String
		de la estacion actual
		2 - sabiendo la estacion actual, se podría hacer un getIndex()
		https://www.geeksforgeeks.org/how-to-find-index-of-a-given-element-in-a-vector-in-cpp/
		Entonces se haria un getIndex(estacionActual) - getIndex(estacionDestino),
		y el resultado de esa operacion es la distancia entre las estaciones.

		OJO: Esa operacion puede dar resultados negativos, por eso habria que guardarlo
		en una variable, checkear si es negativa, y en ese caso pasarla a positivo

		3 - Llamar a la funcion calcularPasaje(int price_per_estation, int distance)


		*/
	}
};

#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#define SS_PIN 21
#define RST_PIN 22
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16
#define greenPin 12
#define redPin 32
// used in authentication
MFRC522::MIFARE_Key key;
// authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

// reads data from card/tag
void readingData()
{
	// prints the technical details of the card/tag
	mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

	// prepare the key - all keys are set to FFFFFFFFFFFFh
	for (byte i = 0; i < 6; i++)
		key.keyByte[i] = 0xFF;

	// buffer for read data
	byte buffer[SIZE_BUFFER] = { 0 };

	// the block to operate
	byte block = 1;
	byte size = SIZE_BUFFER; // authenticates the block to operate
	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		digitalWrite(redPin, HIGH);
		delay(1000);
		digitalWrite(redPin, LOW);
		return;
	}

	// read data from block
	status = mfrc522.MIFARE_Read(block, buffer, &size);
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		digitalWrite(redPin, HIGH);
		delay(1000);
		digitalWrite(redPin, LOW);
		return;
	} else {
		digitalWrite(greenPin, HIGH);
		delay(1000);
		digitalWrite(greenPin, LOW);
	}

	Serial.print(F("\nData from block ["));
	Serial.print(block);
	Serial.print(F("]: "));

	// prints read data
	for (uint8_t i = 0; i < MAX_SIZE_BLOCK; i++) {
		Serial.write(buffer[i]);
	}
	Serial.println(" ");
}

void writingData()
{

	// prints thecnical details from of the card/tag
	mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

	// waits 30 seconds dor data entry via Serial
	Serial.setTimeout(30000L);
	Serial.println(F("Enter the data to be written with the '#' character at the end \n[maximum of 16 characters]:"));

	// prepare the key - all keys are set to FFFFFFFFFFFFh
	for (byte i = 0; i < 6; i++)
		key.keyByte[i] = 0xFF;

	// buffer para armazenamento dos dados que iremos gravar
	// buffer for storing data to write
	byte buffer[MAX_SIZE_BLOCK] = "";
	byte block; // the block to operate
	byte dataSize; // size of data (bytes)

	// recover on buffer the data from Serial
	// all characters before chacactere '#'
	dataSize = Serial.readBytesUntil('#', (char*)buffer, MAX_SIZE_BLOCK);
	// void positions that are left in the buffer will be filled with whitespace
	for (byte i = dataSize; i < MAX_SIZE_BLOCK; i++) {
		buffer[i] = ' ';
	}

	block = 1; // the block to operate
	String str = (char*)buffer; // transforms the buffer data in String
	Serial.println(str);

	// authenticates the block to operate
	// Authenticate is a command to hability a secure communication
	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
		block, &key, &(mfrc522.uid));

	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("PCD_Authenticate() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		digitalWrite(redPin, HIGH);
		delay(1000);
		digitalWrite(redPin, LOW);
		return;
	}
	// else Serial.println(F("PCD_Authenticate() success: "));

	// Writes in the block
	status = mfrc522.MIFARE_Write(block, buffer, MAX_SIZE_BLOCK);
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("MIFARE_Write() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		digitalWrite(redPin, HIGH);
		delay(1000);
		digitalWrite(redPin, LOW);
		return;
	} else {
		Serial.println(F("MIFARE_Write() success: "));
		digitalWrite(greenPin, HIGH);
		delay(1000);
		digitalWrite(greenPin, LOW);
	}
}

// menu to operation choice
int menu()
{
	Serial.println(F("\nChoose an option:"));
	Serial.println(F("0 - Reading data"));
	Serial.println(F("1 - Writing data\n"));

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
	SPI.begin(); // Init SPI bus
	pinMode(greenPin, OUTPUT);
	pinMode(redPin, OUTPUT);

	// Init MFRC522
	mfrc522.PCD_Init();
	Serial.println("Approach your reader card...");
	Serial.println();
}

void loop()
{
	// Aguarda a aproximacao do cartao
	// waiting the card approach
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}
	// Select a card
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));</p><p> //call menu function and retrieve the desired option
	int op = menu();

	if (op == 0)
		readingData();
	else if (op == 1)
		writingData();
	else {
		Serial.println(F("Incorrect Option!"));
		return;
	}

	// instructs the PICC when in the ACTIVE state to go to a "STOP" state
	mfrc522.PICC_HaltA();
	// "stop" the encryption of the PCD, it must be called after communication with authentication, otherwise new communications can not be initiated
	mfrc522.PCD_StopCrypto1();
}