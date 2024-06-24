#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ASSENTOS 50
#define MAX_FILMES 15

// Definição da estrutura de um assento
typedef struct {
  int numero;
  char estado; // 'L' para livre, 'R' para reservado, 'O' para ocupado
} Assento;

// Definição da estrutura de um filme
typedef struct {
  char nome[50];
  int numero;
  char horario[10]; // Horário do filme como string (por exemplo, "15:00")
  double valor;     // Valor do ingresso
  Assento assentos[MAX_ASSENTOS]; // Array de assentos para o filme
} Filme;

// Definição da estrutura para armazenar os filmes em um array
typedef struct {
  Filme filmes[MAX_FILMES];
} ListaFilmes;

// Protótipo da Função adicionarSala
void adicionarSala(ListaFilmes *listaFilmes);

// Função auxiliar para verificar se uma string representa um horário no formato
// HH:MM
bool ehHorarioValido(const char *horario) {
  if (strlen(horario) != 5) {
    return false;
  }

  for (int i = 0; i < 5; ++i) {
    if (i == 2) {
      if (horario[i] != ':') {
        return false;
      }
    } else {
      if (!isdigit(horario[i])) {
        return false;
      }
    }
  }

  int hora = (horario[0] - '0') * 10 + (horario[1] - '0');
  int minuto = (horario[3] - '0') * 10 + (horario[4] - '0');

  if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
    return false;
  }

  return true;
}

// Função para adicionar um filme
void adicionarFilme(ListaFilmes *listaFilmes) {
  printf("Adicionar Filme\n");

  Filme novoFilme;

  printf("Digite o nome do filme: ");
  scanf("%s", novoFilme.nome);

  printf("Digite o número do filme: ");
  scanf("%d", &novoFilme.numero);

  printf("Digite o valor do ingresso para o filme: ");
  scanf("%lf", &novoFilme.valor);

  char horario[10];
  bool horarioValido = false;
  while (!horarioValido) {
    printf("Digite o horário do filme (formato HH:MM): ");
    scanf("%s", horario);

    if (!ehHorarioValido(horario)) {
      printf("Horário inválido. Digite novamente no formato HH:MM.\n");
    } else {
      horarioValido = true;
    }
  }
  strcpy(novoFilme.horario, horario);

  // Verifica se o número do novo filme é válido
  if (novoFilme.numero <= 0 || novoFilme.numero > MAX_FILMES) {
    printf("Opção inválida. O número do filme deve estar entre 1 e %d.\n",
           MAX_FILMES);
    return;
  }

  // Inicializa os assentos do filme como livres
  for (int i = 0; i < MAX_ASSENTOS; ++i) {
    novoFilme.assentos[i].numero = i + 1;
    novoFilme.assentos[i].estado = 'L'; // 'L' para livre inicialmente
  }

  // Adiciona o novo filme à lista de filmes na posição correspondente ao seu
  // número
  listaFilmes->filmes[novoFilme.numero - 1] = novoFilme;

  // Registra a adição do filme no arquivo listadefilmes.txt
  FILE *arquivoLista = fopen("listadefilmes.txt", "a");
  if (arquivoLista == NULL) {
    printf("Erro ao abrir o arquivo listadefilmes.txt para escrita.\n");
    return;
  }
  fprintf(arquivoLista, "O filme %s de número %d foi adicionado.\n",
          novoFilme.nome, novoFilme.numero);
  fclose(arquivoLista);
}

// Função para reservar um assento
void reservarAssento(Filme *filme, int numeroAssento) {
  if (numeroAssento < 1 || numeroAssento > MAX_ASSENTOS) {
    printf("Número de assento inválido.\n");
    return;
  }

  if (filme->assentos[numeroAssento - 1].estado == 'R') {
    printf("Assento já está marcado como reservado.\n");
  } else if (filme->assentos[numeroAssento - 1].estado == 'O') {
    printf("Assento já está ocupado.\n");
  } else {
    filme->assentos[numeroAssento - 1].estado = 'O'; // 'O' para ocupado
    printf("Assento %d reservado com sucesso para o filme %s.\n", numeroAssento,
           filme->nome);
  }
}

// Função para cancelar a reserva de um assento
void cancelarReservaAssento(Filme *filme, int numeroAssento) {
  if (numeroAssento < 1 || numeroAssento > MAX_ASSENTOS) {
    printf("Número de assento inválido.\n");
    return;
  }

  if (filme->assentos[numeroAssento - 1].estado == 'L') {
    printf("Assento não está reservado.\n");
  } else if (filme->assentos[numeroAssento - 1].estado == 'R') {
    printf("Assento está marcado como reservado e não pode ser cancelado.\n");
  } else {
    filme->assentos[numeroAssento - 1].estado = 'L'; // 'L' para livre
    printf("Reserva do assento %d para o filme %s foi cancelada.\n",
           numeroAssento, filme->nome);
  }
}

// Função para o modo administrador
void modoAdministrador(ListaFilmes *listaFilmes) {
  char senha[20];
  printf("Digite a senha: ");
  scanf("%s", senha);

  // Verifica se a senha está correta
  if (strcmp(senha, "adm123") != 0) {
    printf("Senha incorreta. Acesso negado.\n");
    return;
  }

  printf("Modo Administrador\n");
  bool executando = true;
  while (executando) {
    printf("1 - Adicionar filme\n");
    printf("2 - Adicionar sala (baseado em um filme existente)\n");
    printf("3 - Sair do modo administrador\n");

    int opcao;
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      adicionarFilme(listaFilmes);
      break;
    case 2:
      adicionarSala(listaFilmes);
      break;
    case 3:
      printf("Saindo do modo administrador.\n");
      executando = false;
      break;
    default:
      printf("Opção inválida.\n");
      break;
    }
  }
}

// Função para o modo usuário
void modoUsuario(ListaFilmes *listaFilmes) {
  printf("Modo Usuário\n");

  while (true) {
    printf("Escolha uma opção:\n");
    printf("1 - Visualizar filmes disponíveis\n");
    printf("2 - Visualizar assentos disponíveis de um filme\n");
    printf("3 - Reservar assento\n");
    printf("4 - Cancelar reserva de assento\n");
    printf("5 - Finalizar compra\n");
    printf("6 - Voltar ao menu principal\n");

    int opcao;
    printf("Opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Filmes disponíveis:\n");
      for (int i = 0; i < MAX_FILMES; ++i) {
        if (strlen(listaFilmes->filmes[i].nome) > 0) {
          printf("%d - %s, (%s)\n", listaFilmes->filmes[i].numero,
                 listaFilmes->filmes[i].nome, listaFilmes->filmes[i].horario);
        }
      }
      break;
    case 2: {
      int numFilme;
      printf("Digite o número do filme: ");
      scanf("%d", &numFilme);

      if (numFilme < 1 || numFilme > MAX_FILMES ||
          strlen(listaFilmes->filmes[numFilme - 1].nome) == 0) {
        printf("Filme não encontrado.\n");
        break;
      }

      printf("Assentos disponíveis para o filme %s:\n",
             listaFilmes->filmes[numFilme - 1].nome);
      for (int i = 0; i < MAX_ASSENTOS; ++i) {
        printf("%c ", listaFilmes->filmes[numFilme - 1].assentos[i].estado);
        if ((i + 1) % 10 == 0) {
          printf("\n");
        }
      }
      printf("\n");
      break;
    }
    case 3: {
      int numFilme, numAssento;
      printf("Digite o número do filme: ");
      scanf("%d", &numFilme);

      if (numFilme < 1 || numFilme > MAX_FILMES ||
          strlen(listaFilmes->filmes[numFilme - 1].nome) == 0) {
        printf("Filme não encontrado.\n");
        break;
      }

      printf("Assentos disponíveis para o filme %s:\n",
             listaFilmes->filmes[numFilme - 1].nome);
      for (int i = 0; i < MAX_ASSENTOS; ++i) {
        printf("%c ", listaFilmes->filmes[numFilme - 1].assentos[i].estado);
        if ((i + 1) % 10 == 0) {
          printf("\n");
        }
      }
      printf("\nDigite o número do assento que deseja reservar: ");
      scanf("%d", &numAssento);

      reservarAssento(&listaFilmes->filmes[numFilme - 1], numAssento);
      break;
    }
    case 4: {
      int numFilme, numAssento;
      printf("Digite o número do filme: ");
      scanf("%d", &numFilme);

      if (numFilme < 1 || numFilme > MAX_FILMES ||
          strlen(listaFilmes->filmes[numFilme - 1].nome) == 0) {
        printf("Filme não encontrado.\n");
        break;
      }

      printf("Assentos reservados para o filme %s:\n",
             listaFilmes->filmes[numFilme - 1].nome);
      for (int i = 0; i < MAX_ASSENTOS; ++i) {
        printf("%c ", listaFilmes->filmes[numFilme - 1].assentos[i].estado);
        if ((i + 1) % 10 == 0) {
          printf("\n");
        }
      }
      printf("\nDigite o número do assento que deseja cancelar a reserva: ");
      scanf("%d", &numAssento);

      cancelarReservaAssento(&listaFilmes->filmes[numFilme - 1], numAssento);
      break;
    }
    case 5: {
      printf("Finalizando compra:\n");

      // Solicitar tipo de pagamento
      printf("Escolha o tipo de pagamento:\n");
      printf("1 - Pix\n");
      printf("2 - Cartão de crédito/débito\n");
      printf("3 - Dinheiro\n");
      int tipoPagamento;

      do {
        printf("Opção de pagamento: ");
        scanf("%d", &tipoPagamento);

        switch (tipoPagamento) {
        case 1:
          printf("Pagamento realizado via Pix.\n");
          break;
        case 2:
          printf("Pagamento realizado via Cartão de crédito/débito.\n");
          break;
        case 3:
          printf("Pagamento realizado em Dinheiro.\n");
          break;
        default:
          printf("Opção inválida de pagamento.\n");
          break;
        }
      } while (tipoPagamento < 1 || tipoPagamento > 3);

      printf("Finalizando compra:\n");
      FILE *arquivo = fopen("comprasfinalizadas.txt", "a");
      if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo comprasfinalizadas.txt.\n");
        return;
      }

      bool reservaFeita = false;
      double totalPago = 0.0;
      for (int i = 0; i < MAX_FILMES; ++i) {
        for (int j = 0; j < MAX_ASSENTOS; ++j) {
          if (listaFilmes->filmes[i].assentos[j].estado == 'O') {
            listaFilmes->filmes[i].assentos[j].estado =
                'R'; // Marca como 'R' (reservado)
            printf("Assento %d do filme %s foi marcado como reservado.\n",
                   listaFilmes->filmes[i].assentos[j].numero,
                   listaFilmes->filmes[i].nome);
            fprintf(arquivo, "%s,%d,%d,reservado\n",
                    listaFilmes->filmes[i].nome, listaFilmes->filmes[i].numero,
                    listaFilmes->filmes[i].assentos[j].numero);
            totalPago += listaFilmes->filmes[i].valor;
            reservaFeita = true;
          }
        }
      }
      fclose(arquivo);

      if (!reservaFeita) {
        printf("Nenhuma reserva encontrada.\n");
      } else {
        printf("Compra finalizada. Total pago: R$ %.2lf.\n", totalPago);
        return; // Encerra o programa após finalizar a compra
      }
      break;
    }
    case 6:
      printf("Voltando ao menu principal.\n");
      return;
    default:
      printf("Opção inválida.\n");
      break;
    }
  }
}

// Função para adicionar sala baseada em um filme existente
void adicionarSala(ListaFilmes *listaFilmes) {
  int numFilme;
  printf(
      "Digite o número do filme a ser copiado para adicionar uma nova sala: ");
  scanf("%d", &numFilme);

  if (numFilme < 1 || numFilme > MAX_FILMES ||
      strlen(listaFilmes->filmes[numFilme - 1].nome) == 0) {
    printf("Filme não encontrado.\n");
    return;
  }

  // Encontra o próximo número disponível para o novo filme
  int novoNumeroFilme = -1;
  for (int i = 0; i < MAX_FILMES; ++i) {
    if (strlen(listaFilmes->filmes[i].nome) == 0) {
      novoNumeroFilme = i + 1;
      break;
    }
  }
  if (novoNumeroFilme == -1) {
    printf("Não é possível adicionar uma nova sala, pois o limite de filmes "
           "foi alcançado.\n");
    return;
  }

  Filme novoFilme =
      listaFilmes->filmes[numFilme - 1]; // Copia os dados do filme existente
  novoFilme.numero = novoNumeroFilme;    // Define o novo número do filme

  // Incrementa o horário do filme em uma hora
  int hora = (novoFilme.horario[0] - '0') * 10 + (novoFilme.horario[1] - '0');
  int minuto = (novoFilme.horario[3] - '0') * 10 + (novoFilme.horario[4] - '0');

  hora++; // Incrementa a hora

  if (hora == 24) {
    hora = 0; // Volta para zero se passar de 23
  }

  // Formata o novo horário
  sprintf(novoFilme.horario, "%02d:%02d", hora, minuto);

  // Limpa os estados dos assentos do novo filme
  for (int i = 0; i < MAX_ASSENTOS; ++i) {
    novoFilme.assentos[i].estado =
        'L'; // Todos os assentos são inicialmente livres
  }

  // Adiciona o novo filme à lista de filmes na posição correspondente ao seu
  // número
  listaFilmes->filmes[novoNumeroFilme - 1] = novoFilme;

  // Registra a adição do filme no arquivo listadefilmes.txt
  FILE *arquivoLista = fopen("listadefilmes.txt", "a");
  if (arquivoLista == NULL) {
    printf("Erro ao abrir o arquivo listadefilmes.txt para escrita.\n");
    return;
  }
  fprintf(arquivoLista,
          "Sala adicionada para o filme %s. Novo número do filme: %d.\n",
          novoFilme.nome, novoFilme.numero);
  fclose(arquivoLista);

  printf("Sala adicionada com sucesso para o filme %s. Novo número do filme: "
         "%d. Novo horário: %s\n",
         novoFilme.nome, novoFilme.numero, novoFilme.horario);
}

int main() {
  // Inicializa a lista de filmes
  ListaFilmes listaFilmes;
  for (int i = 0; i < MAX_FILMES; ++i) {
    strcpy(listaFilmes.filmes[i].nome, "");
    listaFilmes.filmes[i].numero = 0;
    for (int j = 0; j < MAX_ASSENTOS; ++j) {
      listaFilmes.filmes[i].assentos[j].numero = j + 1;
      listaFilmes.filmes[i].assentos[j].estado =
          'L'; // Inicialmente todos os assentos estão livres
    }
  }

  char escolha;
  bool executando = true;

  while (executando) {
    printf("\nBem-vindo ao sistema de reserva de assentos para filmes!\n");
    printf("Escolha se você é um administrador (A) ou um usuário (U), ou saia "
           "(S): ");
    scanf(" %c", &escolha);

    switch (escolha) {
    case 'A':
    case 'a':
      modoAdministrador(&listaFilmes);
      break;
    case 'U':
    case 'u':
      modoUsuario(&listaFilmes);
      break;
    case 'S':
    case 's':
      printf("Saindo do programa.\n");
      executando = false;
      break;
    default:
      printf("Opção inválida. Por favor, selecione 'A' para administrador, 'U' "
             "para usuário ou 'S' para sair.\n");
      break;
    }
  }

  return 0;
}
