# ============================================================================
# Makefile - AUGUR
# ============================================================================
#
# Esta versao ja aponta para a organizacao mais enxuta do projeto e, durante a
# transicao, ainda aceita a estrutura antiga para nao quebrar o fluxo do grupo.
# ============================================================================

CC := gcc
PKG_CONFIG := pkg-config

FONTES_PLANAS := $(wildcard src/*.c)
FONTES_ANTIGAS := $(wildcard src/core/*.c) $(wildcard src/regras/*.c) $(wildcard src/stubs/*.c)

ifeq ($(strip $(FONTES_PLANAS)),)
    FONTES := $(FONTES_ANTIGAS)
    INCLUDES := -Iinclude
else
    FONTES := $(FONTES_PLANAS)
    INCLUDES := -Isrc
endif

OBJETOS := $(patsubst src/%.c,build/%.o,$(FONTES))

ifeq ($(OS),Windows_NT)
    EXECUTAVEL := augur.exe
    COMANDO_EXECUTAR := .\augur.exe
    CFLAGS := -Wall -Wextra -std=c11 -g $(INCLUDES) $(shell $(PKG_CONFIG) --cflags raylib)
    LDFLAGS := $(shell $(PKG_CONFIG) --libs raylib) -lopengl32 -lgdi32 -lwinmm -lm
    define CRIAR_PASTA
        if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    endef
else
    EXECUTAVEL := augur
    COMANDO_EXECUTAR := ./augur
    CFLAGS := -Wall -Wextra -std=c11 -g $(INCLUDES) $(shell $(PKG_CONFIG) --cflags raylib)
    LDFLAGS := $(shell $(PKG_CONFIG) --libs raylib) -lm
    define CRIAR_PASTA
        mkdir -p $(1)
    endef
endif

.PHONY: all run executar clean limpar

all: $(EXECUTAVEL)

$(EXECUTAVEL): $(OBJETOS)
	$(CC) $(OBJETOS) -o $@ $(LDFLAGS)
	@echo.
	@echo Base compilada com sucesso.
	@echo.

build/%.o: src/%.c
	@$(call CRIAR_PASTA,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXECUTAVEL)
	$(COMANDO_EXECUTAR)

executar: run

clean:
ifeq ($(OS),Windows_NT)
	@if exist build rmdir /S /Q build
	@if exist augur.exe del /Q augur.exe
	@if exist augur del /Q augur
else
	rm -rf build augur augur.exe
endif

limpar: clean
