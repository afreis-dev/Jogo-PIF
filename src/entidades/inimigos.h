/* ============================================================================
 * inimigos.h - INTERFACE DO MÓDULO DE INIMIGOS
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 3 (Luísa)
 *
 * Cuida de tudo sobre inimigos:
 *   - Mover em direção ao jogador (IA simples: "andar pra onde o jogador tá").
 *   - Remover inimigos marcados como "vivo = false" pela colisão.
 *   - Liberar (free) os nós ao fim do jogo.
 *
 * COMO CRIAR UM INIMIGO:
 *   1. malloc(sizeof(InimigoNo))
 *   2. Preencher: posicao (borda da tela), vida, velocidade, tipo.
 *   3. Inserir na cabeça: novo->proximo = ej->inimigos_cabeca;
 *                          ej->inimigos_cabeca = novo;
 *
 * O módulo colisao.c marca "vivo = false". Este módulo remove da lista.
 * ========================================================================== */

#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "tipos.h"

void inimigos_spawnar_em(
    EstadoJogo *ej,
    Vector2 pos,
    TipoInimigo tipo
);

/* Move cada inimigo em direção ao jogador, remove os mortos. */
void inimigos_atualizar(EstadoJogo *ej);

/* Desenha cada inimigo na tela. */
void inimigos_desenhar(const EstadoJogo *ej);

/* Libera todos os nós da lista. */
void inimigos_liberar_tudo(EstadoJogo *ej);

#endif /* INIMIGOS_H */
