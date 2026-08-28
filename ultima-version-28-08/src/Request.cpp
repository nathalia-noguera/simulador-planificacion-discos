#include "../include/Request.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>

// Genera una lista de solicitudes con cilindro y pista aleatorios.
std::vector<Request> generateRandomRequests(int cantidad, int totalCilindros, int totalPistas){
    std::vector<Request> requests;
    Request r;
    int i;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(i = 0; i < cantidad; i++){
        r.id = i + 1;
        r.cylinder = std::rand() % totalCilindros;
        r.track = std::rand() % totalPistas;
        requests.push_back(r);
    }
    return requests;
}

// Carga las solicitudes desde un archivo de texto, validando cilindro y pista.
std::vector<Request> loadRequestsFromFile(const std::string& fileName, int totalCilindros, int totalPistas){
    std::vector<Request> requests;
    std::ifstream file(fileName);
    Request r;
    int cylinder, track, contador;
    if(!file.is_open()){
        std::cerr << "\n  [ERROR] No se pudo abrir el archivo: " << fileName << "\n";
        return requests;
    }
    contador = 1;
    while(file >> cylinder >> track){
        if(cylinder >= 0 && cylinder < totalCilindros){
            if(track < 0 || track >= totalPistas){
                std::cout << "\n  [ADVERTENCIA] Pista " << track << " fuera de rango (0 - " << (totalPistas - 1) << ") para el cilindro " << cylinder << ". Se asignó la pista 0 por defecto.\n\n";
                track = 0;
            }
            r.id = contador;
            r.cylinder = cylinder;
            r.track = track;
            requests.push_back(r);
            contador++;
        }else{
            std::cout << "\n  [ADVERTENCIA] Se ignoró el cilindro " << cylinder << " porque está fuera del rango (0 - " << (totalCilindros - 1) << ").\n\n";
        }
    }
    file.close();
    return requests;
}

// Permite ingresar las solicitudes manualmente por teclado, validando los datos.
std::vector<Request> inputRequestsManually(int totalCilindros, int totalPistas){
    std::vector<Request> requests;
    Request r;
    int i, cantidad, cylinder, track;
    std::cout << "  ¿Cuántas solicitudes desea ingresar? ";
    std::cin >> cantidad;
    if(std::cin.fail() || cantidad < 0){
        std::cout << "\n  [ERROR] Debe ingresar un número válido.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return requests;
    }
    for(i = 0; i < cantidad; i++){
        std::cout << "  Ingrese el cilindro de la solicitud " << (i + 1) << " (0 - " << (totalCilindros - 1) << "): ";
        std::cin >> cylinder;
        if(std::cin.fail()){
            std::cout << "\n  [ERROR] Debe ingresar un número válido.\n\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            i--;
            continue;
        }
        if(cylinder >= 0 && cylinder < totalCilindros){
            std::cout << "  Ingrese la pista de la solicitud " << (i + 1) << " (0 - " << (totalPistas - 1) << "): ";
            std::cin >> track;
            if(std::cin.fail()){
                std::cout << "\n  [ERROR] Debe ingresar un número válido.\n\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                i--;
                continue;
            }
            if(track < 0 || track >= totalPistas){
                std::cout << "\n  [ADVERTENCIA] Pista fuera de rango, se asignó la pista 0 por defecto.\n\n";
                track = 0;
            }
            r.id = i + 1;
            r.cylinder = cylinder;
            r.track = track;
            requests.push_back(r);
        }else{
            std::cout << "\n  [ADVERTENCIA] Cilindro fuera de rango, no se agregó.\n\n";
            i--;
        }
    }
    return requests;
}

// Muestra por pantalla la lista de solicitudes cargadas.
void printRequests(const std::vector<Request>& requests){
    std::cout << "-------------------------------------------------------------------------------\n\n"
              << "  L i s t a  d e  s o l i c i t u d e s\n\n";
    if(requests.empty()){
        std::cout << "  No hay solicitudes cargadas.\n";
    }else{
        for(const Request& r : requests){
            std::cout << "  Solicitud #" << r.id << " -> Cilindro " << r.cylinder << ", Pista " << r.track << "\n";
        }
    }
    std::cout << "\n-------------------------------------------------------------------------------\n";
}