Projeto Banco de Dados II - Tema 3 (Alunos e Notas)
Grupo: [NOMES DOS INTEGRANTES]
IFSC Campus Gaspar - ADS

Como rodar o projeto:

1. Instalar MongoDB Community
   https://www.mongodb.com/try/download/community

2. Importar os dados do arquivo alunos.json
   mongoimport --db escola --collection alunos --file dados/alunos.json --jsonArray

3. Compilar o programa em C
   cd backend
   gcc -o alunos_crud alunos_crud.c `pkg-config --cflags --libs libmongoc-1.0`

4. Instalar dependencias do Node.js
   cd ../api
   npm install

5. Iniciar a aplicação
   npm start

6. Abrir o navegador em http://localhost:3000

Funcionalidades implementadas:
- Cadastrar aluno (insert)
- Buscar alunos por turma (find)
- Adicionar nota (update)
- Remover aluno (delete) [disponível via API, na interface futuramente]
- Média por disciplina (aggregate)
- Substituir cadastro completo (replace)