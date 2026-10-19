
import os
import pandas as pd
import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt

# Ler o arquivo CSV
pasta = os.path.dirname(os.path.abspath(__file__))
arquivo = os.path.join(pasta, "relatorio_hash.csv")
dados = pd.read_csv(arquivo)

# Criar o gráfico
plt.figure(figsize=(12, 6))

plt.plot(
    range(len(dados)),
    dados["divisao"],
    marker="o",
    label="Método da divisão"
)

plt.plot(
    range(len(dados)),
    dados["multiplicacao"],
    marker="s",
    label="Método da multiplicação"
)

# Mostrar os valores reais de N no eixo X
plt.xticks(
    range(len(dados)),
    [f"{n:,}".replace(",", ".") for n in dados["n"]],
    rotation=45,
    ha="right"
)

# Configurar o gráfico
plt.title("Comparação de Colisões: Divisão x Multiplicação")
plt.xlabel("Tamanho da tabela (N)")
plt.ylabel("Quantidade de colisões")

plt.yscale("log")
plt.grid(axis="y", alpha=0.3)
plt.legend()
plt.tight_layout()

# Salvar o gráfico
saida = os.path.join(pasta, "grafico_colisoes_hash.png")
plt.savefig(saida, dpi=300, bbox_inches="tight")
plt.close()

print("Gráfico salvo com sucesso!")
print(saida)
