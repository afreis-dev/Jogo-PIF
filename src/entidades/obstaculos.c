/* ============================================================================
 * obstaculos.c - STUB DO MODULO DE OBSTACULOS
 * ============================================================================
 *
 * TODO: Dev 3 (Luisa) implementar este arquivo.
 *
 * Por enquanto as 4 funcoes existem mas nao fazem nada. main.c ja chama
 * todas elas - assim que voce escrever o conteudo, o jogo passa a ter
 * obstaculos sem mexer em nenhum outro arquivo.
 *
 * Sugestao de implementacao:
 *
 * void obstaculos_gerar(EstadoJogo *ej, unsigned int seed):
 *
 *   // Stream de rand proprio pra nao conflitar com profecia.c (que ja
 *   // consumiu a seed la). Multiplica e soma um valor magico pra tirar
 *   // uma sequencia diferente, mas ainda deterministica pra mesma seed.
 *   srand(seed * 2654435761u + 12345u);
 *
 *   ej->qtd_obstaculos = 0;
 *   int qtd_alvo = 16 + rand() % 17;   // entre 16 e 32 obstaculos
 *
 *   for (int i = 0; i < qtd_alvo && ej->qtd_obstaculos < MAX_OBSTACULOS; i++) {
 *       // Sortei tipo, raio compativel, posicao em [-2500, 2500]^2.
 *       // Tente algumas vezes pra evitar:
 *       //   - sobreposicao com outro obstaculo ja colocado
 *       //   - spawn dentro da "zona segura" do jogador (~180 px da origem)
 *       // Se nao achou em 8 tentativas, pula esse obstaculo.
 *   }
 *
 * void obstaculos_desenhar(const EstadoJogo *ej):
 *
 *   // PRECISA ser chamada DENTRO de BeginMode2D - usa coord de mundo.
 *   // Para cada obstaculo no array:
 *   //   - ARVORE: tronco marrom (DrawRectangle) + copa verde (DrawCircleV) +
 *   //             highlight verde-claro deslocado pra dar volume
 *   //   - PEDRA:  circulo cinza-escuro + reflexo cinza-claro deslocado +
 *   //             contorno escuro com DrawCircleLines
 *
 * void obstaculos_resolver_jogador(EstadoJogo *ej):
 *
 *   // Para cada obstaculo, se houver overlap com o circulo do jogador:
 *   //   distancia = hypot(dx, dy);
 *   //   overlap = (raio_jogador + raio_obstaculo) - distancia;
 *   //   ej->jogador.posicao += unit(dx, dy) * overlap;
 *   // Caso degenerado (centros sobrepostos): empurra arbitrariamente.
 *
 * void obstaculos_resolver_inimigos(EstadoJogo *ej):
 *
 *   // Mesmo padrao, mas iterando sobre a lista encadeada de inimigos.
 *
 * "(void)ej;" silencia o warning de "parametro nao usado". Remova quando
 * for implementar de verdade. Mesmo no parametro 'seed' do gerar.
 * ========================================================================== */

#include "obstaculos.h"

void obstaculos_gerar(EstadoJogo *ej, unsigned int seed) {
    (void)ej; (void)seed;   /* TODO: Luisa - gerar layout deterministico */
}

void obstaculos_desenhar(const EstadoJogo *ej) {
    (void)ej;   /* TODO: Luisa - desenhar arvores e pedras (dentro de BeginMode2D) */
}

void obstaculos_resolver_jogador(EstadoJogo *ej) {
    (void)ej;   /* TODO: Luisa - empurrar jogador pra fora de obstaculos */
}

void obstaculos_resolver_inimigos(EstadoJogo *ej) {
    (void)ej;   /* TODO: Luisa - empurrar cada inimigo pra fora de obstaculos */
}
