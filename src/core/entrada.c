/*
 * Arquivo: entrada.c
 * Propósito: traduzir teclado em um estado de entrada previsível para o frame atual.
 * Quem depende dele: contexto_jogo.c e cenas.c.
 */

#include "raylib.h"

#include "core_interno.h"

void atualizar_entrada_jogo(EstadoEntrada* entrada)
{
    entrada->mover_esquerda = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    entrada->mover_direita = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    entrada->mover_cima = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
    entrada->mover_baixo = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    entrada->confirmar = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
    entrada->voltar = IsKeyPressed(KEY_ESCAPE);
    entrada->alternar_debug = IsKeyPressed(KEY_F3);
}
