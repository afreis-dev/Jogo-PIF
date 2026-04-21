/* ============================================================================
 * inimigos.h - INTERFACE DO MODULO DE INIMIGOS
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 3
 *
 * Cuida de tudo sobre inimigos:
 *   - Mover em direcao ao jogador (IA simples: "andar pra onde o jogador ta").
 *   - Remover inimigos marcados como "vivo = false" pela colisao.
 *   - Liberar (free) os nos ao fim do jogo.
 *
 * COMO CRIAR UM INIMIGO:
 *   1. malloc(sizeof(InimigoNo))
 *   2. Preencher: posicao (borda da tela), vida, velocidade, tipo.
 *   3. Inserir na cabeca: novo->proximo = ej->inimigos_cabeca;
 *                          ej->inimigos_cabeca = novo;
 *
 * O modulo colisao.c marca "vivo = false". Este modulo remove da lista.
 * ========================================================================== */

#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "tipos.h"

/* Move cada inimigo em direcao ao jogador, remove os mortos. */
void inimigos_atualizar(EstadoJogo *ej);

/* Desenha cada inimigo na tela. */
void inimigos_desenhar(const EstadoJogo *ej);

/* Libera todos os nos da lista. */
void inimigos_liberar_tudo(EstadoJogo *ej);

#endif /* INIMIGOS_H */
