/* ============================================================================
 * magias.c - ENGINE DE MAGIAS/PROJÉTEIS
 * ============================================================================
 *
 * Lista encadeada simples: novos projéteis vão pra cabeça (O(1) no insert),
 * mortos são removidos com ponteiro duplo no fim do frame.
 *
 * O auto-fire em si vive em magias_tipos.c — esta engine apenas chama
 * magias_tipos_processar_auto_fire(ej) no início do update.
 * ============================================================================ */

#include "magias.h"
#include "magias_tipos.h"
#include <stdlib.h>


/* Conta nós da lista pra respeitar MAX_PROJETEIS. */
static int contar_nos(const MagiaNo *cabeca) {
    int n = 0;
    for (const MagiaNo *p = cabeca; p != NULL; p = p->proxima) n++;
    return n;
}


void magias_spawnar(EstadoJogo *ej,
                    Vector2     posicao,
                    Vector2     direcao_normalizada,
                    Elemento    elemento) {
    if ((int)elemento < 0 || (int)elemento >= QTD_PARAMETROS_MAGIA) return;
    if (contar_nos(ej->magias_cabeca) >= MAX_PROJETEIS) return;

    MagiaNo *novo = (MagiaNo *)malloc(sizeof(MagiaNo));
    if (novo == NULL) return;

    const ParametrosMagia *p = &PARAMETROS_MAGIA[elemento];

    novo->dados.posicao       = posicao;
    novo->dados.velocidade.x  = direcao_normalizada.x * p->velocidade_projetil;
    novo->dados.velocidade.y  = direcao_normalizada.y * p->velocidade_projetil;
    novo->dados.dano          = p->dano_base;
    novo->dados.tempo_de_vida = p->tempo_de_vida;
    novo->dados.raio          = p->raio_projetil;
    novo->dados.elemento      = elemento;
    novo->dados.viva          = true;

    novo->proxima        = ej->magias_cabeca;
    ej->magias_cabeca    = novo;
}


void magias_atualizar(EstadoJogo *ej) {
    /* 1. Auto-fire — pode criar novos projéteis (entram na cabeça da lista). */
    magias_tipos_processar_auto_fire(ej);

    float dt = ej->delta_tempo;

    /* 2. Movimento + expiração. */
    for (MagiaNo *mno = ej->magias_cabeca; mno != NULL; mno = mno->proxima) {
        if (!mno->dados.viva) continue;
        mno->dados.posicao.x += mno->dados.velocidade.x * dt;
        mno->dados.posicao.y += mno->dados.velocidade.y * dt;
        mno->dados.tempo_de_vida -= dt;
        if (mno->dados.tempo_de_vida <= 0.0f) {
            mno->dados.viva = false;
        }
    }

    /* 3. Remoção dos mortos (ponteiro duplo). */
    MagiaNo **atual = &ej->magias_cabeca;
    while (*atual != NULL) {
        if (!(*atual)->dados.viva) {
            MagiaNo *morta = *atual;
            *atual = morta->proxima;
            free(morta);
        } else {
            atual = &(*atual)->proxima;
        }
    }
}


void magias_desenhar(const EstadoJogo *ej) {
    for (const MagiaNo *mno = ej->magias_cabeca;
         mno != NULL;
         mno = mno->proxima) {
        if (!mno->dados.viva) continue;
        if ((int)mno->dados.elemento < 0 ||
            (int)mno->dados.elemento >= QTD_PARAMETROS_MAGIA) continue;

        Color cor = PARAMETROS_MAGIA[mno->dados.elemento].cor;
        DrawCircleV(mno->dados.posicao, mno->dados.raio, cor);
        /* Núcleo branco pra dar punch visual. */
        DrawCircleV(mno->dados.posicao, mno->dados.raio * 0.4f, WHITE);
    }
}


void magias_liberar_tudo(EstadoJogo *ej) {
    MagiaNo *atual = ej->magias_cabeca;
    while (atual != NULL) {
        MagiaNo *prox = atual->proxima;
        free(atual);
        atual = prox;
    }
    ej->magias_cabeca = NULL;
}
