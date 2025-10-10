# --- 1. Configuration Générale du Projet ---
cmake_minimum_required(VERSION 3.14)
project(GeoMatrix LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- 2. Gestion des Dépendances avec FetchContent ---
include(FetchContent)

# Forcer Google Test à utiliser le même runtime que le projet
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    pybind11
    URL https://github.com/pybind/pybind11/archive/refs/tags/v2.12.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest pybind11)

# --- 3. Définition de notre Bibliothèque "header-only" ---
add_library(geomath INTERFACE)
target_include_directories(geomath INTERFACE include)

# --- 4. Configuration des Tests Unitaires ---
# Créer l'exécutable de test avec un nom différent
add_executable(geomath_tests tests/test_vector.cpp)
target_link_libraries(geomath_tests PRIVATE geomath GTest::gtest_main)

# Ajouter les tests CTest (optionnel, peut être désactivé)
enable_testing()
add_test(NAME GeomathTests COMMAND geomath_tests)

# --- 5. Configuration des Bindings Python ---
pybind11_add_module(geomath_py bindings/geomath_bindings.cpp)
target_link_libraries(geomath_py PRIVATE geomath)