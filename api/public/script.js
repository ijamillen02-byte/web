function mostrarAba(nome) {
    const abas = ['cadastrar', 'buscar', 'notas', 'estatisticas', 'substituir'];
    abas.forEach(aba => {
        const elemento = document.getElementById(aba);
        if (elemento) elemento.style.display = 'none';
    });
    document.getElementById(nome).style.display = 'block';
}

async function cadastrar() {
    const nome = document.getElementById('nome').value;
    const turma = document.getElementById('turma').value;
    const n1 = parseFloat(document.getElementById('n1').value);
    const n2 = parseFloat(document.getElementById('n2').value);
    const n3 = parseFloat(document.getElementById('n3').value);
    
    const resposta = await fetch('http://localhost:3000/api/alunos', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ nome, turma, notas: [n1, n2, n3] })
    });
    
    const dados = await resposta.json();
    document.getElementById('resCadastro').innerHTML = JSON.stringify(dados, null, 2);
}

async function buscar() {
    const turma = document.getElementById('turmaBusca').value;
    const resposta = await fetch(`http://localhost:3000/api/alunos/turma/${turma}`);
    const dados = await resposta.json();
    document.getElementById('resBusca').innerHTML = JSON.stringify(dados, null, 2);
}

async function buscarTodos() {
    const resposta = await fetch('http://localhost:3000/api/alunos');
    const dados = await resposta.json();
    document.getElementById('resBusca').innerHTML = JSON.stringify(dados, null, 2);
}

async function atualizarNota() {
    const id = document.getElementById('idAluno').value;
    const disciplina = document.getElementById('disciplinaNota').value;
    const nota = parseFloat(document.getElementById('novaNota').value);
    
    const resposta = await fetch(`http://localhost:3000/api/alunos/${id}/nota`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ disciplina, nota })
    });
    
    const dados = await resposta.json();
    document.getElementById('resNota').innerHTML = JSON.stringify(dados, null, 2);
}

async function carregarEstatisticas() {
    const resposta = await fetch('http://localhost:3000/api/estatisticas');
    const dados = await resposta.json();
    
    if (dados.dados) {
        document.getElementById('resEstatisticas').innerHTML = dados.dados;
    } else {
        document.getElementById('resEstatisticas').innerHTML = JSON.stringify(dados, null, 2);
    }
}

async function substituir() {
    const id = document.getElementById('idSubst').value;
    const nome = document.getElementById('nomeSubst').value;
    const turma = document.getElementById('turmaSubst').value;
    const status = document.getElementById('statusSubst').value;
    
    const resposta = await fetch(`http://localhost:3000/api/alunos/${id}/substituir`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ nome, turma, status })
    });
    
    const dados = await resposta.json();
    document.getElementById('resSubst').innerHTML = JSON.stringify(dados, null, 2);
}