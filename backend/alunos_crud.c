#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void inserir_aluno(const char *nome, const char *turma, float n1, float n2, float n3) {
    mongoc_client_t *cliente;
    mongoc_collection_t *colecao;
    bson_t *documento;
    bson_error_t erro;
    
    srand(time(NULL));
    mongoc_init();
    
    cliente = mongoc_client_new("mongodb://localhost:27017");
    colecao = mongoc_client_get_collection(cliente, "escola", "alunos");
    
    documento = bson_new();
    BSON_APPEND_INT32(documento, "_id", rand() % 10000);
    BSON_APPEND_UTF8(documento, "nome", nome);
    BSON_APPEND_UTF8(documento, "turma", turma);
    
    bson_t disciplinas;
    BSON_APPEND_ARRAY_BEGIN(documento, "disciplinas", &disciplinas);
    
    bson_t disc;
    BSON_APPEND_DOCUMENT_BEGIN(&disciplinas, "0", &disc);
    BSON_APPEND_UTF8(&disc, "nome", "Banco de Dados II");
    
    bson_t notas;
    BSON_APPEND_ARRAY_BEGIN(&disc, "notas", &notas);
    BSON_APPEND_DOUBLE(&notas, "0", n1);
    BSON_APPEND_DOUBLE(&notas, "1", n2);
    BSON_APPEND_DOUBLE(&notas, "2", n3);
    bson_append_array_end(&disc, &notas);
    
    float media = (n1 + n2 + n3) / 3;
    BSON_APPEND_DOUBLE(&disc, "media", media);
    bson_append_document_end(&disciplinas, &disc);
    bson_append_array_end(documento, &disciplinas);
    
    BSON_APPEND_UTF8(documento, "status", "matriculado");
    
    if (!mongoc_collection_insert_one(colecao, documento, NULL, NULL, &erro)) {
        printf("{\"sucesso\": false, \"erro\": \"%s\"}\n", erro.message);
    } else {
        printf("{\"sucesso\": true, \"mensagem\": \"Aluno cadastrado com ID: %d\"}\n", rand() % 10000);
    }
    
    bson_destroy(documento);
    mongoc_collection_destroy(colecao);
    mongoc_client_destroy(cliente);
    mongoc_cleanup();
}

void buscar_por_turma(const char *turma) {
    mongoc_client_t *cliente;
    mongoc_collection_t *colecao;
    mongoc_cursor_t *cursor;
    bson_t *filtro;
    const bson_t *doc;
    
    mongoc_init();
    
    cliente = mongoc_client_new("mongodb://localhost:27017");
    colecao = mongoc_client_get_collection(cliente, "escola", "alunos");
    
    filtro = bson_new();
    BSON_APPEND_UTF8(filtro, "turma", turma);
    
    cursor = mongoc_collection_find_with_opts(colecao, filtro, NULL, NULL);
    
    printf("[");
    int primeiro = 1;
    while (mongoc_cursor_next(cursor, &doc)) {
        char *json_str = bson_as_canonical_extended_json(doc, NULL);
        if (!primeiro) printf(",");
        printf("%s", json_str);
        primeiro = 0;
        bson_free(json_str);
    }
    printf("]\n");
    
    bson_destroy(filtro);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(colecao);
    mongoc_client_destroy(cliente);
    mongoc_cleanup();
}

void buscar_todos() {
    mongoc_client_t *cliente;
    mongoc_collection_t *colecao;
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    
    mongoc_init();
    
    cliente = mongoc_client_new("mongodb://localhost:27017");
    colecao = mongoc_client_get_collection(cliente, "escola", "alunos");
    
    cursor = mongoc_collection_find_with_opts(colecao, bson_new(), NULL, NULL);
    
    printf("[");
    int primeiro = 1;
    while (mongoc_cursor_next(cursor, &doc)) {
        char *json_str = bson_as_canonical_extended_json(doc, NULL);
        if (!primeiro) printf(",");
        printf("%s", json_str);
        primeiro = 0;
        bson_free(json_str);
    }
    printf("]\n");
    
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(colecao);
    mongoc_client_destroy(cliente);
    mongoc_cleanup();
}

void atualizar_nota(int id, const char *disciplina, float nova_nota) {
    mongoc_client_t *cliente;
    mongoc_collection_t *colecao;
    bson_t *filtro;
    bson_t *atualizacao;
    bson_error_t erro;
    
    mongoc_init();
    
    cliente = mongoc_client_new("mongodb://localhost:27017");
    colecao = mongoc_client_get_collection(cliente, "escola", "alunos");
    
    filtro = bson_new();
    BSON_APPEND_INT32(filtro, "_id", id);
    
    atualizacao = BCON_NEW("$push", "{", "disciplinas.0.notas", BCON_DOUBLE(nova_nota), "}");
    
    if (!mongoc_collection_update_one(colecao, filtro, atualizacao, NULL, NULL, &erro)) {
        printf("{\"sucesso\": false, \"erro\": \"%s\"}\n", erro.message);
    } else {
        printf("{\"sucesso\": true, \"mensagem\": \"Nota adicionada ao aluno %d\"}\n", id);
    }
    
    bson_destroy(filtro);
    bson_destroy(atualizacao);
    mongoc_collection_destroy(colecao);
    mongoc_client_destroy(cliente);
    mongoc_cleanup();
}

void deletar_aluno(int id) {
    mongoc_client_t *cliente;
    mongoc_collection_t *colecao;
    bson_t *filtro;
    bson_error_t erro;
    
    mongoc_init();
    
    cliente = mongoc_client_new("mongodb://localhost:27017");
    colecao = mongoc_client_get_collection(cliente, "escola", "alunos");
    
    filtro = bson_new();
    BSON_APPEND_INT32(filtro, "_id", id);
    
    if (!mongoc_collection_delete_one(colecao, filtro, NULL, NULL, &erro)) {
        printf("{\"sucesso\": false, \"erro\": \"%s\"}\n", erro.message);
    } else {
        printf("{\"sucesso\": true, \"mensagem\": \"Aluno %d removido\"}\n", id);
    }
    
    bson_destroy(filtro);
    mongoc_collection_destroy(colecao);
    mongoc_client_destroy(cliente);
    mongoc_cleanup();
}

void media_por_disciplina() {
    mongoc_client_t *cliente;
    mongoc_collection_t *colecao;
    mongoc_cursor_t *cursor;
    bson_t *pipeline;
    const bson_t *doc;
    
    mongoc_init();
    
    cliente = mongoc_client_new("mongodb://localhost:27017");
    colecao = mongoc_client_get_collection(cliente, "escola", "alunos");
    
    pipeline = BCON_NEW(
        "pipeline", "[",
            "{", "$unwind", "{", "path", BCON_UTF8("$disciplinas"), "}", "}",
            "{", "$group", "{",
                "_id", BCON_UTF8("$disciplinas.nome"),
                "media_geral", "{", "$avg", BCON_UTF8("$disciplinas.media"), "}",
                "total_alunos", "{", "$sum", BCON_INT32(1), "}",
            "}", "}",
        "]"
    );
    
    cursor = mongoc_collection_aggregate(colecao, MONGOC_QUERY_NONE, pipeline, NULL, NULL);
    
    printf("[");
    int primeiro = 1;
    while (mongoc_cursor_next(cursor, &doc)) {
        char *json_str = bson_as_canonical_extended_json(doc, NULL);
        if (!primeiro) printf(",");
        printf("%s", json_str);
        primeiro = 0;
        bson_free(json_str);
    }
    printf("]\n");
    
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(colecao);
    mongoc_client_destroy(cliente);
    mongoc_cleanup();
}

void substituir_aluno(int id, const char *nome, const char *turma, const char *status) {
    mongoc_client_t *cliente;
    mongoc_collection_t *colecao;
    bson_t *filtro;
    bson_t *novo_doc;
    bson_error_t erro;
    
    mongoc_init();
    
    cliente = mongoc_client_new("mongodb://localhost:27017");
    colecao = mongoc_client_get_collection(cliente, "escola", "alunos");
    
    filtro = bson_new();
    BSON_APPEND_INT32(filtro, "_id", id);
    
    novo_doc = bson_new();
    BSON_APPEND_INT32(novo_doc, "_id", id);
    BSON_APPEND_UTF8(novo_doc, "nome", nome);
    BSON_APPEND_UTF8(novo_doc, "turma", turma);
    BSON_APPEND_UTF8(novo_doc, "status", status);
    
    if (!mongoc_collection_replace_one(colecao, filtro, novo_doc, NULL, NULL, &erro)) {
        printf("{\"sucesso\": false, \"erro\": \"%s\"}\n", erro.message);
    } else {
        printf("{\"sucesso\": true, \"mensagem\": \"Cadastro do aluno %d substituido\"}\n", id);
    }
    
    bson_destroy(filtro);
    bson_destroy(novo_doc);
    mongoc_collection_destroy(colecao);
    mongoc_client_destroy(cliente);
    mongoc_cleanup();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("{\"erro\": \"Use: alunos_crud <comando> [args]\"}\n");
        return 1;
    }
    
    if (strcmp(argv[1], "inserir") == 0 && argc == 7) {
        inserir_aluno(argv[2], argv[3], atof(argv[4]), atof(argv[5]), atof(argv[6]));
    }
    else if (strcmp(argv[1], "buscar") == 0 && argc == 3) {
        buscar_por_turma(argv[2]);
    }
    else if (strcmp(argv[1], "todos") == 0) {
        buscar_todos();
    }
    else if (strcmp(argv[1], "atualizar") == 0 && argc == 5) {
        atualizar_nota(atoi(argv[2]), argv[3], atof(argv[4]));
    }
    else if (strcmp(argv[1], "deletar") == 0 && argc == 3) {
        deletar_aluno(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "media") == 0) {
        media_por_disciplina();
    }
    else if (strcmp(argv[1], "substituir") == 0 && argc == 6) {
        substituir_aluno(atoi(argv[2]), argv[3], argv[4], argv[5]);
    }
    else {
        printf("{\"erro\": \"Comando invalido. Use: inserir, buscar, todos, atualizar, deletar, media, substituir\"}\n");
    }
    
    return 0;
}