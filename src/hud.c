/* ============================================================================
 * hud.c - STUB DO HUD
 * ============================================================================*/
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
    DrawText(buf, x + 4, y + 2, HUD_FONTE_PEQUENA, WHITE);

}


void hud_desenhar_onda(const EstadoJogo *ej){

    char buf[32];
    snprintf(buf, 32, "Onda %d", ej->onda_atual.numero);
    DrawText(buf, LARGURA_TELA / 2 - 40, HUD_MARGEM, HUD_FONTE_GRANDE ,GOLD);
}

void hud_desenhar_biomassa(const EstadoJogo *ej)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", ej->jogador.biomassa);

    
    int moeda_raio = HUD_MARGEM - 2;
    int moeda_x = HUD_MARGEM + moeda_raio;
    int moeda_y = HUD_MARGEM + HUD_BARRA_ALT + HUD_MARGEM + moeda_raio;
    
    DrawCircle(moeda_x, moeda_y, moeda_raio + 2, GOLD);
    DrawCircle(moeda_x, moeda_y, moeda_raio, YELLOW);
    int cifrao = MeasureText("B", HUD_FONTE_PEQUENA);
    DrawText("$", moeda_x - cifrao / 2, moeda_y - HUD_FONTE_PEQUENA / 2, HUD_FONTE_PEQUENA, GOLD);
    
    int quant_moeda_x = moeda_x + moeda_raio + 7;
    int quant_moeda_y = moeda_y - HUD_FONTE_MEDIA / 2;

    DrawText(buf, quant_moeda_x, quant_moeda_y, HUD_FONTE_MEDIA, WHITE);
}

void desenhar_hud(const EstadoJogo *ej){
    hud_desenhar_barra_vida(ej);
    hud_desenhar_onda(ej);
    hud_desenhar_biomassa(ej);
}