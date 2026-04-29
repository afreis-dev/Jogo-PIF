/* ============================================================================
 * inimigos.c - STUB DO MODULO DE INIMIGOS
 * ============================================================================
 *
 * TODO: Dev 3 implementar este arquivo.
 *
 * Dica de implementacao da IA basica (mover em direcao ao jogador):
 *
 *   Vector2 direcao;
 *   direcao.x = jogador.posicao.x - inimigo.posicao.x;
 *   direcao.y = jogador.posicao.y - inimigo.posicao.y;
 *   // normalizar (igual ao jogador.c)
 *   float len = sqrtf(direcao.x*direcao.x + direcao.y*direcao.y);
 *   if (len > 0) { direcao.x /= len; direcao.y /= len; }
 *   inimigo.posicao.x += direcao.x * inimigo.velocidade_movimento * dt;
 *   inimigo.posicao.y += direcao.y * inimigo.velocidade_movimento * dt;
 * ========================================================================== */

#include "inimigos.h"
#include <stdlib.h>

void inimigos_atualizar(EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 3 - mover inimigos, remover mortos da lista */
}

void inimigos_desenhar(const EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 3 - DrawCircleV vermelho pra cada InimigoNo */
}

void inimigos_liberar_tudo(EstadoJogo *ej) {
    /* TODO: Dev 3 - mesmo padrao de free do magias.c:
     *
     * InimigoNo *atual = ej->inimigos_cabeca;
     * while (atual != NULL) {
     *     InimigoNo *proximo = atual->proximo;
     *     free(atual);
     *     atual = proximo;
     * }
     * ej->inimigos_cabeca = NULL;
     */
    (void)ej;
}
