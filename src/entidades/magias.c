/* ============================================================================
 * magias.c - STUB DO MÓDULO DE MAGIAS
 * ============================================================================
 *
 * TODO: Dev 3 (Luísa) implementar este arquivo.
 *
 * Por enquanto as funções existem mas não fazem nada. Isso permite que o
 * projeto compile e rode sem erros de "função não definida". Quando o Dev 3
 * implementar, o jogo passará a ter projéteis sem mudar nada no main.c.
 *
 * "(void)ej;" silencia o warning de "parâmetro não usado". Remova quando
 * for implementar de verdade.
 * ========================================================================== */

#include "magias.h"
#include <stdlib.h>   /* malloc, free — precisará disso na implementação */

void magias_atualizar(EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 3 — mover projéteis, reduzir vida, remover mortos */
}

void magias_desenhar(const EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 3 — DrawCircleV pra cada MagiaNo da lista */
}

void magias_liberar_tudo(EstadoJogo *ej) {
    /* TODO: Dev 3 — percorrer lista e dar free em cada nó:
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
