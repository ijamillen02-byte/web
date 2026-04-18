const { exec } = require('child_process');
const path = require('path');

const caminhoC = path.join(__dirname, '../backend/alunos_crud');

function executarComando(args) {
    return new Promise((resolve) => {
        const comando = `${caminhoC} ${args.join(' ')}`;
        
        exec(comando, (erro, stdout, stderr) => {
            if (erro) {
                resolve({ sucesso: false, erro: erro.message });
                return;
            }
            try {
                const resultado = JSON.parse(stdout);
                resolve(resultado);
            } catch {
                resolve({ sucesso: true, dados: stdout });
            }
        });
    });
}

module.exports = {
    inserir: (nome, turma, n1, n2, n3) => {
        return executarComando(['inserir', nome, turma, n1.toString(), n2.toString(), n3.toString()]);
    },
    buscarPorTurma: (turma) => {
        return executarComando(['buscar', turma]);
    },
    buscarTodos: () => {
        return executarComando(['todos']);
    },
    atualizarNota: (id, disciplina, nota) => {
        return executarComando(['atualizar', id.toString(), disciplina, nota.toString()]);
    },
    deletar: (id) => {
        return executarComando(['deletar', id.toString()]);
    },
    mediaDisciplinas: () => {
        return executarComando(['media']);
    },
    substituir: (id, nome, turma, status) => {
        return executarComando(['substituir', id.toString(), nome, turma, status]);
    }
};