#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar dados da pessoa
typedef struct pessoa {
    char nome[100];
    char masp[100];
    char cpf[100];
} pessoa;

// Estrutura da reunião agora contém uma 'pessoa'
typedef struct reuniao {
    pessoa participante;        // Associa uma pessoa à reunião
    char data_reuniao[11];      // Formato: DD/MM/AAAA
    char horario[6];            // Formato: HH:MM
    char local[50];             // Local da reunião
    char assunto[100];          // Assunto da reunião
} reuniao;


#define ARQUIVO_REGISTROS "reunioes.bat"

void cabecalho() {
    printf("\n=== CALENDARIO DE REUNIOES ===\n");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Nenhuma mudança necessária aqui
int salvarRegistros(struct reuniao *registros, int contador) {
    FILE *arquivo = fopen(ARQUIVO_REGISTROS, "wb");
    if(arquivo == NULL) {
        return 0;
    }
    
    fwrite(&contador, sizeof(int), 1, arquivo);
    fwrite(registros, sizeof(struct reuniao), contador, arquivo);
    
    fclose(arquivo);
    return 1;
}

// Nenhuma mudança necessária aqui
int carregarRegistros(struct reuniao **registros, int *contador, int *capacidade) {
    FILE *arquivo = fopen(ARQUIVO_REGISTROS, "rb");
    if(arquivo == NULL) {
        return 0;
    }
    
    fread(contador, sizeof(int), 1, arquivo);
    *capacidade = (*contador < 5) ? 5 : *contador + 5;
    *registros = malloc(*capacidade * sizeof(struct reuniao));
    
    if(*registros == NULL) {
        fclose(arquivo);
        return 0;
    }
    
    fread(*registros, sizeof(struct reuniao), *contador, arquivo);
    fclose(arquivo);
    return 1;
}

// Nenhuma mudança, pois sempre recebe um usuário válido
void agendarReuniao(struct reuniao **registros, int *contador, int *capacidade, pessoa usuario) {
    cabecalho();
    printf("AGENDAR NOVA REUNIAO PARA %s\n\n", usuario.nome);
    
    if(*contador >= *capacidade) {
        *capacidade += 5;
        *registros = realloc(*registros, *capacidade * sizeof(struct reuniao));
        if(*registros == NULL) {
            printf("Erro ao alocar memoria!\n");
            return;
        }
    }
    
    strcpy((*registros)[*contador].participante.nome, usuario.nome);
    strcpy((*registros)[*contador].participante.masp, usuario.masp);
    strcpy((*registros)[*contador].participante.cpf, usuario.cpf);
    
    printf("Data da reuniao (DD/MM/AAAA): ");
    limparBuffer();
    fgets((*registros)[*contador].data_reuniao, 11, stdin);
    limparBuffer();
    
    printf("Horario (HH:MM): ");
    fgets((*registros)[*contador].horario, 6, stdin);
    limparBuffer();
    
    printf("Local: ");
    fgets((*registros)[*contador].local, 50, stdin);
    (*registros)[*contador].local[strcspn((*registros)[*contador].local, "\n")] = 0;
    
    printf("Assunto: ");
    fgets((*registros)[*contador].assunto, 100, stdin);
    (*registros)[*contador].assunto[strcspn((*registros)[*contador].assunto, "\n")] = 0;
    
    (*contador)++;
    printf("\nReuniao agendada com sucesso!\n");
}

// Nenhuma mudança, continua filtrando pelo MASP
void listarReunioes(struct reuniao *registros, int contador, pessoa usuario) {
    cabecalho();
    printf("LISTA DE REUNIOES AGENDADAS PARA %s\n\n", usuario.nome);
    
    int encontradas = 0;
    for(int i = 0; i < contador; i++) {
        if (strcmp(registros[i].participante.masp, usuario.masp) == 0) {
            printf("Reuniao %d:\n", encontradas + 1);
            printf("   Participante: %s\n", registros[i].participante.nome);
            printf("   MASP: %s\n", registros[i].participante.masp);
            printf("   Data: %s\n", registros[i].data_reuniao);
            printf("   Horario: %s\n", registros[i].horario);
            printf("   Local: %s\n", registros[i].local);
            printf("   Assunto: %s\n", registros[i].assunto);
            printf("   --------------------\n");
            encontradas++;
        }
    }

    if(encontradas == 0) {
        printf("Nenhuma reuniao agendada para este usuario.\n");
    }
}

// Nenhuma mudança, continua filtrando pelo MASP
void cancelarReuniao(struct reuniao *registros, int *contador, pessoa usuario) {
    cabecalho();
    printf("CANCELAR REUNIAO DE %s\n\n", usuario.nome);
    
    int indicesReais[(*contador)];
    int contadorUsuario = 0;

    printf("Suas reunioes agendadas:\n");
    for(int i = 0; i < *contador; i++) {
        if (strcmp(registros[i].participante.masp, usuario.masp) == 0) {
            printf("%d. %s as %s - %s\n", contadorUsuario + 1, registros[i].data_reuniao, registros[i].horario, registros[i].assunto);
            indicesReais[contadorUsuario] = i;
            contadorUsuario++;
        }
    }
    
    if(contadorUsuario == 0) {
        printf("\nNenhuma reuniao agendada para cancelar.\n");
        return;
    }
    
    int indiceEscolhido;
    printf("\nDigite o numero da reuniao que deseja cancelar: ");
    scanf("%d", &indiceEscolhido);
    
    if(indiceEscolhido < 1 || indiceEscolhido > contadorUsuario) {
        printf("Numero invalido!\n");
        return;
    }
    
    int indiceRealParaApagar = indicesReais[indiceEscolhido - 1];
    
    for(int i = indiceRealParaApagar; i < *contador - 1; i++) {
        registros[i] = registros[i + 1];
    }
    
    (*contador)--;
    printf("Reuniao cancelada com sucesso!\n");
}

// Nenhuma mudança, continua filtrando pelo MASP
void reunioesDoMes(struct reuniao *registros, int contador, pessoa usuario) {
    cabecalho();
    printf("REUNIOES DO MES PARA %s\n\n", usuario.nome);
    
    char mes[3];
    printf("Digite o mes (MM) para consulta: ");
    scanf("%s", mes);
    
    printf("\nReunioes agendadas para o mes %s:\n", mes);
    int encontradas = 0;
    
    for(int i = 0; i < contador; i++) {
        if (strcmp(registros[i].participante.masp, usuario.masp) == 0) {
            if(registros[i].data_reuniao[3] == mes[0] && registros[i].data_reuniao[4] == mes[1]) {
                printf("   %s - %s - %s\n", 
                       registros[i].data_reuniao, 
                       registros[i].horario,
                       registros[i].assunto);
                encontradas++;
            }
        }
    }
    
    if(encontradas == 0) {
        printf("Nenhuma reuniao encontrada para este usuario no mes selecionado.\n");
    }
}

int menuPrincipal(int contador, pessoa usuario) {
    cabecalho();
    printf("Usuario: %s (MASP: %s)\n", usuario.nome, usuario.masp);
    printf("Total de Reunioes no Sistema: %d\n\n", contador);
    
    printf("1. Agendar nova reuniao\n");
    printf("2. Listar minhas reunioes\n");
    printf("3. Cancelar reuniao\n");
    printf("4. Consultar minhas reunioes do mes\n");
    printf("5. Sair do programa\n");
    
    int opcao;
    printf("\nEscolha uma opcao: ");
    scanf("%d", &opcao);
    return opcao;
}

// MODIFICADO: Nova função de login
// MODIFICADO: Nova função de login corrigida
void realizarLogin(pessoa *usuario, reuniao *registros, int contador) {
    char maspLogin[100];
    char cpfLogin[100];

    cabecalho();
    printf("--- LOGIN DO USUARIO ---\n");
    
    printf("Digite seu MASP: ");
    limparBuffer();
    fgets(maspLogin, 100, stdin);
    maspLogin[strcspn(maspLogin, " ")] = 0;

    printf("Digite seu CPF: ");
    fgets(cpfLogin, 100, stdin);
    cpfLogin[strcspn(cpfLogin, " ")] = 0;

    // Procura por um usuário existente nos registros
    int usuarioEncontrado = 0;
    for (int i = 0; i < contador; i++) {
        if (strcmp(registros[i].participante.masp, maspLogin) == 0 && 
            strcmp(registros[i].participante.cpf, cpfLogin) == 0) {
            
            *usuario = registros[i].participante;
            usuarioEncontrado = 1;
            printf("\nBem-vindo(a) de volta, %s!\n", usuario->nome);
            break;
        }
    }

    // Se não encontrou, faz novo cadastro
    if (!usuarioEncontrado) {
        printf("\nUsuario nao encontrado. Realizando novo cadastro...\n");
        printf("Digite seu Nome Completo: ");
        fgets(usuario->nome, 100, stdin);
        usuario->nome[strcspn(usuario->nome, "\n")] = 0;
        
        strcpy(usuario->masp, maspLogin);
        strcpy(usuario->cpf, cpfLogin);
        
        printf("\nCadastro realizado com sucesso! Bem-vindo(a), %s!\n", usuario->nome);
    }
    
    printf("Pressione Enter para continuar...");
    getchar();
}


int main() {
    struct reuniao *registros = malloc(5 * sizeof(struct reuniao));
    int contador = 0;
    int capacidade = 5;
    int executando = 1;
    pessoa usuarioAtual;

    if(carregarRegistros(&registros, &contador, &capacidade)) {
        printf("Dados de reunioes anteriores carregados com sucesso!\n");
    }

    // MODIFICADO: Chama a nova função de login
    realizarLogin(&usuarioAtual, registros, contador);

    while(executando) {
        int opcao = menuPrincipal(contador, usuarioAtual);
        
        switch(opcao) {
            case 1:
                agendarReuniao(&registros, &contador, &capacidade, usuarioAtual);
                break;
            case 2:
                listarReunioes(registros, contador, usuarioAtual);
                break;
            case 3:
                cancelarReuniao(registros, &contador, usuarioAtual);
                break;
            case 4:
                reunioesDoMes(registros, contador, usuarioAtual);
                break;
            case 5:
                executando = 0;
                break;
            default:
                printf("Opcao invalida!\n");
        }
        
        if(opcao >= 1 && opcao <= 4) {
            if(!salvarRegistros(registros, contador)) {
                printf("Erro ao salvar dados!\n");
            }
        }
        
        if (executando) {
            printf("\nPressione Enter para continuar...");
            limparBuffer();
            getchar();
        }
    }

    salvarRegistros(registros, contador);
    free(registros);
    printf("\nPrograma encerrado. Ate mais, %s!\n", usuarioAtual.nome);
    
    return 0;
}