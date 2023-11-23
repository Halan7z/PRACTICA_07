// Materia: Programación I, Paralelo 1

// Autor: Alan Franz Flores Campos

// Fecha creación: 16/11/2023

// Fecha modificación: 22/11/2023

// Número de ejericio: 2

// Problema planteado: Escribe un programa que tenga un menú de tres opciones: Ingreso de datos estudiantes. Ingreso de materias y notas....

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

const char* NOMBRE_ARCHIVO_ESTUDIANTES = "Estudiantes.bin";
const char* NOMBRE_ARCHIVO_TEMPORAL_ESTUDIANTES = "TemporalEstudiantes.bin";

const char* NOMBRE_ARCHIVO_NOTAS = "Notas.bin";
const char* NOMBRE_ARCHIVO_TEMPORAL_NOTAS = "TemporalNotas.bin";

struct structEstudiante
{
    char ci[10];
    char nombres[30];
    char apellidos[30];
};

struct structNotas
{
    char ci[10];
    char materia[30];
    int nota;
};

structEstudiante ingresoDatosEstudiante();
void escribirArchivoBinarioEstudiantes(structEstudiante);
void leerArchivoBinarioEstudiantes();
bool buscarDatoArchivoBinarioEstudiantes(char[]);
void leerNotasPorEstudiante(char[]);

structNotas ingresoDatosNotas();
void escribirArchivoBinarioNotas(structNotas);
void leerArchivoBinarioNotas();
void leerArchivoBinarioNotasEstudiante(char[10]);

int main()
{
    int opcion;

    do
    {
        cout<<"1.- Ingreso de datos estudiantes "<<endl;
        cout<<"2.- Ingreso de materias y notas "<<endl;
        cout<<"3.- Reporte de estudiantes y notas ingresadas"<<endl;
        cout<<"4.- Salir "<<endl;
        cout<<"Presione un numero según la opción que desee: ";cin>>opcion;
        switch (opcion)
        {
        case 1:
            {
                structEstudiante estudiante = ingresoDatosEstudiante();
                escribirArchivoBinarioEstudiantes(estudiante);
            }
            break;
        case 2:
            {
                structNotas notas = ingresoDatosNotas();
                escribirArchivoBinarioNotas(notas);
            }
            break;
        case 3:
            leerArchivoBinarioEstudiantes();
            // Selecciona un estudiante para mostrar sus notas
            {
                structEstudiante estudiante;
                cout<<"Ingrese el CI del estudiante para ver sus notas: ";
                cin>>estudiante.ci;
                leerArchivoBinarioNotasEstudiante(estudiante.ci);
            }
            break;
        case 4:
            cout<<"SALIDA DEL REGISTRO"<<endl;
            break;
        default:
            cout<<"Opcion no valida, intentelo de nuevo"<<endl;
        }
    } while (opcion!=4);

    return 0;
}

structEstudiante ingresoDatosEstudiante()
{
    structEstudiante estudiante;
    cout<<"INGRESO DE DATOS DEL ESTUDIANTE"<<endl;
    cout<<"==============================="<<endl;
    cout<<"\tIngrese el CI: ";
    cin.ignore();
    cin.getline(estudiante.ci, 10);
    if (buscarDatoArchivoBinarioEstudiantes(estudiante.ci))
    {
        cout<<"Error: Ya existe un estudiante con ese CI"<<endl;
        estudiante.ci[0] = '\0';
        return estudiante;
    }

    cout << "\tIngrese el nombre del estudiante: ";
    cin.getline(estudiante.nombres, 30);
    cout << "\tIngrese el apellido del estudiante: ";
    cin.getline(estudiante.apellidos, 30);
    return estudiante;
}

void escribirArchivoBinarioEstudiantes(structEstudiante estudiante)
{
    if (estudiante.ci[0] != '\0')
    {
        ofstream archivoEscritura;
        archivoEscritura.open(NOMBRE_ARCHIVO_ESTUDIANTES, ios::app | ios::binary);
        archivoEscritura.write((char*)&estudiante, sizeof(structEstudiante));
        archivoEscritura.close();
        cout << "Estudiante ingresado exitosamente" << endl;
    }
}

void leerArchivoBinarioEstudiantes()
{
    ifstream archivoLectura;
    structEstudiante estudiante;
    archivoLectura.open(NOMBRE_ARCHIVO_ESTUDIANTES, ios::in | ios::binary);

    cout << "\nLISTADO DE ESTUDIANTES" << endl;
    cout << "========================" << endl;

    while (archivoLectura.read((char*)&estudiante, sizeof(structEstudiante)))
    {
        cout << "\tCI: " << estudiante.ci << endl;
        cout << "\tNombres: " << estudiante.nombres << endl;
        cout << "\tApellidos: " << estudiante.apellidos << endl << endl;
    }

    archivoLectura.close();
}

bool buscarDatoArchivoBinarioEstudiantes(char buscar[10])
{
    ifstream archivoLectura;
    structEstudiante estudiante;
    archivoLectura.open(NOMBRE_ARCHIVO_ESTUDIANTES, ios::in | ios::binary);

    while (archivoLectura.read((char*)&estudiante, sizeof(structEstudiante)))
    {
        if (strcmp(buscar, estudiante.ci) == 0)
        {
            archivoLectura.close();
            return true;
        }
    }

    archivoLectura.close();
    return false;
}

structNotas ingresoDatosNotas()
{
    structNotas notas;
    cout << "INGRESO DE NOTAS DEL ESTUDIANTE" << endl;
    cout << "===============================" << endl;
    cout << "\tIngrese el CI del estudiante: ";
    cin.ignore();
    cin.getline(notas.ci, 10);

    if (!buscarDatoArchivoBinarioEstudiantes(notas.ci))
    {
        cout << "Error: El estudiante con ese CI no existe." << endl;
        notas.ci[0] = '\0';
        return notas;
    }

    cout << "\tIngrese la materia: ";
    cin.getline(notas.materia, 30);
    cout << "\tIngrese la nota: ";
    cin >> notas.nota;
    return notas;
}

void escribirArchivoBinarioNotas(structNotas notas)
{
    if (notas.ci[0] != '\0')
    {
        ofstream archivoEscritura;
        archivoEscritura.open(NOMBRE_ARCHIVO_NOTAS, ios::app | ios::binary);
        archivoEscritura.write((char*)&notas, sizeof(structNotas));
        archivoEscritura.close();
        cout << "Nota ingresada exitosamente." << endl;
    }
}

void leerArchivoBinarioNotas()
{
    ifstream archivoLectura;
    structNotas notas;
    archivoLectura.open(NOMBRE_ARCHIVO_NOTAS, ios::in | ios::binary);

    cout << "\nLISTADO DE NOTAS" << endl;
    cout << "================" << endl;

    while (archivoLectura.read((char*)&notas, sizeof(structNotas))) // Leer las notas del archivo mientras haya datos
    {
        cout << "\tCI: " << notas.ci << endl;
        cout << "\tMateria: " << notas.materia << endl;
        cout << "\tNota: " << notas.nota << endl;
        cout << "----------------" << endl;
    }

    archivoLectura.close();
}

void leerArchivoBinarioNotasEstudiante(char buscar[10])
{
    ifstream archivoLectura;
    archivoLectura.open(NOMBRE_ARCHIVO_NOTAS, ios::in | ios::binary);
    structNotas notas;

    cout << "\nLISTADO DE NOTAS PARA EL ESTUDIANTE CON CI: " << buscar << endl;
    cout << "=============================================" << endl;

    bool notasEncontradas = false;

    while (archivoLectura.read((char*)&notas, sizeof(structNotas)))
    {
        if (strcmp(buscar, notas.ci) == 0)
        {
            cout<<"\tMateria: "<<notas.materia<<endl;
            cout<<"\tNota: "<<notas.nota<<endl;
            cout<<"--------------------------------"<<endl;
            notasEncontradas=true;
        }
    }

    if (!notasEncontradas)
    {
        cout<<"No se encontraron notas para el estudiante con CI: "<<buscar<<endl;
    }

    archivoLectura.close();
}

void leerNotasPorEstudiante(char buscar[10])
{
    ifstream archivoLecturaNotas;
    structNotas notas;

    archivoLecturaNotas.open(NOMBRE_ARCHIVO_NOTAS, ios::in | ios::binary);

    cout<<"\nNOTAS POR ESTUDIANTE"<<endl;
    cout<<"======================="<<endl;

    while (archivoLecturaNotas.read((char*)&notas, sizeof(structNotas)))
    {
        if (strcmp(buscar, notas.ci)==0) // Comprobar si el CI coincide

        {
            cout<<"\tMateria: "<<notas.materia<<endl;
            cout<<"\tNota: "<<notas.nota<<endl<<endl;
        }
    }
    archivoLecturaNotas.close();
}
