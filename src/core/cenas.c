/*
 * Arquivo: cenas.c
 * Propósito: controlar o fluxo entre telas e a jogabilidade mínima desta base.
 * Quem depende dele: contexto_jogo.c e, futuramente, os stubs de integração.
 */

#include <math.h>
#include <stddef.h>

#include "augur/contexto.h"
#include "raylib.h"

#include "core_interno.h"

static const Rectangle AREA_COMBATE = { 90.0f, 90.0f, 1100.0f, 540.0f };

static const char* nome_cena(IdCena cena)
{
    switch (cena) {
        case CENA_MENU:
            return "Menu";
        case CENA_INICIO_RUN:
            return "Início da Run";
        case CENA_COMBATE:
            return "Combate";
        case CENA_UPGRADE:
            return "Upgrade";
        case CENA_FIM_DE_JOGO:
            return "Fim de Jogo";
        default:
            return "Cena desconhecida";
    }
}

static void desenhar_titulo_cena(const char* titulo, const char* subtitulo)
{
    DrawText(titulo, 90, 80, 54, RAYWHITE);
    DrawText(subtitulo, 92, 142, 22, (Color){162, 170, 191, 255});
}

static void atualizar_jogador_base(ContextoJogo* contexto)
{
    float direcao_x = 0.0f;
    float direcao_y = 0.0f;
    const float velocidade = contexto->jogador.velocidade_movimento;

    if (contexto->entrada.mover_esquerda) {
        direcao_x -= 1.0f;
    }

    if (contexto->entrada.mover_direita) {
        direcao_x += 1.0f;
    }

    if (contexto->entrada.mover_cima) {
        direcao_y -= 1.0f;
    }

    if (contexto->entrada.mover_baixo) {
        direcao_y += 1.0f;
    }

    if (direcao_x != 0.0f || direcao_y != 0.0f) {
        const float comprimento = sqrtf((direcao_x * direcao_x) + (direcao_y * direcao_y));
        direcao_x /= comprimento;
        direcao_y /= comprimento;
    }

    contexto->jogador.posicao_x += direcao_x * velocidade * contexto->tempo_delta;
    contexto->jogador.posicao_y += direcao_y * velocidade * contexto->tempo_delta;

    if (contexto->jogador.posicao_x < AREA_COMBATE.x + contexto->jogador.raio_colisao) {
        contexto->jogador.posicao_x = AREA_COMBATE.x + contexto->jogador.raio_colisao;
    }

    if (contexto->jogador.posicao_x > AREA_COMBATE.x + AREA_COMBATE.width - contexto->jogador.raio_colisao) {
        contexto->jogador.posicao_x = AREA_COMBATE.x + AREA_COMBATE.width - contexto->jogador.raio_colisao;
    }

    if (contexto->jogador.posicao_y < AREA_COMBATE.y + contexto->jogador.raio_colisao) {
        contexto->jogador.posicao_y = AREA_COMBATE.y + contexto->jogador.raio_colisao;
    }

    if (contexto->jogador.posicao_y > AREA_COMBATE.y + AREA_COMBATE.height - contexto->jogador.raio_colisao) {
        contexto->jogador.posicao_y = AREA_COMBATE.y + AREA_COMBATE.height - contexto->jogador.raio_colisao;
    }
}

void reiniciar_run_base(ContextoJogo* contexto)
{
    contexto->estado_run.wave_atual = 1;
    contexto->estado_run.tempo_da_run = 0.0f;
    contexto->estado_run.seed_atual = (unsigned int)GetRandomValue(1000, 999999);
    contexto->estado_run.lista_inimigos = NULL;
    contexto->estado_run.lista_projeteis = NULL;
    contexto->estado_run.deve_ir_para_upgrade = false;
    contexto->estado_run.deve_encerrar_run = false;

    contexto->jogador.posicao_x = AREA_COMBATE.x + (AREA_COMBATE.width * 0.5f);
    contexto->jogador.posicao_y = AREA_COMBATE.y + (AREA_COMBATE.height * 0.5f);
    contexto->jogador.vida_atual = contexto->jogador.vida_maxima;
    contexto->jogador.biomassa_run = 0;
}

static void atualizar_cena_menu(ContextoJogo* contexto)
{
    if (contexto->entrada.confirmar) {
        trocar_cena(contexto, CENA_INICIO_RUN);
    }
}

static void atualizar_cena_inicio_run(ContextoJogo* contexto)
{
    if (contexto->entrada.confirmar) {
        trocar_cena(contexto, CENA_COMBATE);
        return;
    }

    if (contexto->entrada.voltar) {
        trocar_cena(contexto, CENA_MENU);
    }
}

static void atualizar_cena_combate(ContextoJogo* contexto)
{
    atualizar_jogador_base(contexto);

    if (contexto->entrada.confirmar) {
        contexto->estado_run.deve_ir_para_upgrade = true;
    }

    if (contexto->entrada.voltar) {
        contexto->estado_run.deve_encerrar_run = true;
    }

    if (contexto->estado_run.deve_ir_para_upgrade) {
        contexto->estado_run.deve_ir_para_upgrade = false;
        trocar_cena(contexto, CENA_UPGRADE);
        return;
    }

    if (contexto->estado_run.deve_encerrar_run) {
        contexto->estado_run.deve_encerrar_run = false;
        trocar_cena(contexto, CENA_FIM_DE_JOGO);
    }
}

static void atualizar_cena_upgrade(ContextoJogo* contexto)
{
    if (contexto->entrada.confirmar) {
        contexto->estado_run.wave_atual += 1;
        trocar_cena(contexto, CENA_COMBATE);
        return;
    }

    if (contexto->entrada.voltar) {
        trocar_cena(contexto, CENA_FIM_DE_JOGO);
    }
}

static void atualizar_cena_fim_de_jogo(ContextoJogo* contexto)
{
    if (contexto->entrada.confirmar) {
        trocar_cena(contexto, CENA_MENU);
    }
}

static void desenhar_cena_menu(void)
{
    desenhar_titulo_cena("AUGUR", "Base integrável do Dev 1");
    DrawText("Enter ou Espaço: iniciar uma nova run", 92, 240, 28, RAYWHITE);
    DrawText("F3: alternar debug", 92, 280, 24, (Color){162, 170, 191, 255});
    DrawText("Este menu já existe para orientar a navegação do grupo.", 92, 350, 24,
             (Color){121, 132, 160, 255});
}

static void desenhar_cena_inicio_run(const ContextoJogo* contexto)
{
    desenhar_titulo_cena("Início da Run", "Esta tela antecipa seed e profecia.");
    DrawText(TextFormat("Seed atual: %u", contexto->estado_run.seed_atual), 92, 240, 28, RAYWHITE);
    DrawText("A profecia determinística entra na próxima etapa da base.", 92, 286, 24,
             (Color){162, 170, 191, 255});
    DrawText("Enter ou Espaço: entrar em combate", 92, 352, 24, RAYWHITE);
    DrawText("Esc: voltar ao menu", 92, 386, 24, (Color){162, 170, 191, 255});
}

static void desenhar_cena_combate(const ContextoJogo* contexto)
{
    DrawRectangleRounded(AREA_COMBATE, 0.02f, 1, (Color){27, 34, 48, 255});
    DrawRectangleLinesEx(AREA_COMBATE, 3.0f, (Color){82, 146, 214, 255});

    DrawCircleV((Vector2){contexto->jogador.posicao_x, contexto->jogador.posicao_y},
                contexto->jogador.raio_colisao, (Color){247, 208, 109, 255});

    DrawText("Combate", 92, 28, 44, RAYWHITE);
    DrawText("WASD ou setas movem o jogador", 92, 640, 22, RAYWHITE);
    DrawText("Enter ou Espaço abre o placeholder de upgrade", 92, 668, 22, RAYWHITE);
    DrawText("Esc encerra a run e abre a tela de fim de jogo", 92, 696, 20,
             (Color){162, 170, 191, 255});

    DrawText(TextFormat("Wave atual: %d", contexto->estado_run.wave_atual), 930, 28, 28, RAYWHITE);
    DrawText(TextFormat("Tempo da run: %.1f s", contexto->estado_run.tempo_da_run), 930, 60, 24,
             (Color){162, 170, 191, 255});
}

static void desenhar_cena_upgrade(const ContextoJogo* contexto)
{
    (void)contexto;

    desenhar_titulo_cena("Upgrade", "Placeholder guiado para o Dev 2 plugar cartas e dados.");
    DrawRectangleRounded((Rectangle){92.0f, 240.0f, 330.0f, 220.0f}, 0.03f, 1, (Color){44, 52, 73, 255});
    DrawRectangleRounded((Rectangle){446.0f, 240.0f, 330.0f, 220.0f}, 0.03f, 1, (Color){44, 52, 73, 255});
    DrawRectangleRounded((Rectangle){800.0f, 240.0f, 330.0f, 220.0f}, 0.03f, 1, (Color){44, 52, 73, 255});

    DrawText("Carta 1", 128, 280, 34, RAYWHITE);
    DrawText("Carta 2", 482, 280, 34, RAYWHITE);
    DrawText("Carta 3", 836, 280, 34, RAYWHITE);
    DrawText("Enter ou Espaço: voltar ao combate", 92, 540, 24, RAYWHITE);
    DrawText("Esc: ir para fim de jogo", 92, 574, 24, (Color){162, 170, 191, 255});
}

static void desenhar_cena_fim_de_jogo(const ContextoJogo* contexto)
{
    desenhar_titulo_cena("Fim de Jogo", "Placeholder para score, biomassa e seed compartilhável.");
    DrawText(TextFormat("Wave alcançada: %d", contexto->estado_run.wave_atual), 92, 240, 28, RAYWHITE);
    DrawText(TextFormat("Biomassa da run: %d", contexto->jogador.biomassa_run), 92, 278, 28, RAYWHITE);
    DrawText(TextFormat("Seed: %u", contexto->estado_run.seed_atual), 92, 316, 28, RAYWHITE);
    DrawText("Enter ou Espaço: voltar ao menu", 92, 396, 24, RAYWHITE);
}

void atualizar_cena_atual(ContextoJogo* contexto)
{
    switch (contexto->cena_atual) {
        case CENA_MENU:
            atualizar_cena_menu(contexto);
            break;
        case CENA_INICIO_RUN:
            atualizar_cena_inicio_run(contexto);
            break;
        case CENA_COMBATE:
            atualizar_cena_combate(contexto);
            break;
        case CENA_UPGRADE:
            atualizar_cena_upgrade(contexto);
            break;
        case CENA_FIM_DE_JOGO:
            atualizar_cena_fim_de_jogo(contexto);
            break;
        default:
            break;
    }
}

void desenhar_cena_atual(const ContextoJogo* contexto)
{
    DrawText(nome_cena(contexto->cena_atual), 1030, 28, 18, (Color){121, 132, 160, 255});

    switch (contexto->cena_atual) {
        case CENA_MENU:
            desenhar_cena_menu();
            break;
        case CENA_INICIO_RUN:
            desenhar_cena_inicio_run(contexto);
            break;
        case CENA_COMBATE:
            desenhar_cena_combate(contexto);
            break;
        case CENA_UPGRADE:
            desenhar_cena_upgrade(contexto);
            break;
        case CENA_FIM_DE_JOGO:
            desenhar_cena_fim_de_jogo(contexto);
            break;
        default:
            break;
    }
}
