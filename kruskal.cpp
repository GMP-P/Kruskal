#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

struct Aresta {
    int origem;
    int destino;
    int peso;
};


// ordena as arestas pelo peso, em ordem crescente
void conquer(Aresta* a, Aresta* aux, int low, int middle, int high){
    for (int k = low; k <= high; k++) aux[k] = a[k];
    int i = low, j = middle + 1;
    for (int k = low; k <= high; k++){
        if      (i > middle)                a[k] = aux[j++];
        else if (j > high)                  a[k] = aux[i++];
        else if (aux[j].peso < aux[i].peso) a[k] = aux[j++];
        else                                a[k] = aux[i++];
    }
}

void divide(Aresta* a, Aresta* aux, int low, int high){
    if (low >= high) return;
    int middle = (low + high) / 2;
    divide(a, aux, low, middle);
    divide(a, aux, middle + 1, high);
    conquer(a, aux, low, middle, high);
}

void mergeSort(Aresta* a, int n){
    Aresta* aux = new Aresta[n];
    divide(a, aux, 0, n - 1);
    delete[] aux;
}

// Union-Find
// pai[v]    -> pai de v na arvore do conjunto (a raiz e o representante)
// altura[v] -> limite superior da altura da arvore com raiz em v

int* pai;
int* altura;

void criarConjuntos(int n){
    pai = new int[n];
    altura = new int[n];
    for (int v = 0; v < n; v++){
        pai[v] = v;      // no inicio cada vertice e o seu proprio conjunto
        altura[v] = 0;
    }
}

// devolve o representante do conjunto de v. no retorno, liga v direto na raiz para fazer compressão de caminho.
int encontrar(int v){
    if (pai[v] != v) pai[v] = encontrar(pai[v]);
    return pai[v];
}

// junta dois conjuntos. a arvore mais baixa fica embaixo da mais alta.
void unir(int raizA, int raizB){
    if      (altura[raizA] < altura[raizB]) pai[raizA] = raizB;
    else if (altura[raizA] > altura[raizB]) pai[raizB] = raizA;
    else {
        pai[raizB] = raizA;
        altura[raizA]++;
    }
}


// Kruskal - recebe as arestas já ordenadas e preenche "arvore". retorna quantas arestas entraram na arvore.

int analisadas = 0;   
int descartadas = 0;

int kruskal(Aresta* arestas, int V, int E, Aresta* arvore, bool mostrarPassos){
    criarConjuntos(V);
    int qtdArvore = 0;

    // condição de parada: acabaram as arestas ou a arvore ja tem V-1 arestas
    for (int i = 0; i < E && qtdArvore < V - 1; i++){
        analisadas++;
        int raizOrigem  = encontrar(arestas[i].origem);
        int raizDestino = encontrar(arestas[i].destino);

        if (mostrarPassos)
            cout << "Passo " << i + 1 << ": aresta (" << arestas[i].origem << " - "
                 << arestas[i].destino << ") peso " << arestas[i].peso;

        // escolha gulosa: a aresta mais leve que liga duas componentes diferentes
        if (raizOrigem != raizDestino){
            unir(raizOrigem, raizDestino);
            arvore[qtdArvore++] = arestas[i];
            if (mostrarPassos) cout << " -> ACEITA\n";
        } else {
            // os dois vertices ja estao ligados: a aresta fecharia um ciclo
            descartadas++;
            if (mostrarPassos) cout << " -> DESCARTADA (formaria ciclo)\n";
        }
    }

    delete[] pai;
    delete[] altura;
    return qtdArvore;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout << "Uso: kruskal arquivo.txt\n";
        return 1;
    }

    ifstream arquivo(argv[1]);
    if (!arquivo){
        cout << "Erro ao abrir " << argv[1] << "\n";
        return 1;
    }

    auto t0 = chrono::steady_clock::now();

    int V, E;
    arquivo >> V >> E;
    Aresta* arestas = new Aresta[E];
    for (int i = 0; i < E; i++)
        arquivo >> arestas[i].origem >> arestas[i].destino >> arestas[i].peso;
    arquivo.close();

    auto t1 = chrono::steady_clock::now();
    mergeSort(arestas, E);

    auto t2 = chrono::steady_clock::now();
    bool mostrarPassos = (E <= 100);   // so imprime passo a passo em grafos pequenos
    Aresta* arvore = new Aresta[V];
    int qtdArvore = kruskal(arestas, V, E, arvore, mostrarPassos);
    auto t3 = chrono::steady_clock::now();
-
    long long pesoTotal = 0;
    for (int i = 0; i < qtdArvore; i++) pesoTotal += arvore[i].peso;

    if (mostrarPassos){
        cout << "\nArestas da arvore geradora minima:\n";
        for (int i = 0; i < qtdArvore; i++)
            cout << "  (" << arvore[i].origem << " - " << arvore[i].destino
                 << ") peso " << arvore[i].peso << "\n";
    }

    double tLeitura   = chrono::duration<double, milli>(t1 - t0).count();
    double tOrdenacao = chrono::duration<double, milli>(t2 - t1).count();
    double tKruskal   = chrono::duration<double, milli>(t3 - t2).count();

    cout << "\nVertices: " << V << " | Arestas: " << E << "\n";
    cout << "Arestas na arvore: " << qtdArvore << " | Peso total: " << pesoTotal << "\n";
    cout << "Arestas analisadas: " << analisadas << " | Descartadas (ciclo): " << descartadas
         << " | Nao analisadas: " << E - analisadas << "\n";
    if (qtdArvore < V - 1)
        cout << "Atencao: o grafo e desconexo, o resultado e uma floresta geradora minima ("
             << V - qtdArvore << " componentes).\n";
    cout << "Tempo leitura: " << tLeitura << " ms | ordenacao: " << tOrdenacao
         << " ms | kruskal: " << tKruskal << " ms\n";

    delete[] arestas;
    delete[] arvore;
    return 0;
}
