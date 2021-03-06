#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#include <math.h>
#include "Chrono.hpp"


unsigned long gP;

// Programme qui trouve à l'aide de la passoire d'Ératosthène,
// tous les nombres premiers inférieurs à un certain seuil
// spécifié sur la ligne de commande.
// Attention, ce programme n'est aucunement optimisé!
int main(int argc, char *argv[])
{

    // Déterminer la limite supérieure pour la recherche;
    // par défaut, prendre 1000
    unsigned long lMax = 1000;
    if (argc >= 2) {
        lMax = atol(argv[1]);
    }

    // Déterminer le nombre de threads
    int lThreads = 1; // par défault = 1
    if (argc >= 3) {
        lThreads = atoi(argv[2]);
    }
    omp_set_num_threads(lThreads);

    // Démarrer le chronomètre
    Chrono lChrono(true);

    // Allouer le tableau des drapeaux (flags) d'invalidation
    char *lFlags = (char*) calloc(lMax, sizeof(*lFlags));
    assert(lFlags != 0);

    // Process even numbers first
    unsigned long i;
    #pragma omp parallel shared(i)
    {
        #pragma omp for nowait schedule(static)
        for (i=4; i < lMax; i+=2) {
            lFlags[i]++;
        }
    }

    unsigned long lastSquared = sqrt(lMax) + 1;

    // Process odd numbers
    #pragma omp parallel shared(gP) private(i)
    {
        #pragma omp for schedule(dynamic)
        for (gP = 3; gP <= lastSquared; gP+=2) {
            if (lFlags[gP] == 0) {
                // invalider tous les multiples
                for (unsigned long i = gP; i*gP < lMax; i+=2) {
                    lFlags[i*gP]++;
                }
            }
        }
    }

    // Arrêter le chronomètre
    lChrono.pause();

    // Afficher les nombres trouvés à la console
    for (unsigned long p=2; p<lMax; p++) {
        if (lFlags[p] == 0) printf("%ld ", p);
    }
    printf("\n");

    // Afficher le temps d'exécution dans le stderr
    fprintf(stderr, "Temps d'execution = %f sec\n", lChrono.get());

    return 0;
}
