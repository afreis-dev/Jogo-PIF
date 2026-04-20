/*
 * Arquivo: tempo.c
 * Propósito: atualizar os relógios globais do jogo a cada frame.
 * Quem depende dele: contexto_jogo.c e cenas.c.
 */

#include "raylib.h"

#include "core_interno.h"

void atualizar_tempo_jogo(ContextoJogo* contexto)
{
    contexto->tempo_delta = GetFrameTime();
    contexto->tempo_total += contexto->tempo_delta;

    if (contexto->cena_atual == CENA_COMBATE) {
        contexto->estado_run.tempo_da_run += contexto->tempo_delta;
    }
}
