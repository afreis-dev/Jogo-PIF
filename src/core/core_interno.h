/*
 * Arquivo: core_interno.h
 * Propósito: declarar helpers internos do core que não precisam ser públicos.
 * Quem depende dele: apenas os módulos dentro de src/core.
 */

#ifndef CORE_INTERNO_H
#define CORE_INTERNO_H

#include "augur/tipos.h"

void configurar_janela(ContextoJogo* contexto);
void encerrar_janela(void);
void atualizar_tempo_jogo(ContextoJogo* contexto);
void atualizar_entrada_jogo(EstadoEntrada* entrada);
void atualizar_cena_atual(ContextoJogo* contexto);
void desenhar_cena_atual(const ContextoJogo* contexto);
void reiniciar_run_base(ContextoJogo* contexto);

#endif
