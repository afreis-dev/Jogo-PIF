/*
 * Arquivo: stubs.h
 * Propósito: declarar os pontos temporários de integração dos outros devs.
 * Quem depende deste arquivo: core e módulos do grupo.
 */

#ifndef AUGUR_STUBS_H
#define AUGUR_STUBS_H

#include "augur/tipos.h"

void atualizar_stub_cartas(ContextoJogo* contexto);
void desenhar_stub_cartas(const ContextoJogo* contexto);

void atualizar_stub_dados(ContextoJogo* contexto);
void desenhar_stub_dados(const ContextoJogo* contexto);

void atualizar_stub_hud(ContextoJogo* contexto);
void desenhar_stub_hud(const ContextoJogo* contexto);

void atualizar_stub_inimigos(ContextoJogo* contexto);
void desenhar_stub_inimigos(const ContextoJogo* contexto);

void atualizar_stub_magias(ContextoJogo* contexto);
void desenhar_stub_magias(const ContextoJogo* contexto);

void atualizar_stub_waves(ContextoJogo* contexto);
void desenhar_stub_waves(const ContextoJogo* contexto);

#endif
