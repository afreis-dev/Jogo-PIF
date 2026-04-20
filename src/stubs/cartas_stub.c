/*
 * Arquivo: cartas_stub.c
 * Propósito: reservar o ponto de integração do sistema de cartas.
 * Quem depende dele: cena de upgrade e futuro módulo do Dev 2.
 */

#include "augur/stubs.h"

#include "raylib.h"

void atualizar_stub_cartas(ContextoJogo* contexto)
{
    (void)contexto;

    /*
     * TODO[DEV2]: sortear cartas com base na profecia, wave e estado do jogador.
     * Alterações esperadas: upgrades permanentes na run e possíveis efeitos imediatos.
     */
}

void desenhar_stub_cartas(const ContextoJogo* contexto)
{
    (void)contexto;

    DrawRectangleRounded((Rectangle){92.0f, 240.0f, 330.0f, 220.0f}, 0.03f, 1, (Color){44, 52, 73, 255});
    DrawRectangleRounded((Rectangle){446.0f, 240.0f, 330.0f, 220.0f}, 0.03f, 1, (Color){44, 52, 73, 255});
    DrawRectangleRounded((Rectangle){800.0f, 240.0f, 330.0f, 220.0f}, 0.03f, 1, (Color){44, 52, 73, 255});

    DrawText("Carta 1", 128, 280, 34, RAYWHITE);
    DrawText("Carta 2", 482, 280, 34, RAYWHITE);
    DrawText("Carta 3", 836, 280, 34, RAYWHITE);
    DrawText("TODO[DEV2]: preencher efeito, valor base e roll", 128, 344, 20, (Color){180, 190, 214, 255});
}
