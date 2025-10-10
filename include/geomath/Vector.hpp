#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <initializer_list>
#include <cassert>
#include <cmath>

namespace geomath {

template<typename T, size_t N>
class Vector {
private:
    std::array<T, N> data_;

public:
    // ===== CONSTRUCTEURS =====
    
    // Constructeur par défaut : crée un vecteur nul
    Vector() : data_{} {}
    
    // Constructeur par liste d'initialisation
    Vector(std::initializer_list<T> list) {
        assert(list.size() == N && "La taille de la liste d'initialisation doit correspondre à N");
        std::copy(list.begin(), list.end(), data_.begin());
    }
    
    // ===== ACCÈS AUX ÉLÉMENTS =====
    
    // Version non-const : permet la modification
    T& operator[](size_t index) {
        assert(index < N && "Index hors limites");
        return data_[index];
    }
    
    // Version const : permet la lecture seule
    const T& operator[](size_t index) const {
        assert(index < N && "Index hors limites");
        return data_[index];
    }
    
    // ===== OPÉRATEURS ARITHMÉTIQUES =====
    
    // Addition de deux vecteurs
    Vector<T, N> operator+(const Vector<T, N>& autre) const {
        Vector<T, N> resultat;
        for (size_t i = 0; i < N; ++i) {
            resultat[i] = data_[i] + autre.data_[i];
        }
        return resultat;
    }
    
    // Soustraction de deux vecteurs
    Vector<T, N> operator-(const Vector<T, N>& autre) const {
        Vector<T, N> resultat;
        for (size_t i = 0; i < N; ++i) {
            resultat[i] = data_[i] - autre.data_[i];
        }
        return resultat;
    }
    
    // Multiplication par un scalaire
    Vector<T, N> operator*(T scalaire) const {
        Vector<T, N> resultat;
        for (size_t i = 0; i < N; ++i) {
            resultat[i] = data_[i] * scalaire;
        }
        return resultat;
    }
    
    // Division par un scalaire
    Vector<T, N> operator/(T scalaire) const {
        Vector<T, N> resultat;
        for (size_t i = 0; i < N; ++i) {
            resultat[i] = data_[i] / scalaire;
        }
        return resultat;
    }
    
    // ===== FONCTIONS MATHÉMATIQUES =====
    
    // Norme euclidienne du vecteur
    double norm() const {
        T somme_carres = T{};
        for (size_t i = 0; i < N; ++i) {
            somme_carres += data_[i] * data_[i];
        }
        return std::sqrt(static_cast<double>(somme_carres));
    }
    
    // Taille du vecteur (utile pour les itérations)
    constexpr size_t size() const {
        return N;
    }
};

// ===== FONCTIONS LIBRES =====

// Produit scalaire entre deux vecteurs
template<typename T, size_t N>
T dotProduct(const Vector<T, N>& v1, const Vector<T, N>& v2) {
    T resultat = T{};
    for (size_t i = 0; i < N; ++i) {
        resultat += v1[i] * v2[i];
    }
    return resultat;
}

// Opérateur de sortie pour affichage
template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v) {
    os << "[";
    for (size_t i = 0; i < N; ++i) {
        os << v[i];
        if (i < N - 1) os << ", ";
    }
    os << "]";
    return os;
}

} // namespace geomath