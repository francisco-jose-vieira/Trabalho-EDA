
import os
import pandas as pd
import matplotlib

# Usar modo sem janela
matplotlib.use("Agg")

import matplotlib.pyplot as plt

# Ler o arquivo CSV na mesma pasta
pasta = os.path.dirname(os.path.abspath(__file__))
dados = pd.read_csv(os.path.join(pasta, "relatorio.csv"))

# Criar o gráfico
plt.plot(dados["tamanho"], dados["tempo_segundos"], marker="o")

# Mostrar o tempo aproximado em cada ponto
for x, y in zip(dados["tamanho"], dados["tempo_segundos"]):
    if y < 1:
        tempo = f"{y * 1000:.0f} ms"
    elif y < 60:
        tempo = f"{y:.1f} s"
    else:
        tempo = f"{int(y // 60)} min {round(y % 60)} s"

    plt.annotate(
        tempo,
        (x, y),
        xytext=(5, 5),
        textcoords="offset points",
        fontsize=8
    )

# Configurar o gráfico
plt.title("Heap Sort: Tamanho do Vetor x Tempo")
plt.xlabel("Tamanho do vetor (elementos)")
plt.ylabel("Tempo aproximado")

plt.xscale("log")
plt.yticks([])
plt.grid(axis="x", alpha=0.3)
plt.tight_layout()

# Salvar o gráfico como imagem
saida = os.path.join(pasta, "grafico_heap_sort.png")
plt.savefig(saida, dpi=300, bbox_inches="tight")
plt.close()

print("Gráfico salvo com sucesso!")
print(saida)
