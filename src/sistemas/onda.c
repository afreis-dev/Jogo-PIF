/* ============================================================================
 * onda.c - STUB DO SISTEMA DE ONDAS
 * ============================================================================
 *
 * TODO: Dev 3 implementar este arquivo.
 *
 * Sugestao de formula pra escalada de dificuldade:
 *   inimigos_para_spawnar = 5 + numero_da_onda * 3;
 *   tempo_entre_spawns    = fmaxf(0.5f, 2.0f - numero_da_onda * 0.1f);
 *
 * Isso faz a onda 1 ter 8 inimigos com 1.9s entre spawns,
 * e a onda 10 ter 35 inimigos com 1.0s entre spawns.
 * ========================================================================== */

#include "onda.h"

void onda_inicializar(Onda *o, int numero_da_onda) {
    o->numero             = numero_da_onda;
    o->inimigos_restantes = 0;
    o->timer_spawn        = 0.0f;
    o->completa           = false;

    /* TODO: Dev 3 - calcular inimigos_para_spawnar e tempo_entre_spawns */
    o->inimigos_para_spawnar = 0;
    o->tempo_entre_spawns    = 2.0f;
}

void onda_atualizar(Onda *o, EstadoJogo *ej) {
    (void)o;
    (void)ej;
    /* TODO: Dev 3 - decrementar timer, spawnar inimigo quando zerar,
     * verificar se todos morreram pra marcar o->completa = true. */
}
