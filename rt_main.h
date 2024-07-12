#ifndef RT_MAIN_H
#define RT_MAIN_H
// Header principal do programa Ray Tracing

#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

using std::fabs;
using std::cout;
using std::endl;
using std::ofstream;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

//Constantes
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Funções
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Retorna um número real aleatório entre 0 e 1
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    //Retorna um número real aleatório entre min e max
    return min + (max-min)*random_double();
}

//Headers comuns ao programa
#include "ray.h"
#include "color.h"
#include "vec3.h"
#include "interval.h"

#endif