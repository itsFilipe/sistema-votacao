import random

FILENAME = "cenario_real.txt"
TOTAL_VOTOS = 800  # Vamos encher quase até o limite de 1000
CPFS_USADOS = set()

# Candidatos: (Numero, Nome, Partido, Peso de Popularidade)
CANDIDATOS = [
    (10, "Linus Torvalds", "Partido Linux", 0.45),  # Favorito (45%)
    (20, "Bill Gates", "Partido Micro", 0.25),  # Concorrente (25%)
    (30, "Ada Lovelace", "Partido Code", 0.15),  # Histórico (15%)
    (40, "Elon Musk", "Partido X", 0.10),  # Polêmico (10%)
    (50, "Candidato Null", "Partido Void", 0.05)  # Nanico (5%)
]


def gerar_cpf_unico(index):
    # Gera um CPF fake sequencial para garantir unicidade e validade de tamanho
    base = 10000000000 + index
    return str(base)


with open(FILENAME, "w") as f:
    # --- FASE 1: CADASTRO ---
    for cand in CANDIDATOS:
        f.write("1\n")  # Opção Cadastrar
        f.write(f"{cand[0]}\n")  # Numero
        f.write(f"{cand[1]}\n")  # Nome
        f.write(f"{cand[2]}\n")  # Partido

    # --- FASE 2: VOTAÇÃO ---
    for i in range(TOTAL_VOTOS):

        # 5% de chance de o eleitor errar o menu (digitar 9 ou letras)
        if random.random() < 0.05:
            f.write("9\n")  # Opção inválida
            # O sistema deve ignorar e pedir opção de novo, então não mandamos dados extras

        f.write("3\n")  # Opção Votar

        # Gera CPF
        cpf = gerar_cpf_unico(i)
        f.write(f"{cpf}\n")

        # Escolhe candidato baseado na popularidade (Pesos)
        # random.choices retorna uma lista, pegamos o primeiro item [0]
        # Depois pegamos o indice 0 da tupla (que é o numero do candidato)
        escolhido = random.choices(CANDIDATOS, weights=[c[3] for c in CANDIDATOS])[0]

        # 2% de chance de errar o numero do candidato (voto nulo/invalido na logica do sistema)
        if random.random() < 0.02:
            f.write("999\n")
        else:
            f.write(f"{escolhido[0]}\n")

    # --- FASE 3: RELATÓRIO E SAÍDA ---
    f.write("4\n")  # Ver Resultados
    f.write("5\n")  # Salvar e Sair

print(f"✅ Arquivo '{FILENAME}' gerado com {TOTAL_VOTOS} votos simulados.")