#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include "Disk.hpp"
#include "Request.hpp"

// Función principal para First-Come, First-Served
void ejecutarFCFS(Disk disco, std::vector<Request> solicitudes);

#endif