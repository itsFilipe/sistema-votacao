#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int main() {
    // Inicialização segura de memória
    Candidato candidatos[MAX_CANDIDATOS];
    ControleVoto votos[MAX_VOTOS];
    
    // Variáveis locais (não globais) controlam o estado
    int contadorCandidatos = 0;
    int contadorVotos = 0;
    
    int op; 
    char buffer[50];

    // Carrega dados ao iniciar
    carregarDados(candidatos, &contadorCandidatos, votos, &contadorVotos);

    do {
        listaMenu();
        fgets(buffer, sizeof(buffer), stdin);
        op = (int)strtol(buffer, NULL, 10);

        switch(op) {
        case 1:
            cadastrarCandidato(candidatos, &contadorCandidatos);
            // NOTA: Removemos salvarDados daqui. I/O é lento.
            // Se o sistema for crítico, salvaríamos apenas o novo registro (append), não tudo.
            break;

        case 2:
            listarCandidatos(candidatos, contadorCandidatos);
            break;
        
        case 3:
            // Lógica de votação encapsulada e desacoplada da I/O de disco
            if (realizarVoto(votos, candidatos, &contadorVotos, contadorCandidatos)) {
                printf(">> Voto registrado com sucesso (em memoria)!\n");
            }
            break;
        
        case 4:
            // Ordena apenas na hora de mostrar o resultado (Lazy processing)
            if(contadorVotos > 0) {
                // Criamos uma cópia para ordenar sem bagunçar a lista original (opcional, mas boa prática)
                // Aqui ordenamos o original mesmo para simplificar
                ordenarPorVoto(candidatos, contadorCandidatos);
                mostrarResultados(candidatos, contadorCandidatos, contadorVotos);
            } else {
                printf(">> Nao ha votos registrados.\n");
            }
            break;

        case 5:
            printf(">> Salvando dados e saindo...\n");
            salvarDados(candidatos, contadorCandidatos, votos, contadorVotos);
            break;

        default:
            printf(">> Opcao invalida.\n");
            break;
        }
        /*
        printf("\nPressione ENTER para continuar...");
        getchar();
        comentando para testes
        */
         // Pausa UX
        // system("clear || cls"); // Opcional: limpar tela

    } while (op != 5);
    
    return 0;
}


