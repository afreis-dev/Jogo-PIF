/* ============================================================================
 * onda.h - INTERFACE DO SISTEMA DE ONDAS (WAVE GENERATOR)
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 3
 *
 * Uma "onda" e um conjunto de inimigos que spawnam ao longo do tempo.
 * A logica e:
 *   1. onda_inicializar: define quantos inimigos vao spawnar com base no
 *      numero da onda (quanto maior, mais dificil).
 *   2. onda_atualizar: a cada frame, decrementa o timer. Quando o timer
 *      zera, spawna um inimigo e reinicia o timer. Quando todos spawnaram
 *      E foram mortos, marca "completa = true".
 *   3. Main ve "onda_atual.completa" e transiciona pra CARTAS_UPGRADE.
 * ========================================================================== */

#ifndef ONDA_H
#define ONDA_H

#include "tipos.h"

/* Prepara a onda N. Calcula inimigos_para_spawnar, zera timers. */
void onda_inicializar(Onda *o, int numero_da_onda);

/* Atualiza o timer de spawn. Chama inimigos_spawnar quando necessario. */
void onda_atualizar(Onda *o, EstadoJogo *ej);

#endif /* ONDA_H */
