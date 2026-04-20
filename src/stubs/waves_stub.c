/*
 * Arquivo: waves_stub.c
 * Propósito: reservar o ponto de integração do gerador de waves.
 * Quem depende dele: combate e futuro módulo do Dev 3.
 */

#include "augur/stubs.h"

#include "raylib.h"

void atualizar_stub_waves(ContextoJogo* contexto)
{
    (void)contexto;

    /*
     * TODO[DEV3]: decidir quando iniciar e encerrar waves, além de disparar spawns.
     * O campo contexto->estado_run.wave_atual já está pronto para escalar a run.
     */
}

void desenhar_stub_waves(const ContextoJogo* contexto)
{
    DrawText(TextFormat("TODO[DEV3] Wave procedural %d", contexto->estado_run.wave_atual),
             120, 162, 20, (Color){145, 214, 170, 255});
}
