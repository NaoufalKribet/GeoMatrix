/**
 * @file Vector.hpp
 * @brief Implémentation d'une classe Vector générique avec allocation statique
 *
 * Cette classe template permet de manipuler des vecteurs de taille fixe
 * avec vérification des dimensions à la compilation. Toutes les données
 * sont allouées sur la pile pour des performances optimales.
 *
 * @author Kribet Naoufal
 * @date 2025
 */
#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <initializer_list>
#include <cassert>
#include <cmath>

namespace geomath {

/**
 * @class Vector
 * @brief Classe template représentant un vecteur mathématique de dimension fixe
 *
 * @tparam T Type des éléments du vecteur (int, float, double, etc.)
 * @tparam N Dimension du vecteur (taille fixe déterminée à la compilation)
 *
 * Cette classe fournit les opérations vectorielles courantes :
 * - Opérations arithmétiques (addition, soustraction, multiplication par scalaire)
 * - Calcul de norme euclidienne
 * - Produit scalaire
 * - Accès indexé aux composantes
 */
template<typename T, size_t N>
class Vector {
private:
    /**
     * @brief Conteneur interne stockant les composantes du vecteur
     *
     * Utilise std::array pour une allocation statique sur la pile,
     * garantissant des performances optimales et une taille connue à la compilation.
     */
    std::array<T, N> data_;

public:
    // ========================================================================
    // CONSTRUCTEURS
    // ========================================================================
   
    /**
     * @brief Constructeur par défaut
     *
     * Crée un vecteur nul avec toutes les composantes initialisées à zéro.
     * Utilise l'initialisation par défaut de std::array.
     */
    Vector() : data_{} {}
   
    /**
     * @brief Constructeur par liste d'initialisation
     *
     * Permet de construire un vecteur avec une syntaxe concise :
     * Vector<double, 3> v = {1.0, 2.0, 3.0};
     *
     * @param list Liste d'initialisation contenant les valeurs des composantes
     * @pre La taille de la liste doit être exactement égale à N
     * @throws assert si la taille ne correspond pas
     */
    Vector(std::initializer_list<T> list) {
        assert(list.size() == N && "La taille de la liste d'initialisation doit correspondre à N");
        std::copy(list.begin(), list.end(), data_.begin());
    }
   
    // ========================================================================
    // ACCÈS AUX ÉLÉMENTS
    // ========================================================================
   
    /**
     * @brief Opérateur d'accès par index (version modifiable)
     *
     * Permet de lire et modifier une composante du vecteur.
     *
     * @param index Position de la composante (0 <= index < N)
     * @return Référence modifiable vers l'élément à la position index
     * @throws assert si l'index est hors limites
     */
    T& operator[](size_t index) {
        assert(index < N && "Index hors limites");
        return data_[index];
    }
   
    /**
     * @brief Opérateur d'accès par index (version constante)
     *
     * Permet de lire une composante du vecteur sans pouvoir la modifier.
     * Utilisé automatiquement sur les objets const.
     *
     * @param index Position de la composante (0 <= index < N)
     * @return Référence constante vers l'élément à la position index
     * @throws assert si l'index est hors limites
     */
    const T& operator[](size_t index) const {
        assert(index < N && "Index hors limites");
        return data_[index];
    }
   
    // ========================================================================
    // OPÉRATEURS ARITHMÉTIQUES
    // ========================================================================
   
    /**
     * @brief Addition vectorielle
     *
     * Effectue l'addition composante par composante de deux vecteurs.
     * Opération : result[i] = this[i] + autre[i]
     *
     * @param autre Vecteur à ajouter
     * @return Nouveau vecteur résultant de l'addition
     */
    Vector<T, N> operator+(const Vector<T, N>& autre) const {
        Vector<T, N> resultat;
        for (size_t i = 0; i < N; ++i) {
            resultat[i] = data_[i] + autre.data_[i];
        }
        return resultat;
    }
   
    /**
     * @brief Soustraction vectorielle
     *
     * Effectue la soustraction composante par composante de deux vecteurs.
     * Opération : result[i] = this[i] - autre[i]
     *
     * @param autre Vecteur à soustraire
     * @return Nouveau vecteur résultant de la soustraction
     */
    Vector<T, N> operator-(const Vector<T, N>& autre) const {
        Vector<T, N> resultat;
        for (size_t i = 0; i < N; ++i) {
            resultat[i] = data_[i] - autre.data_[i];
        }
        return resultat;
    }
   
    /**
     * @brief Multiplication par un scalaire
     *
     * Multiplie chaque composante du vecteur par un scalaire.
     * Opération : result[i] = this[i] * scalaire
     *
     * @param scalaire Valeur scalaire multiplicatrice
     * @return Nouveau vecteur résultant de la multiplication
     */
    Vector<T, N> operator*(T scalaire) const {
        Vector<T, N> resultat;
        for (size_t i = 0; i < N; ++i) {
            resultat[i] = data_[i] * scalaire;
        }
        return resultat;
    }
   
    /**
     * @brief Division par un scalaire
     *
     * Divise chaque composante du vecteur par un scalaire.
     * Opération : result[i] = this[i] / scalaire
     *
     * @param scalaire Valeur scalaire diviseur (ne doit pas être zéro)
     * @return Nouveau vecteur résultant de la division
     * @warning Aucune vérification de division par zéro n'est effectuée
     */
    Vector<T, N> operator/(T scalaire) const {
        Vector<T, N> resultat;
        for (size_t i = 0; i < N; ++i) {
            resultat[i] = data_[i] / scalaire;
        }
        return resultat;
    }
   
    // ========================================================================
    // FONCTIONS MATHÉMATIQUES
    // ========================================================================
   
    /**
     * @brief Calcule la norme euclidienne du vecteur
     *
     * La norme euclidienne est définie comme :
     * ||v|| = sqrt(v[0]² + v[1]² + ... + v[N-1]²)
     *
     * @return Norme euclidienne du vecteur (valeur positive ou nulle)
     * @note Le résultat est toujours retourné en double pour plus de précision
     */
    double norm() const {
        T somme_carres = T{};
        for (size_t i = 0; i < N; ++i) {
            somme_carres += data_[i] * data_[i];
        }
        return std::sqrt(static_cast<double>(somme_carres));
    }
   
    /**
     * @brief Retourne la dimension du vecteur
     *
     * Fonction utilitaire pour obtenir la taille du vecteur,
     * particulièrement utile dans les contextes génériques.
     *
     * @return Dimension du vecteur (valeur de N)
     * @note Fonction constexpr : évaluée à la compilation
     */
    constexpr size_t size() const {
        return N;
    }
};

// ============================================================================
// FONCTIONS LIBRES (NON-MEMBRES)
// ============================================================================

/**
 * @brief Calcule le produit scalaire de deux vecteurs
 *
 * Le produit scalaire est défini comme :
 * v1 · v2 = v1[0]*v2[0] + v1[1]*v2[1] + ... + v1[N-1]*v2[N-1]
 *
 * @tparam T Type des éléments des vecteurs
 * @tparam N Dimension des vecteurs
 * @param v1 Premier vecteur
 * @param v2 Second vecteur
 * @return Résultat du produit scalaire (valeur de type T)
 *
 * @note Le produit scalaire est commutatif : v1·v2 = v2·v1
 * @note Deux vecteurs orthogonaux ont un produit scalaire nul
 */
template<typename T, size_t N>
T dotProduct(const Vector<T, N>& v1, const Vector<T, N>& v2) {
    T resultat = T{};
    for (size_t i = 0; i < N; ++i) {
        resultat += v1[i] * v2[i];
    }
    return resultat;
}

/**
 * @brief Opérateur d'affichage pour les vecteurs
 *
 * Permet d'afficher un vecteur dans un flux de sortie avec un format lisible.
 * Format de sortie : [v[0], v[1], ..., v[N-1]]
 *
 * @tparam T Type des éléments du vecteur
 * @tparam N Dimension du vecteur
 * @param os Flux de sortie (std::cout, fichier, etc.)
 * @param v Vecteur à afficher
 * @return Référence vers le flux de sortie (pour chaînage)
 *
 * Exemple d'utilisation :
 * @code
 * Vector<int, 3> v = {1, 2, 3};
 * std::cout << v << std::endl;  // Affiche : [1, 2, 3]
 * @endcode
 */
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