#include <gtest/gtest.h>
#include <geomath/Vector.hpp>

using namespace geomath;

// ===== TEST DE CONSTRUCTION =====
TEST(VectorTest, Construction) {
    // Test du constructeur par défaut
    Vector<double, 3> v_defaut;
    ASSERT_EQ(v_defaut[0], 0.0);
    ASSERT_EQ(v_defaut[1], 0.0);
    ASSERT_EQ(v_defaut[2], 0.0);
    
    // Test du constructeur par liste d'initialisation
    Vector<int, 4> v_init = {1, 2, 3, 4};
    ASSERT_EQ(v_init[0], 1);
    ASSERT_EQ(v_init[1], 2);
    ASSERT_EQ(v_init[2], 3);
    ASSERT_EQ(v_init[3], 4);
    
    // Test avec des doubles
    Vector<double, 3> v_double = {1.5, 2.5, 3.5};
    ASSERT_DOUBLE_EQ(v_double[0], 1.5);
    ASSERT_DOUBLE_EQ(v_double[1], 2.5);
    ASSERT_DOUBLE_EQ(v_double[2], 3.5);
}

// ===== TEST D'ACCÈS AUX ÉLÉMENTS =====
TEST(VectorTest, ElementAccess) {
    // Création d'un vecteur
    Vector<int, 3> v = {10, 20, 30};
    
    // Vérification des valeurs
    ASSERT_EQ(v[0], 10);
    ASSERT_EQ(v[1], 20);
    ASSERT_EQ(v[2], 30);
    
    // Modification via operator[]
    v[0] = 100;
    v[1] = 200;
    v[2] = 300;
    
    // Vérification des modifications
    ASSERT_EQ(v[0], 100);
    ASSERT_EQ(v[1], 200);
    ASSERT_EQ(v[2], 300);
    
    // Test avec un vecteur const
    const Vector<double, 2> v_const = {1.1, 2.2};
    ASSERT_DOUBLE_EQ(v_const[0], 1.1);
    ASSERT_DOUBLE_EQ(v_const[1], 2.2);
}

// ===== TEST DES OPÉRATIONS ARITHMÉTIQUES =====
TEST(VectorTest, ArithmeticOps) {
    Vector<double, 3> v1 = {1.0, 2.0, 3.0};
    Vector<double, 3> v2 = {4.0, 5.0, 6.0};
    
    // Test de l'addition
    Vector<double, 3> v3 = v1 + v2;
    ASSERT_DOUBLE_EQ(v3[0], 5.0);
    ASSERT_DOUBLE_EQ(v3[1], 7.0);
    ASSERT_DOUBLE_EQ(v3[2], 9.0);
    
    // Test de la soustraction
    Vector<double, 3> v4 = v2 - v1;
    ASSERT_DOUBLE_EQ(v4[0], 3.0);
    ASSERT_DOUBLE_EQ(v4[1], 3.0);
    ASSERT_DOUBLE_EQ(v4[2], 3.0);
    
    // Test de la multiplication par un scalaire
    Vector<double, 3> v5 = v1 * 2.0;
    ASSERT_DOUBLE_EQ(v5[0], 2.0);
    ASSERT_DOUBLE_EQ(v5[1], 4.0);
    ASSERT_DOUBLE_EQ(v5[2], 6.0);
    
    // Test de la division par un scalaire
    Vector<double, 3> v6 = v2 / 2.0;
    ASSERT_DOUBLE_EQ(v6[0], 2.0);
    ASSERT_DOUBLE_EQ(v6[1], 2.5);
    ASSERT_DOUBLE_EQ(v6[2], 3.0);
    
    // Test avec des entiers
    Vector<int, 2> vi1 = {10, 20};
    Vector<int, 2> vi2 = {5, 15};
    Vector<int, 2> vi3 = vi1 + vi2;
    ASSERT_EQ(vi3[0], 15);
    ASSERT_EQ(vi3[1], 35);
}

// ===== TEST DES FONCTIONS MATHÉMATIQUES =====
TEST(VectorTest, MathFunctions) {
    // Test de la norme
    Vector<double, 3> v1 = {3.0, 4.0, 0.0};
    ASSERT_NEAR(v1.norm(), 5.0, 1e-10);
    
    Vector<double, 2> v2 = {1.0, 1.0};
    ASSERT_NEAR(v2.norm(), std::sqrt(2.0), 1e-10);
    
    Vector<int, 3> v3 = {1, 2, 2};
    ASSERT_NEAR(v3.norm(), 3.0, 1e-10);
    
    // Test du produit scalaire
    Vector<double, 3> va = {1.0, 2.0, 3.0};
    Vector<double, 3> vb = {4.0, 5.0, 6.0};
    double dot = dotProduct(va, vb);
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    ASSERT_NEAR(dot, 32.0, 1e-10);
    
    // Test avec des vecteurs orthogonaux
    Vector<double, 2> vx = {1.0, 0.0};
    Vector<double, 2> vy = {0.0, 1.0};
    ASSERT_NEAR(dotProduct(vx, vy), 0.0, 1e-10);
    
    // Test avec des entiers
    Vector<int, 3> vi1 = {1, 2, 3};
    Vector<int, 3> vi2 = {2, 3, 4};
    int dot_int = dotProduct(vi1, vi2);
    // 1*2 + 2*3 + 3*4 = 2 + 6 + 12 = 20
    ASSERT_EQ(dot_int, 20);
}

// ===== TEST DE CAS PARTICULIERS =====
TEST(VectorTest, EdgeCases) {
    // Vecteur de taille 1
    Vector<double, 1> v1 = {5.0};
    ASSERT_DOUBLE_EQ(v1[0], 5.0);
    ASSERT_NEAR(v1.norm(), 5.0, 1e-10);
    
    // Vecteur nul
    Vector<double, 3> v_nul;
    ASSERT_NEAR(v_nul.norm(), 0.0, 1e-10);
    
    // Opérations sur des vecteurs nuls
    Vector<int, 3> v0 = {0, 0, 0};
    Vector<int, 3> v_test = {1, 2, 3};
    Vector<int, 3> v_result = v_test + v0;
    ASSERT_EQ(v_result[0], 1);
    ASSERT_EQ(v_result[1], 2);
    ASSERT_EQ(v_result[2], 3);
    
    // Multiplication par zéro
    Vector<double, 2> v_mult = {5.0, 10.0};
    Vector<double, 2> v_zero = v_mult * 0.0;
    ASSERT_DOUBLE_EQ(v_zero[0], 0.0);
    ASSERT_DOUBLE_EQ(v_zero[1], 0.0);
}

// Point d'entrée principal pour Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}