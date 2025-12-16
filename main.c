#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Possiveis erros, cpf é salvo mesmo que numero de candidato é errado **resolvido
percentual divide por zero, validar antes **resolvido
nao pode entrar em resultado se nao tiver votacao **resolvido
salvar arquivos nao esta feito para recarregar dados 
retorno na validacao de cpf , valores sao iguais e mensagem n fica clara **resolvido
*/

#define MAX_CANDIDATOS 30
#define MAX_VOTOS 1000

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

void listaMenu();
void cadastrarCandidato(Canditato *c);
void listarCandidato(Canditato *c);
int ControleVotos(ControleVoto *v, Canditato *c, int qtdCand);
int ProcuraCandidato(Canditato *c, int tamanho, int numero);
void mostrarResultados(Canditato *c, int qtdCand, int qtdVotos);
void ordenarPorVoto(Canditato *c, int qtdCand);
void salvarArquivoCandidatos(Canditato *c, int qtdCand);
void salvarArquivoVotos(ControleVoto *v, int qtdVotos);
int carregarArquivoCandidatos(Canditato *c);
int carregarArquivoVotos(ControleVoto *v);

int main() {
    Canditato candidatos[MAX_CANDIDATOS];
    ControleVoto votos[MAX_VOTOS];
    int op; 
    char buffer[50];

    //inicia sistema e carrega dados de arquivos, se existirem
    contadorCandidatos = carregarArquivoCandidatos(candidatos);
    contadorVotos = carregarArquivoVotos(votos);

    do
    {
        listaMenu();

        fgets(buffer, sizeof(buffer), stdin);
        op = (int)strtol(buffer, NULL, 10);

        switch(op)
        {
        case 1:
            cadastrarCandidato(candidatos);
            salvarArquivoCandidatos(candidatos, contadorCandidatos);
            break;

        case 2:
            printf("---- Candidatos ----\n");
            for(int i = 0; i < contadorCandidatos; i++) {
                listarCandidato(&candidatos[i]);
            }
            break;
        
        case 3:
            {
                //procurar pelo numero e atribuir 1 voto
                int voto, indice;
                voto = ControleVotos(votos, candidatos, contadorCandidatos);

                if(voto == -1) {
                    printf("CPF ja utilizado. Voto nao permitido.\n");
                    break;
                } else if (voto == -2) {
                    printf("Cpf deve ter 11 digitos, tente novamente.\n");
                    break;
                } else if (voto == -3) {
                    printf("Cpf deve ser apenas numeros, tente novamente.\n");
                    break;
                } else if (voto == -4) {
                    printf("Candidato nao encontrado.\n");
                    break;
                }

                indice = ProcuraCandidato(candidatos, contadorCandidatos, voto);

                if(indice != -1){
                    printf("Voto atribuido com sucesso.\n");
                    contadorVotos++;
                    candidatos[indice].votos++;
                    ordenarPorVoto(candidatos, contadorCandidatos);
                    salvarArquivoCandidatos(candidatos, contadorCandidatos);
                    salvarArquivoVotos(votos, contadorVotos);
                }

                break;
            }
        
        case 4:
            printf("---- RESULTADO DA ELEICAO ----\n");
            if(contadorVotos > 0) {
                ordenarPorVoto(candidatos, contadorCandidatos);
                mostrarResultados(candidatos, contadorCandidatos, contadorVotos);
                break;
            } else {
                printf("Nao existe votos\n");
                break;
            }

        case 5:
            printf("Saindo do sistema.\n");
            break;

        default:
            printf("Opcao invalida, tente novamente.");
            break;
        }
    } while (op != 5);
    
    return 0;
}

void listaMenu() {
    printf("#### SISTEMA VOTACAO ELETRONICA ####\n");
    printf("---- Menu de Opcoes ----\n");
    printf("1 - Cadastrar Candidato\n");    
    printf("2 - Listar Candidatos\n");
    printf("3 - Realizar voto\n");
    printf("4 - Resultado eleicao\n");
    printf("5 - Sair\n");
    printf("Digite a opcao desejada: \n");
}

void cadastrarCandidato(Canditato *c) { 
    if(contadorCandidatos >= 30) {
        printf("Quantidade maior\n");
        return;
    }

    char buffer[50];
    char *endptr; //para verificar se ano é numero.

    printf("---- Cadastro de Candidato ----\n");
    do {
        printf("Digite o numero: \n");
        fgets(buffer, sizeof(buffer), stdin);
        c[contadorCandidatos].numero = (int)strtol(buffer, &endptr, 10);

        if (*endptr != '\n' && *endptr != '\0') {
            printf("ERRO: Digite apenas numeros!\n");
        }

        if (ProcuraCandidato(c, contadorCandidatos, c[contadorCandidatos].numero) != -1) {
            printf("ERRO: Numero ja cadastrado! Digite outro numero.\n");
            *endptr = 'a'; //força repetição do loop
        }

        if (c[contadorCandidatos].numero <= 0 || c[contadorCandidatos].numero > 99) {
            printf("ERRO: Numero invalido! Digite um numero positivo.\n");
            *endptr = 'a'; //força repetição do loop
        }

    } while (*endptr != '\n' && *endptr != '\0');
    
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

int ControleVotos(ControleVoto *v, Canditato *c, int qtdCand) {
    char buffer[50];
    char buffer2[50];
    int numero;

    printf("---- Votacao ----\n");
    printf("Digite seu cpf: \n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; 

    // valida tamanho
    if (strlen(buffer) != 11) return -2;

    // valida numeros
    for (int i = 0; i < 11; i++) {
        if (buffer[i] < '0' || buffer[i] > '9') return -3;
    }

    //verifica duplicidade
    for (int i = 0; i < contadorVotos; i++) {
        if (strcmp(buffer, v[i].cpf) == 0) return -1;
    }

    printf("Digite o numero do candidato: \n");
    fgets(buffer2, sizeof(buffer2), stdin);
    numero = (int)strtol(buffer2, NULL, 10);

    int resultado = ProcuraCandidato(c, qtdCand, numero);

    if (resultado == -1) {
        return -4; // candidato inexistente
    }

    strcpy(v[contadorVotos].cpf, buffer);
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
        printf("%d - [%d] %s: %d votos (%.0f%%)\n", i + 1, c[i].numero, c[i].nome, c[i].votos, percentual * 100);
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

void salvarArquivoCandidatos(Canditato *c, int qtdCand) {
    FILE *arquivo = fopen("candidatos.txt", "w");

    if (!arquivo) {
        printf("ERRO: nao foi possivel abrir candidatos.txt\n");
        return;
    }

    for (int i = 0; i < qtdCand; i++) {
        fprintf(
            arquivo,
            "%d %s %s %d\n",
            c[i].numero,
            c[i].nome,
            c[i].partido,
            c[i].votos
        );
    }

    fclose(arquivo);
}

void salvarArquivoVotos(ControleVoto *v, int qtdVotos) {
    FILE *arquivo = fopen("votos.txt", "w");

    if (!arquivo) {
        printf("ERRO: nao foi possivel abrir votos.txt\n");
        return;
    }

    for (int i = 0; i < qtdVotos; i++) {
        fprintf(arquivo, "%s\n", v[i].cpf);
    }

    fclose(arquivo);
}

int carregarArquivoCandidatos(Canditato *c) {
    FILE *arquivo = fopen("candidatos.txt", "r");
    int qtd = 0;

    if (!arquivo) {
        printf("Nenhum arquivo de candidatos encontrado.\n");
        return 0;
    }

    while (fscanf(
        arquivo,
        "%d %49s %9s %d",
        &c[qtd].numero,
        c[qtd].nome,
        c[qtd].partido,
        &c[qtd].votos
    ) == 4) {
        qtd++;
    }

    fclose(arquivo);
    return qtd;
}

int carregarArquivoVotos(ControleVoto *v) {
    FILE *arquivo = fopen("votos.txt", "r");
    int qtd = 0;

    if (!arquivo) {
        printf("Nenhum arquivo de votos encontrado.\n");
        return 0;
    }

    while (fscanf(arquivo, "%11s", v[qtd].cpf) == 1) {
    qtd++;
    }       

    fclose(arquivo);
    return qtd;
}

