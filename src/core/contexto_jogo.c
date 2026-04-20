/*
 * Arquivo: contexto_jogo.c
 * Propósito: centralizar o contexto global do jogo.
 * Quem depende dele: principal.c e os módulos que usam a API pública de contexto.
 */

#include "augur/contexto.h"

#include <string.h>

#include "raylib.h"

#include "core_interno.h"

static void iniciar_jogador_base(Jogador* jogador, int largura_tela, int altura_tela)
{
    jogador->posicao_x = largura_tela * 0.5f;
    jogador->posicao_y = altura_tela * 0.5f;
    jogador->velocidade_movimento = 320.0f;
    jogador->raio_colisao = 18.0f;
    jogador->vida_maxima = 5;
    jogador->vida_atual = jogador->vida_maxima;
    jogador->magias_equipadas = NULL;
    jogador->dados_escolhidos[0] = 6;
    jogador->dados_escolhidos[1] = 6;
    jogador->biomassa_run = 0;
}

static void desenhar_debug(const ContextoJogo* contexto)
{
    const int posicao_x = 16;
    int posicao_y = 16;

    DrawRectangle(posicao_x - 8, posicao_y - 8, 260, 122, Fade(BLACK, 0.65f));
    DrawText(TextFormat("Cena: %d", contexto->cena_atual), posicao_x, posicao_y, 20, RAYWHITE);
    posicao_y += 24;
    DrawText(TextFormat("Delta: %.4f", contexto->tempo_delta), posicao_x, posicao_y, 20, RAYWHITE);
    posicao_y += 24;
    DrawText(TextFormat("Jogador: %.0f x %.0f", contexto->jogador.posicao_x, contexto->jogador.posicao_y),
             posicao_x, posicao_y, 20, RAYWHITE);
    posicao_y += 24;
    DrawText(TextFormat("Wave: %d", contexto->estado_run.wave_atual), posicao_x, posicao_y, 20, RAYWHITE);
    posicao_y += 24;
    DrawText(TextFormat("Seed: %u", contexto->estado_run.seed_atual), posicao_x, posicao_y, 20, RAYWHITE);
}

void iniciar_contexto(ContextoJogo* contexto)
{
    memset(contexto, 0, sizeof(*contexto));

    contexto->largura_janela = LARGURA_PADRAO_JANELA;
    contexto->altura_janela = ALTURA_PADRAO_JANELA;
    contexto->cena_atual = CENA_MENU;
    contexto->debug_ativo = false;

    iniciar_jogador_base(&contexto->jogador, contexto->largura_janela, contexto->altura_janela);
    configurar_janela(contexto);
}

void atualizar_contexto(ContextoJogo* contexto)
{
    atualizar_tempo_jogo(contexto);
    atualizar_entrada_jogo(&contexto->entrada);

    if (contexto->entrada.alternar_debug) {
        contexto->debug_ativo = !contexto->debug_ativo;
    }

    atualizar_cena_atual(contexto);
}

void desenhar_contexto(const ContextoJogo* contexto)
{
    BeginDrawing();
    ClearBackground((Color){18, 18, 28, 255});

    desenhar_cena_atual(contexto);

    if (contexto->debug_ativo) {
        desenhar_debug(contexto);
    }

    EndDrawing();
}

void finalizar_contexto(ContextoJogo* contexto)
{
    (void)contexto;
    encerrar_janela();
}

void trocar_cena(ContextoJogo* contexto, IdCena nova_cena)
{
    contexto->cena_atual = nova_cena;

    if (nova_cena == CENA_INICIO_RUN) {
        reiniciar_run_base(contexto);
    }
}
