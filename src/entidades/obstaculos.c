/* ============================================================================
 * obstaculos.c - IMPLEMENTACAO DO MODULO DE OBSTACULOS (SANDBOX)
 * ============================================================================
 *
 * Geracao deterministica:
 *   srand(seed * MULT + OFFSET) garante que o stream de rand seja diferente
 *   do que a profecia consumiu, mas continua reproduzivel pra mesma seed.
 *
 * Layout: distribuido em uma area quadrada generosa (-AREA..+AREA) ao redor
 * da origem do mundo. Sao deixadas duas restricoes:
 *   - "zona segura" no spawn do jogador (origem) - nao queremos arvore em
 *     cima dele assim que a run comeca
 *   - sem sobreposicao entre obstaculos (retentativa simples)
 * ========================================================================== */

#include "obstaculos.h"
#include "raylib.h"
#include <stdlib.h>
#include <math.h>


#define AREA_MAPA           2500.0f   /* metade do lado do quadrado de spawn */
#define ZONA_SEGURA_RAIO    180.0f    /* sem obstaculos a menos disso da origem */
#define MAX_TENTATIVAS      8         /* re-roll pra evitar sobreposicao */

#define QTD_MIN             16
#define QTD_MAX             32


/* ----- HELPERS ----- */

static float rand_float(float minimo, float maximo) {
    float t = (float)rand() / (float)RAND_MAX;
    return minimo + t * (maximo - minimo);
}

/* Verifica se um candidato (pos, raio) entra em conflito com algum dos
 * obstaculos ja existentes. */
static bool conflita_com_existentes(const EstadoJogo *ej, Vector2 pos, float raio) {
    for (int i = 0; i < ej->qtd_obstaculos; i++) {
        const Obstaculo *o = &ej->obstaculos[i];
        float dx = pos.x - o->posicao.x;
        float dy = pos.y - o->posicao.y;
        float min_dist = raio + o->raio + 8.0f;   /* respiro de 8px */
        if (dx * dx + dy * dy < min_dist * min_dist) {
            return true;
        }
    }
    return false;
}


/* ----- GERAR ----- */
void obstaculos_gerar(EstadoJogo *ej, unsigned int seed) {
    /* Stream proprio: nao reutiliza o que a profecia consumiu, mas continua
     * deterministico pra mesma seed da run. */
    srand(seed * 2654435761u + 12345u);

    int qtd_alvo = QTD_MIN + rand() % (QTD_MAX - QTD_MIN + 1);
    if (qtd_alvo > MAX_OBSTACULOS) qtd_alvo = MAX_OBSTACULOS;

    ej->qtd_obstaculos = 0;

    for (int i = 0; i < qtd_alvo; i++) {
        /* Sortei tipo, raio compativel com o tipo, e posicao. Tento de novo
         * se cair em cima de algum ja existente ou na zona segura. */
        bool ok = false;
        for (int t = 0; t < MAX_TENTATIVAS && !ok; t++) {
            TipoObstaculo tipo = (TipoObstaculo)(rand() % OBSTACULO_TIPO_TOTAL);

            float raio;
            if (tipo == OBSTACULO_ARVORE) raio = rand_float(24.0f, 34.0f);
            else                          raio = rand_float(18.0f, 28.0f);

            Vector2 pos = {
                rand_float(-AREA_MAPA, AREA_MAPA),
                rand_float(-AREA_MAPA, AREA_MAPA)
            };

            /* Zona segura: nao spawnar em cima do jogador inicial. */
            if (pos.x * pos.x + pos.y * pos.y < ZONA_SEGURA_RAIO * ZONA_SEGURA_RAIO) {
                continue;
            }

            if (conflita_com_existentes(ej, pos, raio)) {
                continue;
            }

            ej->obstaculos[ej->qtd_obstaculos].posicao = pos;
            ej->obstaculos[ej->qtd_obstaculos].raio    = raio;
            ej->obstaculos[ej->qtd_obstaculos].tipo    = tipo;
            ej->qtd_obstaculos++;
            ok = true;
        }
    }
}


/* ----- DESENHO -----
 * Arvore: copa verde + tronco marrom (retangulo pequeno na base).
 * Pedra: dois tons de cinza pra dar volume - circulo escuro + reflexo claro
 * em cima/esquerda. */
void obstaculos_desenhar(const EstadoJogo *ej) {
    for (int i = 0; i < ej->qtd_obstaculos; i++) {
        const Obstaculo *o = &ej->obstaculos[i];

        if (o->tipo == OBSTACULO_ARVORE) {
            /* Tronco: retangulo marrom abaixo da copa. */
            Color cor_tronco = (Color){ 80, 50, 30, 255 };
            int largura_tronco = (int)(o->raio * 0.35f);
            int altura_tronco  = (int)(o->raio * 0.60f);
            DrawRectangle((int)(o->posicao.x - largura_tronco / 2),
                          (int)o->posicao.y,
                          largura_tronco, altura_tronco, cor_tronco);

            /* Copa: dois tons de verde pra dar profundidade. */
            DrawCircleV(o->posicao, o->raio, (Color){ 30, 90, 40, 255 });
            DrawCircleV((Vector2){ o->posicao.x - o->raio * 0.25f,
                                   o->posicao.y - o->raio * 0.25f },
                        o->raio * 0.55f, (Color){ 60, 130, 60, 255 });
        } else { /* OBSTACULO_PEDRA */
            DrawCircleV(o->posicao, o->raio, (Color){ 90, 90, 100, 255 });
            DrawCircleV((Vector2){ o->posicao.x - o->raio * 0.30f,
                                   o->posicao.y - o->raio * 0.30f },
                        o->raio * 0.55f, (Color){ 140, 140, 150, 255 });
            DrawCircleLines((int)o->posicao.x, (int)o->posicao.y, o->raio,
                            (Color){ 50, 50, 60, 255 });
        }
    }
}


/* ----- HELPER COMUM DE PUSH-OUT -----
 * Empurra (pos) pra fora de (centro_obstaculo, raio_obstaculo) considerando
 * a soma dos raios. Modifica pos no lugar. */
static void empurrar_para_fora(Vector2 *pos, float raio_movel,
                                Vector2 centro_obs, float raio_obs) {
    float dx = pos->x - centro_obs.x;
    float dy = pos->y - centro_obs.y;
    float dist2 = dx * dx + dy * dy;
    float soma_raios = raio_movel + raio_obs;

    if (dist2 < soma_raios * soma_raios) {
        if (dist2 > 0.0001f) {
            float distancia = sqrtf(dist2);
            float overlap   = soma_raios - distancia;
            pos->x += (dx / distancia) * overlap;
            pos->y += (dy / distancia) * overlap;
        } else {
            /* Centros sobrepostos: empurra arbitrariamente pra direita. */
            pos->x += soma_raios;
        }
    }
}


/* ----- RESOLVER JOGADOR ----- */
void obstaculos_resolver_jogador(EstadoJogo *ej) {
    for (int i = 0; i < ej->qtd_obstaculos; i++) {
        const Obstaculo *o = &ej->obstaculos[i];
        empurrar_para_fora(&ej->jogador.posicao, ej->jogador.raio,
                            o->posicao, o->raio);
    }
}


/* ----- RESOLVER INIMIGOS ----- */
void obstaculos_resolver_inimigos(EstadoJogo *ej) {
    for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo) {
        if (!ino->dados.vivo) continue;
        for (int i = 0; i < ej->qtd_obstaculos; i++) {
            const Obstaculo *o = &ej->obstaculos[i];
            empurrar_para_fora(&ino->dados.posicao, ino->dados.raio,
                                o->posicao, o->raio);
        }
    }
}
