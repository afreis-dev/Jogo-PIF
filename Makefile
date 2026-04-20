CC := gcc

ifeq ($(OS),Windows_NT)
    EXECUTAVEL := build/augur.exe
    COMANDO_EXECUTAR := .\build\augur.exe
    LIMPAR_BUILD := if exist build rmdir /S /Q build
    define CRIAR_PASTA
        if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    endef
else
    EXECUTAVEL := build/augur
    COMANDO_EXECUTAR := ./build/augur
    LIMPAR_BUILD := rm -rf build
    define CRIAR_PASTA
        mkdir -p $(1)
    endef
endif

FONTES := $(wildcard src/core/*.c) $(wildcard src/regras/*.c) $(wildcard src/stubs/*.c)
OBJETOS := $(patsubst %.c,build/obj/%.o,$(FONTES))

CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude $(shell pkg-config --cflags raylib)
LDFLAGS := $(shell pkg-config --libs raylib)

.PHONY: all limpar executar

all: $(EXECUTAVEL)

$(EXECUTAVEL): $(OBJETOS)
	@$(call CRIAR_PASTA,$(dir $@))
	$(CC) $(OBJETOS) -o $@ $(LDFLAGS)

build/obj/%.o: %.c
	@$(call CRIAR_PASTA,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

executar: $(EXECUTAVEL)
	$(COMANDO_EXECUTAR)

limpar:
	@$(LIMPAR_BUILD)
