#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "funcoes.h"

typedef struct {
    int numero;
    char nome[50];
    char partido[20];
    int votos;
    int contador;
} Canditato;

void cadastrarCandidato(Canditato *c);
void listarCandidato(Canditato *c);

int main() {
    
    Canditato candidatos[30];
    candidatos->contador = 1;
    int op; 
    char buffer[50];

    printf("Digite uma opcao: \n");
    fgets(buffer, sizeof(buffer), stdin);
    op = (int)strtol(buffer, NULL, 50);

    do
    {
        switch(op)
        {
        case 1:
            cadastrarCandidato(candidatos);
            break;

        case 2:
            listarCandidato(candidatos);
            break;
        
        default:
            break;
        }
    } while (op != 3);
    
    return 0;
}

void cadastrarCandidato(Canditato *c) { 
    if(c->contador > 30) {
        printf("Quantidade maior\n");
        return;
    }

    char buffer[50];

    printf("**** Cadastro de Candidato\n");
    printf("Digite o numero: \n");
    fgets(buffer, sizeof(buffer), stdin);
    c[c->contador].numero = (int)strtol(buffer, NULL, 2);

    printf("Digite o nome: \n");
    fgets(c[c->contador].nome, sizeof(c[c->contador].nome), stdin);
    c[c->contador].nome[strcspn(c[c->contador].nome, "\n")] = '\0';  

    printf("Digite o partido: \n");
    fgets(c[c->contador].partido, sizeof(c[c->contador].partido), stdin);
    c[c->contador].partido[strcspn(c[c->contador].partido, "\n")] = '\0'; 

    printf("Candidato adicionado com sucesso!\n");

    c->contador++;
}

void listarCandidato(Canditato *c) {
    printf("Numero: %d\n", c->numero);
    printf("Nome: %s\n", c->nome);
    printf("Partido: %s\n", c->partido);
}


/*
Se a função espera T*, passe algo que já seja um endereço de T
Arrays já são endereços
Use & só quando a variável NÃO for ponteiro
*/