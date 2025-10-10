#include <pybind11/pybind11.h>
#include <geomath/Vector.hpp>

// --- INCLUSION MANQUANTE À AJOUTER ---
#include <sstream> // Pour std::stringstream

namespace py = pybind11;

PYBIND11_MODULE(geomath_py, m) {
    m.doc() = "Bindings Python pour la bibliothèque C++ GeoMatrix";

    py::class_<geomath::Vector<double, 3>>(m, "Vector3d")
        .def(py::init<>())
        .def("__repr__",
            // Cette lambda capture une référence vers notre vecteur
            [](const geomath::Vector<double, 3> &v) {
                // 'ss' est maintenant un type connu
                std::stringstream ss;
                // Le compilateur trouvera maintenant geomath::operator<<
                ss << v; 
                return ss.str();
            }
        );
}