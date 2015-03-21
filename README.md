# Arcabouço Unix

Adaptação para ambiente unix 
do ambiente de desenvolvimento e teste automatizado 
"Arcabouço de Testes" 
utilizado na disciplina INF1301 Programação Modular.

## ATENÇÃO 

* Siga as instruções de entrega dadas pelo professor.
* Este projeto é principalmente para o desenvolvimento e depuração do projeto.
* Se o ambiente windows for requisitado, compile para windows como instruído em aula.

#### Este repositório contém:

* /arcaboucounix - A biblioteca arcabouço a ser compilada, gerando o libArcaboucoTeste.a

* /template - Template básico de um projeto a ser testado, tendo um diretório estruturado e Makefiles de exemplo.

#### Como rodar:

  ```
  cd template/unixdevel
  ./config.sh
  ```
  Isso compilará o arcabouço gerando a biblioteca arcaboucounix/arcabouc/Fontes/libArcaboucoTeste.a 
  
  Além disso, criará uma série de pastas e no diretório do projeto (/template)
  
  Após ter um projeto estruturado, será possível compilar e testar cada modulo:
  ```
  cd ..
  cd modulo1
  make
  cd ..
  cd modulo2
  make
  ```
  Gerando assim dois executáveis, um em cada pasta. 
  
  Passando o .script como entrada para o executável, é possível testar seus modulos.
  
##### Este projeto não seria possível sem a ajuda de:

* [Marcelo Politzer](https://github.com/mpolitzer)
* [Rafael Bertoche](https://github.com/rbertoche)
* [Danilo Nascimento](https://github.com/daniloanp) 
* [Eduardo Verdeja](https://github.com/eaverdeja) 
