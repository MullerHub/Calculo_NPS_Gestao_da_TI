#include <stdio.h>
#include <auxiliar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>

#define MAX_CLIENTS 100

// prototipos
bool fechouNegocio();
double calcularNPS(int notas[], int tamanho);

bool perguntas_e_calculos(const char *question, int *count);

void calcularMetricas(int return_bussiness, int total_interacoes, int up_selling_count, int cross_selling_count, int churn_count);

// Main limpa e refatorada!
int main(int argc, string argv[])
{
    setlocale(LC_ALL, "");
    int total_interacoes = 0;
    int up_selling_count = 0;
    int cross_selling_count = 0;

    int return_bussiness = 0;
    char feedbacks[MAX_CLIENTS][100];
    int num_de_clientes_churn = 0;
    int churn_count = 0;

    int notas_promotores = 0;
    int notas_detratores = 0;
    int notas_neutros = 0;
    int notas_total = 0;

    if (argc < 2)
    {
        printf("Especifique o numero de clientes.\n[Exemplo]: ./PI (quantidade de clientes)");
        return 1;
    }

    const int numero_de_clientes = atoi(argv[1]);

    //
    while (total_interacoes < numero_de_clientes)
    {
        bool fechou = fechouNegocio(&return_bussiness, &total_interacoes, &up_selling_count, &cross_selling_count, &churn_count);

        if (!fechou)
        {
            // Cliente não fechou negócio, coletar feedback
            printf("Que triste que não fechamos negócio, pode nos dar um feedback onde podemos melhorar?\n");
            char feedback[100];
            scanf(" %99s", feedback);
            strcpy(feedbacks[num_de_clientes_churn], feedback);
            num_de_clientes_churn++;
        }
    }
    calcularMetricas(return_bussiness, total_interacoes, up_selling_count, cross_selling_count, churn_count);
    return 0;
}

// Funções do codigo abaixo!!!

bool fechouNegocio(int *return_bussiness, int *total_interacoes, int *up_selling_count, int *cross_selling_count, int *churn_count)
{
    char FechouNegocio;
    do
    {
        printf("Você fechou negócio com a gente?? (S/n): ");
        scanf(" %c", &FechouNegocio);

        if (FechouNegocio == 's' || FechouNegocio == 'S')
        {
            perguntas_e_calculos("Voce fez up-seeling???", &up_selling_count);
            perguntas_e_calculos("Você fez cross-selling???", &cross_selling_count);
            perguntas_e_calculos("Você voltaria a fazer negocios com a gente??", &return_bussiness);
            perguntas_e_calculos("Você voltaria a fazer negocios com a gente??", &churn_count);

            printf("contagem do up_selling %i\n", up_selling_count);
            printf("contagem do cross %i\n", cross_selling_count);
            printf("contagem do return %i\n", return_bussiness);
            printf("contagem do churn %i\n", churn_count);

            (*total_interacoes)++;
            return true;
        }
        else if (FechouNegocio == 'N' || FechouNegocio == 'n')
        {
         (*return_bussiness)++;
            return false;
        }
        else
        {
            printf("erro, digite um comando válido [sim ou não]\n");
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
    double porcentagemNeutros = (double)neutros / tamanho * 100;

    return porcentagemPromotores - porcentagemDetratores;
}

bool perguntas_e_calculos(const char *question, int *count)
{
    bool result;
    char resposta;
    printf("%s\n", question);
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's')
    {
        (*count)++;
        return true;
    }
    else
    {
        result = false;
    }
};

void calcularMetricas(int return_bussiness, int total_interacoes, int up_selling_count, int cross_selling_count, int churn_count)
{
    // Calcule a porcentagem das pessoas que mantiveram interação após o primeiro contato
    double porcentagem_manteve_interacao = (double)total_interacoes / numero_de_clientes * 100;

    // Calcule a porcentagem de up-selling e cross-selling entre os que fecharam negócio
    double porcentagem_fecharam_up_selling = (double)up_selling_count / total_interacoes * 100;
    double porcentagem_fecharam_cross_selling = (double)cross_selling_count / total_interacoes * 100;

    // Calcule a porcentagem de retorno e churn entre os que fecharam negócio
    double porcentagem_clientes_retornaram = (double)return_bussiness / total_interacoes * 100;
    double porcentagem_clientes_churned = (double)churn_count / total_interacoes * 100;

    // Imprima as métricas
    printf("Porcentagem de pessoas que mantiveram interacao: %.2f%%\n", porcentagem_manteve_interacao);
    printf("Porcentagem de up-selling entre os que fecharam negócio: %.2f%%\n", porcentagem_fecharam_up_selling);
    printf("Porcentagem de cross-selling entre os que fecharam negócio: %.2f%%\n", porcentagem_fecharam_cross_selling);
    printf("Porcentagem de clientes que retornaram: %.2f%%\n", porcentagem_clientes_retornaram);
    printf("Porcentagem de clientes que entraram em churn: %.2f%%\n", porcentagem_clientes_churned);
}
