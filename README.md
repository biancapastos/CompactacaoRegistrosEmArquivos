# CompactacaoRegistrosEmArquivos
Projeto da disciplina de Estrutura de Dados 2 do curso Bacharelado em Ciências da Computação - Unesp Rio Claro

Objetivo: gerenciar os livros de uma biblioteca a partir de arquivos. Dados no arquivo insere: ISBN, Titulo, Autor, Ano.
Proposta: 
- Ler arquivo insere.bin com os dados dos livros e inserir em um arquivo principal.
Para isso, verificar se no arquivo existe espaço para inserção de um registro no lugar de outro que já foi removido.
- Ler arquivo remove.bin com ISBNs dos livros a serem removidos e realizar a remoção destes no arquivo principal.

Detalhes: 
- marcar nos arquivos insere e remove aqueles que já foram modificados.
- utilização de lista ligada e header (cabecalho) no arquivo principal para manipulação dos espaços entre os registros de campo e tamanho variável.
