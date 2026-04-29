/* ============================================================================
 * magias.h - INTERFACE DO MODULO DE MAGIAS/PROJETEIS
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 3
 *
 * Este modulo cuida de TUDO relacionado a magias e projeteis:
 *   - Disparar automaticamente em intervalos fixos (o jogador nao aperta
 *     botao de ataque - as magias ficam rodando sozinhas).
 *   - Mover cada projetil da lista a cada frame.
 *   - Reduzir o tempo_de_vida do projetil e marcar como "viva = false"
 *     quando expirar.
 *   - Liberar (free) os nos da lista quando o jogo terminar.
 *
 * COMO ADICIONAR UM PROJETIL:
 *   1. Alocar um novo MagiaNo com malloc.
 *   2. Preencher os campos (posicao, velocidade, dano, etc.).
 *   3. Inserir na cabeca da lista: novo->proxima = ej->magias_cabeca;
 *                                   ej->magias_cabeca = novo;
 *
 * O modulo colisao.c e quem marca "viva = false" quando acerta inimigo.
 * Este modulo remove da lista os projeteis com "viva = false".
 * ========================================================================== */

#ifndef MAGIAS_H
#define MAGIAS_H

#include "tipos.h"

/* Atualiza todos os projeteis: move, reduz tempo_de_vida, remove mortos. */
void magias_atualizar(EstadoJogo *ej);

/* Desenha cada projetil da lista na tela. */
void magias_desenhar(const EstadoJogo *ej);

/* Libera (free) todos os nos da lista. Chamada no encerramento do jogo. */
void magias_liberar_tudo(EstadoJogo *ej);

#endif /* MAGIAS_H */
