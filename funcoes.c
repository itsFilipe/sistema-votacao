#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void listaMenu() {
    printf("\n#### SISTEMA VOTACAO ####\n");
    printf("1 - Cadastrar Candidato\n");    
    printf("2 - Listar Candidatos\n");
    printf("3 - Realizar voto\n");
    printf("4 - Resultado parcial\n");
    printf("5 - Salvar e Sair\n");
    printf("Opcao: ");
}

void cadastrarCandidato(Candidato *c, int *qtdCand) { 
    // MELHORIA DE SEGURANÇA: Checagem de limite antes de processar
    if(*qtdCand >= MAX_CANDIDATOS) {
        printf("ERRO: Limite de candidatos (%d) atingido.\n", MAX_CANDIDATOS);
        return;
    }

    char buffer[50];
    int novoNumero;

    printf("---- Cadastro ----\n");
    while (1) {
        printf("Numero: ");
        fgets(buffer, sizeof(buffer), stdin);
        novoNumero = (int)strtol(buffer, NULL, 10);

        if (novoNumero <= 0) {
            printf("Erro: Numero deve ser positivo.\n");
            continue;
        }
        if (procurarCandidatoIndice(c, *qtdCand, novoNumero) != -1) {
            printf("Erro: Numero ja existe.\n");
            continue;
        }
        break; // Input válido
    }
    
    c[*qtdCand].numero = novoNumero;

    printf("Nome: ");
    fgets(c[*qtdCand].nome, sizeof(c[*qtdCand].nome), stdin);
    c[*qtdCand].nome[strcspn(c[*qtdCand].nome, "\n")] = '\0';  

    printf("Partido: ");
    fgets(c[*qtdCand].partido, sizeof(c[*qtdCand].partido), stdin);
    c[*qtdCand].partido[strcspn(c[*qtdCand].partido, "\n")] = '\0'; 

    c[*qtdCand].votos = 0;
    (*qtdCand)++; // Incrementa valor via ponteiro
    printf(">> Candidato cadastrado!\n");
}

void listarCandidatos(Candidato *c, int qtdCand) {
    printf("---- Lista de Candidatos ----\n");
    for(int i = 0; i < qtdCand; i++) {
        printf("[%d] %s (Partido: %s) - Votos: %d\n", c[i].numero, c[i].nome, c[i].partido, c[i].votos);
    }
}

int realizarVoto(ControleVoto *v, Candidato *c, int *qtdVotos, int qtdCand) {
    // MELHORIA DE SEGURANÇA: Checagem de limite de array de votos
    if (*qtdVotos >= MAX_VOTOS) {
        printf("ERRO CRITICO: Urna cheia (%d votos). Contate o administrador.\n", MAX_VOTOS);
        return 0;
    }

    char cpfInput[50];
    char numInput[50];
    int numeroVoto;

    printf("---- Votacao ----\n");
    printf("CPF (somente numeros): ");
    fgets(cpfInput, sizeof(cpfInput), stdin);
    cpfInput[strcspn(cpfInput, "\n")] = '\0'; 

    if (strlen(cpfInput) != 11) {
        printf("ERRO: CPF deve ter 11 digitos.\n");
        return 0;
    }

    // Verifica duplicidade (Performance: O(N) - aceitável para 1000 votos)
    // Em sistemas reais, usaríamos Hash Table ou Busca Binária aqui.
    for (int i = 0; i < *qtdVotos; i++) {
        if (strcmp(cpfInput, v[i].cpf) == 0) {
            printf("ERRO: CPF ja votou.\n");
            return 0;
        }
    }

    printf("Numero do Candidato: ");
    fgets(numInput, sizeof(numInput), stdin);
    numeroVoto = (int)strtol(numInput, NULL, 10);

    int indice = procurarCandidatoIndice(c, qtdCand, numeroVoto);

    if (indice == -1) {
        printf("ERRO: Candidato inexistente.\n");
        return 0;
    }

    // Commit do voto na memória
    strcpy(v[*qtdVotos].cpf, cpfInput);
    (*qtdVotos)++;
    c[indice].votos++;
    
    // NOTA: Removemos "ordenarPorVoto" e "salvarArquivo" daqui.
    // Ganhamos performance massiva não fazendo I/O a cada voto.
    return 1;
}

int procurarCandidatoIndice(Candidato *c, int tamanho, int numero) {
    for(int i = 0; i < tamanho; i++) {
        if(numero == c[i].numero) return i;
    }
    return -1; 
}

void mostrarResultados(Candidato *c, int qtdCand, int qtdVotos) {
    printf("---- RESULTADO FINAL ----\n");
    for(int i = 0; i < qtdCand; i++) {
        float percentual = (qtdVotos > 0) ? ((float)c[i].votos / qtdVotos * 100) : 0;
        printf("%do Lugar: [%d] %s - %d votos (%.1f%%)\n", 
               i + 1, c[i].numero, c[i].nome, c[i].votos, percentual);
    }
    printf("Total de Votos Computados: %d\n", qtdVotos);
}

void ordenarPorVoto(Candidato *c, int qtdCand) {
    // Bubble sort simples (Estável, mas O(N^2)). 
    // Para MAX_CANDIDATOS = 30, isso é instantâneo. Não precisa otimizar prematuramente.
    for (int i = 0; i < qtdCand - 1; i++) {
        for (int j = 0; j < qtdCand - i - 1; j++) {
            if (c[j].votos < c[j + 1].votos) { // Decrescente
                Candidato temp = c[j];
                c[j] = c[j + 1];
                c[j + 1] = temp;
            }
        }
    }
}

// -------------------------------------------------------------------------
// CAMADA DE PERSISTÊNCIA (DATA LAYER)
// -------------------------------------------------------------------------

void salvarDados(Candidato *c, int qtdCand, ControleVoto *v, int qtdVotos) {
    // Tenta abrir o arquivo dentro da pasta
    FILE *fCand = fopen(ARQUIVO_CANDIDATOS, "w");
    
    // VERIFICAÇÃO DE SEGURANÇA
    if (fCand == NULL) {
        printf("\nERRO CRITICO AO SALVAR:\n");
        printf("Nao foi possivel criar o arquivo '%s'.\n", ARQUIVO_CANDIDATOS);
        printf("VERIFIQUE: A pasta 'dados' existe no diretorio do projeto?\n");
        // Não damos return aqui para tentar salvar os votos pelo menos
    } else {
        for (int i = 0; i < qtdCand; i++) {
            fprintf(fCand, "%d;%s;%s;%d\n", c[i].numero, c[i].nome, c[i].partido, c[i].votos);
        }
        fclose(fCand);
    }

    FILE *fVotos = fopen(ARQUIVO_VOTOS, "w");
    if (fVotos == NULL) {
        printf("ERRO: Nao foi possivel criar '%s'. Verifique a pasta.\n", ARQUIVO_VOTOS);
    } else {
        for (int i = 0; i < qtdVotos; i++) {
            fprintf(fVotos, "%s\n", v[i].cpf);
        }
        fclose(fVotos);
    }
}

void carregarDados(Candidato *c, int *qtdCand, ControleVoto *v, int *qtdVotos) {
    FILE *fCand = fopen(ARQUIVO_CANDIDATOS, "r");
    char linha[200]; // Buffer para ler a linha inteira do arquivo

    // Carregamento com PARSEAMENTO MANUAL (Mais robusto que fscanf puro para strings)
    if (fCand) {
        while (fgets(linha, sizeof(linha), fCand)) {
            // Remove o \n do final da linha lida pelo fgets
            linha[strcspn(linha, "\n")] = 0;

            // strtok divide a string nos delimitadores ";"
            char *token = strtok(linha, ";"); 
            if(token == NULL) continue;
            c[*qtdCand].numero = atoi(token);

            token = strtok(NULL, ";");
            if(token == NULL) continue;
            strcpy(c[*qtdCand].nome, token);

            token = strtok(NULL, ";");
            if(token == NULL) continue;
            strcpy(c[*qtdCand].partido, token);

            token = strtok(NULL, ";");
            if(token == NULL) continue;
            c[*qtdCand].votos = atoi(token);

            (*qtdCand)++;
            if(*qtdCand >= MAX_CANDIDATOS) break; // Proteção de memória
        }
        fclose(fCand);
        printf(">> Sistema: %d candidatos carregados.\n", *qtdCand);
    }

    // Carregamento de Votos (simples pois CPF não tem espaço)
    FILE *fVotos = fopen(ARQUIVO_VOTOS, "r");
    if (fVotos) {
        while (fscanf(fVotos, "%11s", v[*qtdVotos].cpf) == 1) {
            (*qtdVotos)++;
            if(*qtdVotos >= MAX_VOTOS) break; // Proteção de memória
        }
        fclose(fVotos);
        printf(">> Sistema: %d votos carregados.\n", *qtdVotos);
    }
}

// --- AREA DE ESTUDO: FUNÇÃO DE PERSISTÊNCIA ROBUSTA (APPEND) ---
/*
void registrarVotoSeguroEmDisco(char *cpf) {
    // ESTUDO: O modo "a" (append) abre o arquivo e posiciona o cursor no FINAL.
    // Se o arquivo não existir, ele cria. Se existir, ele adiciona sem apagar o anterior.
    
    FILE *f = fopen(ARQUIVO_VOTOS, "a"); 
    
    if (f == NULL) {
        printf("ERRO CRITICO: Falha ao abrir disco para logar voto.\n");
        return; 
    }

    // Escreve apenas o CPF e quebra linha.
    // Vantagem: Operação O(1) - Tempo constante, independente do tamanho do arquivo.
    fprintf(f, "%s\n", cpf);
    
    // ESTUDO: fflush força o sistema operacional a tirar o dado do buffer da RAM 
    // e escrever fisicamente no disco AGORA. Essencial para sistemas críticos 
    // onde a energia pode cair milissegundos depois.
    fflush(f); 
    
    fclose(f);
}
*/