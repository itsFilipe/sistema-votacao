#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Se a função espera T*, passe algo que já seja um endereço de T
Arrays já são endereços
Use & só quando a variável NÃO for ponteiro
*/

typedef struct {
    int numero;
    char nome[50];
    char partido[20];
    int votos;
} Canditato;

typedef struct {
    char cpf[12]; // CPF sem formatação (11 dígitos + \0)
    int ja_votou; // 1 = já votou, 0 = não votou
} ControleVoto;

int contadorCandidatos = 0;
int contadorVotos = 0;

void cadastrarCandidato(Canditato *c);
void listarCandidato(Canditato *c);
int ControleVotos(ControleVoto *c);
int ProcuraCandidato(Canditato *c, int tamanho, int numero);
void ordenar (Canditato *c);

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
        printf("4 - Sair\n");
        printf("Digite a opcao desejada: \n");

        fgets(buffer, sizeof(buffer), stdin);
        op = (int)strtol(buffer, NULL, 10);

        switch(op)
        {
        case 1:
            cadastrarCandidato(candidatos);
            break;

        case 2:
            for(int i = 0; i < contadorCandidatos; i++) {
                printf("**** Candidatos ****\n");
                listarCandidato(&candidatos[i]);
            }
            break;
        
        case 3:
            {
                //procurar pelo numero e atribuir 1 voto
                int voto, tamanho, indice;
                voto = ControleVotos(votos);

                if(voto == -1) {
                    printf("CPF ja utilizado. Voto nao permitido.\n");
                    break;
                }

                contadorVotos++;

                tamanho = sizeof(candidatos) / sizeof(candidatos[0]);
                ordenar (candidatos);
                indice = ProcuraCandidato(candidatos, tamanho, voto);

                if(indice == -1){
                    printf("Candidato nao encontrado.\n");
                } else {
                    printf("Voto atribuido com sucesso.\n");
                    candidatos[indice].votos++;
                }
            }
        
        case 4:
            printf("Saindo...\n");
            break;

        default:
            break;
        }
    } while (op != 4);
    
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
    fgets(c[contadorVotos].cpf, sizeof(c[contadorVotos].cpf), stdin);
    c[contadorVotos].cpf[strcspn(c[contadorVotos].cpf, "\n")] = '\0';  

    //confere se cpf ja foi utilizado
    //talvez posso fazer essa conferencia apenas uma vez no switch..
    if(contadorVotos >= 1) {
        for (int i = 0; i < contadorVotos; i++)
        {
            if(strcmp(c[contadorVotos].cpf, c[i].cpf) == 0)
            {
                c[contadorVotos].ja_votou = 1; // sim
                return -1;
            } else {
                c[contadorVotos].ja_votou = 0;
                printf("Digite o numero do candidato: \n");
                fgets(buffer, sizeof(buffer), stdin);
                numero = (int)strtol(buffer, NULL, 10);
            }
        }
    }

    return numero;
}

int ProcuraCandidato(Canditato *c, int tamanho, int numero) {
    int inicio = 0;
	int fim = tamanho - 1;
	
	while (inicio <= fim) {
		int meio = (inicio + fim) / 2;
		
		if (c[meio].numero == numero) {
			return meio; // posicao do candidato encontrado
		}
		else if (c[meio].numero < numero) {
			inicio = meio + 1;
		}
		else {
			fim = meio - 1;
		}
	}

	return -1; // elemento não encontrado	
}

void ordenar (Canditato *c) {
    int passagem;
    int i;
    int auxiliar;

    /* bubble sort */
    for (passagem = 1; passagem < contadorCandidatos; passagem++)
    {
        for( i = 0; i < contadorCandidatos - 1; i++)
        {
            if(c[i].numero > c[i + 1].numero)
            {
                auxiliar = c[i].numero;
                c[i].numero = c[i + 1].numero;
                c[i + 1].numero = auxiliar;
            }
        }
    }
}