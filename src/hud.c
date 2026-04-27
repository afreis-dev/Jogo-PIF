/* ============================================================================
 * hud.c - STUB DO HUD
 * ============================================================================
 *
 * TODO: Dev 2 implementar este arquivo.
 *
 * Sugestao de implementacao de hud_desenhar:
 *
 *   // Barra de vida (canto superior esquerdo)
 *   DrawRectangle(10, 10, 200, 20, DARKGRAY);
 *   int largura_vida = (int)(200 * ej->jogador.vida / ej->jogador.vida_maxima);
 *   DrawRectangle(10, 10, largura_vida, 20, RED);
 *   char buf[32];
 *   snprintf(buf, 32, "HP: %d/%d", ej->jogador.vida, ej->jogador.vida_maxima);
 *   DrawText(buf, 14, 12, 16, WHITE);
 *
 *   // Numero da onda (topo centro)
 *   snprintf(buf, 32, "Onda %d", ej->onda_atual.numero);
 *   DrawText(buf, LARGURA_TELA/2 - 40, 10, 22, GOLD);
 *
 *   // Biomassa (canto superior direito)
 *   snprintf(buf, 32, "Biomassa: %d", ej->jogador.biomassa);
 *   DrawText(buf, LARGURA_TELA - 160, 10, 18, GREEN);
 * ========================================================================== */

#include "hud.h"
#include <stdio.h>

#define HUD_MARGEM         10
#define HUD_BARRA_LARG    200
#define HUD_BARRA_ALT      20
#define HUD_FONTE_PEQUENA  18
#define HUD_FONTE_MEDIA    20
#define HUD_FONTE_GRANDE   22

#define DADO_TAMANHO       40   
#define DADO_ESPACO        10  

void hud_desenhar_barra_vida(const EstadoJogo *ej)
{
    int x = HUD_MARGEM;
    int y = HUD_MARGEM;
    DrawRectangle(x, y, HUD_BARRA_LARG, HUD_BARRA_ALT, DARKGRAY);

    int largura_vida = 0;
    if (ej->jogador.vida_maxima > 0) {
        largura_vida = (int)(HUD_BARRA_LARG * ej->jogador.vida / (float)ej->jogador.vida_maxima);
        }

    float porcentagem_vida;
    if (ej->jogador.vida_maxima > 0){
    porcentagem_vida = (float)ej->jogador.vida / ej->jogador.vida_maxima;
    }
    else{
    porcentagem_vida = 0.0;
    }

    Color cor_vida;
    if (porcentagem_vida >= 0.50) cor_vida = GREEN;
    else if (porcentagem_vida > 0.30) cor_vida = YELLOW;
    else cor_vida = RED;

    DrawRectangle(x, y, largura_vida, HUD_BARRA_ALT, cor_vida);
    DrawRectangleLines(x, y, HUD_BARRA_LARG, HUD_BARRA_ALT, DARKGRAY);

    char buf[32];
    snprintf(buf, sizeof(buf), "HP: %d/%d", ej->jogador.vida, ej->jogador.vida_maxima);
    DrawText(buf, x + 4, y + 6, HUD_FONTE_PEQUENA, WHITE);

}


void hud_desenhar_onda(const EstadoJogo *ej){

    char buf[32];
    snprintf(buf, 32, "Onda %d", ej->onda_atual.numero);
    DrawText(buf, LARGURA_TELA / 2 - 40, HUD_MARGEM, HUD_FONTE_GRANDE ,GOLD);
}