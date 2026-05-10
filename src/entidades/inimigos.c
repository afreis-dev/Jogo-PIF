/* ============================================================================
 * inimigos.c - STUB DO MÓDULO DE INIMIGOS
 * ============================================================================
 *
 * TODO: Dev 3 (Luísa) implementar este arquivo.
 *
 * Dica de implementação da IA básica (mover em direção ao jogador):
 *
 *   Vector2 direcao;
 *   direcao.x = jogador.posicao.x - inimigo.posicao.x;
 *   direcao.y = jogador.posicao.y - inimigo.posicao.y;
 *   // normalizar (igual ao jogador.c)
 *   float len = sqrtf(direcao.x*direcao.x + direcao.y*direcao.y);
 *   if (len > 0) { direcao.x /= len; direcao.y /= len; }
 *   inimigo.posicao.x += direcao.x * inimigo.velocidade_movimento * dt;
 *   inimigo.posicao.y += direcao.y * inimigo.velocidade_movimento * dt;
 * ========================================================================== */

#include "inimigos.h"
#include <stdlib.h>
#include <math.h>

void inimigos_spawnar_em(EstadoJogo *ej, Vector2 pos, TipoInimigo tipo) {
    InimigoNo *novo = malloc(sizeof(InimigoNo));
    if (!novo) return;

    novo->dados.posicao = pos;
    novo->dados.velocidade = (Vector2){0,0};
    novo->dados.vivo = true;
    novo->dados.tipo = tipo;

    switch (tipo) {

        case INIMIGO_CORPO_A_CORPO:
            novo->dados.vida = 30;
            novo->dados.vida_maxima = 30;
            novo->dados.velocidade_movimento = 90.0f;
            novo->dados.raio = 16.0f;
            novo->dados.dano = 1;
            break;

        case INIMIGO_A_DISTANCIA:
            novo->dados.vida = 15;
            novo->dados.vida_maxima = 15;
            novo->dados.velocidade_movimento = 60.0f;
            novo->dados.raio = 14.0f;
            novo->dados.dano = 3;
            break;

        case INIMIGO_ELITE:
            novo->dados.vida = 80;
            novo->dados.vida_maxima = 80;
            novo->dados.velocidade_movimento = 50.0f;
            novo->dados.raio = 20.0f;
            novo->dados.dano = 5;
            break;

        case INIMIGO_CHEFE:
            novo->dados.vida = 200;
            novo->dados.vida_maxima = 200;
            novo->dados.velocidade_movimento = 30.0f;
            novo->dados.raio = 30.0f;
            novo->dados.dano = 10;
            break;
    }

    novo->proximo = ej->inimigos_cabeca;
    ej->inimigos_cabeca = novo;
}

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

void inimigos_desenhar(const EstadoJogo *ej) {
    for (InimigoNo *ino = ej->inimigos_cabeca;
        ino != NULL;
        ino = ino->proximo) {

        if (!ino->dados.vivo) continue;

        Inimigo *i = &ino->dados;

        // razão da vida
        float vida_ratio =
            (float)i->vida / i->vida_maxima;

        Color cor;

        switch (i->tipo) {

            // 🔴 Corpo a corpo
            case INIMIGO_CORPO_A_CORPO:

                cor = (Color){
                    255,
                    (unsigned char)(80 + 175 * (1.0f - vida_ratio)),
                    (unsigned char)(80 + 175 * (1.0f - vida_ratio)),
                    255
                };

                DrawCircleV(i->posicao, i->raio, cor);

                break;

            // 🔵 A distância
            case INIMIGO_A_DISTANCIA:

                cor = (Color){
                    (unsigned char)(80 + 175 * (1.0f - vida_ratio)),
                    (unsigned char)(80 + 175 * (1.0f - vida_ratio)),
                    255,
                    255
                };

                DrawCircleV(i->posicao, i->raio, cor);

                // núcleo branco
                DrawCircleV(i->posicao, 4, WHITE);

                break;

            // 🟠 Elite
            case INIMIGO_ELITE:

                cor = (Color){
                    255,
                    (unsigned char)(120 + 135 * (1.0f - vida_ratio)),
                    0,
                    255
                };

                DrawCircleV(i->posicao, i->raio, cor);

                DrawCircleLines(
                    (int)i->posicao.x,
                    (int)i->posicao.y,
                    i->raio,
                    YELLOW
                );

                break;

            // 🟣 Chefe
            case INIMIGO_CHEFE:

                cor = (Color){
                    180,
                    (unsigned char)(50 + 150 * (1.0f - vida_ratio)),
                    255,
                    255
                };

                DrawCircleV(i->posicao, i->raio, cor);

                // núcleo interno
                DrawCircleV(i->posicao, i->raio / 2, BLACK);

                // contorno
                DrawCircleLines(
                    (int)i->posicao.x,
                    (int)i->posicao.y,
                    i->raio,
                    WHITE
                );

                break;
        }
    }
}

void inimigos_liberar_tudo(EstadoJogo *ej) {
    InimigoNo *atual = ej->inimigos_cabeca;
    while (atual != NULL) {
        InimigoNo *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    ej->inimigos_cabeca = NULL;
}
