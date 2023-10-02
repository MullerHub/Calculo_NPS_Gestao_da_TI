#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CLIENTS 100

// Definir a estrutura Survey antes de usá-la
typedef struct Survey
{
    int value;
} Survey;

// Protótipos das funções
bool fechouNegocio(int *return_bussiness, int *total_interacoes, int *up_selling_count, int *cross_selling_count, int *churn_count, int *num_de_clientes_churn, int maxSurveys);
void perguntas_e_calculos(const char *question, int *count, int maxSurveys);
void calcularMetricas(int return_bussiness, int total_interacoes, int up_selling_count, int cross_selling_count, int churn_count, int numero_de_clientes);
void calcularTaxaRetornoChurn(int fecharam_negocio, int retornaram, int churned);
void perguntas_e_calculos_NPS(Survey surveys[], int maxSurveys);

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
    int up_selling_count = 0;
    int cross_selling_count = 0;
    int return_bussiness = 0;
    int churn_count = 0;
    int num_de_clientes_churn = 0;
    int maxSurveys;

    printf("Informe o numero maximo de respostas da pesquisa: ");
    scanf("%d", &maxSurveys);

    Survey *surveys = (Survey *)malloc(maxSurveys * sizeof(Survey));

    if (surveys == NULL)
    {
        printf("Erro ao alocar memória para as respostas da pesquisa NPS.\n");
        return 1;
    }

    // Perguntas sobre marketing, cross-selling, e taxa de retorno de clientes/churn
    fechouNegocio(&return_bussiness, &total_interacoes, &up_selling_count, &cross_selling_count, &churn_count, &num_de_clientes_churn, maxSurveys);

    // Perguntas e cálculos NPS
    printf("Responda a pesquisa NPS:\n");
    perguntas_e_calculos_NPS(surveys, maxSurveys);

    calcularMetricas(return_bussiness, total_interacoes, up_selling_count, cross_selling_count, churn_count, maxSurveys);

    // calculos finais sobre o NPS armazenado dos clientes / usuarios
    NpsResult result = calculateNps(surveys, maxSurveys);
    printf("Detratores: %d\n", result.detractors);
    printf("Promotores: %d\n", result.promoters);
    printf("Passivos/neutros: %d\n", result.passive);
    printf("Total de respostas: %d\n", result.allAnswers);
    printf("NPS: %.2lf\n", result.nps);

    // Libere a memória alocada
    free(surveys);

    return 0;
}

void perguntas_e_calculos_NPS(Survey surveys[], int maxSurveys)
{
    for (int i = 0; i < maxSurveys; i++)
    {
        int value;
        printf("Resposta %d, Qual nota voce da para nossa empresa? (de 1 / 10)", i + 1);
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
}

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
}

void calcularTaxaRetornoChurn(int fecharam_negocio, int retornaram, int churned)
{
    double taxa_retorno = (double)retornaram / fecharam_negocio * 100;
    double taxa_churn = (double)churned / fecharam_negocio * 100;

    printf("Dos que fecharam algum negocio, %.2lf%% retornaram a fazer negocio em 3 meses.\n", taxa_retorno);
    printf("Dos que fecharam algum negocio, %.2lf%% entraram em churn.\n", taxa_churn);
}

bool fechouNegocio(int *return_bussiness, int *total_interacoes, int *up_selling_count, int *cross_selling_count, int *churn_count, int *num_de_clientes_churn, int maxSurveys)
{
    char FechouNegocio;
    do
    {
        printf("Voce fechou negocio com a gente?? (S/n): \n");
        scanf(" %c", &FechouNegocio);

        if (FechouNegocio == 's' || FechouNegocio == 'S')
        {
            perguntas_e_calculos("Voce fez up-selling???", up_selling_count, maxSurveys);
            perguntas_e_calculos("Voce fez cross-selling???", cross_selling_count, maxSurveys);
            perguntas_e_calculos("Voce voltaria a fazer negocios com a gente??", return_bussiness, maxSurveys);

            printf("contagem do up-selling %i\n", *up_selling_count);
            printf("contagem do cross %i\n", *cross_selling_count);
            printf("contagem do return %i\n", *return_bussiness);

            (*total_interacoes)++;

            // Chamar a função para calcular a taxa de retorno e churn
            if (*return_bussiness > 0)
            {
                calcularTaxaRetornoChurn(*total_interacoes, *return_bussiness, *num_de_clientes_churn);
            }

            return true;
        }
        else if (FechouNegocio == 'N' || FechouNegocio == 'n')
        {
            (*return_bussiness)++;
            printf("Que triste que nao fechamos negocio, pode nos dar um feedback onde podemos melhorar?\n");
            char feedback[100];
            scanf(" %99s", feedback);
            (*num_de_clientes_churn)++;

            // Chamar a função para calcular a taxa de retorno e churn
            if (*return_bussiness > 0)
            {
                calcularTaxaRetornoChurn(*total_interacoes, *return_bussiness, *num_de_clientes_churn);
            }

            return false;
        }
        else
        {
            printf("Erro, digite um comando valido [S ou N]\n");
        }
    } while (FechouNegocio != 'N' && FechouNegocio != 'n' && FechouNegocio != 'S' && FechouNegocio != 's');
    return false;
}

void perguntas_e_calculos(const char *question, int *count, int maxSurveys)
{
    char resposta;
    printf("%s (S/N): ", question);
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's')
    {
        (*count)++;
    }
}

void calcularMetricas(int return_bussiness, int total_interacoes, int up_selling_count, int cross_selling_count, int churn_count, int numero_de_clientes)
{
    double porcentagem_manteve_interacao = (double)total_interacoes / numero_de_clientes * 100;
    double porcentagem_fecharam_up_selling = (double)up_selling_count / total_interacoes * 100;
    double porcentagem_fecharam_cross_selling = (double)cross_selling_count / total_interacoes * 100;
    double porcentagem_clientes_retornaram = (double)return_bussiness / total_interacoes * 100;
    double porcentagem_clientes_churned = (double)churn_count / total_interacoes * 100;

    printf("Porcentagem de pessoas que mantiveram interacao: %.2f%%\n", porcentagem_manteve_interacao);
    printf("Porcentagem de up-selling entre os que fecharam negocio: %.2f%%\n", porcentagem_fecharam_up_selling);
    printf("Porcentagem de cross-selling entre os que fecharam negocio: %.2f%%\n", porcentagem_fecharam_cross_selling);
    printf("Porcentagem de clientes que retornaram: %.2f%%\n", porcentagem_clientes_retornaram);
    printf("Porcentagem de clientes que entraram em churn: %.2f%%\n", porcentagem_clientes_churned);
}



/*

// Protótipos de funções


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <quantidade_de_clientes>\n", argv[0]);
        return 1;
    }

    char feedbacks[MAX_CLIENTS][100];
    int notas_cliente[MAX_CLIENTS];

    const int numero_de_clientes = atoi(argv[1]);

    while (total_interacoes < numero_de_clientes)
    {
        bool fechou = fechouNegocio(&return_bussiness, &total_interacoes, &up_selling_count, &cross_selling_count, &churn_count, feedbacks, &num_de_clientes_churn);
        if (!fechou)
        {
            printf("Que triste que nao fechamos negocio, pode nos dar um feedback onde podemos melhorar?\n");
            char feedback[100];
            scanf(" %99s", feedback);
            strcpy(feedbacks[num_de_clientes_churn], feedback);
            num_de_clientes_churn++;
        }
    }

}
 */