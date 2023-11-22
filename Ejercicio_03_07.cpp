// Materia: Programación I, Paralelo 1

// Autor: Alan Franz Flores Campos

// Fecha creación: 20/11/2023

// Fecha modificación: 22/11/2023

// Número de ejericio: 3

/* Problema planteado: Escribe un programa que tenga un menú de tres opciones:

·	Ingreso de datos de los equipo de la LPFB.
·	Ingreso los resultados de los partidos.
·	Reporte de la tabla de posiciones
·	Salir..............
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

struct structEquiposLPFB
{
    char nombreEquipo[50];
    char colorCamiseta[50];
    char departamento[30];
    int anioCreacion;
    int puntos; // Se agrega la variable puntos
};

struct structResultadosLPFB
{
    char nombreEquipoLocal[50];
    char nombreEquipoVisitante[50];
    int golesEquipoLocal;
    int golesEquipoVisitante;
    char fechaPartido[11];;
};

void ingresarEquipos();
bool compararEquipos(const structEquiposLPFB &equipo1, const structEquiposLPFB &equipo2);
void ingresarResultados();
int calcularDiferenciaGoles(const structEquiposLPFB &equipo);
void generarReporte();



int main()
{
    int opcion;

    do
    {
        cout << "\nMenu Principal" << endl;
        cout << "1. Ingresar equipos" << endl;
        cout << "2. Ingresar resultados" << endl;
        cout << "3. Generar reporte" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione para continuar: ";cin>>opcion;

        switch (opcion)
        {
        case 1:
            ingresarEquipos();
            break;
        case 2:
            ingresarResultados();
            break;
        case 3:
            generarReporte();
            break;
        case 4:
            cout << "Salida exitosa" << endl;
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;
        }

    } while (opcion!=4);
    return 0;
}

//==============================================================================================================================================================

void ingresarEquipos()
{
    ofstream archivoEquipos("EquiposLPFB.bin", ios::binary | ios::app);

    if (!archivoEquipos)
    {
        cout<<"Error al abrir el archivo EquiposLPFB.bin"<<endl;
        return;
    }
    structEquiposLPFB equipoNuevo;
    cout<<"Ingrese el nombre del equipo: ";
    cin.ignore();
    cin.getline(equipoNuevo.nombreEquipo, 50);

    ifstream verificarEquipo("EquiposLPFB.bin", ios::binary);
    structEquiposLPFB equipoExistente;
    bool equipoDuplicado=false;

    // Verificar duplicados
    while (verificarEquipo.read(reinterpret_cast<char *>(&equipoExistente), sizeof(structEquiposLPFB)))
    {
        if (strcmp(equipoExistente.nombreEquipo, equipoNuevo.nombreEquipo)==0)
        {
            equipoDuplicado=true;
            break;
        }
    }
    verificarEquipo.close();
    if (equipoDuplicado)
    {
        cout<<"El equipo ya existe" << endl;
        return;
    }
    cout<<"Ingrese el color de la camiseta: ";
    cin.getline(equipoNuevo.colorCamiseta, 50);

    cout<<"Ingrese el departamento: ";
    cin.getline(equipoNuevo.departamento, 30);

    cout<<"Ingrese el anio de creacion: ";
    cin>>equipoNuevo.anioCreacion;

    // Inicializar puntos a 0
    equipoNuevo.puntos=0;
    archivoEquipos.write(reinterpret_cast<char *>(&equipoNuevo), sizeof(structEquiposLPFB));
    cout<<"Equipo ingresado exitosamente."<<endl;
    archivoEquipos.close();
}

//==============================================================================================================================================================

bool compararEquipos(const structEquiposLPFB &equipo1, const structEquiposLPFB &equipo2)
{
    // Comparar por puntos (mayor a menor)
    if (equipo1.puntos != equipo2.puntos)
        return (equipo1.puntos > equipo2.puntos);
    else
    {
        // Si los puntos son iguales, comparar por diferencia de goles (mayor a menor)
        int difGolesEquipo1=calcularDiferenciaGoles(equipo1);
        int difGolesEquipo2=calcularDiferenciaGoles(equipo2);
        return (difGolesEquipo1>difGolesEquipo2);
    }
}

//==============================================================================================================================================================

int calcularDiferenciaGoles(const structEquiposLPFB &equipo)
{
    // Leer resultados
    ifstream archivoResultados("ResultadosLPFB.bin", ios::binary);
    structResultadosLPFB resultado;
    int golesFavor=0, golesContra=0;

    // Calcular estadísticas
    while (archivoResultados.read(reinterpret_cast<char *>(&resultado), sizeof(structResultadosLPFB)))
    {
        if (strcmp(resultado.nombreEquipoLocal, equipo.nombreEquipo)==0 ||
            strcmp(resultado.nombreEquipoVisitante, equipo.nombreEquipo)==0)
        {
            if (strcmp(resultado.nombreEquipoLocal, equipo.nombreEquipo)==0)
            {
                golesFavor+=resultado.golesEquipoLocal;
                golesContra+=resultado.golesEquipoVisitante;
            }
            else if (strcmp(resultado.nombreEquipoVisitante, equipo.nombreEquipo)==0)
            {
                golesFavor+=resultado.golesEquipoVisitante;
                golesContra+=resultado.golesEquipoLocal;
            }
        }
    }

    archivoResultados.close();
    return golesFavor-golesContra;
}

//==============================================================================================================================================================

void generarReporte()
{
    ifstream archivoEquipos("EquiposLPFB.bin", ios::binary);

    if (!archivoEquipos)
    {
        cout<<"No hay equipos registrados"<<endl;
        return;
    }

    structEquiposLPFB equipo;
    vector<structEquiposLPFB> equipos;

    // Leer equipos
    while (archivoEquipos.read(reinterpret_cast<char *>(&equipo), sizeof(structEquiposLPFB)))
    {
        equipos.push_back(equipo);
    }

    archivoEquipos.close();

    ifstream archivoResultados("ResultadosLPFB.bin", ios::binary);

    if (!archivoResultados)
    {
        cout<<"No hay resultados registrados" << endl;
        return;
    }

    structResultadosLPFB resultado;
    vector<structResultadosLPFB> resultados;

    // Leer resultados
    while (archivoResultados.read(reinterpret_cast<char *>(&resultado), sizeof(structResultadosLPFB)))
    {
        resultados.push_back(resultado);
    }

    archivoResultados.close();

    // Calcular puntos y llenar el vector de equipos
    for (auto &equipo : equipos)
    {
        int partidosJugados = 0, partidosGanados = 0, partidosEmpatados = 0, partidosPerdidos = 0;
        int golesFavor = 0, golesContra = 0;

        // Calcular estadísticas
        for (const auto &res : resultados)
        {
            if (strcmp(res.nombreEquipoLocal, equipo.nombreEquipo) == 0 ||
                strcmp(res.nombreEquipoVisitante, equipo.nombreEquipo) == 0)
            {
                partidosJugados++;

                if (strcmp(res.nombreEquipoLocal, equipo.nombreEquipo) == 0)
                {
                    golesFavor += res.golesEquipoLocal;
                    golesContra += res.golesEquipoVisitante;

                    if (res.golesEquipoLocal > res.golesEquipoVisitante)
                        partidosGanados++;
                    else if (res.golesEquipoLocal == res.golesEquipoVisitante)
                        partidosEmpatados++;
                    else
                        partidosPerdidos++;
                }
                else if (strcmp(res.nombreEquipoVisitante, equipo.nombreEquipo) == 0)
                {
                    golesFavor += res.golesEquipoVisitante;
                    golesContra += res.golesEquipoLocal;

                    if (res.golesEquipoVisitante > res.golesEquipoLocal)
                        partidosGanados++;
                    else if (res.golesEquipoVisitante == res.golesEquipoLocal)
                        partidosEmpatados++;
                    else
                        partidosPerdidos++;
                }
            }
        }

        int diferenciaGol = golesFavor - golesContra;
        int puntos = 3 * partidosGanados + partidosEmpatados;

        equipo.puntos = puntos;
    }

    // Ordenar el vector de equipos por puntos y acomodarlos de manera ordenada
    sort(equipos.begin(), equipos.end(), compararEquipos);
    cout<<setw(3)<<"  # | "<<setw(20)<<"Nombre equipo"
         <<setw(5)<<"PJ"<<setw(5)<<"G"<<setw(5)<<"E"<<setw(5)<<"P"
         <<setw(5)<<"GF"<<setw(5)<<"GC"<<setw(5)<<"DG"<<setw(5)<<"Pts"<<endl;
    int posicion = 1;

    for (const auto &equipo : equipos)
    {
        int partidosJugados = 0, partidosGanados = 0, partidosEmpatados = 0, partidosPerdidos = 0;
        int golesFavor = 0, golesContra = 0;

        // Calcular estadísticas
        for (const auto &res : resultados)
        {
            if (strcmp(res.nombreEquipoLocal, equipo.nombreEquipo) == 0 ||
                strcmp(res.nombreEquipoVisitante, equipo.nombreEquipo) == 0)
            {
                partidosJugados++;

                if (strcmp(res.nombreEquipoLocal, equipo.nombreEquipo) == 0)
                {
                    golesFavor += res.golesEquipoLocal;
                    golesContra += res.golesEquipoVisitante;

                    if (res.golesEquipoLocal > res.golesEquipoVisitante)
                        partidosGanados++;
                    else if (res.golesEquipoLocal == res.golesEquipoVisitante)
                        partidosEmpatados++;
                    else
                        partidosPerdidos++;
                }
                else if (strcmp(res.nombreEquipoVisitante, equipo.nombreEquipo) == 0)
                {
                    golesFavor += res.golesEquipoVisitante;
                    golesContra += res.golesEquipoLocal;

                    if (res.golesEquipoVisitante > res.golesEquipoLocal)
                        partidosGanados++;
                    else if (res.golesEquipoVisitante == res.golesEquipoLocal)
                        partidosEmpatados++;
                    else
                        partidosPerdidos++;
                }
            }
        }

        int diferenciaGol = golesFavor - golesContra;
        int puntos = 3 * partidosGanados + partidosEmpatados;

        cout <<setw(3)<<posicion<<" | "<<setw(20)<<equipo.nombreEquipo
             << setw(5)<<partidosJugados << setw(5)<<partidosGanados<<setw(5)<<partidosEmpatados<<setw(5)<<partidosPerdidos
             << setw(5)<<golesFavor<<setw(5)<<golesContra<<setw(5)<<diferenciaGol<<setw(5)<<puntos<<endl;
        posicion++;
    }
}

void ingresarResultados()
{
    ofstream archivoResultados("ResultadosLPFB.bin", ios::binary | ios::app);

    if (!archivoResultados)
    {
        cout<<"Error al abrir el archivo ResultadosLPFB.bin"<<endl;
        return;
    }

    structResultadosLPFB resultado;

    cout<<"Ingrese el nombre del equipo local: ";
    cin.ignore();
    cin.getline(resultado.nombreEquipoLocal, 50);
    ifstream verificarLocal("EquiposLPFB.bin", ios::binary);
    bool localExistente = false;
    structEquiposLPFB equipoLocal;

    // Verificar si el equipo local existe
    while (verificarLocal.read(reinterpret_cast<char *>(&equipoLocal), sizeof(structEquiposLPFB)))
    {
        if (strcmp(equipoLocal.nombreEquipo, resultado.nombreEquipoLocal) == 0)
        {
            localExistente=true;
            break;
        }
    }
    verificarLocal.close();
    if (!localExistente)
    {
        cout << "El equipo local no existe. Ingrese equipos que estén registrados." << endl;
        return;
    }

    cout << "Ingrese el nombre del equipo visitante: ";
    cin.getline(resultado.nombreEquipoVisitante, 50);

    ifstream verificarVisitante("EquiposLPFB.bin", ios::binary);
    bool visitanteExistente = false;
    structEquiposLPFB equipoVisitante;

// Verificar si el equipo visitante existe
    while (verificarVisitante.read(reinterpret_cast<char *>(&equipoVisitante), sizeof(structEquiposLPFB)))
    {
        if (strcmp(equipoVisitante.nombreEquipo, resultado.nombreEquipoVisitante) == 0)
        {
            visitanteExistente = true;
            break;
        }
    }


    verificarVisitante.close();

    if (!visitanteExistente)
    {
        cout<<"El equipo visitante no existe. Ingrese equipos que estén registrados."<<endl;
        return;
    }
    cout<<"Ingrese la cantidad de goles del equipo local: ";
    cin>>resultado.golesEquipoLocal;

    cout<<"Ingrese la cantidad de goles del equipo visitante: ";
    cin>>resultado.golesEquipoVisitante;

    cout<<"Ingrese la fecha del partido (DD/MM/AAAA): ";
    cin.ignore(); // Limpiar el búfer de entrada
    cin.getline(resultado.fechaPartido, 11);

    // Escribir el resultado en el archivo
    archivoResultados.write(reinterpret_cast<char *>(&resultado), sizeof(structResultadosLPFB));

    cout<<"Resultado ingresado exitosamente."<<endl;
    archivoResultados.close();
}
