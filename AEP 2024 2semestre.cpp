#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define FILENAME "usuarios.txt"


typedef struct {
    char nome[50];
    char senha[50];
    char email[100];
} User;


void criptografar(char *str);
void descriptografar(char *str);
int senha_forte(char *senha);
void adicionar_usuario();
void listar_usuarios();
void alterar_usuario();
void excluir_usuario();



void criptografar(char *str) {
    while (*str) {
        *str = *str + 3; 
        str++;
    }
}


void descriptografar(char *str) {
    while (*str) {
        *str = *str - 3;
        str++;
    }
}


int senha_forte(char *senha) {
    int tem_numero = 0, caractere_especial = 0, tamanho = strlen(senha);

    if (tamanho < 5) {
        printf("A senha deve ter pelo menos 5 caracteres.\n");
        return 0;
    }

    for (int i = 0; i < tamanho; i++) {
        if (isdigit(senha[i])) tem_numero = 1;
        if (ispunct(senha[i])) caractere_especial = 1;
    }

    if (!tem_numero) printf("A senha deve conter pelo menos um n�mero.\n");
    if (!caractere_especial) printf("A senha deve conter pelo menos um caractere especial.\n");

    return tem_numero && caractere_especial;
}


void adicionar_usuario() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    User user;
    printf("Digite o nome de usu�rio: ");
    scanf("%s", user.nome);

    do {
        printf("Digite sua senha, dever� ter no m�nimo 5 caracteres, incluindo n�meros e caracteres especiais): ");
        scanf("%s", user.senha);
    } while (!senha_forte(user.senha));

    printf("Digite o e-mail: ");
    scanf("%s", user.email);

    criptografar(user.nome);
    criptografar(user.senha);
    criptografar(user.email);

    fprintf(file, "%s %s %s\n", user.nome, user.senha, user.email);
    fclose(file);

    printf("Usu�rio adicionado com sucesso!\n");
}


void listar_usuarios() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    User user;
    printf("Lista de usu�rios:\n");
    while (fscanf(file, "%s %s %s", user.nome, user.senha, user.email) != EOF) {
        descriptografar(user.nome);
        descriptografar(user.email);
      printf("Usu�rio: %s, Senha (criptografada): %s, Email: %s\n", user.nome, user.senha, user.email);
    }
    fclose(file);
}


void alterar_usuario() {
    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    User user;
    char nome[50];
    int encontrado = 0;
    printf("Digite o nome do usu�rio a ser alterado: ");
    scanf("%s", nome);

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao abrir o arquivo tempor�rio.\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%s %s %s", user.nome, user.senha, user.email) != EOF) {
        descriptografar(user.nome);
        descriptografar(user.senha);
        descriptografar(user.email);

        if (strcmp(user.nome, nome) == 0) {
            encontrado = 1;
            printf("Digite os novos dados.\n");
            printf("Novo nome de usu�rio: ");
            scanf("%s", user.nome);
            printf("Nova senha: ");
            scanf("%s", user.senha);
            printf("Novo e-mail: ");
            scanf("%s", user.email);
        }

        criptografar(user.nome);
        criptografar(user.senha);
        criptografar(user.email);
        fprintf(tempFile, "%s %s %s\n", user.nome, user.senha, user.email);
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (encontrado) {
        printf("Usu�rio alterado com sucesso!\n");
    } else {
        printf("Usu�rio n�o encontrado.\n");
    }
}


void excluir_usuario() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    User user;
    char nome[50];
    int encontrado = 0;
    printf("Digite o nome do usu�rio a ser exclu�do: ");
    scanf("%s", nome);

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao abrir o arquivo tempor�rio.\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%s %s %s", user.nome, user.senha, user.email) != EOF) {
        descriptografar(user.nome);
        descriptografar(user.senha);
        descriptografar(user.email);

        if (strcmp(user.nome, nome) != 0) {
            criptografar(user.nome);
            criptografar(user.senha);
            criptografar(user.email);
            fprintf(tempFile, "%s %s %s\n", user.nome, user.senha, user.email);
        } else {
            encontrado = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (encontrado) {
        printf("Usu�rio exclu�do com sucesso!\n");
    } else {
        printf("Usu�rio n�o encontrado.\n");
    }
}


int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Adicionar usu�rio\n");
        printf("2. Listar usu�rios\n");
        printf("3. Alterar usu�rio\n");
        printf("4. Excluir usu�rio\n");
        printf("5. Sair\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionar_usuario();
                break;
            case 2:
                listar_usuarios();
                break;
            case 3:
                alterar_usuario();
                break;
            case 4:
                excluir_usuario();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Op��o inv�lida!\n");
        }
    } while (opcao != 5);

    return 0;
}

