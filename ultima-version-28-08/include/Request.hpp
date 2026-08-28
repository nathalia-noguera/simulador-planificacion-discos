#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <vector>
#include <string>

// Solicitud de lectura/escritura al disco.
struct Request{
    int id; // Identificador de la solicitud.
    int cylinder; // Número de cilindro que se pide.
    int track; // Pista dentro del cilindro (superficie/cabezal que se usa).
};

// Generar una determinada cantidad de solicitudes con cilindros aleatorios entre 0 y totalCilindros - 1.
std::vector<Request> generateRandomRequests(int cantidad, int totalCilindros, int totalPistas = 4);

// Leer solicitudes desde un archivo de texto.
std::vector<Request> loadRequestsFromFile(const std::string& fileName, int totalCilindros, int totalPistas = 4);

// Escribir las solicitudes desde el teclado.
std::vector<Request> inputRequestsManually(int totalCilindros, int totalPistas = 4);

// Mostrar la lista de solicitudes.
void printRequests(const std::vector<Request>& requests);

#endif