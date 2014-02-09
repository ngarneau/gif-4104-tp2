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

    // Démarrer le chronomètre
    Chrono lChrono(true);

    // Allouer le tableau des drapeaux (flags) d'invalidation
    char *lFlags = (char*) calloc(lMax, sizeof(*lFlags));
    assert(lFlags != 0);

    unsigned long lastSquared = sqrt(lMax) + 1;

    // Appliquer la passoire d'Ératosthène
    #pragma omp parallel
    {
        #pragma omp parallel for schedule(dynamic) shared(gP)
        for (gP = 2; gP <= lastSquared; gP++) {
            if (lFlags[gP] == 0) {
                // invalider tous les multiples
                for (unsigned long i = gP; i*gP < lMax; i++) {
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
