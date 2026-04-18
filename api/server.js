const express = require('express');
const cors = require('cors');
const path = require('path');
const db = require('./mongodb_helper');

const app = express();
const porta = 3000;

app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));

app.post('/api/alunos', async (req, res) => {
    const { nome, turma, notas } = req.body;
    const resultado = await db.inserir(nome, turma, notas[0], notas[1], notas[2]);
    res.json(resultado);
});

app.get('/api/alunos/turma/:turma', async (req, res) => {
    const resultado = await db.buscarPorTurma(req.params.turma);
    res.json(resultado);
});

app.get('/api/alunos', async (req, res) => {
    const resultado = await db.buscarPorTurma('');
    res.json(resultado);
});

app.put('/api/alunos/:id/nota', async (req, res) => {
    const resultado = await db.atualizarNota(req.params.id, req.body.disciplina, req.body.nota);
    res.json(resultado);
});

app.delete('/api/alunos/:id', async (req, res) => {
    const resultado = await db.deletar(req.params.id);
    res.json(resultado);
});

app.get('/api/estatisticas', async (req, res) => {
    const resultado = await db.mediaDisciplinas();
    res.json(resultado);
});

app.put('/api/alunos/:id/substituir', async (req, res) => {
    const resultado = await db.substituir(req.params.id, req.body.nome, req.body.turma, req.body.status);
    res.json(resultado);
});

app.listen(porta, () => {
    console.log(`Servidor rodando em http://localhost:${porta}`);
});