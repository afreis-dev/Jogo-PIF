/*
 * Arquivo: janela.c
 * Propósito: encapsular a criação e o encerramento da janela principal.
 * Quem depende dele: contexto_jogo.c.
 */

#include "raylib.h"

#include "core_interno.h"

void configurar_janela(ContextoJogo* contexto)
{
    InitWindow(contexto->largura_janela, contexto->altura_janela, "AUGUR");
    SetTargetFPS(60);

    /*
     * O Escape fica livre para navegação interna entre cenas.
     * Isso evita fechar o jogo sem querer durante os testes.
     */
    SetExitKey(KEY_NULL);
}

void encerrar_janela(void)
{
    CloseWindow();
}
