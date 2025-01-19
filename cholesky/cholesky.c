#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float *newVect(int dim);
float **newMat(int dim);
void problem(char* message);
void getData(int* dim, float*** A, float** b, float** x);
void cholesky(int dim, float** A, float* b, float* x);
void decompose(int dim, float** A);
void substitutionAvant(int dim, float** A, float* b);
void substitutionArriere(int dim, float** A, float* b, float* x);
void displayMatrice(float** A, int n);
void displayVecteur(float* b, int n);
void displayResult(int dim, float** A, float* b, float* x);

int main() {
    printf("Calcul de x tel que Ax=b\n");

/// Entree des donnees
    int dim = 1;
    float ** A = NULL;          // adresse de la matrice du probleme
    float *b = NULL;            // adresse du vecteur second membre
    float *x = NULL;            // adresse du vecteur solution
    getData(&dim, &A, &b, &x);  // donne a getData les adresses a utiliser

/// Calcul: methode de Cholesky
    cholesky(dim, A, b, x);

/// Sortie de resultat
    displayResult(dim, A, b, x);

/// Netoyage des espaces alloues dynamiquement
    free(x);
    free(b);
    for (int i = 0; i < dim; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}

void cholesky(int dim, float** A, float* b, float* x) {
    decompose(dim, A);
    substitutionAvant(dim, A, b);
    substitutionArriere(dim, A, b, x);
}

void decompose(int dim, float** A) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j <= i; j++) {
            float sum = 0;
            for (int k = 0; k < j; k++) {
                sum += A[i][k] * A[j][k];
            }
            if (i == j) {
                A[i][j] = sqrt(A[i][i] - sum);
            } else {
                A[i][j] = (1 / A[j][j]) * (A[i][j] - sum);
            }
        }
    }
}

void substitutionAvant(int dim, float** A, float* b) {
    for (int i = 0; i < dim; i++) {
        float sum = 0;
        for (int j = 0; j < i; j++) {
            sum += A[i][j] * b[j];
        }
        b[i] = (1 / A[i][i]) * (b[i] - sum);
    }
}

void substitutionArriere(int dim, float** A, float* b, float* x) {
    for (int i = dim - 1; i >= 0; i--) {
        float sum = 0;
        for (int j = i + 1; j < dim; j++) {
            sum += A[j][i] * x[j];
        }
        x[i] = (1 / A[i][i]) * (b[i] - sum);
    }
}

void displayResult(int dim, float** A, float* b, float* x) {
    printf("\nLa forme decomposée de A:\n");
    displayMatrice(A, dim);

    printf("Le vecteur b est:\n");
    displayVecteur(b, dim);

    printf("Le vecteur solution x est:\n");
    displayVecteur(x, dim);
}

void displayMatrice(float** a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%g ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void displayVecteur(float* b, int n) {
    for (int i = 0; i < n; i++) {
        printf("%g ", b[i]);
    }
    printf("\n");
}

void getData(int* dim, float*** A, float** b, float** x) {
/// Pointeurs pour les donnees
    float** M = NULL, *v = NULL, *sol = NULL;
    FILE *pf = NULL;

    pf = fopen("data0.txt", "r");

    if (pf != NULL) {
        fscanf(pf, "%d", dim);      // recupere la dimension
        M = newMat(*dim);           // alloue de la place pour la matrice
        v = newVect(*dim);          // alloue de la place pour le second membre
        sol = newVect(*dim);        // alloue de la place pour la solution
        for (int i = 0; i < *dim; i++) {
            for (int j = 0; j < *dim; j++) 
                fscanf(pf, "%f", &M[i][j]); // lit les donnees ligne par ligne
        }
        for (int i = 0; i < *dim; i++) 
            fscanf(pf, "%f", &v[i]);
        fclose(pf);
        
        for(int i = 0; i < *dim; i++)
            sol[i] = 0; // initialise le vecteur solution
    } else {
        problem("Ne peut pas ouvrir de fichier");
    }

/// Renvoie les adresses des contenus vers main
    *A = M; 
    *b = v;
    *x = sol;
}

float *newVect(int dim) {
    float* v = NULL;
    v = malloc(dim * sizeof(float));
    if (v == NULL)
        problem("Ne peut allouer un vecteur");
    return v;
}

float **newMat(int dim) {
    float **A = NULL;
    A = malloc(dim * sizeof(float*));
    if (A == NULL)
        problem("Ne peut allouer une matrice");
    for (int i = 0; i < dim; i++) {
        A[i] = malloc(dim * sizeof(float));
        if (A[i] == NULL)
            problem("Ne peut allouer une ligne de matrice");
    }
    return A;
}

void problem(char* message) {
    printf("%s\n", message);
    exit(2);
}