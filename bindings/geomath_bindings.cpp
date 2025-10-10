#include <pybind11/pybind11.h>
#include <geomath/Dummy.hpp> // On inclut la fonction à exposer

namespace py = pybind11;

// PYBIND11_MODULE(nom_du_module_en_python, handle_du_module)
PYBIND11_MODULE(geomath_py, m) {
    m.doc() = "Bindings Python pour la bibliothèque C++ GeoMatrix";

    // On expose notre fonction C++ 'get_hello_message' sous le nom 'hello' en Python
    m.def("hello", &geomath::get_hello_message, "Une fonction de test qui retourne un message");
}
