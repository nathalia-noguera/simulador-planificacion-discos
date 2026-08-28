#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include "Disk.hpp"
#include "Request.hpp"

// Función principal para First-Come, First-Served
int ejecutarFCFS(Disk disco, std::vector<Request> solicitudes);

// Función principal para Shortest Seek Time First (SSTF)
int ejecutarSSTF(Disk disco, std::vector<Request> solicitudes);

// Función principal para el algoritmo del ascensor (SCAN)
int ejecutarSCAN(Disk disco, std::vector<Request> solicitudes, int direccion);

// Función principal para Circular SCAN (C-SCAN)
int ejecutarCSCAN(Disk disco, std::vector<Request> solicitudes, int direccion);

#endif