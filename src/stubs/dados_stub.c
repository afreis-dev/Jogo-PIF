/*
 * Arquivo: dados_stub.c
 * Propósito: reservar o ponto de integração do sistema de dados.
 * Quem depende dele: cena de upgrade e futuro módulo do Dev 2.
 */

#include "augur/stubs.h"

#include "raylib.h"

void atualizar_stub_dados(ContextoJogo* contexto)
{
    (void)contexto;

    /*
     * TODO[DEV2]: consumir os dados escolhidos e aplicar risco de roll nas cartas.
     * O contexto já expõe profecia, jogador e estado da run para orientar essa lógica.
     */
}

void desenhar_stub_dados(const ContextoJogo* contexto)
{
    DrawText(TextFormat("Dados equipados: d%d e d%d",
                        contexto->jogador.dados_escolhidos[0],
                        contexto->jogador.dados_escolhidos[1]),
             92, 540, 24, RAYWHITE);
    DrawText("TODO[DEV2]: exibir roll, risco e resultado aplicado", 92, 574, 22,
             (Color){162, 170, 191, 255});
}
