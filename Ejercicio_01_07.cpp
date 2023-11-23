#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct Estudiante
{
    char nombres[30];
    char apellido[30];
    char materia[30];
    int nota;
};

// prototipos:
void ingresarDatos();
void leerArchivoYAlmacenar();
void adicionarDatos();

int main()
{
    int opcion;
    do
    {
        cout<<"1.- Ingresar datos"<<endl;
        cout<<"2.- Leer archivo y almacenar en vector"<<endl;
        cout<<"3.- Adicionar otros datos"<<endl;
        cout<<"4.- Salir"<<endl;
        cout<<"Seleccione una opcion: ";cin>>opcion;

        switch (opcion)
        {
        case 1:
            ingresarDatos();
            break;
        case 2:
            leerArchivoYAlmacenar();
            break;
        case 3:
            adicionarDatos();
            break;
        case 4:
            cout<<"Saliendo del programa"<<endl;
            break;
        default:
            cout<<"Opcion no valida, intentelo de nuevo"<< endl;
        }
    } while (opcion!=4);

    return 0;
}

// funcion para ingresar datos y almacenarlos en el archivo
void ingresarDatos()
{
    ofstream archivo("Personas.txt", ios::app);

    if (!archivo)
    {
        cout<<"Error al abrir el archivo Personas.txt"<<endl;
        return;
    }
    Estudiante estudiante;
    cout<<"Ingrese el nombre: ";
    cin.ignore();
    cin.getline(estudiante.nombres, 30);
    cout<<"Ingrese el apellido: ";
    cin.getline(estudiante.apellido, 30);
    cout<<"Ingrese la materia: ";
    cin.getline(estudiante.materia, 30);
    cout<<"Ingrese la nota: ";cin>>estudiante.nota;
    archivo <<estudiante.nombres<<";"
            <<estudiante.apellido<<";"
            <<estudiante.materia<<";"
            <<estudiante.nota<<"\n";
    cout<<"Datos ingresados exitosamente"<<endl;

    archivo.close();
}

// funcion para leer el archivo y almacenar los datos en un vector
void leerArchivoYAlmacenar()
{
    ifstream archivo("Personas.txt");
    if (!archivo)
    {
        cout << "Error al abrir el archivo Personas.txt" << endl;
        return;
    }
    vector<Estudiante> estudiantes;
    Estudiante estudiante;

    while (archivo.getline(estudiante.nombres, 30, ';') &&
           archivo.getline(estudiante.apellido, 30, ';') &&
           archivo.getline(estudiante.materia, 30, ';') &&
           (archivo >> estudiante.nota))
    {
        estudiantes.push_back(estudiante);
    }
    archivo.close();
    // mostrar los datos almacenados en el vector
    for (const auto &est : estudiantes)
    {
        cout<<"Nombre: "<<est.nombres<<endl;
        cout<<"Apellido: "<<est.apellido<<endl;
        cout<<"Materia: "<<est.materia<<endl;
        cout<<"Nota: "<<est.nota<<endl;
        cout<<"------------------------"<<endl;
    }
}

// funcion para adicionar datos al archivo
void adicionarDatos()
{
    ingresarDatos();
}
