# ============================================================================
# @file CMakeLists.txt
# @brief Configuration CMake pour la bibliothèque GeoMatrix
#
# Ce fichier configure la compilation de la bibliothèque C++ header-only
# GeoMatrix, incluant :
# - La bibliothèque principale (interface)
# - Les tests unitaires avec Google Test
# - Les bindings Python avec pybind11
#
# @author Kribet Naoufal
# @date 2025
# ============================================================================

# ============================================================================
# Configuration Générale du Projet
# ============================================================================
# Version minimale de CMake requise pour construire le projet
cmake_minimum_required(VERSION 3.14)

# Déclaration du projet GeoMatrix avec support des langages C++ et C
project(GeoMatrix LANGUAGES CXX C)

# Configuration du standard C++17 pour garantir la compatibilité
# et l'utilisation de fonctionnalités modernes
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Gestion des Dépendances avec FetchContent
# ============================================================================
# Module CMake permettant de télécharger et intégrer automatiquement
# des dépendances externes au moment de la configuration
include(FetchContent)

# Configuration de Google Test pour utiliser le même runtime que le projet
# Évite les conflits de linkage entre debug/release sur Windows (MSVC)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Déclaration et téléchargement de Google Test v1.14.0
# Framework de tests unitaires pour C++
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Déclaration et téléchargement de pybind11 v2.12.0
# Bibliothèque pour créer des bindings Python/C++
FetchContent_Declare(
    pybind11
    URL https://github.com/pybind/pybind11/archive/refs/tags/v2.12.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Rend les dépendances disponibles pour le reste du projet
FetchContent_MakeAvailable(googletest pybind11)

# ============================================================================
# Définition de la Bibliothèque Header-Only
# ============================================================================
# Création d'une bibliothèque interface (header-only)
# Aucun fichier source à compiler, uniquement des headers
add_library(geomath INTERFACE)

# Ajout du répertoire include aux chemins de recherche d'en-têtes
# INTERFACE : les cibles qui dépendent de geomath hériteront de ce chemin
target_include_directories(geomath INTERFACE include)

# ============================================================================
# Configuration des Tests Unitaires
# ============================================================================
# Création de l'exécutable de test à partir des fichiers de test
# Contient tous les tests unitaires de la bibliothèque
add_executable(geomath_tests tests/test_vector.cpp)

# Linkage avec la bibliothèque geomath et Google Test
# gtest_main fournit une fonction main() automatique pour les tests
target_link_libraries(geomath_tests PRIVATE geomath GTest::gtest_main)

# Activation du système de tests intégré CTest
enable_testing()

# Enregistrement des tests pour exécution via CTest
# Permet de lancer les tests avec 'ctest' ou 'make test'
add_test(NAME GeomathTests COMMAND geomath_tests)

# ============================================================================
# Configuration des Bindings Python
# ============================================================================
# Création du module Python à partir des bindings C++
# Génère un fichier .so (Linux/Mac) ou .pyd (Windows)
pybind11_add_module(geomath_py bindings/geomath_bindings.cpp)

# Linkage du module Python avec la bibliothèque geomath
target_link_libraries(geomath_py PRIVATE geomath)

# Définition du répertoire de sortie personnalisé pour le module Python
# Place le module dans un dossier dédié à la racine du projet
set(PYTHON_MODULE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/geomath_py)

# Création automatique du dossier de sortie après compilation
# Exécuté après la construction de la cible geomath_py
add_custom_command(
    TARGET geomath_py POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PYTHON_MODULE_OUTPUT_DIR}
)

# Copie automatique du module compilé vers le dossier de sortie Python
# Permet d'importer directement le module depuis geomath_py/
add_custom_command(
    TARGET geomath_py POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:geomath_py>
            ${PYTHON_MODULE_OUTPUT_DIR}
)