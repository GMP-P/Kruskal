@echo off
REM Compila, gera as instancias grandes e roda todos os testes.
REM Saida completa vai para resultados.txt

g++ -std=c++17 -O2 kruskal.cpp -o kruskal.exe
g++ -std=c++17 -O2 gerador.cpp -o gerador.exe

gerador.exe 2000 10000 grande_10000.txt
gerador.exe 20000 100000 grande_100000.txt
gerador.exe 100000 500000 grande_500000.txt
gerador.exe 200000 1000000 grande_1000000.txt
gerador.exe 400000 2000000 grande_2000000.txt
gerador.exe 1000000 5000000 grande_5000000.txt

echo ===== Teste 1 - pequeno ===== > resultados.txt
kruskal.exe teste1_pequeno.txt >> resultados.txt
echo ===== Teste 2 - medio ===== >> resultados.txt
kruskal.exe teste2_medio.txt >> resultados.txt
echo ===== Teste extra - desconexo ===== >> resultados.txt
kruskal.exe teste_desconexo.txt >> resultados.txt

for %%E in (10000 100000 500000 1000000 2000000 5000000) do (
    for %%R in (1 2 3) do (
        echo ===== Teste 3 - grande E=%%E execucao %%R ===== >> resultados.txt
        kruskal.exe grande_%%E.txt >> resultados.txt
    )
)

echo Pronto. Veja resultados.txt
