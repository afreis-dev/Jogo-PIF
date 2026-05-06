/* ============================================================================
 * inimigos_tipos.c - TABELA DE STATS + IA POR TIPO (LUÍSA EDITA AQUI)
 * ============================================================================
 *
 * Os números abaixo são chutes razoáveis pra começar — ajuste conforme o
 * playtest pedir. Não precisa mexer em mais nenhum arquivo da engine quando
 * for balancear: tudo que a engine faz vem das tabelas e do dispatch de IA.
 * ============================================================================ */

#include "inimigos_tipos.h"
#include "inimigos.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>


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


/* Hash determinístico de um ponteiro pra um float em [-1, 1]. Usado pra dar
 * "personalidade" estável a um inimigo sem precisar de um campo id. Como o
 * endereço do nó não muda enquanto ele vive, o valor é constante por toda a
 * vida do inimigo. Multiplicação com a constante de Knuth dispersa bits. */
static float hash_pointer_para_unitario(const void *p) {
    uintptr_t x = (uintptr_t)p;
    uint32_t h = (uint32_t)(x ^ (x >> 16));
    h *= 2654435761u;
    /* Range [0, 1) → [-1, 1) */
    return ((float)h / 4294967296.0f) * 2.0f - 1.0f;
}


/* Chase: anda na direção do jogador, com pequena perturbação angular única
 * por inimigo. Antes, todos vinham em linha reta e aglomeravam num cone
 * único; agora cada um aproxima por um leve flanco diferente, espalhando
 * naturalmente os vetores de aproximação. */
static void ia_chase(Inimigo *i, EstadoJogo *ej) {
    Vector2 dir = direcao_ate_jogador(i, ej);
    if (dir.x == 0.0f && dir.y == 0.0f) {
        i->velocidade.x = 0.0f;
        i->velocidade.y = 0.0f;
        return;
    }

    /* Offset angular fixo por inimigo, no intervalo ~ ±0.4 rad (~±23°).
     * Suficiente pra desfilar lateralmente sem perder a sensação de "vai
     * pra cima do jogador". */
    float offset = hash_pointer_para_unitario(i) * 0.4f;
    float c = cosf(offset);
    float s = sinf(offset);

    /* Rotação 2D do vetor direção pelo offset. */
    float rx = dir.x * c - dir.y * s;
    float ry = dir.x * s + dir.y * c;

    i->velocidade.x = rx * i->velocidade_movimento;
    i->velocidade.y = ry * i->velocidade_movimento;
}


/* Kiter: forma um círculo coordenado em volta do jogador, fechando ângulos
 * de fuga. Cada kiter:
 *   1. Olha pra lista de inimigos e descobre quantos kiters vivos existem (N)
 *      e qual é o índice "ordenado por ângulo polar" deste kiter (k).
 *   2. Calcula a posição-alvo do seu slot no círculo:
 *        raio  = RAIO_BASE + 18 * N            (mais kiters → círculo maior)
 *        ang   = base + 2π * k / N             (espaçamento uniforme)
 *        base  = tempo_total * VEL_ORBITAL     (rotação lenta comum a todos)
 *   3. Move em direção ao alvo. Zona morta de 6px evita jitter.
 *
 * Sem campo de id na struct: o "k" é recalculado todo frame ordenando os
 * kiters pelo ângulo polar atual ao redor do jogador. Isso preserva a
 * proximidade — um kiter já no flanco direito tende a ficar no flanco
 * direito mesmo se outro for adicionado/removido. Custo O(N²) por frame
 * (cada kiter olha todos os outros), aceitável pra N pequeno.
 *
 * Disparo de projétil pelo kiter ainda é TODO; por enquanto só
 * posicionamento. Mas o cerco já fecha rotas de fuga, criando pressão real. */
static void ia_kiter(Inimigo *i, EstadoJogo *ej) {
    const float RAIO_BASE        = 260.0f;
    const float RAIO_POR_KITER   =  18.0f;
    const float VEL_ORBITAL      =   0.15f;   /* rad/s — rotação lenta comum */
    const float ZONA_MORTA       =   6.0f;    /* px */
    const float TWO_PI           =   6.28318530718f;

    /* Vetor do jogador até este kiter — base pra calcular o ângulo polar. */
    float meu_dx = i->posicao.x - ej->jogador.posicao.x;
    float meu_dy = i->posicao.y - ej->jogador.posicao.y;
    float meu_ang = atan2f(meu_dy, meu_dx);

    /* Conta kiters vivos e descobre o índice ordenado deste kiter. Como
     * atan2 retorna em (-π, π], "menor ângulo" tem ordenação total bem
     * definida; em caso de empate exato, desempata pelo endereço pra
     * garantir que dois kiters não disputem o mesmo slot. */
    int N = 0;
    int k = 0;
    for (const InimigoNo *no = ej->inimigos_cabeca;
         no != NULL;
         no = no->proximo) {
        if (!no->dados.vivo) continue;
        if (no->dados.tipo != INIMIGO_A_DISTANCIA) continue;

        N++;

        if (&no->dados == i) continue;   /* não conta a si mesmo no índice */

        float odx = no->dados.posicao.x - ej->jogador.posicao.x;
        float ody = no->dados.posicao.y - ej->jogador.posicao.y;
        float oang = atan2f(ody, odx);

        if (oang < meu_ang) {
            k++;
        } else if (oang == meu_ang && (uintptr_t)&no->dados < (uintptr_t)i) {
            k++;
        }
    }

    if (N <= 0) N = 1;   /* defensivo; não deveria acontecer */

    /* Posição alvo do slot. */
    float raio_circulo = RAIO_BASE + RAIO_POR_KITER * (float)N;
    float ang_base     = ej->tempo_total * VEL_ORBITAL;
    float ang_slot     = ang_base + (TWO_PI * (float)k) / (float)N;

    float alvo_x = ej->jogador.posicao.x + raio_circulo * cosf(ang_slot);
    float alvo_y = ej->jogador.posicao.y + raio_circulo * sinf(ang_slot);

    /* Move em direção ao alvo. */
    float dx = alvo_x - i->posicao.x;
    float dy = alvo_y - i->posicao.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < ZONA_MORTA) {
        i->velocidade.x = 0.0f;
        i->velocidade.y = 0.0f;
        return;
    }

    i->velocidade.x = (dx / dist) * i->velocidade_movimento;
    i->velocidade.y = (dy / dist) * i->velocidade_movimento;
}


/* Boss: chase implacável com fases por % de vida.
 *   - Acima de 50%:  velocidade base.
 *   - Abaixo de 50%: 1.25× velocidade.
 *   - Abaixo de 25%: 1.50× velocidade + spawna 1 corpo a corpo a cada 5s.
 *
 * O timer dos minions vive como `static float` interno à função: como o
 * cronograma garante que existe no máximo 1 chefão na run inteira, não há
 * conflito entre múltiplas instâncias compartilhando o timer. Reset em
 * jogo_resetar_run() não toca isso, mas o timer começa em 0 no primeiro
 * uso e só conta enquanto pct < 0.25, então qualquer "lixo" residual é
 * sobrescrito pelo `>= INTERVALO_MINIONS` na primeira passagem. */
static void ia_boss_fases(Inimigo *i, EstadoJogo *ej) {
    static float timer_minions = 0.0f;
    const float INTERVALO_MINIONS = 5.0f;
    const float OFFSET_MINION     = 80.0f;   /* spawn perto do boss */

    Vector2 dir = direcao_ate_jogador(i, ej);

    float modificador_velocidade = 1.0f;
    float pct = 1.0f;
    if (i->vida_maxima > 0) {
        pct = (float)i->vida / (float)i->vida_maxima;
        if (pct < 0.50f) modificador_velocidade = 1.25f;
        if (pct < 0.25f) modificador_velocidade = 1.50f;
    }

    i->velocidade.x = dir.x * i->velocidade_movimento * modificador_velocidade;
    i->velocidade.y = dir.y * i->velocidade_movimento * modificador_velocidade;

    /* Fase 3 (< 25% vida): chama reforços. */
    if (pct < 0.25f) {
        timer_minions += ej->delta_tempo;
        if (timer_minions >= INTERVALO_MINIONS) {
            timer_minions = 0.0f;

            float ang = ((float)rand() / (float)RAND_MAX) * 6.28318530718f;
            Vector2 pos = {
                i->posicao.x + cosf(ang) * OFFSET_MINION,
                i->posicao.y + sinf(ang) * OFFSET_MINION,
            };
            inimigos_spawnar_em(ej, pos, INIMIGO_CORPO_A_CORPO);
        }
    } else {
        /* Fora da fase final, mantém o timer pronto pra disparar no
         * primeiro tick assim que a vida cair abaixo de 25%. */
        timer_minions = INTERVALO_MINIONS;
    }
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
