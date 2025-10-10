/**
 * @file test_matrix.cpp
 * @brief Suite de tests unitaires pour la classe Matrix
 * 
 * Ce fichier contient tous les tests pour valider le comportement
 * de la classe Matrix, incluant :
 * - Construction et initialisation
 * - Accès aux éléments
 * - Opérations arithmétiques de base
 * - Multiplication matricielle et matrice-vecteur
 * - Opérations avancées (transposée, déterminant, inverse, trace)
 * - Cas limites et situations particulières
 * 
 * Framework : Google Test
 * 
 * @author Votre Nom
 * @date 2025
 */

#include <gtest/gtest.h>
#include "geomath/Matrix.hpp"
#include "geomath/Vector.hpp"

using namespace geomath;

// ============================================================================
// TESTS DE CONSTRUCTION ET INITIALISATION
// ============================================================================

/**
 * @brief Teste les différents constructeurs de Matrix
 * 
 * Vérifie :
 * - Constructeur par défaut (matrice nulle)
 * - Constructeur par liste d'initialisation
 * - Création de matrice identité
 */
TEST(MatrixTest, Construction) {
    // Test du constructeur par défaut : tous les éléments doivent être à zéro
    Matrix<double, 2, 3> m_defaut;
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            ASSERT_DOUBLE_EQ(m_defaut(i, j), 0.0) 
                << "L'élément (" << i << "," << j << ") devrait être 0.0";
        }
    }
    
    // Test du constructeur par liste d'initialisation
    Matrix<int, 2, 3> m_init = {
        {1, 2, 3},
        {4, 5, 6}
    };
    ASSERT_EQ(m_init(0, 0), 1);
    ASSERT_EQ(m_init(0, 1), 2);
    ASSERT_EQ(m_init(0, 2), 3);
    ASSERT_EQ(m_init(1, 0), 4);
    ASSERT_EQ(m_init(1, 1), 5);
    ASSERT_EQ(m_init(1, 2), 6);
    
    // Test de la matrice identité : 1 sur la diagonale, 0 ailleurs
    Matrix<double, 3, 3> m_id = Matrix<double, 3, 3>::identity();
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (i == j) {
                ASSERT_DOUBLE_EQ(m_id(i, j), 1.0) 
                    << "L'élément diagonal (" << i << "," << j << ") devrait être 1.0";
            } else {
                ASSERT_DOUBLE_EQ(m_id(i, j), 0.0)
                    << "L'élément hors diagonale (" << i << "," << j << ") devrait être 0.0";
            }
        }
    }
    
    // Test avec différentes tailles de matrices
    Matrix<float, 1, 1> m_1x1 = {{5.5f}};
    ASSERT_FLOAT_EQ(m_1x1(0, 0), 5.5f);
    
    Matrix<int, 4, 4> m_4x4 = Matrix<int, 4, 4>::identity();
    ASSERT_EQ(m_4x4(0, 0), 1);
    ASSERT_EQ(m_4x4(3, 3), 1);
}

// ============================================================================
// TESTS D'ACCÈS AUX ÉLÉMENTS
// ============================================================================

/**
 * @brief Teste l'accès et la modification des éléments
 * 
 * Vérifie :
 * - Lecture des éléments (operator() const)
 * - Modification des éléments (operator() non-const)
 * - Fonctionnement avec des matrices const
 */
TEST(MatrixTest, ElementAccess) {
    // Création et vérification des valeurs initiales
    Matrix<double, 2, 2> m = {
        {1.5, 2.5},
        {3.5, 4.5}
    };
    
    ASSERT_DOUBLE_EQ(m(0, 0), 1.5);
    ASSERT_DOUBLE_EQ(m(0, 1), 2.5);
    ASSERT_DOUBLE_EQ(m(1, 0), 3.5);
    ASSERT_DOUBLE_EQ(m(1, 1), 4.5);
    
    // Test de modification via operator()
    m(0, 0) = 10.0;
    m(1, 1) = 20.0;
    ASSERT_DOUBLE_EQ(m(0, 0), 10.0) << "La modification de m(0,0) a échoué";
    ASSERT_DOUBLE_EQ(m(1, 1), 20.0)