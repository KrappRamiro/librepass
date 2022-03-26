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