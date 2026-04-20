/*
 * Arquivo: inimigos_stub.c
 * Propósito: reservar o ponto de integração de inimigos e da lista encadeada.
 * Quem depende dele: combate e futuro módulo do Dev 3.
 */

#include "augur/stubs.h"

#include "raylib.h"

void atualizar_stub_inimigos(ContextoJogo* contexto)
{
    (void)contexto;

    /*
     * TODO[DEV3]: popular contexto->estado_run.lista_inimigos com alocação dinâmica.
     * O módulo poderá ler o jogador e a wave atual para mover e escalar dificuldade.
     */
}

void desenhar_stub_inimigos(const ContextoJogo* contexto)
{
    (void)contexto;
    DrawText("TODO[DEV3] Inimigos entram nesta camada", 120, 110, 20, (Color){212, 124, 124, 255});
}
