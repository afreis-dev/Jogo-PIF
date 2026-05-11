/* ============================================================================
 * inimigos.c - ENGINE DE INIMIGOS
 * ============================================================================
 *
 * Lista encadeada de inimigos. Esta camada NÃO conhece tipos específicos —
 * ela orquestra o frame em três passes:
 *
 *   PASS 1 (IA + movimento):
 *     Pra cada inimigo vivo, chama inimigos_tipos_executar_ia(), que escreve
 *     em i->velocidade. Em seguida aplica `posicao += velocidade * dt`.
 *
 *   PASS 2 (push-out inimigo↔inimigo):
 *     Loop O(n²): pra cada par sobrepondo, empurra metade do overlap em cada
 *     direção. Resultado: inimigos não se empilham.
 *
 *   PASS 3 (remoção):
 *     Caminha a lista com ponteiro duplo e dá free() em quem foi marcado
 *     como morto pela colisão.
 *
 * Render usa a cor de PARAMETROS_INIMIGO[tipo].cor — é só a engine olhando
 * a tabela do conteúdo.
 * ============================================================================ */

#include "inimigos.h"
#include "inimigos_tipos.h"
#include <math.h>
#include <stdlib.h>


/* Conta nós da lista — usado pra respeitar MAX_INIMIGOS no spawn. */
static int contar_nos(const InimigoNo *cabeca) {
    int n = 0;
    for (const InimigoNo *p = cabeca; p != NULL; p = p->proximo) n++;
    return n;
}


void inimigos_spawnar_em(EstadoJogo *ej, Vector2 posicao, TipoInimigo tipo) {
    /* Validações de segurança: tipo dentro do enum e limite de lista. */
    if ((int)tipo < 0 || (int)tipo >= QTD_PARAMETROS_INIMIGO) return;
    if (contar_nos(ej->inimigos_cabeca) >= MAX_INIMIGOS) return;

    InimigoNo *novo = (InimigoNo *)malloc(sizeof(InimigoNo));
    if (novo == NULL) return;

    const ParametrosInimigo *p = &PARAMETROS_INIMIGO[tipo];

    novo->dados.posicao              = posicao;
    novo->dados.velocidade           = (Vector2){ 0.0f, 0.0f };
    novo->dados.raio                 = p->raio;
    novo->dados.vida                 = p->vida_base;
    novo->dados.vida_maxima          = p->vida_base;
    novo->dados.dano                 = p->dano;
    novo->dados.velocidade_movimento = p->velocidade_movimento;
    novo->dados.tipo                 = tipo;
    novo->dados.recompensa_biomassa  = p->recompensa_biomassa;
    novo->dados.vivo                 = true;

    novo->proximo            = ej->inimigos_cabeca;
    ej->inimigos_cabeca      = novo;
}


void inimigos_atualizar(EstadoJogo *ej) {
    float dt = ej->delta_tempo;

    /* ----- PASS 1: IA + movimento ----- */
    for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo) {
        if (!ino->dados.vivo) continue;
        inimigos_tipos_executar_ia(&ino->dados, ej);
        ino->dados.posicao.x += ino->dados.velocidade.x * dt;
        ino->dados.posicao.y += ino->dados.velocidade.y * dt;
    }

    /* ----- PASS 2: push-out inimigo↔inimigo (O(n²)) -----
     * Pra cada par (a, b) com a vindo antes de b na lista: se os círculos
     * se sobrepõem, empurra cada um por metade do overlap. Caso degenerado
     * (centros coincidentes): empurra "a" pra direita, deixa "b" parado. */
    for (InimigoNo *a = ej->inimigos_cabeca; a != NULL; a = a->proximo) {
        if (!a->dados.vivo) continue;
        for (InimigoNo *b = a->proximo; b != NULL; b = b->proximo) {
            if (!b->dados.vivo) continue;

            float dx = b->dados.posicao.x - a->dados.posicao.x;
            float dy = b->dados.posicao.y - a->dados.posicao.y;
            float dist2 = dx * dx + dy * dy;
            float soma = a->dados.raio + b->dados.raio;

            if (dist2 < soma * soma && dist2 > 0.0001f) {
                float dist = sqrtf(dist2);
                float overlap = soma - dist;
                float empurrar_x = (dx / dist) * (overlap * 0.5f);
                float empurrar_y = (dy / dist) * (overlap * 0.5f);
                a->dados.posicao.x -= empurrar_x;
                a->dados.posicao.y -= empurrar_y;
                b->dados.posicao.x += empurrar_x;
                b->dados.posicao.y += empurrar_y;
            } else if (dist2 <= 0.0001f) {
                /* Coincidência exata — separa minimamente. */
                a->dados.posicao.x -= 1.0f;
            }
        }
    }

    /* ----- PASS 3: remoção dos mortos (ponteiro duplo) -----
     * `**atual` aponta pro próximo ponteiro a ser modificado: ou a cabeça
     * da lista, ou o campo `proximo` do nó anterior. Permite remover nós
     * sem if especial pra cabeça. */
    InimigoNo **atual = &ej->inimigos_cabeca;
    while (*atual != NULL) {
        if (!(*atual)->dados.vivo) {
            InimigoNo *morto = *atual;
            *atual = morto->proximo;
            free(morto);
        } else {
            atual = &(*atual)->proximo;
        }
    }
}


void inimigos_desenhar(const EstadoJogo *ej) {
    for (const InimigoNo *ino = ej->inimigos_cabeca;
         ino != NULL;
         ino = ino->proximo) {
        if (!ino->dados.vivo) continue;
        if ((int)ino->dados.tipo < 0 ||
            (int)ino->dados.tipo >= QTD_PARAMETROS_INIMIGO) continue;

        const ParametrosInimigo *p = &PARAMETROS_INIMIGO[ino->dados.tipo];
        DrawCircleV(ino->dados.posicao, p->raio_visual, p->cor);
        /* Outline mais escuro pra dar volume ao sprite circular. */
        DrawCircleLines((int)ino->dados.posicao.x,
                        (int)ino->dados.posicao.y,
                        p->raio_visual,
                        (Color){ 0, 0, 0, 150 });
    }
}


void inimigos_liberar_tudo(EstadoJogo *ej) {
    InimigoNo *atual = ej->inimigos_cabeca;
    while (atual != NULL) {
        InimigoNo *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    ej->inimigos_cabeca = NULL;
}
