#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_CLIENTS 100

// Definir a estrutura Survey antes de usá-la
typedef struct Survey
{
    int value;
} Survey;

// Protótipos das funções
bool fechouNegocio(int *return_bussiness, int *total_interacoes, int *up_selling_count, int *cross_selling_count, int *churn_count, int *num_de_clientes_churn, int maxSurveys);
void perguntas_e_calculos(const char *question, int *count, int maxSurveys);
void calcularMetricas(int return_bussiness, int total_interacoes, int up_selling_count, int cross_selling_count, int churn_count, int maxSurveys);
void calcularTaxaRetornoChurn(int fecharam_negocio, int retornaram, int churned);
void perguntas_e_calculos_NPS(Survey surveys[], int maxSurveys);
int maxSurveys;
bool terminar_aplicacao = true;

typedef struct NpsResult
{
    int detractors;
    int promoters;
    int passive;
    int allAnswers;
    double nps;
} NpsResult;

NpsResult calculateNps(Survey surveys[], int size);

int main()
{
    int total_interacoes = 0;
    int up_selling_count = 0;      // contador de up-selling
    int cross_selling_count = 0;   // contador de cross-selling
    int return_bussiness = 0;      // voltariam a fazer negocios com a gente
    int churn_count = 0;           // desistiram de fazer negocios com a gente
    int num_de_clientes_churn = 0; // variavel para o calculo de porcentagem do churn

    printf("Informe o numero maximo de respostas da pesquisa: ");
    scanf("%d", &maxSurveys);

    Survey *surveys = (Survey *)malloc(maxSurveys * sizeof(Survey));

    if (surveys == NULL)
    {
        printf("Erro ao alocar memória para as respostas da pesquisa NPS.\n");
        return 1;
    }

    // Perguntas sobre marketing, cross-selling, e taxa de retorno de clientes/churn
    for (int i = 0; i < maxSurveys; i++)
    {
        fechouNegocio(&return_bussiness, &total_interacoes, &up_selling_count, &cross_selling_count, &churn_count, &num_de_clientes_churn, maxSurveys);
        printf("Respostas do cliente => %i. \n", i + 1);
    };

    // Perguntas e cálculos NPS
    printf("Responda a pesquisa NPS:\n");
    perguntas_e_calculos_NPS(surveys, maxSurveys);

    calcularMetricas(return_bussiness, total_interacoes, up_selling_count, cross_selling_count, churn_count, maxSurveys);

    // Chamar a função para calcular a taxa de retorno e churn
    calcularTaxaRetornoChurn(total_interacoes, return_bussiness, num_de_clientes_churn);

    // calculos finais sobre o NPS armazenado dos clientes / usuarios
    NpsResult result = calculateNps(surveys, maxSurveys);
    printf("Detratores: %d\n", result.detractors);
    printf("Promotores: %d\n", result.promoters);
    printf("Passivos/neutros: %d\n", result.passive);
    printf("Total de respostas: %d\n", result.allAnswers);
    printf("NPS: %.2lf\n", result.nps);

    // Libere a memória alocada
    printf("terminar a aplicacao: (sim)");
    scanf("%d", terminar_aplicacao);
    free(surveys);

    return 0;
};

void perguntas_e_calculos_NPS(Survey surveys[], int maxSurveys)
{
    for (int i = 0; i < maxSurveys; i++)
    {
        int value;
        printf("Cliente %d, de 1 ao 10, Qual nota voce da para nossa empresa?", i + 1);
        scanf("%d", &value);

        if (value == -1)
        {
            break;
        }

        if (i < maxSurveys)
        {
            surveys[i].value = value;
        }
        else
        {
            printf("Limite máximo de respostas atingido.\n");
            break;
        }
    }
};

NpsResult calculateNps(Survey surveys[], int size)
{
    NpsResult result;
    result.detractors = 0;
    result.promoters = 0;
    result.passive = 0;
    result.allAnswers = size;

    for (int i = 0; i < size; i++)
    {
        int value = surveys[i].value;
        if (value >= 0 && value <= 6)
        {
            result.detractors++;
        }
        else if (value >= 9 && value <= 10)
        {
            result.promoters++;
        }
        else if (value >= 7 && value <= 8)
        {
            result.passive++;
        }
    }

    result.nps = ((double)result.promoters - result.detractors) / result.allAnswers * 100;
    return result;
};

void calcularTaxaRetornoChurn(int fecharam_negocio, int retornaram, int churned)
{
    double taxa_retorno = (double)retornaram / fecharam_negocio * 100;
    double taxa_churn = 100.0 - taxa_retorno;

    printf("Dos que fecharam algum negocio, %.2lf%% retornaram a fazer negocio em 3 meses.\n", taxa_retorno);
    printf("Dos que fecharam algum negocio, %.2lf%% entraram em churn.\n", taxa_churn);
};

bool fechouNegocio(int *return_bussiness, int *total_interacoes, int *up_selling_count, int *cross_selling_count, int *churn_count, int *num_de_clientes_churn, int maxSurveys)
{
    char FechouNegocio;
    do
    {
        printf("Voce fechou negocio com a gente devido ao marketing?? ");
        scanf(" %c", &FechouNegocio);
        FechouNegocio = tolower(FechouNegocio);

        if (FechouNegocio == 's')
        {
            perguntas_e_calculos("Voce fez up-selling???", up_selling_count, maxSurveys);
            perguntas_e_calculos("Voce fez cross-selling???", cross_selling_count, maxSurveys);
            perguntas_e_calculos("Voce voltaria a fazer negocios com a gente??", return_bussiness, maxSurveys);

            printf("contagem do up-selling %i\n", *up_selling_count);
            printf("contagem do cross %i\n", *cross_selling_count);
            printf("contagem do return %i\n", *return_bussiness);

            (*total_interacoes)++;

            return true;
        }
        else if (FechouNegocio == 'N' || FechouNegocio == 'n') // REVISAR OS CALCULOS DO CHURN QUE NÃO ATUALIZAM AQUI, CHURN tem que aumentar e return de clientes diminuir a porcentagem
        {
            (*num_de_clientes_churn)++;

            // Chamar a função para calcular a taxa de retorno e churn
            calcularTaxaRetornoChurn(*total_interacoes, *return_bussiness, *churn_count); // calculos aqui estão errados, mas no final corrige (ficou complexo que não consegui corrigir esse bug a tempo, provavelmente erro com os ponteiros usados)

            return false;
        }
        else
        {
            printf("Erro, digite um comando valido [S ou N]\n");
        }
    } while (FechouNegocio != 'N' && FechouNegocio != 'n' && FechouNegocio != 'S' && FechouNegocio != 's');
    return false;
};

void perguntas_e_calculos(const char *question, int *count, int maxSurveys)
{
    char resposta;
    printf("%s (S/N): ", question);
    scanf(" %c", &resposta);

    resposta = tolower(resposta);

    if (resposta == 'S' || resposta == 's')
    {
        (*count)++;
    }
};

void calcularMetricas(int return_bussiness, int total_interacoes, int up_selling_count, int cross_selling_count, int churn_count, int maxSurveys)
{
    double porcentagem_manteve_interacao = (double)total_interacoes / maxSurveys * 100;
    double porcentagem_fecharam_up_selling = (double)up_selling_count / total_interacoes * 100;
    double porcentagem_fecharam_cross_selling = (double)cross_selling_count / total_interacoes * 100;
    double porcentagem_clientes_retornaram = (double)return_bussiness / total_interacoes * 100;
    double porcentagem_clientes_churned = (double)churn_count / total_interacoes * 100;

    printf("Porcentagem de pessoas que mantiveram interacao: %.2f%%\n", porcentagem_manteve_interacao);
    printf("Porcentagem de up-selling entre os que fecharam negocio: %.2f%%\n", porcentagem_fecharam_up_selling);
    printf("Porcentagem de cross-selling entre os que fecharam negocio: %.2f%%\n", porcentagem_fecharam_cross_selling);
};



//
//
//
//

/* Logica antiga, que calcula os clientes de marketing estático e com as porcentagens sobre minha regra de negocios


  // Passo 1: Site criado
  printf("Passo 1: Site criado e fluxo PDCA novo em andamento\n");

  // Passo 2: Solicitar número de clientes

  // Passo 3: Perguntar se o usuário tem conta
  char temConta;
  printf("Passo 3: Todo usuarios tem conta? (S para sim, N para nao): (em caso de nao, sera adicionado 30 por cento ao total de clientes da campanha) ");
  scanf(" %c", &temConta); // Usamos " %c" spara descartar espaços em branco

  // Passo 4: Verificar se o usuário tem conta e criar conta com sucesso
  if (temConta == 'N' || temConta == 'n')
  {
    // O usuário já tem conta, vá para o Passo 5 diretamente
    printf("Passo 5: O usuario fez o login\n");
  }

  // Passo 6: O usuário acessa o site e visualiza a home da página
  printf("Passo 6: O usuario acessou o site e visualizou a home da pagina\n");

  // Passo 7: O cliente selecionou algum produto
  int porcentagemSucessoProduto = 40;
  int sucessoProduto = (porcentagemSucessoProduto * numero_clientes) / 100;
  printf("Passo 7: Clientes que selecionaram algum produto foram %d clientes (40 por cento)\n", sucessoProduto);
  numero_clientes += sucessoProduto;

  // Passo 8: Simular o frete com uma taxa de sucesso
  int porcentagemSucessoFrete = 70;
  int sucessoFrete = (porcentagemSucessoFrete * sucessoProduto) / 100;
  printf("Passo 8: Clientes que simularam frete foram: %d (70 por cento)\n", sucessoFrete);

  // Passo 9: Usuários que conferiram o carrinho e foram para pagamentos
  int porcentagemPagamento = 20;
  int clientesPagamento = (porcentagemPagamento * sucessoFrete) / 100;
  printf("Passo 9: Usuarios que conferiram o carrinho e foram para pagamentos: %d (20 por cento)\n", clientesPagamento);

  // Passo 10: Recomendar produtos do mesmo nicho para complementar a venda
  printf("Passo 10: Recomendar produtos do mesmo nicho para complementar a venda (20 por cento adicionais)\n ");

  int porcentagemVendaCasada = 120;
  int clientesPagamentoCasado = (porcentagemVendaCasada * clientesPagamento) / 100;
  printf("Passo 11: Usuarios que foram para pagamentos e compraram algo adicional: %d (20 por cento de vendas adicionais aos usuarios normais)\n", clientesPagamentoCasado);

  // Passo 11: Concluído a venda para tantos clientes
  int porcentagemConclusaoVenda = 50;
  int clientesConclusaoVenda = (porcentagemConclusaoVenda * clientesPagamentoCasado) / 100;
  printf("Passo 12: Concluido a venda para tantos clientes: %d\n", clientesConclusaoVenda);

  */
