#include "krapp_utils.h"
String getDateTime()
{
	/*
	Esta funcion retorna la fecha y hora en el formato: dd/mm/yyyy hh:mm:ss
	Args:
		None
	Returns:
		String con la fecha y hora
	*/
	time_t now
		= time(nullptr);
	struct tm* timeinfo = localtime(&now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", timeinfo);
	return buffer;
}
String getUID(MFRC522& rfid_mfrc522)
{
	/*
	Esta funcion retorna el UID del tag RFID
	Args:
		rfid_mfrc522: objeto de la clase MFRC522
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
	for (byte i = 0; i < rfid_mfrc522.uid.size; i++) {
		content.concat(String(rfid_mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
		content.concat(String(rfid_mfrc522.uid.uidByte[i], HEX));
	}
	content.toUpperCase();
	String theUID = content.substring(1);
	return theUID;
}

void readingData(MFRC522& rfid_mfrc522, MFRC522::MIFARE_Key& key, MFRC522::StatusCode& status, int SIZE_BUFFER, int MAX_SIZE_BLOCK, int redPin, int greenPin)
{
	/*
	Esta funcion lee los datos del tag que se esta leyendo, y los imprime en el Serial
	Args:
		None
	Returns:
		None
	*/

	// Imprime la información técnica del tag
	rfid_mfrc522.PICC_DumpDetailsToSerial(&(rfid_mfrc522.uid));

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
	// En caso de lograrlo, devuelve STATUS_OK, segun la inea 750 de MFRC522.cpp
	status = rfid_mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid_mfrc522.uid));

	// Intenta comunicarse con el PICC
	// SI no lo logró, tira el codigo de error y sale de la funcion
	// Si lo logró, sigue de largo
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Authentication failed: "));
		Serial.println(rfid_mfrc522.GetStatusCodeName(status));
		digitalWrite(redPin, HIGH);
		delay(1000);
		digitalWrite(redPin, LOW);
		return;
	}

	// Intenta leer el bloque n del tag
	// Si no lo logro, tira codigo de error y sale de la funcion
	// Si lo logró, va al else
	status = rfid_mfrc522.MIFARE_Read(block, buffer, &size);
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Reading failed: "));
		Serial.println(rfid_mfrc522.GetStatusCodeName(status));
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

void notFound(AsyncWebServerRequest* request)
{
	request->send(404, "text/plain", "Not found");
	return;
}

String processor(const String& var)
{
	/*
	Esta funcion procesa los placeholders que se encuentran en el index.html
	Args:
		var: String con el placeholder a procesar
	Returns:
		String con el valor de el placeholder procesado
	*/
	Log.info(F("The processor function is processing: %s" CR), var);
	if (var == "TEMPERATURE") {
		return String("Consiguiendo temperatura...");
	} else if (var == "HUMIDITY") {
		return String("Consiguiendo humedad...");
	} else if (var == "HOUR") {
		return String("Consiguiendo hora...");
	} else if (var == "SE_LEYO") {
		return String("Esperando lectura");
	} else {
		return String("");
	}
}
