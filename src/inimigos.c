/* ============================================================================
 * inimigos.c - IMPLEMENTACAO MINIMA (SANDBOX)
 * ============================================================================
 *
 * Versao SO da branch test/sandbox-integracao. Existe pra testar os modulos
 * que dependem de inimigos vivos: HUD (vida cai por contato), biomassa
 * (sobe quando inimigo morre), colisao (player <-> inimigo, magia <-> inimigo).
 *
 * NAO vai pra main. Luisa (Dev 3) escreve a versao oficial.
 *
 * IA = chase puro: andar em linha reta na direcao do jogador.
 * ========================================================================== */

#include "inimigos.h"
#include <stdlib.h>
#include <math.h>


/* ----- SPAWN -----
 * Aloca um InimigoNo, preenche com defaults de teste e insere na cabeca da
 * lista. O(1) por inimigo. */
void inimigos_spawnar_em(EstadoJogo *ej, Vector2 posicao) {
    InimigoNo *novo = (InimigoNo *)malloc(sizeof(InimigoNo));
    if (novo == NULL) return;   /* sem memoria, ignora silenciosamente */

    novo->dados.posicao             = posicao;
    novo->dados.velocidade          = (Vector2){ 0.0f, 0.0f };
    novo->dados.raio                = 16.0f;
    novo->dados.vida_maxima         = 30;
    novo->dados.vida                = 30;
    novo->dados.dano                = 1.0f;     /* cast pra int em colisao */
    novo->dados.velocidade_movimento = 90.0f;   /* mais lento que o jogador */
    novo->dados.tipo                = INIMIGO_CORPO_A_CORPO;
    novo->dados.recompensa_biomassa = 1;
    novo->dados.vivo                = true;

    novo->proximo = ej->inimigos_cabeca;
    ej->inimigos_cabeca = novo;
}


/* ----- ATUALIZACAO ----- */
void inimigos_atualizar(EstadoJogo *ej) {
    float dt = ej->delta_tempo;

    /* PRIMEIRA PASSADA: mover quem esta vivo, na direcao do jogador. */
    for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo) {
        if (!ino->dados.vivo) continue;

        Vector2 dir;
        dir.x = ej->jogador.posicao.x - ino->dados.posicao.x;
        dir.y = ej->jogador.posicao.y - ino->dados.posicao.y;

        float comprimento = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (comprimento > 0.001f) {
            dir.x /= comprimento;
            dir.y /= comprimento;
        }

        ino->dados.posicao.x += dir.x * ino->dados.velocidade_movimento * dt;
        ino->dados.posicao.y += dir.y * ino->dados.velocidade_movimento * dt;
    }

    /* SEGUNDA PASSADA: COLISAO ENTRE INIMIGOS.
     * Pra cada par (a, b) de inimigos vivos, se os circulos estao se sobrepondo,
     * empurra cada um pelo overlap/2 em direcoes opostas. Resultado: eles
     * formam aglomerados em vez de empilhar no mesmo pixel.
     *
     * O(N^2). Pra ate ~50 inimigos vivos da uns 1250 pares - tranquilo. */
    for (InimigoNo *a = ej->inimigos_cabeca; a != NULL; a = a->proximo) {
        if (!a->dados.vivo) continue;

        for (InimigoNo *b = a->proximo; b != NULL; b = b->proximo) {
            if (!b->dados.vivo) continue;

            float dx = b->dados.posicao.x - a->dados.posicao.x;
            float dy = b->dados.posicao.y - a->dados.posicao.y;
            float dist2 = dx * dx + dy * dy;
            float soma_raios = a->dados.raio + b->dados.raio;

            if (dist2 < soma_raios * soma_raios && dist2 > 0.0001f) {
                float distancia = sqrtf(dist2);
                float overlap   = soma_raios - distancia;
                /* Cada um anda metade do overlap em direcoes opostas. */
                float empurra_x = (dx / distancia) * (overlap * 0.5f);
                float empurra_y = (dy / distancia) * (overlap * 0.5f);

                a->dados.posicao.x -= empurra_x;
                a->dados.posicao.y -= empurra_y;
                b->dados.posicao.x += empurra_x;
                b->dados.posicao.y += empurra_y;
            }
        }
    }

    /* TERCEIRA PASSADA: remover os mortos (vivo = false foi marcado pela
     * colisao.c). Truque do "ponteiro pro ponteiro" pra remover sem precisar
     * tratar o caso especial de "removendo a cabeca". */
    InimigoNo **ind = &ej->inimigos_cabeca;
    while (*ind != NULL) {
        InimigoNo *atual = *ind;
        if (!atual->dados.vivo) {
            *ind = atual->proximo;
            free(atual);
        } else {
            ind = &atual->proximo;
        }
    }
}


/* ----- DESENHO -----
 * Bolinha vermelha. Cor varia pela vida (vermelho saturado quando full,
 * mais claro quando ferido) - serve pra ver o efeito dos projeteis. */
void inimigos_desenhar(const EstadoJogo *ej) {
    for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo) {
        if (!ino->dados.vivo) continue;

        DrawCircleV(ino->dados.posicao, ino->dados.raio, MAROON);
        DrawCircleLines((int)ino->dados.posicao.x,
                        (int)ino->dados.posicao.y,
                        ino->dados.raio, RED);
    }
}


/* ----- LIBERAR TUDO ----- */
void inimigos_liberar_tudo(EstadoJogo *ej) {
    InimigoNo *atual = ej->inimigos_cabeca;
    while (atual != NULL) {
        InimigoNo *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    ej->inimigos_cabeca = NULL;
}
