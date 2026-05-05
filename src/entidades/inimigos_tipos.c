/* ============================================================================
 * inimigos_tipos.c - TABELA DE STATS + IA POR TIPO (LUÍSA EDITA AQUI)
 * ============================================================================
 *
 * Os números abaixo são chutes razoáveis pra começar — ajuste conforme o
 * playtest pedir. Não precisa mexer em mais nenhum arquivo da engine quando
 * for balancear: tudo que a engine faz vem das tabelas e do dispatch de IA.
 * ============================================================================ */

#include "inimigos_tipos.h"
#include <math.h>


/* ----------------------------------------------------------------------------
 * 1. TABELA DE STATS POR TIPO
 * ----------------------------------------------------------------------------
 * A ordem das linhas TEM que casar com a ordem do enum TipoInimigo:
 *   [0] INIMIGO_CORPO_A_CORPO
 *   [1] INIMIGO_A_DISTANCIA
 *   [2] INIMIGO_ELITE
 *   [3] INIMIGO_CHEFE
 *
 * Pra adicionar um tipo novo: adicione no enum (tipos.h) E na tabela aqui.
 * --------------------------------------------------------------------------*/
const ParametrosInimigo PARAMETROS_INIMIGO[] = {
    /* corpo a corpo — base do jogo, anda direto no jogador. */
    {
        .vida_base            = 30,
        .dano                 = 10,
        .velocidade_movimento = 90.0f,
        .raio                 = 14.0f,
        .raio_visual          = 14.0f,
        .cor                  = (Color){ 220,  60,  60, 255 },  /* vermelho */
        .recompensa_biomassa  = 5,
        .comportamento        = IA_CHASE,
    },

    /* à distância — mantém afastado e atira. */
    {
        .vida_base            = 20,
        .dano                 = 8,
        .velocidade_movimento = 60.0f,
        .raio                 = 12.0f,
        .raio_visual          = 12.0f,
        .cor                  = (Color){ 240, 140,  40, 255 },  /* laranja */
        .recompensa_biomassa  = 7,
        .comportamento        = IA_KITER,
    },

    /* elite — corpo a corpo mais forte e mais rápido. */
    {
        .vida_base            = 90,
        .dano                 = 18,
        .velocidade_movimento = 110.0f,
        .raio                 = 20.0f,
        .raio_visual          = 20.0f,
        .cor                  = (Color){ 160,  80, 220, 255 },  /* roxo */
        .recompensa_biomassa  = 25,
        .comportamento        = IA_CHASE,
    },

    /* chefão — encerra a run. Tank com fases por % de vida. */
    {
        .vida_base            = 1500,
        .dano                 = 30,
        .velocidade_movimento = 70.0f,
        .raio                 = 45.0f,
        .raio_visual          = 50.0f,
        .cor                  = (Color){ 240, 200,  60, 255 },  /* dourado */
        .recompensa_biomassa  = 500,
        .comportamento        = IA_BOSS_FASES,
    },

    /* === SUGESTÃO PRÓXIMA: elite à distância (deixa pra depois) ===
     * Pra criar, adicione INIMIGO_ELITE_DISTANCIA no enum em tipos.h e
     * acrescente uma linha aqui copiando o ranged base e bombando os
     * stats (vida ~70, dano ~16, velocidade ~80, cor ciano). Reuse
     * IA_KITER. */
};

const int QTD_PARAMETROS_INIMIGO =
    sizeof(PARAMETROS_INIMIGO) / sizeof(PARAMETROS_INIMIGO[0]);


/* ----------------------------------------------------------------------------
 * 2. FUNÇÕES DE IA
 * ----------------------------------------------------------------------------
 * Cada função recebe o inimigo e o estado do jogo, e ESCREVE em i->velocidade
 * o vetor de movimento desejado neste frame. A engine aplica a posição em
 * seguida (posicao += velocidade * dt).
 *
 * Como são `static`, ficam internas a este arquivo — o exterior só vê o
 * dispatch inimigos_tipos_executar_ia().
 * --------------------------------------------------------------------------*/

/* Helper: vetor unitário do inimigo até o jogador. Retorna (0,0) se for o
 * caso degenerado de coincidência exata. */
static Vector2 direcao_ate_jogador(const Inimigo *i, const EstadoJogo *ej) {
    float dx = ej->jogador.posicao.x - i->posicao.x;
    float dy = ej->jogador.posicao.y - i->posicao.y;
    float comprimento = sqrtf(dx * dx + dy * dy);
    if (comprimento < 0.0001f) return (Vector2){ 0.0f, 0.0f };
    return (Vector2){ dx / comprimento, dy / comprimento };
}


/* Chase: anda direto na direção do jogador. */
static void ia_chase(Inimigo *i, EstadoJogo *ej) {
    Vector2 dir = direcao_ate_jogador(i, ej);
    i->velocidade.x = dir.x * i->velocidade_movimento;
    i->velocidade.y = dir.y * i->velocidade_movimento;
}


/* Kiter: tenta manter distância "ideal". Anda na direção do jogador se
 * estiver longe, recua se estiver perto demais. (Disparo de projétil
 * inimigo é um TODO futuro — por ora só posicionamento.) */
static void ia_kiter(Inimigo *i, EstadoJogo *ej) {
    const float DISTANCIA_IDEAL = 220.0f;

    float dx = ej->jogador.posicao.x - i->posicao.x;
    float dy = ej->jogador.posicao.y - i->posicao.y;
    float distancia = sqrtf(dx * dx + dy * dy);
    if (distancia < 0.0001f) {
        i->velocidade.x = 0.0f;
        i->velocidade.y = 0.0f;
        return;
    }

    /* Sinal: 1 quando longe demais (aproxima), -1 quando perto demais (recua),
     * 0 quando dentro de uma faixa morta perto da distância ideal. */
    float sinal = 0.0f;
    if (distancia > DISTANCIA_IDEAL + 30.0f) sinal =  1.0f;
    else if (distancia < DISTANCIA_IDEAL - 30.0f) sinal = -1.0f;

    i->velocidade.x = (dx / distancia) * sinal * i->velocidade_movimento;
    i->velocidade.y = (dy / distancia) * sinal * i->velocidade_movimento;
}


/* Boss: chase mais lento porém implacável. Por enquanto reutiliza chase —
 * deixei o caso separado pra ficar fácil acrescentar fases por % de vida
 * (ex.: abaixo de 50% acelera; abaixo de 25% spawna minions). */
static void ia_boss_fases(Inimigo *i, EstadoJogo *ej) {
    Vector2 dir = direcao_ate_jogador(i, ej);

    float modificador_velocidade = 1.0f;
    if (i->vida_maxima > 0) {
        float pct = (float)i->vida / (float)i->vida_maxima;
        if (pct < 0.50f) modificador_velocidade = 1.25f;
        if (pct < 0.25f) modificador_velocidade = 1.50f;
    }

    i->velocidade.x = dir.x * i->velocidade_movimento * modificador_velocidade;
    i->velocidade.y = dir.y * i->velocidade_movimento * modificador_velocidade;
}


/* Dispatcher chamado pela engine. Olha o comportamento do tipo (lido da
 * tabela) e escolhe a função certa. Pra adicionar IA inédita: novo valor
 * em ComportamentoIA, novo case aqui. */
void inimigos_tipos_executar_ia(Inimigo *i, EstadoJogo *ej) {
    /* A tabela é indexada pelo tipo. Se o tipo for inválido (não deveria
     * acontecer), trata como chase silenciosamente. */
    if ((int)i->tipo < 0 || (int)i->tipo >= QTD_PARAMETROS_INIMIGO) {
        ia_chase(i, ej);
        return;
    }

    switch (PARAMETROS_INIMIGO[i->tipo].comportamento) {
        case IA_CHASE:      ia_chase(i, ej);      break;
        case IA_KITER:      ia_kiter(i, ej);      break;
        case IA_BOSS_FASES: ia_boss_fases(i, ej); break;
        default:            ia_chase(i, ej);      break;
    }
}
