/**
 * @file bindings.cpp
 * @brief Bindings Python pour la bibliothèque GeoMath C++
 * 
 * Ce fichier crée une interface Python pour les classes Vector et Matrix
 * implémentées en C++, permettant leur utilisation depuis Python avec
 * des performances natives C++.
 * 
 * Utilise pybind11 pour :
 * - Exposer les classes C++ à Python
 * - Gérer automatiquement la conversion des types
 * - Créer une représentation Python naturelle des objets
 * 
 * @note Nécessite pybind11 >= 2.6
 * 
 * @author KRIBET Naoufal 
 * @date 2025
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>  // Pour exposer les opérateurs (+, -, *, /)
#include <pybind11/stl.h>        // Pour conversion automatique std::vector, etc.
#include <geomath/Vector.hpp>
#include <sstream>                // Pour std::stringstream (formatage de chaînes)

/// Alias pour simplifier l'utilisation de pybind11
namespace py = pybind11;

/**
 * @brief Point d'entrée du module Python
 * @param m Module pybind11 à configurer
 * 
 * Cette macro crée le module Python 'geomath_py' qui sera importable
 * depuis Python avec : import geomath_py
 * 
 * Le premier argument (geomath_py) DOIT correspondre au nom du fichier
 * binaire généré par CMake.
 */
PYBIND11_MODULE(geomath_py, m) {
    // ========================================================================
    // DOCUMENTATION DU MODULE
    // ========================================================================
    
    /**
     * Chaîne de documentation du module, visible dans Python via :
     * >>> import geomath_py
     * >>> help(geomath_py)
     */
    m.doc() = "Bindings Python pour la bibliothèque C++ GeoMath\n\n"
              "Ce module expose les classes Vector et Matrix implémentées en C++\n"
              "avec des performances optimales pour les calculs géométriques.";
    
    // ========================================================================
    // BINDING DE LA CLASSE Vector3d (Vector<double, 3>)
    // ========================================================================
    
    /**
     * Crée une classe Python "Vector3d" correspondant à Vector<double, 3>
     * 
     * @tparam geomath::Vector<double, 3> Type C++ à exposer
     * @param m Module parent
     * @param "Vector3d" Nom de la classe côté Python
     * 
     * Usage Python :
     * >>> v = geomath_py.Vector3d()
     */
    py::class_<geomath::Vector<double, 3>>(m, "Vector3d")
        
        // ====================================================================
        // CONSTRUCTEURS
        // ====================================================================
        
        /**
         * @brief Expose le constructeur par défaut
         * 
         * Permet de créer un vecteur nul depuis Python :
         * >>> v = geomath_py.Vector3d()  # Vecteur [0, 0, 0]
         */
        .def(py::init<>(), 
             "Constructeur par défaut - crée un vecteur nul [0, 0, 0]")
        
        /**
         * @brief Expose le constructeur par liste d'initialisation
         * 
         * Grâce à pybind11, on peut passer une liste Python directement :
         * >>> v = geomath_py.Vector3d([1.0, 2.0, 3.0])
         * 
         * @note pybind11 convertit automatiquement list Python -> std::initializer_list
         */
        .def(py::init<std::initializer_list<double>>(),
             py::arg("values"),
             "Constructeur par liste de valeurs\n\n"
             "Args:\n"
             "    values (list): Liste de 3 nombres flottants\n\n"
             "Example:\n"
             "    >>> v = Vector3d([1.0, 2.0, 3.0])")
        
        // ====================================================================
        // REPRÉSENTATION TEXTUELLE
        // ====================================================================
        
        /**
         * @brief Définit la représentation en chaîne de caractères
         * 
         * __repr__ est appelé par Python lors de :
         * - print(v)
         * - repr(v)
         * - Affichage dans l'interpréteur interactif
         * 
         * Implémentation :
         * 1. Capture le vecteur par référence constante (pas de copie)
         * 2. Utilise std::stringstream pour formater
         * 3. Réutilise operator<< défini dans Vector.hpp
         * 4. Retourne une std::string convertie automatiquement en str Python
         * 
         * Résultat Python :
         * >>> v = Vector3d([1.0, 2.0, 3.0])
         * >>> print(v)
         * [1, 2, 3]
         */
        .def("__repr__",
            [](const geomath::Vector<double, 3> &v) {
                // Crée un flux de sortie en mémoire pour le formatage
                std::stringstream ss;
                
                // Utilise l'opérateur << surchargé dans Vector.hpp
                // qui formate le vecteur comme : [x, y, z]
                ss << v;
                
                // Convertit le contenu du flux en std::string
                // pybind11 le convertira automatiquement en str Python
                return ss.str();
            },
            "Retourne une représentation en chaîne du vecteur")
        
        // ====================================================================
        // ACCÈS AUX ÉLÉMENTS
        // ====================================================================
        
        /**
         * @brief Expose operator[] pour l'accès indexé
         * 
         * Permet l'accès Python naturel :
         * >>> v = Vector3d([1.0, 2.0, 3.0])
         * >>> print(v[0])  # Affiche : 1.0
         * >>> v[1] = 5.0   # Modification
         * 
         * @note La version non-const permet lecture ET écriture
         * @note pybind11 gère automatiquement les exceptions Python
         *       si l'index est hors limites (grâce à assert)
         */
        .def("__getitem__",
            [](const geomath::Vector<double, 3> &v, size_t index) {
                if (index >= 3) {
                    throw py::index_error("Index hors limites (doit être 0, 1 ou 2)");
                }
                return v[index];
            },
            py::arg("index"),
            "Accède à un élément du vecteur par index\n\n"
            "Args:\n"
            "    index (int): Index de l'élément (0, 1 ou 2)\n\n"
            "Returns:\n"
            "    float: Valeur de l'élément\n\n"
            "Raises:\n"
            "    IndexError: Si l'index est hors limites")
        
        .def("__setitem__",
            [](geomath::Vector<double, 3> &v, size_t index, double value) {
                if (index >= 3) {
                    throw py::index_error("Index hors limites (doit être 0, 1 ou 2)");
                }
                v[index] = value;
            },
            py::arg("index"),
            py::arg("value"),
            "Modifie un élément du vecteur par index\n\n"
            "Args:\n"
            "    index (int): Index de l'élément (0, 1 ou 2)\n"
            "    value (float): Nouvelle valeur")
        
        // ====================================================================
        // PROPRIÉTÉS ET MÉTHODES UTILITAIRES
        // ====================================================================
        
        /**
         * @brief Expose la propriété "size" (lecture seule)
         * 
         * Usage Python :
         * >>> v = Vector3d()
         * >>> print(len(v))  # Grâce à __len__
         * 3
         */
        .def("__len__",
            [](const geomath::Vector<double, 3> &v) {
                return v.size();
            },
            "Retourne la taille du vecteur (toujours 3)")
        
        // ====================================================================
        // OPÉRATEURS ARITHMÉTIQUES
        // ====================================================================
        
        /**
         * @brief Expose les opérateurs binaires (+, -, *, /)
         * 
         * py::self est un placeholder pour le type courant
         * 
         * Permet en Python :
         * >>> v1 = Vector3d([1, 2, 3])
         * >>> v2 = Vector3d([4, 5, 6])
         * >>> v3 = v1 + v2        # Addition
         * >>> v4 = v1 - v2        # Soustraction
         * >>> v5 = v1 * 2.0       # Multiplication scalaire
         * >>> v6 = v1 / 2.0       # Division scalaire
         */
        .def(py::self + py::self,
             "Addition de deux vecteurs (élément par élément)")
        .def(py::self - py::self,
             "Soustraction de deux vecteurs (élément par élément)")
        .def(py::self * double(),
             "Multiplication par un scalaire")
        .def(py::self / double(),
             "Division par un scalaire")
        
        /**
         * @brief Multiplication scalaire-vecteur (ordre inversé)
         * 
         * Permet : 2.0 * v (en plus de v * 2.0)
         */
        .def(double() * py::self,
             "Multiplication scalaire-vecteur (ordre inversé)")
        
        // ====================================================================
        // MÉTHODES MATHÉMATIQUES
        // ====================================================================
        
        /**
         * @brief Expose la méthode norm()
         * 
         * Usage Python :
         * >>> v = Vector3d([3, 4, 0])
         * >>> print(v.norm())  # Affiche : 5.0
         */
        .def("norm",
            &geomath::Vector<double, 3>::norm,
            "Calcule la norme euclidienne du vecteur\n\n"
            "Returns:\n"
            "    float: ||v|| = sqrt(x² + y² + z²)")
        
        /**
         * @brief Méthode pour normaliser le vecteur
         * 
         * Crée un nouveau vecteur de norme 1 dans la même direction
         * 
         * Usage Python :
         * >>> v = Vector3d([3, 4, 0])
         * >>> v_norm = v.normalized()
         * >>> print(v_norm.norm())  # Affiche : 1.0
         */
        .def("normalized",
            [](const geomath::Vector<double, 3> &v) {
                double n = v.norm();
                if (n < 1e-10) {
                    throw std::runtime_error("Impossible de normaliser un vecteur nul");
                }
                return v / n;
            },
            "Retourne une version normalisée du vecteur (norme = 1)\n\n"
            "Returns:\n"
            "    Vector3d: Vecteur de norme 1 dans la même direction\n\n"
            "Raises:\n"
            "    RuntimeError: Si le vecteur est nul")
        
        /**
         * @brief Méthode pour le carré de la norme (optimisation)
         * 
         * Plus rapide que norm() car évite le sqrt()
         * Utile pour les comparaisons de distances
         */
        .def("norm_squared",
            [](const geomath::Vector<double, 3> &v) {
                return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
            },
            "Calcule le carré de la norme (plus rapide que norm())\n\n"
            "Returns:\n"
            "    float: ||v||² = x² + y² + z²");
    
    // ========================================================================
    // FONCTIONS LIBRES (MODULE-LEVEL FUNCTIONS)
    // ========================================================================
    
    /**
     * @brief Expose la fonction dotProduct() comme fonction du module
     * 
     * Usage Python :
     * >>> v1 = Vector3d([1, 2, 3])
     * >>> v2 = Vector3d([4, 5, 6])
     * >>> dot = geomath_py.dot(v1, v2)
     * >>> print(dot)  # Affiche : 32.0 (1*4 + 2*5 + 3*6)
     */
    m.def("dot",
        &geomath::dotProduct<double, 3>,
        py::arg("v1"),
        py::arg("v2"),
        "Calcule le produit scalaire de deux vecteurs\n\n"
        "Args:\n"
        "    v1 (Vector3d): Premier vecteur\n"
        "    v2 (Vector3d): Deuxième vecteur\n\n"
        "Returns:\n"
        "    float: v1 · v2 = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]\n\n"
        "Example:\n"
        "    >>> dot(Vector3d([1,0,0]), Vector3d([0,1,0]))  # Retourne 0 (orthogonaux)");
    
    /**
     * @brief Fonction pour calculer l'angle entre deux vecteurs
     * 
     * Formule : θ = arccos((v1 · v2) / (||v1|| * ||v2||))
     * 
     * Usage Python :
     * >>> v1 = Vector3d([1, 0, 0])
     * >>> v2 = Vector3d([0, 1, 0])
     * >>> angle = geomath_py.angle_between(v1, v2)
     * >>> print(angle)  # Affiche : 1.5707... (π/2 radians = 90°)
     */
    m.def("angle_between",
        [](const geomath::Vector<double, 3>& v1, 
           const geomath::Vector<double, 3>& v2) {
            double dot = geomath::dotProduct(v1, v2);
            double norm_product = v1.norm() * v2.norm();
            
            if (norm_product < 1e-10) {
                throw std::runtime_error("Impossible de calculer l'angle avec un vecteur nul");
            }
            
            // Clamp pour éviter les erreurs numériques avec acos
            double cos_angle = std::max(-1.0, std::min(1.0, dot / norm_product));
            return std::acos(cos_angle);
        },
        py::arg("v1"),
        py::arg("v2"),
        "Calcule l'angle entre deux vecteurs (en radians)\n\n"
        "Args:\n"
        "    v1 (Vector3d): Premier vecteur\n"
        "    v2 (Vector3d): Deuxième vecteur\n\n"
        "Returns:\n"
        "    float: Angle en radians [0, π]\n\n"
        "Raises:\n"
        "    RuntimeError: Si l'un des vecteurs est nul");
    
    /**
     * @brief Fonction pour calculer le produit vectoriel (cross product)
     * 
     * Uniquement défini en 3D : v1 × v2
     * Le résultat est orthogonal à v1 et v2
     * 
     * Usage Python :
     * >>> v1 = Vector3d([1, 0, 0])
     * >>> v2 = Vector3d([0, 1, 0])
     * >>> v3 = geomath_py.cross(v1, v2)
     * >>> print(v3)  # Affiche : [0, 0, 1]
     */
    m.def("cross",
        [](const geomath::Vector<double, 3>& v1,
           const geomath::Vector<double, 3>& v2) {
            geomath::Vector<double, 3> result;
            result[0] = v1[1] * v2[2] - v1[2] * v2[1];
            result[1] = v1[2] * v2[0] - v1[0] * v2[2];
            result[2] = v1[0] * v2[1] - v1[1] * v2[0];
            return result;
        },
        py::arg("v1"),
        py::arg("v2"),
        "Calcule le produit vectoriel de deux vecteurs 3D\n\n"
        "Le vecteur résultant est orthogonal à v1 et v2.\n"
        "Sa norme est ||v1|| * ||v2|| * sin(θ)\n\n"
        "Args:\n"
        "    v1 (Vector3d): Premier vecteur\n"
        "    v2 (Vector3d): Deuxième vecteur\n\n"
        "Returns:\n"
        "    Vector3d: v1 × v2");
    
    // ========================================================================
    // INFORMATIONS DE VERSION
    // ========================================================================
    
    /**
     * @brief Expose la version du module
     * 
     * Accessible en Python via :
     * >>> import geomath_py
     * >>> print(geomath_py.__version__)
     */
    m.attr("__version__") = "1.0.0";
    
    /**
     * @brief Expose le nom de l'auteur ;)
     */
    m.attr("__author__") = "KRIBET Naoufal";
}