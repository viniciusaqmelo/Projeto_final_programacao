#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_PLYLST 200
#define TAM_NOME 50
#define TAM_LOGIN 15
#define TAM_SENHA 15
#define TAM_PLAYLIST 100
#define TAM_TITULO 50
#define TAM_MUSICAS 100

typedef struct Musica {
    int codigo; // autoincremento
    char titulo[TAM_TITULO]; // título da música
    char artista[TAM_NOME]; // nome do artista que canta esta música
} Musica;

typedef struct Playlist {
    int codigo; // autoincremento
    int codUsuario; // o código do usuário que criou a playlist
    char titulo[TAM_TITULO]; // título desta playlist
    int qtdMusicas; // qtd atual de músicas incluídas nesta playlist
    int musicas[TAM_MUSICAS]; // vetor contendo os códigos das músicas
} Playlist;

typedef struct Usuario {
    int codigo; // autoincremento
    char nome[TAM_NOME];
    char login[TAM_LOGIN];
    char senha[TAM_SENHA];
    int qtdPlaylists; // quantid. de playlists criadas pelo usuário
    Playlist playlists[TAM_PLAYLIST]; // todas playlists criadas
    int qtdPlaylistsFav; // quantid. de playlists favoritadas pelo usuário
    int playlistsFav[TAM_PLAYLIST]; // códigos das playlists favoritadas pelo usuário
} Usuario;

// V. Globais
int user_code = 1;
int music_code = 1;
int playlist_code = 1;
Usuario* usuarios[100];
Musica* musicas[300];
Playlist* playlists[500];
//

// Assinaturas
Usuario* criar_usuario(char nome[TAM_NOME], char login[TAM_LOGIN], char senha[TAM_SENHA]);
Usuario* buscar_usuario(int codigo);
Usuario* buscar_u_nome(char nome[TAM_NOME]);
void usuario_to_string(Usuario* user);
void alterar_senha(int codigo, char nova_senha[TAM_SENHA]);
void excluir_usuario(int codigo);
void mostrar_usuarios();
void liberar_usuarios();

Musica* criar_musica(char titulo[TAM_TITULO], char artista[TAM_NOME]);
Musica* buscar_musica(int codigo);
void musica_to_string(Musica* musica);
void alterar_titulo(int codigo, char titulo[TAM_TITULO]);
void alterar_artista(int codigo, char artista[TAM_NOME]);
void excluir_musica(int codigo);
void mostrar_musicas();
void liberar_musicas();

Playlist* criar_playlist(Usuario *user, char titulo[TAM_TITULO]);
Playlist* buscar_playlist(int codigo);
void adicionar_musica(int codigo_playlist, int codigo_musica);
void consultar_playlist_por_musica(int codigo_musica);
void playlist_to_string(Playlist* playlist);
void excluir_playlist(int codigo);
void excluir_musica_p(int codigo_p, int codigo_m);
void mostrar_playlists();
void liberar_playlists();

void menuAdm();

void menuUser();
Usuario* menuLoginUser();
void menuRegisUser();

void biblioteca();
void limpar_buffer();
//

// Programa
int main(void) {
    usuarios[0] = NULL;
    musicas[0] = NULL;
    playlists[0] = NULL;

    biblioteca();

    int opcao;
    int sair = 0;

    while (!sair) {
        printf("----- Seja bem vindo -----\n");
        printf("Voce e um usuario normal ou administrador?\n");
        printf("Opcao 1 para adm:\nOpcao 2 para usuario:\n");
        printf("Opcao 0 para sair:\n");

        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Voce escolheu acessar como administrador\n");
                menuAdm();
                break;
            case 2:
                printf("Voce escolheu acessar como usuario\n");
                menuUser();
                break;
            case 0:
                sair = 1;
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida\n");
                break;
        }
    }

    // Liberar memória alocada
    liberar_usuarios();
    liberar_musicas();
    liberar_playlists();
    //

    return 0;
}

// Funções
Usuario* criar_usuario(char nome[TAM_NOME], char login[TAM_LOGIN], char senha[TAM_SENHA]) {
    Usuario* user = malloc(sizeof(Usuario));

    user->codigo = user_code;
    user_code++;

    strcpy(user->nome, nome);
    strcpy(user->login, login);
    strcpy(user->senha, senha);

    user->qtdPlaylists = 0;
    user->qtdPlaylistsFav = 0;

    int const pos = user->codigo-1;
    usuarios[pos] = user;
    usuarios[pos+1] = NULL;

    return user;
}

Usuario* buscar_usuario(int codigo) {
    for (int i = 0; i < 100 && usuarios[i] != NULL; i++) {
        if (usuarios[i]->codigo == codigo) {
            return usuarios[i];
        }
    }

    return NULL;
}

Usuario* buscar_u_nome(char nome[TAM_NOME]) {
    for (int i = 0; i < 100 && usuarios[i] != NULL; i++) {
        if (!strcmp(usuarios[i]->nome, nome)) {
            return usuarios[i];
        }
    }

    return NULL;
}

void usuario_to_string (Usuario* user) {
    printf("Code: %d\n", user->codigo);
    printf("Name: %s\n", user->nome);
    printf("Login: %s\n", user->login);
    printf("Playlists: %d\n", user->qtdPlaylists);
    printf("PlaylistsFav: %d\n", user->qtdPlaylistsFav);
    printf("\n");
}

void alterar_senha(int codigo, char nova_senha[TAM_SENHA]) {
    Usuario* user = buscar_usuario(codigo);

    if (user != NULL) {
        strcpy(user->senha, nova_senha);
    }
}

void excluir_usuario(int codigo) {
    int const pos = codigo-1;

    Usuario* u = usuarios[pos];
    free(u);

    for (int i = pos; i < 100; i++) {
        usuarios[i] = usuarios[i+1];

        if (usuarios[i+1] != NULL) {
            usuarios[i+1]->codigo--;
        }

        if (usuarios[i+1] == NULL) {
            i = 100;
        }
    }

    user_code--;
}

void mostrar_usuarios() {
    int i = 0;
    while(usuarios[i] != NULL) {
        printf("Code: %d\n", usuarios[i]->codigo);
        printf("Name: %s\n", usuarios[i]->nome);
        printf("Login: %s\n", usuarios[i]->login);
        printf("Playlists: %d\n", usuarios[i]->qtdPlaylists);
        printf("PlaylistsFav: %d\n", usuarios[i]->qtdPlaylistsFav);
        printf("\n");
        i++;
    }
}

void liberar_usuarios() {
    for (int i = 0; i < 100; i++) {
        if (usuarios[i] != NULL) {
            free(usuarios[i]);
        }

        if (usuarios[i] == NULL) break;
    }
}

Musica* criar_musica(char titulo[TAM_TITULO], char artista[TAM_NOME]) {
    Musica* music = malloc(sizeof(Musica));

    music->codigo = music_code;
    music_code++;

    strcpy(music->titulo, titulo);
    strcpy(music->artista, artista);

    int const pos = music->codigo-1;
    musicas[pos] = music;
    musicas[pos+1] = NULL;

    return music;
}

Musica* buscar_musica(int codigo) {
    for (int i = 0; i < 300 && musicas[i] != NULL; i++) {
        if (musicas[i]->codigo == codigo) {
            return musicas[i];
        }
    }

    return NULL;
}

void musica_to_string(Musica* musica) {
    printf("Code: %d\n", musica->codigo);
    printf("Title: %s\n", musica->titulo);
    printf("Artist: %s\n", musica->artista);
    printf("\n");
}

void alterar_titulo(int codigo, char titulo[TAM_TITULO]) {
    Musica* musica = buscar_musica(codigo);

    if (musica != NULL) {
        strcpy(musica->titulo, titulo);
    }
}

void alterar_artista(int codigo, char artista[TAM_NOME]) {
    Musica* musica = buscar_musica(codigo);

    if (musica != NULL) {
        strcpy(musica->artista, artista);
    }
}

void excluir_musica(int codigo) {
    int const pos = codigo-1;

    Musica* m = musicas[pos];
    free(m);

    for (int i = pos; i < 300; i++) {
        musicas[i] = musicas[i+1];

        if (musicas[i+1] != NULL) {
            musicas[i+1]->codigo--;
        }

        if (musicas[i+1] == NULL) {
            i = 300;
        }
    }

    music_code--;
}

void mostrar_musicas() {
    int i = 0;
    while(musicas[i] != NULL) {
        printf("Code: %d\n", musicas[i]->codigo);
        printf("Title: %s\n", musicas[i]->titulo);
        printf("Artist: %s\n", musicas[i]->artista);
        printf("\n");
        i++;
    }
}

void liberar_musicas() {
    for (int i = 0; i < 300; i++) {
        if (musicas[i] != NULL) {
            free(musicas[i]);
        }

        if (musicas[i] == NULL) break;
    }
}

Playlist* criar_playlist(Usuario *user, char titulo[TAM_TITULO]) {
    Playlist* playlist = malloc(sizeof(Playlist));

    playlist->codigo = playlist_code;
    playlist_code++;

    playlist->codUsuario = user->codigo;

    playlist->qtdMusicas = 0;

    strcpy(playlist->titulo, titulo);

    user->playlists[user->qtdPlaylists] = *playlist;
    user->qtdPlaylists++;

    // Controle de onde acaba
    playlist->musicas[0] = 0;

    int const pos = playlist->codigo-1;
    playlists[pos] = playlist;
    playlists[pos+1] = NULL;

    return playlist;
}

Playlist* buscar_playlist(int codigo) {
    for (int i = 0; i < 500 && playlists[i] != NULL; i++) {
        if (playlists[i]->codigo == codigo) {
            return playlists[i];
        }
    }

    return NULL;
}

void adicionar_musica(int codigo_playlist, int codigo_musica) {
    Playlist* pl = buscar_playlist(codigo_playlist);

    pl->musicas[pl->qtdMusicas] = codigo_musica;
    pl->qtdMusicas++;
    pl->musicas[pl->qtdMusicas] = 0;
}

void consultar_playlist_por_musica(int codigo_musica) {
    for (int i = 0; i < 500 && playlists[i] != NULL; i++) {
        for (int j = 0; j < TAM_MUSICAS && playlists[i]->musicas[j] != 0; j++) {
            if (playlists[i]->musicas[j] == codigo_musica) {
                playlist_to_string(playlists[i]);
            }
        }
    }
}

void playlist_to_string(Playlist* playlist) {
    printf("Code: %d\n", playlist->codigo);
    printf("User: %s\n", buscar_usuario(playlist->codUsuario)->nome);
    printf("Title: %s\n", playlist->titulo);
    printf("qtdMusicas: %d\n", playlist->qtdMusicas);

    if (playlist->musicas[0] != 0) {
        printf("Musicas:\n\n");
    }

    for (int i = 0; i < TAM_MUSICAS && playlist->musicas[i] != 0; i++) {
        musica_to_string(buscar_musica(playlist->musicas[i]));
    }

    printf("\n");
}

void excluir_musica_p(int codigo_p, int codigo_m) {
    Playlist* p = buscar_playlist(codigo_p);

    int c = 0;

    for (int i = 0; i < 100 && p->musicas[i] != 0; i++) {
        if (p->musicas[i] == codigo_m) {
            p->musicas[i] = p->musicas[i+1];
            c = 1; 
        }

        if (c) {
            p->musicas[i] = p->musicas[i+1];
        }
    }

    p->qtdMusicas--;
}

void excluir_playlist(int codigo) {
    int const pos = codigo-1;

    Playlist* p = playlists[pos];
    free(p);

    for (int i = pos; i < 500; i++) {
        playlists[i] = playlists[i+1];

        if (playlists[i+1] != NULL) {
            playlists[i+1]->codigo--;
        }

        if (playlists[i+1] == NULL) {
            i = 500;
        }
    }

    playlist_code--;
}

void mostrar_playlists() {
    int i = 0;
    while(playlists[i] != NULL) {
        playlist_to_string(playlists[i]);
        i++;
    }
}

void liberar_playlists() {
    for (int i = 0; i < 100; i++) {
        if (playlists[i] != NULL) {
            free(playlists[i]);
        }

        if (playlists[i] == NULL) break;
    }
}

void menuAdm() {
    char senha[TAM_SENHA] = "m@st3r2024";
    char senhaDigitada[TAM_SENHA];
    int code;

    printf("Digite a senha de ADM:\n");
    fgets(senhaDigitada, TAM_SENHA, stdin);

    size_t len = strlen(senhaDigitada);
    if (len > 0 && senhaDigitada[len - 1] == '\n') {
        senhaDigitada[len - 1] = '\0';
    }

    if (strcmp(senha, senhaDigitada) == 0) {
        int opcaoAdm;
        printf("Acesso liberado\n");

        do {
            printf("Escolha uma das funcionalidades abaixo:\n");
            printf("[0] Voltar ao menu principal\n");
            printf("[1] Listar Usuarios\n");
            printf("[2] Consultar Usuarios\n");
            printf("[3] Alterar Senha de Usuario\n");
            printf("[4] Cadastrar Musicas\n");
            printf("[5] Listar Musicas\n");
            printf("[6] Consultar Musica\n");
            printf("[7] Alterar Dados de Musica\n");
            printf("[8] Excluir Musica\n");
            printf("[9] Listar Playlists\n");
            printf("[10] Consultar Playlist\n\n");

            printf("> ");

            scanf("%d", &opcaoAdm);
            getchar();

            switch (opcaoAdm) {
                case 1:
                    mostrar_usuarios();
                    break;
                case 2:
                    printf("Digite o codigo do usuario que voce deseja buscar: ");
                    scanf("%d", &code);

                    if (buscar_usuario(code) != NULL) {
                        usuario_to_string(buscar_usuario(code));
                    }

                    break;
                case 3:
                    printf("Digite o codigo do usuario que voce deseja alterar senha: ");
                    scanf("%d", &code);

                    while(getchar() != '\n');

                    char nova_s1[TAM_SENHA];

                    printf("Digite a nova senha: ");
                    fgets(nova_s1, TAM_SENHA, stdin);

                    char nova_s2[TAM_SENHA];

                    while (strcmp(nova_s1, nova_s2)) {
                        printf("Confirme a nova senha: ");
                        fgets(nova_s2, TAM_SENHA, stdin);
                    }

                    alterar_senha(code, nova_s1);
                    break;
                case 4:
                    char title[TAM_TITULO];
                    printf("Digite o titulo da musica: ");
                    if (fgets(title, TAM_TITULO, stdin) != NULL) {
                        // Remover o caractere de nova linha se presente
                        size_t s = strlen(title);
                        if (s > 0 && title[s - 1] == '\n') {
                            title[s - 1] = '\0';
                        }
                    }

                    char artist[TAM_NOME];
                    printf("Digite o nome do artista: ");
                    if (fgets(artist, TAM_NOME, stdin) != NULL) {
                        // Remover o caractere de nova linha se presente
                        size_t s = strlen(artist);
                        if (s > 0 && artist[s - 1] == '\n') {
                            artist[s - 1] = '\0';
                        }
                    }

                    Musica* m = criar_musica(title, artist);
                    break;
                case 5:
                    mostrar_musicas();
                    break;
                case 6:
                    printf("Digite o codigo da musica que voce deseja buscar: ");
                    scanf("%d", &code);

                    if (buscar_musica(code) != NULL) {
                        musica_to_string(buscar_musica(code));
                    }

                    break;
                case 7:
                    printf("Digite o codigo da musica: ");
                    scanf("%d", &code);

                    if (buscar_musica(code) != NULL) {
                        int op;

                        printf("[0] Sair\n");
                        printf("[1] Alterar titulo\n");
                        printf("[2] Alterar artista\n");
                        scanf("%d", &op);
                        getchar();

                        switch (op) {
                            case 0:
                                break;
                            case 1:
                                char n_title[TAM_TITULO];
                                printf("Digite o novo titulo: ");
                                fgets(n_title, TAM_TITULO, stdin);

                                alterar_titulo(code, n_title);
                                break;
                            case 2:
                                char n_artist[TAM_TITULO];
                                printf("Digite o novo artista: ");
                                fgets(n_artist, TAM_TITULO, stdin);

                                alterar_artista(code, n_artist);
                                break;
                            default:
                                printf("Opcao invalida");
                                break;
                        }
                    }
                    break;
                case 8:
                    printf("Digite o codigo da musica que voce deseja excluir: ");
                    scanf("%d", &code);

                    if (buscar_musica(code) != NULL) {
                        excluir_musica(code);
                    }
                    break;
                case 9:
                    mostrar_playlists();
                    break;
                case 10:
                    int op;

                    printf("[0] Sair\n");
                    printf("[1] Buscar por codigo da playlist\n");
                    printf("[2] Buscar por codigo de musica\n");
                    scanf("%d", &op);

                    switch(op) {
                        case 0:
                            break;
                        case 1:
                            printf("Digite o codigo da playlist que deseja buscar: ");
                            scanf("%d", &code);

                            if (buscar_playlist(code) != NULL) {
                                playlist_to_string(buscar_playlist(code));
                            }
                            break;
                        case 2:
                            printf("Digite o codigo da musica que deseja buscar: ");
                            scanf("%d", &code);

                            if (buscar_musica(code) != NULL) {
                                consultar_playlist_por_musica(code);
                            }
                            break;
                        default:
                            printf("Opcao invalida");
                            break;
                    }
                    break;
                case 0:
                    printf("Voltando ao menu principal...\n");
                    break;
                default:
                    printf("Opção invalida\n");
            }
        } while (opcaoAdm != 0); //alterar aqui
    } else {
        printf("Senha incorreta\n");
    }
}

void menuUser(){
    int op;

    printf("Bem vindo ao menu usuario\n");
    printf("[0] Menu principal\n[1] Login\n[2] Cadastrar-se\n");

    scanf("%d",&op);

    switch(op){

        case 0:
            printf("Voltando ao menu principal");
            break;

        case 1:
            Usuario* user = menuLoginUser();

            if (user != NULL) {

                int op;
                int sair = 0;
                int code;

                char senha[TAM_SENHA];

                while (!sair) {
                    printf("[0] Sair\n");
                    printf("[1] Listar seus dados\n"); //execao senha
                    printf("[2] Alterar nome de usuario\n"); // confirmacao da senhha
                    printf("[3] Alterar login de usuario\n"); // confirmacao da senha
                    printf("[4] Alterar senha do usuario\n"); // solicitar nova senha e confirmação) (mediante confirmação da senha atual
                    printf("[5] Listar os usuarios\n"); //exibir só o nome
                    printf("[6] Consultar usuario por nome\n");
                    printf("[7] Listar todas as musicas\n");
                    printf("[8] Consultar música por codigo\n");
                    printf("[9] Listar todas suas playlists\n");
                    printf("[10] Listar todas as playlists\n");
                    printf("[11] Consultar playlist por codigo, parte do nome, codigo da musica\n");
                    printf("[12] Cadastrar playlist\n");
                    printf("[13] Excluir musica de uma playlist\n");
                    printf("[14] Excluir uma playlist\n");
                    printf("[15] Favoritar uma playlist\n\n");

                    printf("> ");
                    scanf("%d", &op);
                    getchar();

                    switch (op) {
                        case 0:
                            sair = 1;
                            break;
                        case 1:
                            usuario_to_string(user);
                            break;
                        case 2:
                            printf("Confirme sua senha para prosseguir: ");
                            scanf("%s", &senha);
                            getchar();

                            if (!strcmp(user->senha, senha)) {
                                // Se senha for digitada corretamente
                                char n_nome[TAM_NOME];
                                printf("Digite o novo nome: ");
                                fgets(n_nome, TAM_NOME, stdin);

                                size_t len = strlen(n_nome);
                                if (len > 0 && n_nome[len - 1] == '\n') {
                                    n_nome[len - 1] = '\0';
                                }

                                strcpy(user->nome, n_nome);
                            } else {
                                printf("Senha incorreta\n\n");
                            }
                            break;
                        case 3:
                            printf("Confirme sua senha para prosseguir: ");
                            scanf("%s", &senha);
                            getchar();

                            if (!strcmp(user->senha, senha)) {
                                // Se senha for digitada corretamente
                                char login[TAM_LOGIN];
                                printf("Digite o novo login: ");
                                fgets(login, TAM_LOGIN, stdin);

                                size_t len = strlen(login);
                                if (len > 0 && login[len - 1] == '\n') {
                                    login[len - 1] = '\0';
                                }

                                strcpy(user->login, login);
                            } else {
                                printf("Senha incorreta\n\n");
                            }
                            break;
                        case 4:
                            printf("Confirme sua senha para prosseguir: ");
                            scanf("%s", &senha);
                            getchar();

                            if (!strcmp(user->senha, senha)) {
                                // Se senha for digitada corretamente
                                char n_senha1[TAM_SENHA];
                                printf("Digite a nova senha: ");
                                fgets(n_senha1, TAM_SENHA, stdin);

                                char n_senha2[TAM_SENHA];
                                while (strcmp(n_senha1, n_senha2)) {
                                    printf("Confirme a nova senha: ");
                                    fgets(n_senha2, TAM_SENHA, stdin);

                                    if (strcmp(n_senha1, n_senha2)) {
                                        printf("As duas senhas nao estao iguais\n");
                                    }
                                }

                                size_t len = strlen(n_senha1);
                                if (len > 0 && n_senha1[len - 1] == '\n') {
                                    n_senha1[len - 1] = '\0';
                                }

                                strcpy(user->senha, n_senha1);

                            } else {
                                printf("Senha incorreta\n\n");
                            }
                            break;
                        case 5:
                            int i = 0;
                            while(usuarios[i] != NULL) {
                                printf("%d: %s\n", i+1, usuarios[i]->nome);
                                i++;
                            }
                            printf("\n");
                            break;
                        case 6:
                            char nome[TAM_NOME];
                            printf("Digite o nome do usuario que deseja consultar: ");
                            fgets(nome, TAM_NOME, stdin);

                            size_t len = strlen(nome);
                            if (len > 0 && nome[len - 1] == '\n') {
                                nome[len - 1] = '\0';
                            }

                            if (buscar_u_nome(nome) != NULL) {
                                usuario_to_string(buscar_u_nome(nome));
                            }
                            break;
                        case 7:
                            mostrar_musicas();
                            break;
                        case 8:
                            printf("Digite o codigo da musica que voce deseja buscar: ");
                            scanf("%d", &code);

                            if (buscar_musica(code) != NULL) {
                                musica_to_string(buscar_musica(code));
                            }

                            break;
                        case 9:
                            for (int j = 0; j < playlist_code-1; j++) {
                                if (user->codigo == playlists[j]->codUsuario) {
                                    playlist_to_string(playlists[j]);
                                }
                            }
                            break;
                        case 10:
                            mostrar_playlists();
                            break;
                        case 11:
                            int opt;

                            printf("[0] Sair\n");
                            printf("[1] Consultar playlist por codigo\n"); // execao senha
                            printf("[2] Consultar playlist por codigo da musica\n"); // confirmacao da senha

                            scanf("%d", &opt);
                            getchar();

                            switch (opt) {
                                case 1:
                                    printf("Digite o codigo da playlist: ");
                                    scanf("%d", &code);

                                    playlist_to_string(buscar_playlist(code));
                                    break;
                                case 2:
                                    printf("Digite o codigo da musica: ");
                                    scanf("%d", &code);

                                    consultar_playlist_por_musica(code);
                                    break;
                                case 0:
                                    break;
                                default:
                                    printf("Opcao invalida\n");
                                    break;
                            }
                            break;
                        case 12:
                            char nome_playlist[TAM_NOME];

                            printf("Digite o nome da playlist: ");
                            scanf("%s", &nome_playlist);

                            Playlist* p = criar_playlist(user, nome_playlist);

                            int op;

                            while(op != 0) {
                                printf("[0] Sair\n");
                                printf("[1] Adicionar musica na playlist\n");
                                scanf("%d", &op);
                                getchar();

                                switch(op) {
                                    case 1:
                                        printf("Digite o codigo da musica: ");
                                        scanf("%d", &code);

                                        if (buscar_musica(code) != NULL) {
                                            adicionar_musica(p->codigo, code);
                                        }
                                        break;
                                    case 0:
                                        break;
                                    default:
                                        printf("Opcao invalida");
                                        break;
                                }
                            }

                            op = 10;

                            break;
                        case 13:
                            printf("Digite o codigo da playlist: ");
                            scanf("%d", &code);

                            int m_code;
                            printf("Digite o codigo da musica: ");
                            scanf("%d", &m_code);

                            if (buscar_playlist(code) != NULL) {
                                if (buscar_musica(m_code) != NULL) {
                                    excluir_musica_p(code, m_code);
                                }
                            }

                            break;
                        case 14:
                            printf("Digite o codigo da playlist: ");
                            scanf("%d", &code);

                            if (buscar_playlist(code) != NULL) {
                                char ch;

                                printf("\nRealmente quer excluir a playlist: [s]/[n]\n\n");
                                playlist_to_string(buscar_playlist(code));
                                printf(">");
                                scanf("%s", &ch);
                                getchar();

                                if (ch == 'S' || ch == 's') {
                                    excluir_playlist(code);
                                } 
                            }
                            break;
                        default:
                            printf("Opcao invalida");
                            break;
                    }
                }
            }
            break;

        case 2:
            menuRegisUser();
            break;


        default:
            printf("Opção inválida");
            break;

    }

}

Usuario* menuLoginUser() {
    char login[TAM_LOGIN];
    char senha[TAM_SENHA];
    int encontrado = 0;
    Usuario* usuarioLogado = NULL;


    while(getchar() != '\n');
    printf("Digite seu login: ");
    scanf("%s", &login);

    size_t len = strlen(login);
    if (len > 0 && login[len - 1] == '\n') {
        login[len - 1] = '\0';
    }

    printf("Digite a sua senha: ");
    scanf("%s", &senha);

    len = strlen(senha);
    if (len > 0 && senha[len - 1] == '\n') {
        senha[len - 1] = '\0';
    }

    for (int i = 0; i < 100 && usuarios[i] != NULL; i++) {
        if (strcmp(usuarios[i]->login, login) == 0 && strcmp(usuarios[i]->senha, senha) == 0) {
            encontrado = 1;
            usuarioLogado = usuarios[i];
            break;
        }
    }

    if (encontrado) {
        printf("Login bem-sucedido!\n");
        return usuarioLogado;
    }

    printf("Usuário ou senha incorretos!\n");
    return NULL;
}

void menuRegisUser() {
    char nome[TAM_NOME];
    char login[TAM_LOGIN];
    char senha[TAM_SENHA];
    char senhaConfirm[TAM_SENHA];
    int loginExistente = 0;

    while(getchar()!= '\n');
    printf("Digite seu nome:\n");
    fgets(nome, TAM_NOME, stdin);
    // Remove newline character if present
    size_t len = strlen(nome);
    if (len > 0 && nome[len - 1] == '\n') {
        nome[len - 1] = '\0';
    }

    printf("Digite seu login:\n");
    fgets(login, TAM_LOGIN, stdin);
    // Remove newline character if present
    len = strlen(login);
    if (len > 0 && login[len - 1] == '\n') {
        login[len - 1] = '\0';
    }

    // Verificar se o login já existe
    for (int i = 0; i < 100 && usuarios[i] != NULL; i++) {
        if (strcmp(usuarios[i]->login, login) == 0) {
            loginExistente = 1;
            break;
        }
    }

    if (loginExistente) {
        printf("Login já existe, tente novamente.\n");
        return;
    }

    printf("Digite sua senha:\n");
    fgets(senha, TAM_SENHA, stdin);
    // Remove newline character if present
    len = strlen(senha);
    if (len > 0 && senha[len - 1] == '\n') {
        senha[len - 1] = '\0';
    }

    printf("Confirme sua senha:\n");
    fgets(senhaConfirm, TAM_SENHA, stdin);
    //
    len = strlen(senhaConfirm);
    if (len > 0 && senhaConfirm[len - 1] == '\n') {
        senhaConfirm[len - 1] = '\0';
    }

    if (strcmp(senha, senhaConfirm) != 0) {
        printf("As senhas não coincidem. Tente novamente.\n");
        return;
    }


    Usuario* novoUsuario = criar_usuario(nome, login, senha);
    printf("Usuário registrado com sucesso!\n");
    usuario_to_string(novoUsuario);
}

void biblioteca() {
    Usuario* u1 = criar_usuario("Vinicius Melo", "vini@gmail", "12345");
    Usuario* u2 = criar_usuario("Cauã Ferraz", "cau@gmail", "1011");
    Usuario* u3 = criar_usuario("Gustavo Leonel", "guga@gmail", "123");
    Usuario* u4 = criar_usuario("Ana Silva", "ana@gmail", "D4f@3Gz#2s");
    Usuario* u5 = criar_usuario("Bruno Santos", "bru@gmail", "Y1p$9Hj*5q");
    Usuario* u6 = criar_usuario("Diego Souza", "ds87@gmail", "878787");
    Usuario* u7 = criar_usuario("Carlos Oliveira", "carl@gmail", "1231");
    Usuario* u8 = criar_usuario("Daniela Souza", "dani@gmail", "1214");
    Usuario* u9 = criar_usuario("Flavio Caça Rato", "cr7@gmail", "13254");
    Usuario* u10 = criar_usuario("Magrão", "ma@gmail", "5452");

    Musica* m1 = criar_musica("Duas Paixões", "Calcinha Preta");
    Musica* m2 = criar_musica("Agora estou sofrendo", "Calcinha Preta");
    Musica* m3 = criar_musica("Cobertor", "Calcinha Preta");
    Musica* m4 = criar_musica("O vento levou", "Calcinha Preta");
    Musica* m5 = criar_musica("Ceu Azul", "Charlie Brown Jr");
    Musica* m6 = criar_musica("Meu lugar ao sol", "Charlie Brown Jr");
    Musica* m7 = criar_musica("So os loucos sabem", "Charlie Brown Jr");
    Musica* m8 = criar_musica("Always", "Bon Jovi");
    Musica* m9 = criar_musica("Runaway", "Bon Jovi");
    Musica* m10 = criar_musica("Blaze of glory", "Bon Jovi");
    Musica* m11 = criar_musica("November rain", "Guns n Roses");


    Playlist* p1 = criar_playlist(u1, "Forró");
    Playlist* p2 = criar_playlist(u2, "Pagode");
    Playlist* p3 = criar_playlist(u2, "Rock");
    Playlist* p4 = criar_playlist(u2, "Bossa");

    adicionar_musica(1, 1);
    adicionar_musica(2, 1);
    adicionar_musica(2, 3);
    adicionar_musica(2, 5);
    adicionar_musica(2, 11);
    adicionar_musica(4, 1);

}