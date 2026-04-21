/* ============================================================================
 * dados.c - STUB DO SISTEMA DE DADOS
 * ============================================================================
 *
 * TODO: Dev 2 implementar este arquivo.
 *
 * Implementacao simples de dado_rolar:
 *   int dado_rolar(Dado *d) {
 *       d->ultimo_resultado = (rand() % d->faces) + 1;
 *       return d->ultimo_resultado;
 *   }
 *
 * (rand() % faces) da um numero de 0 a faces-1. Somando 1, fica de 1 a faces.
 * ========================================================================== */

#include "dados.h"
#include <stdlib.h>   /* rand */

int dado_rolar(Dado *d) {
    (void)d;
    return 0; /* TODO: Dev 2 - implementar conforme comentario acima */
}

void dado_desenhar(const Dado *d, int posicao_x, int posicao_y) {
    (void)d;
    (void)posicao_x;
    (void)posicao_y; /* TODO: Dev 2 - desenhar face do dado + resultado */
}
