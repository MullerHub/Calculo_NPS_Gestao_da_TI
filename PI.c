#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CLIENTS 100

//
typedef struct Survey {
    int value;
} Survey;

typedef struct NpsResult {
    int detractors;
    int promoters;
    int passive;
    int allAnswers;
    double nps;
} NpsResult;

NpsResult calculateNps(Survey surveys[], int size) {
    NpsResult result;
    result.detractors = 0;
    result.promoters = 0;
    result.passive = 0;
    result.allAnswers = size;

    for (int i = 0; i < size; i++) {
        int value = surveys[i].value;
        if (value >= 0 && value <= 6) {
            result.detractors++;
        } else if (value >= 9 && value <= 10) {
            result.promoters++;
        } else if (value >= 7 && value <= 8) {
            result.passive++;
        }
    }

    result.nps = ((double)result.promoters - result.detractors) / result.allAnswers * 100;
    return result;
}

int main()
{

     int maxSurveys;
    printf("Informe o número máximo de respostas da pesquisa: ");
    scanf("%d", &maxSurveys);

    Survey *surveys = (Survey *)malloc(maxSurveys * sizeof(Survey));

    if (surveys == NULL) {
        printf("Erro ao alocar memória para as respostas da pesquisa.\n");
        return 1;
    }

    int size = 0;
    int value;

    printf("Informe as respostas da pesquisa (-1 para encerrar):\n");

    while (1) {
        printf("Resposta %d: ", size + 1);
        scanf("%d", &value);

        if (value == -1) {
            break;
        }

        if (size < maxSurveys) {
            surveys[size].value = value;
            size++;
        } else {
            printf("Limite máximo de respostas atingido.\n");
            break;
        }
    }

    NpsResult result = calculateNps(surveys, size);

    printf("Detractors: %d\n", result.detractors);
    printf("Promoters: %d\n", result.promoters);
    printf("Passive: %d\n", result.passive);
    printf("All Answers: %d\n", result.allAnswers);
    printf("NPS: %.2lf\n", result.nps);

    free(surveys);
}

/* 

// Protótipos de funções
bool fechouNegocio(int *return_bussiness, int *total_interacoes, int *up_selling_count, int *cross_selling_count, int *churn_count, char feedbacks[][100], int *num_de_clientes_churn);
double calcularNPS(int notas[], int tamanho);
void perguntas_e_calculos(const char *question, int *count);
void calcularMetricas(int return_bussiness, int total_interacoes, int up_selling_count, int cross_selling_count, int churn_count, int numero_de_clientes, int notas_cliente[]);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <quantidade_de_clientes>\n", argv[0]);
        return 1;
    }


    int total_interacoes = 0;
    int up_selling_count = 0;
    int cross_selling_count = 0;
    int return_bussiness = 0;
    char feedbacks[MAX_CLIENTS][100];
    int num_de_clientes_churn = 0;
    int churn_count = 0;
    int notas_cliente[MAX_CLIENTS];

    const int numero_de_clientes = atoi(argv[1]);

    while (total_interacoes < numero_de_clientes)
    {
        bool fechou = fechouNegocio(&return_bussiness, &total_interacoes, &up_selling_count, &cross_selling_count, &churn_count, feedbacks, &num_de_clientes_churn);
        if (!fechou)
        {
            printf("Que triste que não fechamos negócio, pode nos dar um feedback onde podemos melhorar?\n");
            char feedback[100];
            scanf(" %99s", feedback);
            strcpy(feedbacks[num_de_clientes_churn], feedback);
            num_de_clientes_churn++;
        }
    }

    calcularMetricas(return_bussiness, total_interacoes, up_selling_count, cross_selling_count, churn_count, numero_de_clientes, notas_cliente);
    return 0;
}

bool fechouNegocio(int *return_bussiness, int *total_interacoes, int *up_selling_count, int *cross_selling_count, int *churn_count, char feedbacks[][100], int *num_de_clientes_churn)
{
    char FechouNegocio;
    do
    {
        printf("Você fechou negócio com a gente?? (S/n): ");
        scanf(" %c", &FechouNegocio);

        if (FechouNegocio == 's' || FechouNegocio == 'S')
        {
            perguntas_e_calculos("Você fez up-selling???", up_selling_count);
            perguntas_e_calculos("Você fez cross-selling???", cross_selling_count);
            perguntas_e_calculos("Você voltaria a fazer negócios com a gente??", return_bussiness);

            printf("contagem do up-selling %i\n", *up_selling_count);
            printf("contagem do cross %i\n", *cross_selling_count);
            printf("contagem do return %i\n", *return_bussiness);

            (*total_interacoes)++;
            return true;
        }
        else if (FechouNegocio == 'N' || FechouNegocio == 'n')
        {
            (*return_bussiness)++;
            printf("Que triste que não fechamos negócio, pode nos dar um feedback onde podemos melhorar?\n");
            char feedback[100];
            scanf(" %99s", feedback);
            strcpy(feedbacks[*num_de_clientes_churn], feedback);
            (*num_de_clientes_churn)++;
            return false;
        }
        else
        {
            printf("Erro, digite um comando válido [S ou N]\n");
        }
    } while (FechouNegocio != 'N' && FechouNegocio != 'n' && FechouNegocio != 'S' && FechouNegocio != 's');

    return false;
}

double calcularNPS(int notas[], int tamanho)
{
    int promotores = 0;
    int detratores = 0;
    int neutros = 0;

    for (int i = 0; i < tamanho; i++)
    {
        int nota = notas[i];
        printf("Cliente %d: Qual nota você daria para nossa empresa (0 a 10)? ", i + 1);
        if (nota >= 9 && nota <= 10)
        {
            promotores++;
        }
        else if (nota >= 0 && nota <= 6)
        {
            detratores++;
        }
        else
        {
            neutros++;
        }
    }

    double porcentagemPromotores = (double)promotores / tamanho * 100;
    double porcentagemDetratores = (double)detratores / tamanho * 100;

    return porcentagemPromotores - porcentagemDetratores;
}

void perguntas_e_calculos(const char *question, int *count)
{
    char resposta;
    printf("%s (S/N): ", question);
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's')
    {
        (*count)++;
    }
}

void calcularMetricas(int return_bussiness, int total_interacoes, int up_selling_count, int cross_selling_count, int churn_count, int numero_de_clientes, int notas_cliente[])
{
    double porcentagem_manteve_interacao = (double)total_interacoes / numero_de_clientes * 100;
    double porcentagem_fecharam_up_selling = (double)up_selling_count / total_interacoes * 100;
    double porcentagem_fecharam_cross_selling = (double)cross_selling_count / total_interacoes * 100;
    double porcentagem_clientes_retornaram = (double)return_bussiness / total_interacoes * 100;
    double porcentagem_clientes_churned = (double)churn_count / total_interacoes * 100;

    printf("Porcentagem de pessoas que mantiveram interação: %.2f%%\n", porcentagem_manteve_interacao);
    printf("Porcentagem de up-selling entre os que fecharam negócio: %.2f%%\n", porcentagem_fecharam_up_selling);
    printf("Porcentagem de cross-selling entre os que fecharam negócio: %.2f%%\n", porcentagem_fecharam_cross_selling);
    printf("Porcentagem de clientes que retornaram: %.2f%%\n", porcentagem_clientes_retornaram);
    printf("Porcentagem de clientes que entraram em churn: %.2f%%\n", porcentagem_clientes_churned);

    double nps = calcularNPS(notas_cliente, total_interacoes);
    printf("Net Promoter Score (NPS): %.2f\n", nps);
}





 */