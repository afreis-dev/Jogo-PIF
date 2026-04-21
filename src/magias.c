/* ============================================================================
 * magias.c - STUB DO MODULO DE MAGIAS
 * ============================================================================
 *
 * TODO: Dev 3 implementar este arquivo.
 *
 * Por enquanto as funcoes existem mas nao fazem nada. Isso permite que o
 * projeto compile e rode sem erros de "funcao nao definida". Quando o Dev 3
 * implementar, o jogo passara a ter projeteis sem mudar nada no main.c.
 *
 * "(void)ej;" silencia o warning de "parametro nao usado". Remova quando
 * for implementar de verdade.
 * ========================================================================== */

#include "magias.h"
#include <stdlib.h>   /* malloc, free - precisara disso na implementacao */

void magias_atualizar(EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 3 - mover projeteis, reduzir vida, remover mortos */
}

void magias_desenhar(const EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 3 - DrawCircleV pra cada MagiaNo da lista */
}

void magias_liberar_tudo(EstadoJogo *ej) {
    /* TODO: Dev 3 - percorrer lista e dar free em cada no:
     *
     * MagiaNo *atual = ej->magias_cabeca;
     * while (atual != NULL) {
     *     MagiaNo *proximo = atual->proxima;
     *     free(atual);
     *     atual = proximo;
     * }
     * ej->magias_cabeca = NULL;
     */
    (void)ej;
}
