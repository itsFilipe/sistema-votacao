#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numero;
    char nome[50];
    char partido[20];
    int votos;
} Canditato;

typedef struct {
    char cpf[12]; // CPF sem formatação (11 dígitos + \0)
} ControleVoto;

int contadorCandidatos = 0;
int contadorVotos = 0;

void cadastrarCandidato(Canditato *c);
void listarCandidato(Canditato *c);
int ControleVotos(ControleVoto *c);
int ProcuraCandidato(Canditato *c, int tamanho, int numero);
void mostrarResultados(Canditato *c, int qtdCand, int qtdVotos);
void ordenarPorVoto(Canditato *c, int qtdCand);

int main() {
    
    Canditato candidatos[30];
    ControleVoto votos[100];
    int op; 
    char buffer[50];

    do
    {
        printf("**** Menu de Opcoes ****\n");
        printf("1 - Cadastrar Candidato\n");    
        printf("2 - Listar Candidatos\n");
        printf("3 - Realizar voto\n");
        printf("4 - Resultado eleicao\n");
        printf("5 - Sair\n");
        printf("Digite a opcao desejada: \n");

        fgets(buffer, sizeof(buffer), stdin);
        op = (int)strtol(buffer, NULL, 10);

        switch(op)
        {
        case 1:
            cadastrarCandidato(candidatos);
            break;

        case 2:
            printf("**** Candidatos ****\n");
            for(int i = 0; i < contadorCandidatos; i++) {
                listarCandidato(&candidatos[i]);
            }
            break;
        
        case 3:
            {
                //procurar pelo numero e atribuir 1 voto
                int voto, indice;
                voto = ControleVotos(votos);

                if(voto == -1) {
                    printf("CPF ja utilizado. Voto nao permitido.\n");
                    break;
                }

                indice = ProcuraCandidato(candidatos, contadorCandidatos, voto);

                if(indice == -1){
                    printf("Candidato nao encontrado.\n");
                } else {
                    printf("Voto atribuido com sucesso.\n");
                    contadorVotos++;
                    candidatos[indice].votos++;
                }

                break;
            }
        
        case 4:
            printf("**** RESULTADO DA ELEICAO ****\n");
            ordenarPorVoto(candidatos, contadorCandidatos);
            mostrarResultados(candidatos, contadorCandidatos, contadorVotos);
            break;

        case 5:
            printf("Saindo...\n");
            break;

        default:
            break;
        }
    } while (op != 5);
    
    return 0;
}

void cadastrarCandidato(Canditato *c) { 
    if(contadorCandidatos > 30) {
        printf("Quantidade maior\n");
        return;
    }

    char buffer[50];

    printf("**** Cadastro de Candidato\n");
    printf("Digite o numero: \n");
    fgets(buffer, sizeof(buffer), stdin);
    c[contadorCandidatos].numero = (int)strtol(buffer, NULL, 10);

    printf("Digite o nome: \n");
    fgets(c[contadorCandidatos].nome, sizeof(c[contadorCandidatos].nome), stdin);
    c[contadorCandidatos].nome[strcspn(c[contadorCandidatos].nome, "\n")] = '\0';  

    printf("Digite o partido: \n");
    fgets(c[contadorCandidatos].partido, sizeof(c[contadorCandidatos].partido), stdin);
    c[contadorCandidatos].partido[strcspn(c[contadorCandidatos].partido, "\n")] = '\0'; 

    c[contadorCandidatos].votos = 0;

    printf("Candidato adicionado com sucesso!\n");

    contadorCandidatos++;
    
    return;
}

void listarCandidato(Canditato *c) {
    printf("[%d] %s - %s\n", c->numero, c->nome, c->partido);
}

int ControleVotos(ControleVoto *c) {
    char buffer[50];
    int numero;

    printf("**** Votacao ****\n");
    printf("Digite seu cpf: \n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; 

    //confere se cpf ja foi utilizado
    if(contadorVotos >= 1) {
        for (int i = 0; i < contadorVotos; i++)
        {
            if(strcmp(buffer, c[i].cpf) == 0)
            {
                return -1;
            }
        }
    }

    strcpy(c[contadorVotos].cpf, buffer);

    printf("Digite o numero do candidato: \n");
    fgets(buffer, sizeof(buffer), stdin);
    numero = (int)strtol(buffer, NULL, 10);

    return numero;
}

int ProcuraCandidato(Canditato *c, int tamanho, int numero) {
    for(int i = 0; i < tamanho; i++) {
        if(numero == c[i].numero){
            return i;
        } 
    }

	return -1; // elemento não encontrado	
}

void mostrarResultados(Canditato *c, int qtdCand, int qtdVotos) {
    float percentual;
    
    for(int i = 0; i < qtdCand; i++) {
        percentual = (float)c[i].votos / qtdVotos;
        printf("1 - [%d] %s: %d votos (%.0f%%)\n", c[i].numero, c[i].nome, c[i].votos, percentual * 100);
    }

    printf("Total de votos: %d\n", qtdVotos);
}

void ordenarPorVoto(Canditato *c, int qtdCand) {
    int pass;
    int i;
    Canditato temp;

    /* bubble sort invertido*/
    for ( pass = 1; pass < qtdCand; pass++ )
    {
        for( i = 0; i < qtdCand - 1; i++)
        {
            if( c[i].votos < c[i + 1].votos )
            {
                temp = c[ i ];
                c[ i ] = c[ i + 1 ];
                c[ i + 1 ] = temp;
            }
        }
    }
}