# quicksort-arqavançada

## Execução

Após compilação, executar `./dist/main <NumberOfElements> <NumberOfThreads>`.

O programa alocará memória para executar todas as diferentes situações, e efetuará as operações usando os métodos:

- `qsort`, padrão, definida em `stdlib.h`.
- `quickSort_sequential`, definida em `sequential.h`.
- `quickSort_parallel`, definida em `parallel.h`.

Após cada método, o tempo de execução do mesmo é salvo, e seu resultado comparado com o resultado do método padrão, para verificar se foi executado corretamente.

Quando todas operações encerrarem, um sumário da execução é mostrado ao usuário.

## Argumentos

Executando `./dist/main`, há duas possíveis combinações de elementos.

- Se o usuário definir `<NumberOfThreads>`, o algoritmo `quickSort_parallel` será executado com o número de threads pedida.
- Caso o usuário não definir, ele então será executado três vezes, usando `1, 2 e 4` threads.

O sumário conterá os resultados de todas as operações.

## Makefile

- `make` constrói uma biblioteca estática em /lib, com todas as implementações de Quicksort, e após isso cria o executável `main` em /dist, para efetuar testes.

- `make clean` deleta todos os arquivos gerados.
