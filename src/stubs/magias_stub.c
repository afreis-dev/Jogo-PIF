/*
 * Arquivo: magias_stub.c
 * Propósito: reservar o ponto de integração de magias e projéteis.
 * Quem depende dele: combate e futuro módulo do Dev 3.
 */

#include "augur/stubs.h"

#include "raylib.h"

void atualizar_stub_magias(ContextoJogo* contexto)
{
    (void)contexto;

    /*
     * TODO[DEV3]: ler contexto->jogador.magias_equipadas e gerar projéteis na lista apropriada.
     * O core já chama este ponto em todo frame de combate.
     */
}

void desenhar_stub_magias(const ContextoJogo* contexto)
{
    (void)contexto;
    DrawText("TODO[DEV3] Magias e projéteis entram nesta camada", 120, 136, 20, (Color){122, 201, 244, 255});
}
