#ifndef _FUNCOES_H 
#define FUNCOES_H 

// Definições de constantes para fácil manutenção
#define MAX_CANDIDATOS 30
#define MAX_VOTOS 1000
#define ARQUIVO_CANDIDATOS "dados/candidatos.csv" // Mudança para .csv para indicar estrutura
#define ARQUIVO_VOTOS "dados/votos.txt"

typedef struct {
    int numero;
    char nome[50];
    char partido[20];
    int votos;
} Candidato;

typedef struct {
    char cpf[12]; 
} ControleVoto;

// Protótipos atualizados para receber ponteiros de contadores (evita variáveis globais)
void listaMenu();
void cadastrarCandidato(Candidato *c, int *qtdCand);
void listarCandidatos(Candidato *c, int qtdCand);
int realizarVoto(ControleVoto *v, Candidato *c, int *qtdVotos, int qtdCand);
int procurarCandidatoIndice(Candidato *c, int tamanho, int numero);
void mostrarResultados(Candidato *c, int qtdCand, int qtdVotos);
void ordenarPorVoto(Candidato *c, int qtdCand);
void salvarDados(Candidato *c, int qtdCand, ControleVoto *v, int qtdVotos);
void carregarDados(Candidato *c, int *qtdCand, ControleVoto *v, int *qtdVotos);
void limparBuffer();





#endif