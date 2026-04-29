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
            /* Spawn logo ALEM da borda visivel da tela.
             *
             * Como a camera mantem o jogador no centro, a tela mostra um
             * retangulo de LARGURA_TELA x ALTURA_TELA centrado nele. Pra
             * achar o ponto mais proximo a um angulo qualquer que esta JUSTO
             * fora desse retangulo: projeto o vetor (cos, sin) ate ele bater
             * em uma das paredes - quem bater primeiro define a distancia.
             *
             *   t_x = (largura/2) / |cos(angulo)|   -> distancia ate parede vertical
             *   t_y = (altura/2) / |sin(angulo)|   -> distancia ate parede horizontal
             *   t   = min(t_x, t_y)                -> sai pela parede mais proxima
             *
             * Somo uma pequena MARGEM pra que o inimigo nasca um pouquinho
             * fora da tela, e nao exatamente no canto visivel. */
            float angulo = ((float)rand() / (float)RAND_MAX) * 6.28318f;
            float dx     = cosf(angulo);
            float dy     = sinf(angulo);

            const float MARGEM       = 40.0f;
            float       meia_largura = LARGURA_TELA / 2.0f;
            float       meia_altura  = ALTURA_TELA  / 2.0f;

            /* Evita divisao por zero quando o angulo esta quase paralelo a
             * um dos eixos. Usar um valor enorme garante que o outro eixo
             * vai dominar o min(). */
            float t_x = (fabsf(dx) > 0.001f) ? meia_largura / fabsf(dx) : 1e9f;
            float t_y = (fabsf(dy) > 0.001f) ? meia_altura  / fabsf(dy) : 1e9f;
            float t   = fminf(t_x, t_y) + MARGEM;

            Vector2 pos = {
                ej->jogador.posicao.x + dx * t,
                ej->jogador.posicao.y + dy * t
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
