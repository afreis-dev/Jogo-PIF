/*
 * Arquivo: contexto.h
 * Propósito: expor a API principal do loop do jogo e da troca de cenas.
 * Quem depende deste arquivo: principal, cenas e pontos de integração.
 */

#ifndef AUGUR_CONTEXTO_H
#define AUGUR_CONTEXTO_H

#include "augur/tipos.h"

void iniciar_contexto(ContextoJogo* contexto);
void atualizar_contexto(ContextoJogo* contexto);
void desenhar_contexto(const ContextoJogo* contexto);
void finalizar_contexto(ContextoJogo* contexto);
void trocar_cena(ContextoJogo* contexto, IdCena nova_cena);

#endif
