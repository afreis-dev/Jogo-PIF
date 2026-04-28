/* ============================================================================
 * onda.c - IMPLEMENTACAO MINIMA (SANDBOX)
 * ============================================================================
 *
 * Versao SO da branch test/sandbox-integracao. Faz spawn de inimigos em
 * intervalo fixo, em posicoes aleatorias ao redor do jogador (off-screen).
 * Quando todos forem spawnados E mortos, marca completa = true pra main
 * transicionar pra tela de upgrade.
 *
 * NAO vai pra main. Luisa (Dev 3) escreve a versao oficial.
 *
 * Formula de dificuldade da sugestao do header original:
 *   inimigos = 5 + numero_da_onda * 3
 *   intervalo = max(0.5, 2.0 - numero_da_onda * 0.1)
 * ========================================================================== */

#include "onda.h"
#include "inimigos.h"
#include <stdlib.h>
#include <math.h>


/* ----- INICIALIZACAO -----
 * Calcula quantos inimigos a onda vai produzir e a cadencia entre spawns. */
void onda_inicializar(Onda *o, int numero_da_onda) {
    o->numero               = numero_da_onda;
    o->inimigos_para_spawnar = 5 + numero_da_onda * 3;
    o->inimigos_restantes   = o->inimigos_para_spawnar;
    o->tempo_entre_spawns   = fmaxf(0.5f, 2.0f - numero_da_onda * 0.1f);
    o->timer_spawn          = o->tempo_entre_spawns;
    o->completa             = false;
}


/* ----- ATUALIZACAO -----
 * Cada frame: decrementa timer; quando zera, spawna um inimigo numa posicao
 * aleatoria em volta do jogador (em coord de mundo, fora da tela). Quando
 * inimigos_restantes chega a 0 E a lista esta vazia, marca completa. */
void onda_atualizar(Onda *o, EstadoJogo *ej) {
    /* Ainda tem inimigos pra spawnar? */
    if (o->inimigos_restantes > 0) {
        o->timer_spawn -= ej->delta_tempo;
        if (o->timer_spawn <= 0.0f) {
            /* Posicao numa borda imaginaria a 600px do jogador. Como a tela
             * tem 1280x720 e o player fica no centro, esse raio garante que
             * o inimigo nasca off-screen (~50% alem da metade da tela). */
            float angulo  = ((float)rand() / (float)RAND_MAX) * 6.28318f; /* 2pi */
            float raio    = 600.0f;
            Vector2 pos = {
                ej->jogador.posicao.x + cosf(angulo) * raio,
                ej->jogador.posicao.y + sinf(angulo) * raio
            };
            inimigos_spawnar_em(ej, pos);

            o->inimigos_restantes--;
            o->timer_spawn = o->tempo_entre_spawns;
        }
    } else {
        /* Spawn esgotado: a onda so termina quando a lista esvaziar (ou seja,
         * quando todos os inimigos vivos morrerem). */
        if (ej->inimigos_cabeca == NULL) {
            o->completa = true;
        }
    }
}
