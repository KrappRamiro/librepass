#include "empleado.h"
// Constructor lleno

Empleado::Empleado(String name, String dni, int clearanceLevel, String cargoAdministrativo)
{
	Log.infoln("Creando empleado con los siguientes valores: ");
	cuentaEmpleados++;
	setName(name);
	setDni(dni);
	setClearanceLevel(clearanceLevel);
	setCargoAdministrativo(cargoAdministrativo);
}
// Constructor vacio
Empleado::Empleado() { cuentaEmpleados++; }

// Destructor
Empleado::~Empleado()
{
	cuentaEmpleados--;
}
void Empleado::setLifeStatus(bool lifeStatus)
{
	this->isAlive = lifeStatus;
}
bool Empleado::getLifeStatus()
{
	return isAlive;
}
void Empleado::setName(String name)
{
	Log.infoln(("\tSetting name to: %s "), name);
	this->name = name;
}
String Empleado::getName()
{
	return name;
}
void Empleado::setDni(String dni)
{
	Log.infoln(("\tSetting dni to: %s "), dni);
	this->dni = dni;
}
String Empleado::getDni()
{
	return dni;
}
void Empleado::setClearanceLevel(int clearanceLevel)
{
	Log.infoln(("\tSetting clearanceLevel to: %d "), clearanceLevel);
	Serial.println();
	this->clearanceLevel = clearanceLevel;
}
int Empleado::getClearanceLevel()
{
	return clearanceLevel;
}
void Empleado::setCargoAdministrativo(String cargoAdministrativo)
{
	Log.infoln(("\tSetting cargoAdministrativo to: %s "), cargoAdministrativo);
	Serial.println();
	this->cargoAdministrativo = cargoAdministrativo;
}
String Empleado::getCargoAdministrativo()
{
	return cargoAdministrativo;
}