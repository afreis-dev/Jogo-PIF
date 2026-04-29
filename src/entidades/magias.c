/* ============================================================================
 * magias.c - IMPLEMENTACAO MINIMA (SANDBOX)
 * ============================================================================
 *
 * Versao SO da branch test/sandbox-integracao. Auto-disparo simples pra que
 * o jogador consiga matar os inimigos de teste e gerar biomassa - assim a
 * gente valida o HUD e o sistema de colisao.
 *
 * NAO vai pra main. Luisa (Dev 3) escreve a versao oficial.
 *
 * Comportamento:
 *   - A cada INTERVALO_DISPARO segundos, encontra o inimigo mais proximo e
 *     dispara um projetil em direcao a ele.
 *   - Se nao tem inimigo, fica em silencio.
 *   - Cada projetil voa em linha reta com tempo de vida limitado.
 *   - Colisao com inimigo eh tratada por colisao.c (marca viva = false).
 * ========================================================================== */

#include "magias.h"
#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define INTERVALO_DISPARO   0.4f    /* segundos entre tiros */
#define VELOCIDADE_PROJETIL 480.0f  /* px/s */
#define DANO_PROJETIL       15.0f
#define VIDA_PROJETIL       1.5f    /* segundos antes de expirar */


/* ----- ENCONTRAR INIMIGO MAIS PROXIMO -----
 * Retorna o no do inimigo vivo mais proximo do jogador, ou NULL se nao tem
 * nenhum. Usa distancia ao quadrado pra evitar sqrt. */
static InimigoNo *inimigo_mais_proximo(const EstadoJogo *ej) {
    InimigoNo *melhor = NULL;
    float melhor_dist2 = 1e30f;

    for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo) {
        if (!ino->dados.vivo) continue;
        float dx = ino->dados.posicao.x - ej->jogador.posicao.x;
        float dy = ino->dados.posicao.y - ej->jogador.posicao.y;
        float d2 = dx * dx + dy * dy;
        if (d2 < melhor_dist2) {
            melhor_dist2 = d2;
            melhor = ino;
        }
    }
    return melhor;
}


/* ----- SPAWN DE PROJETIL ----- */
static void spawnar_projetil(EstadoJogo *ej, Vector2 origem, Vector2 alvo) {
    float dx = alvo.x - origem.x;
    float dy = alvo.y - origem.y;
    float comprimento = sqrtf(dx * dx + dy * dy);
    if (comprimento < 0.001f) return;
    dx /= comprimento;
    dy /= comprimento;

    MagiaNo *novo = (MagiaNo *)malloc(sizeof(MagiaNo));
    if (novo == NULL) return;

    novo->dados.posicao      = origem;
    novo->dados.velocidade   = (Vector2){ dx * VELOCIDADE_PROJETIL,
                                          dy * VELOCIDADE_PROJETIL };
    novo->dados.dano         = DANO_PROJETIL;
    novo->dados.tempo_de_vida = VIDA_PROJETIL;
    novo->dados.elemento     = ELEMENTO_ARCANO;
    novo->dados.viva         = true;

    novo->proxima = ej->magias_cabeca;
    ej->magias_cabeca = novo;
}


/* ----- ATUALIZACAO ----- */
void magias_atualizar(EstadoJogo *ej) {
    /* "static" guarda o valor entre chamadas. Como so existe UM jogador,
     * dah pra usar essa simplificacao. Numa versao final isso deveria viver
     * em EstadoJogo. */
    static float timer_disparo = 0.0f;
    static bool  atirando      = true;   /* SANDBOX: Q alterna auto-fire */

    float dt = ej->delta_tempo;

    /* Toggle de teste: Q liga/desliga o auto-disparo. Util pra testar
     * comportamento dos inimigos sem ser interrompido pelos projeteis. */
    if (IsKeyPressed(KEY_Q)) {
        atirando = !atirando;
    }

    /* 1. Tentar disparar um novo projetil (so se estiver atirando). */
    if (atirando) {
        timer_disparo -= dt;
        if (timer_disparo <= 0.0f) {
            InimigoNo *alvo = inimigo_mais_proximo(ej);
            if (alvo != NULL) {
                spawnar_projetil(ej, ej->jogador.posicao, alvo->dados.posicao);
            }
            timer_disparo = INTERVALO_DISPARO;
        }
    } else {
        /* Mantem o timer "armado" pra disparar logo quando ligar de novo. */
        timer_disparo = 0.0f;
    }

    /* 2. Mover projeteis vivos e expirar os antigos. */
    for (MagiaNo *mno = ej->magias_cabeca; mno != NULL; mno = mno->proxima) {
        if (!mno->dados.viva) continue;

        mno->dados.posicao.x += mno->dados.velocidade.x * dt;
        mno->dados.posicao.y += mno->dados.velocidade.y * dt;

        mno->dados.tempo_de_vida -= dt;
        if (mno->dados.tempo_de_vida <= 0.0f) {
            mno->dados.viva = false;
        }
    }

    /* 3. Remover os mortos da lista (igual padrao do inimigos.c). */
    MagiaNo **ind = &ej->magias_cabeca;
    while (*ind != NULL) {
        MagiaNo *atual = *ind;
        if (!atual->dados.viva) {
            *ind = atual->proxima;
            free(atual);
        } else {
            ind = &atual->proxima;
        }
    }
}


/* ----- DESENHO ----- */
void magias_desenhar(const EstadoJogo *ej) {
    for (MagiaNo *mno = ej->magias_cabeca; mno != NULL; mno = mno->proxima) {
        if (!mno->dados.viva) continue;
        DrawCircleV(mno->dados.posicao, 5.0f, SKYBLUE);
        DrawCircleV(mno->dados.posicao, 2.5f, WHITE);
    }
}


/* ----- LIBERAR TUDO ----- */
void magias_liberar_tudo(EstadoJogo *ej) {
    MagiaNo *atual = ej->magias_cabeca;
    while (atual != NULL) {
        MagiaNo *proximo = atual->proxima;
        free(atual);
        atual = proximo;
    }
    ej->magias_cabeca = NULL;
}
