# MultithreadedGameOfLife
Trabalho para a UC de Programação Concorrente e Distribuída

Enunciado:

O Jogo da Vida, criado por John H. Conway, utiliza um autômato celular para simular gerações sucessivas de uma sociedade de organismos vivos. É composto por um tabuleiro bi-dimensional, infinito em qualquer direção, de células quadradas idênticas. Cada célula tem exatamente oito células vizinhas (todas as células que compartilham, com a célula original, uma aresta ou um vértice). Cada célula está em um de dois estados: viva ou morta (correspondentes aos valores 1 ou 0). Uma geração da sociedade é representada pelo conjunto dos estados das células do tabuleiro. Sociedades evoluem de uma geração para a próxima aplicando simultaneamente, a todas as células do tabuleiro, regras que estabelecem o próximo estado de cada célula. As regras são:


  A. Células vivas com menos de 2 (dois) vizinhas vivas morrem por abandono;

  
  B. Cada célula viva com 2 (dois) ou 3 (três) vizinhos deve permanecer viva para a próxima geração;

  
  C. Cada célula viva com 4 (quatro) ou mais vizinhos morre por superpopulação;

  
  D. Cada célula morta com exatamente 3 (três) vizinhos deve se tornar viva.


Alternativamente ao Jogo do Vida existe uma versão do mesmo conhecida por Rainbow Game of Life, que tem uma única diferença: as novas células que tornam-se vivas deverão ter como valor a média aritmética dos valores na vizinhança imediata. Dessa forma, o tipo de dados das matrizes correspondentes devem ser do tipo FLOAT e as células vivas têm valor maior que zero

Foram feitas duas versões concorrentes do Rainbow Game of Life, em linguagem C utilizando PThreads e OpenMP, sobre um tabuleiro finito, NxN com bordas infinitas, ou seja, a fronteira esquerda liga-se com a fronteira direita e a fronteira superior liga-se com a fronteira inferior.
