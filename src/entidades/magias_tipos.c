/* ============================================================================
 * magias_tipos.c - TABELA DE MAGIAS + AUTO-FIRE (LUÍSA EDITA AQUI)
 * ============================================================================
 *
 * Os números abaixo são chutes razoáveis pra começar — ajuste no playtest.
 * Pra mudar a frequência ou força de uma magia, mexa só na tabela; pra
 * mudar o jeito que o jogador atira (ex.: spread em leque), mexa em
 * magias_tipos_processar_auto_fire.
 * ============================================================================ */

#include "magias_tipos.h"
#include "magias.h"     /* magias_spawnar — fornecida pela engine */
#include <math.h>


/* ----------------------------------------------------------------------------
 * 1. TABELA DE STATS POR ELEMENTO
 * ----------------------------------------------------------------------------
 * A ordem das linhas casa com a ordem do enum Elemento:
 *   [0] ELEMENTO_FOGO
 *   [1] ELEMENTO_GELO
 *   [2] ELEMENTO_RELAMPAGO
 *   [3] ELEMENTO_VENENO
 *   [4] ELEMENTO_ARCANO
 *   [5] ELEMENTO_SOMBRA
 * --------------------------------------------------------------------------*/
const ParametrosMagia PARAMETROS_MAGIA[] = {
    /* fogo — dano alto, projétil médio. */
    {
        .dano_base           = 18.0f,
        .velocidade_projetil = 480.0f,
        .tempo_de_vida       = 1.5f,
        .raio_projetil       = 7.0f,
        .intervalo_disparo   = 0.45f,
        .cor                 = (Color){ 255, 110,  40, 255 },
    },

    /* gelo — leve, mais lento, fica vivo um pouco mais. */
    {
        .dano_base           = 12.0f,
        .velocidade_projetil = 380.0f,
        .tempo_de_vida       = 1.8f,
        .raio_projetil       = 8.0f,
        .intervalo_disparo   = 0.55f,
        .cor                 = (Color){ 130, 200, 255, 255 },
    },

    /* relâmpago — rápido, dano médio, cooldown curto. */
    {
        .dano_base           = 14.0f,
        .velocidade_projetil = 600.0f,
        .tempo_de_vida       = 1.2f,
        .raio_projetil       = 6.0f,
        .intervalo_disparo   = 0.30f,
        .cor                 = (Color){ 230, 230,  90, 255 },
    },

    /* veneno — dano baixo, mas continua vivo bastante (ideia: DoT futuro). */
    {
        .dano_base           = 10.0f,
        .velocidade_projetil = 350.0f,
        .tempo_de_vida       = 2.5f,
        .raio_projetil       = 9.0f,
        .intervalo_disparo   = 0.50f,
        .cor                 = (Color){ 110, 200,  90, 255 },
    },

    /* arcano — equilibrado, magia "default". */
    {
        .dano_base           = 15.0f,
        .velocidade_projetil = 460.0f,
        .tempo_de_vida       = 1.5f,
        .raio_projetil       = 7.0f,
        .intervalo_disparo   = 0.40f,
        .cor                 = (Color){ 180, 120, 255, 255 },
    },

    /* sombra — alto dano, baixa cadência. */
    {
        .dano_base           = 22.0f,
        .velocidade_projetil = 420.0f,
        .tempo_de_vida       = 1.4f,
        .raio_projetil       = 7.0f,
        .intervalo_disparo   = 0.65f,
        .cor                 = (Color){ 140,  80, 200, 255 },
    },
};

const int QTD_PARAMETROS_MAGIA =
    sizeof(PARAMETROS_MAGIA) / sizeof(PARAMETROS_MAGIA[0]);


/* ----------------------------------------------------------------------------
 * 2. AUTO-FIRE
 * ----------------------------------------------------------------------------
 * Decide a cada frame se é hora de disparar. Hoje:
 *   - Cada elemento tem seu próprio timer interno (estático aqui).
 *   - Quando o timer zera, mira no inimigo VIVO mais próximo.
 *   - Por enquanto SÓ o elemento principal da profecia atira (mods[0]).
 *     Quando a Sofia entregar o sistema de magias múltiplas, esta função
 *     vai iterar sobre as magias equipadas do jogador.
 * --------------------------------------------------------------------------*/

/* Helper: vetor unitário do jogador até o inimigo vivo mais próximo. Retorna
 * false se não houver inimigos vivos. */
static bool encontrar_alvo(const EstadoJogo *ej, Vector2 *out_dir) {
    const InimigoNo *mais_perto = NULL;
    float menor_dist2 = 1e30f;

    for (const InimigoNo *ino = ej->inimigos_cabeca;
         ino != NULL;
         ino = ino->proximo) {
        if (!ino->dados.vivo) continue;
        float dx = ino->dados.posicao.x - ej->jogador.posicao.x;
        float dy = ino->dados.posicao.y - ej->jogador.posicao.y;
        float d2 = dx * dx + dy * dy;
        if (d2 < menor_dist2) {
            menor_dist2 = d2;
            mais_perto  = ino;
        }
    }
    if (mais_perto == NULL) return false;

    float dx = mais_perto->dados.posicao.x - ej->jogador.posicao.x;
    float dy = mais_perto->dados.posicao.y - ej->jogador.posicao.y;
    float comprimento = sqrtf(dx * dx + dy * dy);
    if (comprimento < 0.0001f) return false;

    out_dir->x = dx / comprimento;
    out_dir->y = dy / comprimento;
    return true;
}


void magias_tipos_processar_auto_fire(EstadoJogo *ej) {
    /* Timers persistentes entre frames: um por elemento. Inicializam em 0,
     * o que faz o primeiro tick disparar (sensação imediata ao começar). */
    static float timer_por_elemento[ELEMENTO_TOTAL] = {0};

    /* Por ora, atira apenas o elemento do mod[0] da profecia.
     * Quando a Luísa quiser, dá pra iterar todos os 3 mods. */
    Elemento elemento_ativo = ej->profecia.mods[0].elemento;
    if ((int)elemento_ativo < 0 || (int)elemento_ativo >= QTD_PARAMETROS_MAGIA) {
        elemento_ativo = ELEMENTO_ARCANO;   /* fallback seguro */
    }

    timer_por_elemento[elemento_ativo] -= ej->delta_tempo;
    if (timer_por_elemento[elemento_ativo] > 0.0f) return;

    Vector2 dir;
    if (!encontrar_alvo(ej, &dir)) {
        /* Sem inimigos: não atira, mas evita "guardar disparos" — mantém o
         * timer em zero pra disparar assim que algo aparecer. */
        timer_por_elemento[elemento_ativo] = 0.0f;
        return;
    }

    magias_spawnar(ej, ej->jogador.posicao, dir, elemento_ativo);
    timer_por_elemento[elemento_ativo] +=
        PARAMETROS_MAGIA[elemento_ativo].intervalo_disparo;
}
