/* ============================================================================
 * magias.h - INTERFACE DO MÓDULO DE MAGIAS/PROJÉTEIS
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 3 (Luísa)
 *
 * Este módulo cuida de TUDO relacionado a magias e projéteis:
 *   - Disparar automaticamente em intervalos fixos (o jogador não aperta
 *     botão de ataque — as magias ficam rodando sozinhas).
 *   - Mover cada projétil da lista a cada frame.
 *   - Reduzir o tempo_de_vida do projétil e marcar como "viva = false"
 *     quando expirar.
 *   - Liberar (free) os nós da lista quando o jogo terminar.
 *
 * COMO ADICIONAR UM PROJÉTIL:
 *   1. Alocar um novo MagiaNo com malloc.
 *   2. Preencher os campos (posicao, velocidade, dano, etc.).
 *   3. Inserir na cabeça da lista: novo->proxima = ej->magias_cabeca;
 *                                   ej->magias_cabeca = novo;
 *
 * O módulo colisao.c é quem marca "viva = false" quando acerta inimigo.
 * Este módulo remove da lista os projéteis com "viva = false".
 * ========================================================================== */

#ifndef MAGIAS_H
#define MAGIAS_H

#include "tipos.h"

/* Atualiza todos os projéteis: move, reduz tempo_de_vida, remove mortos. */
void magias_atualizar(EstadoJogo *ej);

/* Desenha cada projétil da lista na tela. */
void magias_desenhar(const EstadoJogo *ej);

/* Libera (free) todos os nós da lista. Chamada no encerramento do jogo. */
void magias_liberar_tudo(EstadoJogo *ej);

#endif /* MAGIAS_H */
