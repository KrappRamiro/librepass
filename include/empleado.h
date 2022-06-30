#ifndef empleado
#define empleado
#include <Arduino.h>
#include <ArduinoLog.h>

class Empleado {
private:
	String name;
	bool isAlive;
	String dni;
	int clearanceLevel;
	String cargoAdministrativo;

public:
	static int cuentaEmpleados;
	// Constructor lleno
	Empleado(String name, String dni, int clearanceLevel, String cargoAdministrativo);
	// Constructor vacio
	Empleado();
	// Destructor
	~Empleado();
	void setLifeStatus(bool lifeStatus);
	bool getLifeStatus();

	void setName(String name);

	String getName();

	void setDni(String dni);

	String getDni();

	void setClearanceLevel(int clearanceLevel);

	int getClearanceLevel();

	void setCargoAdministrativo(String cargoAdministrativo);

	String getCargoAdministrativo();
};
#endif
