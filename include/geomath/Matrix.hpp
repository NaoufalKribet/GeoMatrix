/**
 * @file Matrix.hpp
 * @brief Implémentation d'une classe Matrix générique avec allocation statique
 * 
 * Cette classe template permet de manipuler des matrices de taille fixe
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
#include "Vector.hpp"

namespace geomath {

/**
 * @class Matrix
 * @brief Classe template pour représenter des matrices de taille fixe
 * 
 * @tparam T Type des éléments (float, double, int, etc.)
 * @tparam Rows Nombre de lignes (fixé à la compilation)
 * @tparam Cols Nombre de colonnes (fixé à la compilation)
 * 
 * Caractéristiques principales :
 * - Allocation statique sur la pile (std::array)
 * - Vérification des dimensions à la compilation
 * - Support complet des opérations matricielles
 * - Optimisé pour les petites matrices (2x2, 3x3, 4x4)
 * 
 * @example
 * Matrix<double, 3, 3> m = {
 *     {1.0, 0.0, 0.0},
 *     {0.0, 1.0, 0.0},
 *     {0.0, 0.0, 1.0}
 * };
 */
template<typename T, size_t Rows, size_t Cols>
class Matrix {
private:
    /// Stockage linéaire des données en row-major order
    std::array<T, Rows * Cols> data_;
    
    /**
     * @brief Convertit les indices (row, col) en index linéaire
     * @param row Indice de ligne
     * @param col Indice de colonne
     * @return Index linéaire dans le tableau data_
     * 
     * Utilise le format row-major : index = row * Cols + col
     */
    constexpr size_t index(size_t row, size_t col) const {
        return row * Cols + col;
    }

public:
    // ========================================================================
    // CONSTRUCTEURS
    // ========================================================================
    
    /**
     * @brief Constructeur par défaut - crée une matrice nulle
     * 
     * Tous les éléments sont initialisés à zéro grâce à l'initialisation
     * par accolades vides (value-initialization).
     * 
     * @example
     * Matrix<double, 3, 3> m; // Matrice 3x3 remplie de zéros
     */
    Matrix() : data_{} {}
    
    /**
     * @brief Constructeur par liste d'initialisation
     * @param list Liste de listes représentant les lignes de la matrice
     * 
     * Permet une initialisation intuitive de type :
     * Matrix<int, 2, 3> m = {{1, 2, 3}, {4, 5, 6}};
     * 
     * @note Utilise des assertions pour vérifier que :
     *       - Le nombre de lignes correspond à Rows
     *       - Chaque ligne contient exactement Cols éléments
     * 
     * @warning En mode Debug, un crash se produira si les dimensions
     *          ne correspondent pas (comportement voulu pour détecter les erreurs)
     */
    Matrix(std::initializer_list<std::initializer_list<T>> list) {
        assert(list.size() == Rows && "Le nombre de lignes doit correspondre à Rows");
        
        size_t row = 0;
        for (const auto& row_list : list) {
            assert(row_list.size() == Cols && "Le nombre de colonnes doit correspondre à Cols");
            size_t col = 0;
            for (const T& val : row_list) {
                data_[index(row, col)] = val;
                ++col;
            }
            ++row;
        }
    }
    
    /**
     * @brief Crée une matrice identité
     * @return Matrice identité de taille Rows x Cols
     * 
     * La matrice identité a des 1 sur la diagonale et des 0 ailleurs.
     * Ne peut être appelée que sur des matrices carrées (Rows == Cols).
     * 
     * @example
     * auto I = Matrix<double, 3, 3>::identity();
     * // I = [1 0 0]
     * //     [0 1 0]
     * //     [0 0 1]
     */
    static Matrix<T, Rows, Cols> identity() {
        static_assert(Rows == Cols, "La matrice identité nécessite une matrice carrée");
        Matrix<T, Rows, Cols> result;
        for (size_t i = 0; i < Rows; ++i) {
            result(i, i) = T{1};
        }
        return result;
    }
    
    // ========================================================================
    // ACCÈS AUX ÉLÉMENTS
    // ========================================================================
    
    /**
     * @brief Accès en lecture/écriture à un élément (version non-const)
     * @param row Indice de ligne (0-indexed)
     * @param col Indice de colonne (0-indexed)
     * @return Référence modifiable à l'élément
     * 
     * @note Utilise la notation mathématique m(i, j) plutôt que m[i][j]
     * @warning Les indices sont vérifiés par assertion en mode Debug uniquement
     * 
     * @example
     * Matrix<double, 2, 2> m;
     * m(0, 1) = 3.14; // Modification de l'élément
     */
    T& operator()(size_t row, size_t col) {
        assert(row < Rows && "Indice de ligne hors limites");
        assert(col < Cols && "Indice de colonne hors limites");
        return data_[index(row, col)];
    }
    
    /**
     * @brief Accès en lecture seule à un élément (version const)
     * @param row Indice de ligne (0-indexed)
     * @param col Indice de colonne (0-indexed)
     * @return Référence constante à l'élément
     * 
     * Cette version est appelée sur les objets const et retourne
     * une référence constante pour empêcher toute modification.
     * 
     * @example
     * const Matrix<double, 2, 2> m = {{1, 2}, {3, 4}};
     * double val = m(0, 1); // Lecture seule
     */
    const T& operator()(size_t row, size_t col) const {
        assert(row < Rows && "Indice de ligne hors limites");
        assert(col < Cols && "Indice de colonne hors limites");
        return data_[index(row, col)];
    }
    
    // ========================================================================
    // OPÉRATEURS ARITHMÉTIQUES
    // ========================================================================
    
    /**
     * @brief Addition élément par élément de deux matrices
     * @param autre Matrice à additionner
     * @return Nouvelle matrice résultante
     * 
     * Effectue l'opération : C[i][j] = A[i][j] + B[i][j]
     * Les deux matrices doivent avoir les mêmes dimensions (vérifié à la compilation).
     * 
     * @complexity O(Rows * Cols)
     */
    Matrix<T, Rows, Cols> operator+(const Matrix<T, Rows, Cols>& autre) const {
        Matrix<T, Rows, Cols> resultat;
        for (size_t i = 0; i < Rows * Cols; ++i) {
            resultat.data_[i] = data_[i] + autre.data_[i];
        }
        return resultat;
    }
    
    /**
     * @brief Soustraction élément par élément de deux matrices
     * @param autre Matrice à soustraire
     * @return Nouvelle matrice résultante
     * 
     * Effectue l'opération : C[i][j] = A[i][j] - B[i][j]
     * 
     * @complexity O(Rows * Cols)
     */
    Matrix<T, Rows, Cols> operator-(const Matrix<T, Rows, Cols>& autre) const {
        Matrix<T, Rows, Cols> resultat;
        for (size_t i = 0; i < Rows * Cols; ++i) {
            resultat.data_[i] = data_[i] - autre.data_[i];
        }
        return resultat;
    }
    
    /**
     * @brief Multiplication par un scalaire
     * @param scalaire Valeur scalaire à multiplier
     * @return Nouvelle matrice résultante
     * 
     * Effectue l'opération : C[i][j] = scalaire * A[i][j]
     * 
     * @complexity O(Rows * Cols)
     */
    Matrix<T, Rows, Cols> operator*(T scalaire) const {
        Matrix<T, Rows, Cols> resultat;
        for (size_t i = 0; i < Rows * Cols; ++i) {
            resultat.data_[i] = data_[i] * scalaire;
        }
        return resultat;
    }
    
    /**
     * @brief Division par un scalaire
     * @param scalaire Valeur scalaire diviseur (ne doit pas être zéro)
     * @return Nouvelle matrice résultante
     * 
     * @warning Aucune vérification de division par zéro n'est effectuée
     * 
     * @complexity O(Rows * Cols)
     */
    Matrix<T, Rows, Cols> operator/(T scalaire) const {
        Matrix<T, Rows, Cols> resultat;
        for (size_t i = 0; i < Rows * Cols; ++i) {
            resultat.data_[i] = data_[i] / scalaire;
        }
        return resultat;
    }
    
    /**
     * @brief Multiplication matricielle
     * @tparam OtherCols Nombre de colonnes de la matrice de droite
     * @param autre Matrice à multiplier (doit avoir Cols lignes)
     * @return Matrice résultante de dimension Rows x OtherCols
     * 
     * Effectue le produit matriciel standard :
     * C[i][j] = Σ(k=0 to Cols-1) A[i][k] * B[k][j]
     * 
     * @note La vérification dimensionnelle est faite à la compilation :
     *       Le nombre de colonnes de la matrice de gauche (Cols) doit
     *       correspondre au nombre de lignes de la matrice de droite
     * 
     * @complexity O(Rows * OtherCols * Cols)
     * 
     * @example
     * Matrix<double, 2, 3> A = {{1, 2, 3}, {4, 5, 6}};
     * Matrix<double, 3, 2> B = {{1, 2}, {3, 4}, {5, 6}};
     * Matrix<double, 2, 2> C = A * B; // Résultat 2x2
     */
    template<size_t OtherCols>
    Matrix<T, Rows, OtherCols> operator*(const Matrix<T, Cols, OtherCols>& autre) const {
        Matrix<T, Rows, OtherCols> resultat;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < OtherCols; ++j) {
                T sum = T{};
                for (size_t k = 0; k < Cols; ++k) {
                    sum += (*this)(i, k) * autre(k, j);
                }
                resultat(i, j) = sum;
            }
        }
        return resultat;
    }
    
    /**
     * @brief Multiplication matrice-vecteur
     * @param vec Vecteur colonne à multiplier (doit avoir Cols éléments)
     * @return Vecteur résultant de dimension Rows
     * 
     * Effectue l'opération : y[i] = Σ(j=0 to Cols-1) A[i][j] * x[j]
     * 
     * @note Le vecteur est traité comme une matrice colonne
     * 
     * @complexity O(Rows * Cols)
     * 
     * @example
     * Matrix<double, 3, 3> A = Matrix<double, 3, 3>::identity();
     * Vector<double, 3> v = {1, 2, 3};
     * Vector<double, 3> result = A * v; // result = {1, 2, 3}
     */
    Vector<T, Rows> operator*(const Vector<T, Cols>& vec) const {
        Vector<T, Rows> resultat;
        for (size_t i = 0; i < Rows; ++i) {
            T sum = T{};
            for (size_t j = 0; j < Cols; ++j) {
                sum += (*this)(i, j) * vec[j];
            }
            resultat[i] = sum;
        }
        return resultat;
    }
    
    // ========================================================================
    // OPÉRATIONS MATRICIELLES
    // ========================================================================
    
    /**
     * @brief Calcule la transposée de la matrice
     * @return Matrice transposée de dimension Cols x Rows
     * 
     * La transposée échange les lignes et les colonnes :
     * B[j][i] = A[i][j]
     * 
     * @complexity O(Rows * Cols)
     * 
     * @example
     * Matrix<double, 2, 3> A = {{1, 2, 3}, {4, 5, 6}};
     * Matrix<double, 3, 2> At = A.transpose();
     * // At = {{1, 4}, {2, 5}, {3, 6}}
     */
    Matrix<T, Cols, Rows> transpose() const {
        Matrix<T, Cols, Rows> resultat;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j) {
                resultat(j, i) = (*this)(i, j);
            }
        }
        return resultat;
    }
    
    /**
     * @brief Calcule le déterminant de la matrice
     * @return Valeur du déterminant
     * 
     * Implémentations disponibles :
     * - Matrices 2x2 : formule directe ad - bc
     * - Matrices 3x3 : règle de Sarrus
     * 
     * @note Nécessite une matrice carrée (Rows == Cols)
     * @note Pour les matrices 4x4 et plus, utiliser une bibliothèque
     *       spécialisée ou implémenter la décomposition LU
     * 
     * @complexity O(1) pour 2x2, O(1) pour 3x3
     * 
     * @example
     * Matrix<double, 2, 2> m = {{1, 2}, {3, 4}};
     * double det = m.determinant(); // det = -2
     */
    T determinant() const {
        static_assert(Rows == Cols, "Le déterminant nécessite une matrice carrée");
        
        if constexpr (Rows == 2) {
            // Formule 2x2 : ad - bc
            return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
        } else if constexpr (Rows == 3) {
            // Règle de Sarrus pour 3x3
            return (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1))
                 - (*this)(0, 1) * ((*this)(1, 0) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 0))
                 + (*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));
        } else {
            static_assert(Rows == 2 || Rows == 3, "Déterminant implémenté seulement pour 2x2 et 3x3");
            return T{};
        }
    }
    
    /**
     * @brief Calcule l'inverse de la matrice
     * @return Matrice inverse
     * 
     * Méthode utilisée :
     * - Pour 2x2 : formule directe avec matrice des cofacteurs
     * - Pour 3x3 : méthode de la comatrice (adjugate matrix)
     * 
     * @throws assertion si la matrice est singulière (det ≈ 0)
     * 
     * @note Nécessite une matrice carrée inversible
     * @warning En mode Debug, crash si |det| < 1e-10
     * 
     * @complexity O(1) pour 2x2 et 3x3
     * 
     * @example
     * Matrix<double, 2, 2> m = {{4, 7}, {2, 6}};
     * Matrix<double, 2, 2> m_inv = m.inverse();
     * Matrix<double, 2, 2> I = m * m_inv; // Doit donner l'identité
     */
    Matrix<T, Rows, Cols> inverse() const {
        static_assert(Rows == Cols, "L'inverse nécessite une matrice carrée");
        
        T det = determinant();
        assert(std::abs(det) > 1e-10 && "La matrice est singulière (déterminant nul)");
        
        if constexpr (Rows == 2) {
            // Formule directe pour 2x2
            Matrix<T, 2, 2> resultat;
            resultat(0, 0) =  (*this)(1, 1);
            resultat(0, 1) = -(*this)(0, 1);
            resultat(1, 0) = -(*this)(1, 0);
            resultat(1, 1) =  (*this)(0, 0);
            return resultat / det;
        } else if constexpr (Rows == 3) {
            // Matrice des cofacteurs (comatrice) pour 3x3
            Matrix<T, 3, 3> resultat;
            
            // Première ligne de la comatrice transposée
            resultat(0, 0) = (*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1);
            resultat(0, 1) = (*this)(1, 2) * (*this)(2, 0) - (*this)(1, 0) * (*this)(2, 2);
            resultat(0, 2) = (*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0);
            
            // Deuxième ligne de la comatrice transposée
            resultat(1, 0) = (*this)(0, 2) * (*this)(2, 1) - (*this)(0, 1) * (*this)(2, 2);
            resultat(1, 1) = (*this)(0, 0) * (*this)(2, 2) - (*this)(0, 2) * (*this)(2, 0);
            resultat(1, 2) = (*this)(0, 1) * (*this)(2, 0) - (*this)(0, 0) * (*this)(2, 1);
            
            // Troisième ligne de la comatrice transposée
            resultat(2, 0) = (*this)(0, 1) * (*this)(1, 2) - (*this)(0, 2) * (*this)(1, 1);
            resultat(2, 1) = (*this)(0, 2) * (*this)(1, 0) - (*this)(0, 0) * (*this)(1, 2);
            resultat(2, 2) = (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
            
            return resultat / det;
        } else {
            static_assert(Rows == 2 || Rows == 3, "Inverse implémenté seulement pour 2x2 et 3x3");
            return Matrix<T, Rows, Cols>();
        }
    }
    
    /**
     * @brief Calcule la trace de la matrice (somme des éléments diagonaux)
     * @return Somme des éléments diagonaux
     * 
     * Trace(A) = Σ(i=0 to min(Rows,Cols)-1) A[i][i]
     * 
     * @note Nécessite une matrice carrée
     * @note Propriété importante : Trace(AB) = Trace(BA)
     * 
     * @complexity O(min(Rows, Cols))
     * 
     * @example
     * Matrix<int, 3, 3> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
     * int tr = m.trace(); // tr = 1 + 5 + 9 = 15
     */
    T trace() const {
        static_assert(Rows == Cols, "La trace nécessite une matrice carrée");
        T sum = T{};
        for (size_t i = 0; i < Rows; ++i) {
            sum += (*this)(i, i);
        }
        return sum;
    }
    
    // ========================================================================
    // GETTERS / INFORMATIONS
    // ========================================================================
    
    /**
     * @brief Retourne le nombre de lignes
     * @return Nombre de lignes (Rows)
     * 
     * @note constexpr permet l'évaluation à la compilation
     */
    constexpr size_t rows() const { return Rows; }
    
    /**
     * @brief Retourne le nombre de colonnes
     * @return Nombre de colonnes (Cols)
     * 
     * @note constexpr permet l'évaluation à la compilation
     */
    constexpr size_t cols() const { return Cols; }
};

// ============================================================================
// FONCTIONS LIBRES (FREE FUNCTIONS)
// ============================================================================

/**
 * @brief Opérateur de sortie pour affichage formaté
 * @param os Flux de sortie
 * @param m Matrice à afficher
 * @return Référence au flux de sortie
 * 
 * Format d'affichage :
 * [[a11, a12, a13]
 *  [a21, a22, a23]
 *  [a31, a32, a33]]
 * 
 * @example
 * Matrix<int, 2, 2> m = {{1, 2}, {3, 4}};
 * std::cout << m << std::endl;
 * // Affiche : [[1, 2]
 * //            [3, 4]]
 */
template<typename T, size_t Rows, size_t Cols>
std::ostream& operator<<(std::ostream& os, const Matrix<T, Rows, Cols>& m) {
    os << "[";
    for (size_t i = 0; i < Rows; ++i) {
        if (i > 0) os << " ";
        os << "[";
        for (size_t j = 0; j < Cols; ++j) {
            os << m(i, j);
            if (j < Cols - 1) os << ", ";
        }
        os << "]";
        if (i < Rows - 1) os << "\n";
    }
    os << "]";
    return os;
}

/**
 * @brief Multiplication scalaire-matrice (ordre inversé)
 * @param scalaire Valeur scalaire
 * @param m Matrice à multiplier
 * @return Nouvelle matrice résultante
 * 
 * Permet d'écrire : scalaire * matrice (en plus de matrice * scalaire)
 * 
 * @example
 * Matrix<double, 2, 2> m = {{1, 2}, {3, 4}};
 * auto result = 2.5 * m; // Équivalent à m * 2.5
 */
template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols> operator*(T scalaire, const Matrix<T, Rows, Cols>& m) {
    return m * scalaire;
}

} // namespace geomath