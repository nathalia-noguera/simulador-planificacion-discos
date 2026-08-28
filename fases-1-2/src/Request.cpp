#include "../include/Request.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

std::vector<Request> generateRandomRequests(int cantidad, int totalCilindros){
    std::vector<Request> requests;
    Request r;
    int i;
    // Semilla para que los números aleatorios cambien cada vez que se ejecuta el programa.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(i = 0; i < cantidad; i++){
        r.id = i + 1;
        r.cylinder = std::rand() % totalCilindros; // Número entre 0 y totalCilindros - 1.
        requests.push_back(r); // Agrega r al final del vector y le suma 1 a su tamaño.
    }
    return requests;
}

std::vector<Request> loadRequestsFromFile(const std::string& fileName, int totalCilindros){
    std::vector<Request> requests;
    std::ifstream file(fileName);
    Request r;
    int cylinder, contador;
    if(!file.is_open()){
        std::cerr << "[ERROR] No se pudo abrir el archivo: " << fileName << "\n";
        return requests; // Se devuelve la lista vacía.
    }
    contador = 1;
    while(file >> cylinder){ // Lee el dato del archivo y lo guarda en cylinder.
        // Se verifica que el cilindro leído esté dentro del rango del disco.
        if(cylinder >= 0 && cylinder < totalCilindros){
            r.id = contador;
            r.cylinder = cylinder;
            requests.push_back(r);
            contador++;
        }else{
            std::cout << "[ADVERTENCIA] Se ignoró el cilindro " << cylinder << " porque está fuera del rango (0 - " << (totalCilindros - 1) << ").\n";
        }
    }
    file.close();
    return requests;
}

std::vector<Request> inputRequestsManually(int totalCilindros){
    std::vector<Request> requests;
    Request r;
    int i, cantidad, cylinder;
    std::cout << "¿Cuántas solicitudes desea ingresar? ";
    std::cin >> cantidad;
    for(i = 0; i < cantidad; i++){
        std::cout << "Ingrese el cilindro de la solicitud " << (i + 1) << " (0 - " << (totalCilindros - 1) << "): ";
        std::cin >> cylinder;
        // Se verifica que el cilindro ingresado esté dentro del rango del disco.
        if(cylinder >= 0 && cylinder < totalCilindros){
            r.id = i + 1;
            r.cylinder = cylinder;
            requests.push_back(r);
        }else{
            std::cout << "[ADVERTENCIA] Cilindro fuera de rango, no se agregó.\n";
            i--; // Se repite esta posición para pedir un valor válido.
        }
    }
    return requests;
}

void printRequests(const std::vector<Request>& requests){
    std::cout << "----------------------------------------\n"
              << "        LISTA DE SOLICITUDES            \n"
              << "----------------------------------------\n";
    if(requests.empty()){
        std::cout << "No hay solicitudes cargadas.\n";
    }else{
        for(const Request& r : requests){ // Se recorre elemento a elemento.
            std::cout << "Solicitud #" << r.id << " -> Cilindro " << r.cylinder << "\n";
        }
    }
    std::cout << "----------------------------------------\n";
}