/* ============================================================================
 * cronograma_eventos.c - TIMELINE DE 15 MINUTOS (LUÍSA EDITA AQUI)
 * ============================================================================
 *
 * Esta é a "linha do tempo" da run. Cada linha da tabela diz:
 *   "Do tempo X ao tempo Y, spawnar inimigos do tipo T a cada Z segundos".
 *
 * A engine (cronograma.c) lê essa tabela e faz o resto: copia pra um buffer
 * mutável, mantém timers, pede pros inimigos.c spawnarem na hora certa.
 *
 * ----------------------------------------------------------------------------
 * COMO ADICIONAR UMA NOVA FASE NA TIMELINE:
 *   1. Acrescentar uma linha na tabela abaixo, no formato:
 *        { tempo_inicio, tempo_fim, TIPO, intervalo_spawn, 0.0f, false }
 *   2. Os campos finais (timer_interno, ativo) ficam zerados — a engine
 *      preenche em runtime quando copia a tabela.
 *   3. Tempos em SEGUNDOS. Pra "minuto 5" use 5 * 60 = 300.
 *   4. Pra um spawn ÚNICO (chefão), use intervalo_spawn = 1e9 ou só confie
 *      no fato de que o evento desativa quando passa do tempo_fim.
 *
 * REGRAS DE OURO:
 *   - O CHEFÃO (INIMIGO_CHEFE) é spawnado pela engine automaticamente aos
 *     15:00. NÃO precisa ter linha pra ele aqui — mas se quiser sub-bosses
 *     em outros tempos, a tabela aceita.
 *   - Múltiplos eventos podem rodar ao mesmo tempo. Querer melee + ranged
 *     juntos a partir dos 2:00? Duas linhas, ambas começando em 120.
 *
 * INFINITY (math.h) marca "evento sem fim" — usado quando você quer que o
 * spawn dure até o chefão aparecer e parar tudo.
 * ============================================================================ */

#include "cronograma_eventos.h"
#include <math.h>      /* INFINITY */

const EventoCronograma EVENTOS_CRONOGRAMA[] = {
    /* tempo_inicio,  tempo_fim,    tipo,                    intervalo, timer, ativo */

    /* Onda 1 — corpo a corpo desde o início, baixa pressão. */
    { 0.0f,           15.0f * 60.0f, INIMIGO_CORPO_A_CORPO,   1.5f,      0.0f,  false },

    /* Aos 2:00 entram os ranged. Player precisa começar a se mover mais. */
    { 2.0f * 60.0f,   15.0f * 60.0f, INIMIGO_A_DISTANCIA,     3.0f,      0.0f,  false },

    /* Aos 5:00 elites começam a aparecer (raros). */
    { 5.0f * 60.0f,   15.0f * 60.0f, INIMIGO_ELITE,           8.0f,      0.0f,  false },

    /* Aos 8:00 melee acelera (sobreposto à linha de cima — fica mais denso). */
    { 8.0f * 60.0f,   15.0f * 60.0f, INIMIGO_CORPO_A_CORPO,   0.8f,      0.0f,  false },

    /* Aos 10:00 elites ficam mais frequentes. Caos cresce na reta final. */
    { 10.0f * 60.0f,  15.0f * 60.0f, INIMIGO_ELITE,           5.0f,      0.0f,  false },

    /* Aos 12:00 ranged também acelera. */
    { 12.0f * 60.0f,  15.0f * 60.0f, INIMIGO_A_DISTANCIA,     1.5f,      0.0f,  false },
};

const int QTD_EVENTOS_CRONOGRAMA =
    sizeof(EVENTOS_CRONOGRAMA) / sizeof(EVENTOS_CRONOGRAMA[0]);
