# Informações sobre o projeto

## Estrutura de diretórios
- build/ : Diretório responsável por construir o executável do projeto e linkar as dependências
- cmake/ : Diretório responsável por ter alguns macros para a utilização do `make clean-all`
- include/ : Diretório responsável por armazenar os arquivos de cabeçalho .h
- src/ : Diretório responsável por armazenar os códigos fonte do projeto

## Como rodar o projeto
- Primeiramente, é importante entrar dentro do diretório build/
- Atualize o arquivo makefile utilizando o seguinte comando: `cmake ..`
- Com o arquivo Makefile atualizado, é só utilizar o comando `make` para construir o executável
- Por fim, rode o programa com o comando: `./treeflix`

## Especificações
- O programa criara os seguintes arquivos:
    - root.dat: Arquivo que armazena a raíz da Árvore B+ 
    - movies.dat: Arquivo que armazena os registros/dados dos filmes em si 
    - ibtree.idx: Arquivo binário responsável por armazenar os nós da Árvore B+
    - ititle.idx: Arquivo responsável por armazenar os indices secundários para nossa lista
