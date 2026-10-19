# Árvore Geradora Mínima - Algoritmo de Kruskal

Atividade de Análise e Projeto de Algoritmos (Algoritmos Gulosos).

## Arquivos

- `kruskal.cpp` - implementação do Kruskal (Merge Sort + Union-Find)
- `gerador.cpp` - gera grafos conexos aleatórios para os testes grandes
- `teste1_pequeno.txt` - grafo de 9 vértices e 14 arestas (exemplo do Cormen, peso esperado 37)
- `teste2_medio.txt` - grafo de 20 vértices e 60 arestas
- `teste_desconexo.txt` - grafo com duas componentes (gera uma floresta)
- `rodar_testes.bat` - compila, gera as instâncias grandes e roda tudo (Windows)

## Formato da entrada

```
V E
u v peso
u v peso
...
```

Vértices numerados de 0 a V-1.

## Como compilar e rodar

```
g++ -std=c++17 -O2 kruskal.cpp -o kruskal
g++ -std=c++17 -O2 gerador.cpp -o gerador

kruskal teste1_pequeno.txt
gerador 200000 1000000 grande_1000000.txt
kruskal grande_1000000.txt
```

Para grafos com até 100 arestas o programa mostra cada decisão (aresta aceita ou descartada).
