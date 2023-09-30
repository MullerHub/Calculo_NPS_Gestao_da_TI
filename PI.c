#include <stdio.h>
#include <auxiliar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// prototipos
bool fechouNegocio(void);
double calcularNPS(int notas[], int tamanho);

bool perguntas_e_calculos(const char *question, int *count);

char *printar_na_tela(const char *pergunta, const int *resposta);

int num_de_clientes_churn = 0;
int up_selling_count = 0;
int cross_selling_count = 0;
int return_bussiness = 0;
int churn_count = 0;

int notas_promotores = 0;
int notas_detratores = 0;
int notas_neutros = 0;
int notas_total = 0;

int main(int argc, string argv[])
{
    setlocale(LC_ALL, "");
    if (argc < 2)
    {
        printf("Especifique o numero de clientes.\n[Exemplo]: ./PI (quantidade de clientes)");
        return 1;
    }

    const int numero_de_clientes = atoi(argv[1]);
    fechouNegocio();

    return 0;
}

bool fechouNegocio(void)
{
    string FechouNegocio;

    do
    {
        FechouNegocio = get_string("Voce fechou negocio com a gente?? ");
        if (FechouNegocio[0] == 's' || FechouNegocio[0] == 'S')
        {
            printf("Voce fechou\n");
            perguntas_e_calculos("Voce fez up-seeling???", &up_selling_count);
            perguntas_e_calculos("Você fez cross-selling???", &cross_selling_count);
            perguntas_e_calculos("Você voltaria a fazer negocios com a gente??", &return_bussiness);
            perguntas_e_calculos("Você voltaria a fazer negocios com a gente??", &churn_count);

            printf("contagem do up_selling %i\n", up_selling_count);
            printf("contagem do cross %i\n", cross_selling_count);
            printf("contagem do return %i\n", return_bussiness);

            //
            //
            //
            //
            //
            return true;
        }
        else if (FechouNegocio[0] == 'n' || FechouNegocio[0] == 'N')
        {
            printf("Voce nao fechou\n");
            return false;
        }
        else
        {
            printf("erro, digite um comando válido [sim ou não]\n");
        }
    } while ((FechouNegocio[0] != 'N' || FechouNegocio[0] != 'n') ||
             (FechouNegocio[0] != 's' || FechouNegocio[0] != 'S'));
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
        result = true;
        *count += 1;
    }
    else
    {
        result = false;
    }
    return result;
};
