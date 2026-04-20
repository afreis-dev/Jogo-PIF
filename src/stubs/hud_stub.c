/*
 * Arquivo: hud_stub.c
 * Propósito: reservar o ponto de integração do HUD.
 * Quem depende dele: combate e futuro módulo do Dev 2.
 */

#include "augur/stubs.h"

#include "raylib.h"

void atualizar_stub_hud(ContextoJogo* contexto)
{
    (void)contexto;

    /*
     * TODO[DEV2]: adicionar animações, barras e feedback visual persistente do combate.
     * Este stub existe para o core já chamar o HUD sem depender de refatoração futura.
     */
}

void desenhar_stub_hud(const ContextoJogo* contexto)
{
    DrawRectangle(930, 90, 250, 112, Fade(BLACK, 0.35f));
    DrawText(TextFormat("Vida: %d / %d", contexto->jogador.vida_atual, contexto->jogador.vida_maxima),
             950, 110, 24, RAYWHITE);
    DrawText(TextFormat("Biomassa: %d", contexto->jogador.biomassa_run), 950, 142, 24, RAYWHITE);
    DrawText("TODO[DEV2]: HUD final entra aqui", 950, 176, 18, (Color){162, 170, 191, 255});
}
