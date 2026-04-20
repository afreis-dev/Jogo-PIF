/*
 * Arquivo: tipos.h
 * Propósito: definir os tipos públicos compartilhados pelo grupo.
 * Quem depende deste arquivo: todos os módulos do projeto.
 */

#ifndef AUGUR_TIPOS_H
#define AUGUR_TIPOS_H

#include <stdbool.h>

#define QUANTIDADE_MODIFICADORES_PROFECIA 3
#define QUANTIDADE_DADOS_EQUIPADOS 2
#define TAMANHO_TEXTO_PROFECIA 128
#define QUANTIDADE_TOP_SCORES 10
#define LARGURA_PADRAO_JANELA 1280
#define ALTURA_PADRAO_JANELA 720
#define LARGURA_MAPA_COLISAO 20
#define ALTURA_MAPA_COLISAO 12

typedef enum IdCena {
    CENA_MENU = 0,
    CENA_INICIO_RUN,
    CENA_COMBATE,
    CENA_UPGRADE,
    CENA_FIM_DE_JOGO
} IdCena;

typedef enum ElementoProfecia {
    ELEMENTO_FOGO = 0,
    ELEMENTO_GELO,
    ELEMENTO_RELAMPAGO,
    ELEMENTO_VENENO,
    ELEMENTO_ARCANO,
    ELEMENTO_SOMBRA
} ElementoProfecia;

typedef enum CondicaoProfecia {
    CONDICAO_AO_MATAR = 0,
    CONDICAO_AO_TOMAR_DANO,
    CONDICAO_A_CADA_10_SEGUNDOS,
    CONDICAO_EM_COMBO_ATIVO,
    CONDICAO_AO_ROLAR_DADO,
    CONDICAO_AO_CURAR,
    CONDICAO_NA_PRIMEIRA_HIT,
    CONDICAO_ABAIXO_DE_30_DE_VIDA
} CondicaoProfecia;

typedef enum EfeitoProfecia {
    EFEITO_EXPLODE_AREA = 0,
    EFEITO_CURA_JOGADOR,
    EFEITO_DUPLICA_PROJETIL,
    EFEITO_CONGELA_ALVO,
    EFEITO_DROPA_DADO,
    EFEITO_MAIS_2_NO_PROXIMO_ROLL,
    EFEITO_DANO_TRIPLO,
    EFEITO_INFLAMA_PASSIVO,
    EFEITO_REDUZ_RECARGA,
    EFEITO_ALIADO_TEMPORARIO
} EfeitoProfecia;

/*
 * Representa um modificador individual da profecia.
 * Os campos usam enums estáveis para permitir leitura simples por outros módulos.
 */
typedef struct ModificadorProfecia {
    ElementoProfecia elemento;
    CondicaoProfecia condicao;
    EfeitoProfecia efeito;
    char texto_resumido[TAMANHO_TEXTO_PROFECIA];
} ModificadorProfecia;

/*
 * Profecia gerada no começo de cada run.
 * A seed define o resultado e deve ser preservada para repetibilidade.
 */
typedef struct Profecia {
    unsigned int semente;
    ModificadorProfecia modificadores[QUANTIDADE_MODIFICADORES_PROFECIA];
} Profecia;

/*
 * Persiste entre runs e deve ser salvo em arquivo futuramente.
 * Dev 2 tende a consumir e alterar este tipo em sistemas de meta-progressão.
 */
typedef struct DadosSalvos {
    int biomassa_total;
    int desbloqueios_dados[8];
    int desbloqueios_magias[8];
    int top_scores[QUANTIDADE_TOP_SCORES];
    unsigned int seeds_top_scores[QUANTIDADE_TOP_SCORES];
} DadosSalvos;

/*
 * Nó de inimigo ativo durante a run.
 * A memória desta lista será dona do próprio ciclo de vida em módulos de inimigos.
 */
typedef struct NoInimigo {
    float posicao_x;
    float posicao_y;
    float velocidade_x;
    float velocidade_y;
    float raio_colisao;
    int vida;
    int tipo;
    struct NoInimigo* proximo;
} NoInimigo;

/*
 * Nó de magia equipada pelo jogador.
 * A lista permite adicionar e remover magias sem refatorar o jogador.
 */
typedef struct NoMagia {
    int identificador_magia;
    ElementoProfecia elemento;
    float tempo_recarga;
    float tempo_restante;
    int nivel;
    struct NoMagia* proximo;
} NoMagia;

/*
 * Nó de projétil ativo.
 * É usado tanto por magias quanto por futuros inimigos que atirem na tela.
 */
typedef struct NoProjetil {
    float posicao_x;
    float posicao_y;
    float velocidade_x;
    float velocidade_y;
    float raio_colisao;
    int dano;
    ElementoProfecia elemento;
    struct NoProjetil* proximo;
} NoProjetil;

/*
 * Entrada lida no frame atual.
 * O core escreve estes estados e os outros módulos podem apenas consultá-los.
 */
typedef struct EstadoEntrada {
    bool mover_esquerda;
    bool mover_direita;
    bool mover_cima;
    bool mover_baixo;
    bool confirmar;
    bool voltar;
    bool alternar_debug;
} EstadoEntrada;

/*
 * Jogador principal da run.
 * O core é dono da estrutura; outros módulos devem alterar só os campos necessários.
 */
typedef struct Jogador {
    float posicao_x;
    float posicao_y;
    float velocidade_movimento;
    float raio_colisao;
    int vida_atual;
    int vida_maxima;
    NoMagia* magias_equipadas;
    int dados_escolhidos[QUANTIDADE_DADOS_EQUIPADOS];
    int biomassa_run;
} Jogador;

/*
 * Estado transitório de uma run.
 * O core e os stubs consultam esta estrutura para saber em que ponto a run está.
 */
typedef struct EstadoRun {
    int wave_atual;
    float tempo_da_run;
    unsigned int seed_atual;
    NoInimigo* lista_inimigos;
    NoProjetil* lista_projeteis;
    bool deve_ir_para_upgrade;
    bool deve_encerrar_run;
    int mapa_colisao[ALTURA_MAPA_COLISAO][LARGURA_MAPA_COLISAO];
} EstadoRun;

/*
 * Contexto global do jogo.
 * O core inicializa, atualiza e finaliza este estado; os outros módulos recebem ponteiro
 * para ler e alterar apenas o que fizer sentido dentro do contrato.
 */
typedef struct ContextoJogo {
    int largura_janela;
    int altura_janela;
    float tempo_delta;
    float tempo_total;
    bool debug_ativo;
    IdCena cena_atual;
    EstadoEntrada entrada;
    Jogador jogador;
    Profecia profecia;
    EstadoRun estado_run;
    DadosSalvos dados_salvos;
} ContextoJogo;

#endif
