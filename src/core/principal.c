/*
 * Arquivo: principal.c
 * Propósito: servir como ponto de entrada enxuto do jogo.
 * Quem depende dele: ninguém diretamente; ele apenas amarra o ciclo principal.
 */

#include "augur/augur.h"
#include "raylib.h"

int main(void)
{
    ContextoJogo contexto;

    iniciar_contexto(&contexto);

    while (!WindowShouldClose()) {
        atualizar_contexto(&contexto);
        desenhar_contexto(&contexto);
    }

    finalizar_contexto(&contexto);

    return 0;
}
