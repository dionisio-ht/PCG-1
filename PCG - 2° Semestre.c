#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

//para saber se é ano bisesto
int bi(int year) {
    return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0));
}

// sber o numero de dias desde o ano 0
int DT_day(int day, int mon, int year) {
    int td = 0;
    int dpm[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    for (int i = 0; i < year; i++)
        td += 365 + bi(i);
    for (int i = 0; i < mon - 1; i++)
        td += dpm[i];
    if (mon > 2 && bi(year))
        td += 1;
    td += day;

    return td;
}

double calc_custo(int *Ordem, int N, int *Dificuldade, int *Prioridade, int *te_hour,
                  int *tl_mday, int *tl_mon, int *tl_year,
                  int day_atual, int mon_atual, int year_atual,
                  double peso_dif, double peso_pri, double peso_te, double peso_prazo) {

    double custo = 0;
    int prazo_atual = DT_day(day_atual, mon_atual, year_atual);
    int tempo_acumulado = 0;

    for (int i = 0; i < N; i++) {
        int idx = Ordem[i];
        int prazo_tarefa = DT_day(tl_mday[idx], tl_mon[idx], tl_year[idx]);
        int dias_restantes = prazo_tarefa - prazo_atual;
        double atraso = (tempo_acumulado / 24.0) - dias_restantes;

        if (atraso > 0)
            custo += atraso * 10.0;

        custo += peso_dif * Dificuldade[idx];
        custo -= peso_pri * Prioridade[idx];
        custo += peso_te * te_hour[idx];
        custo += peso_prazo * dias_restantes;

        tempo_acumulado += te_hour[idx];
    }

    return custo;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    srand(time(NULL));
    int N;

    printf("Informe o número de tarefas: ");
    scanf("%d", &N);

    int day_atual, mon_atual, year_atual;
    printf("Informe a data atual (dd:mm:yyyy): ");
    scanf("%d:%d:%d", &day_atual, &mon_atual, &year_atual);

    int *Ordem = malloc(N * sizeof(int));
    int *Dificuldade = malloc(N * sizeof(int));
    int *Prioridade = malloc(N * sizeof(int));
    int *te_hour = malloc(N * sizeof(int));
    int *tl_mday = malloc(N * sizeof(int));
    int *tl_mon = malloc(N * sizeof(int));
    int *tl_year = malloc(N * sizeof(int));

    for (int i = 0; i < N; i++) {
        Ordem[i] = i;
        printf("\nTarefa %d\n", i+1);
        printf("Dificuldade (1-10): ");
        scanf("%d", &Dificuldade[i]);
        printf("Prioridade (1-10): ");
        scanf("%d", &Prioridade[i]);
        printf("Tempo de execução (horas): ");
        scanf("%d", &te_hour[i]);
        printf("Prazo (dd:mm:yyyy): ");
        scanf("%d:%d:%d", &tl_mday[i], &tl_mon[i], &tl_year[i]);
    }

    // Pesos dos fatores
    double peso_dif = 1.0;
    double peso_pri = 2.0;
    double peso_te = 1.0;
    double peso_prazo = -1.0;

    // Têmpera Simulada
    double temperatura = 1000.0;
    double taxa_resfriamento = 0.95;
    int iteracoes_por_temp = 1000;

    double custo_atual = calc_custo(Ordem, N, Dificuldade, Prioridade, te_hour, tl_mday, tl_mon, tl_year,
                                    day_atual, mon_atual, year_atual,
                                    peso_dif, peso_pri, peso_te, peso_prazo);

    while (temperatura > 1e-3) {
        for (int iter = 0; iter < iteracoes_por_temp; iter++) {
            int a = rand() % N;
            int b = rand() % N;
            if (a == b) continue;

            swap(&Ordem[a], &Ordem[b]);

            double novo_custo = calc_custo(Ordem, N, Dificuldade, Prioridade, te_hour, tl_mday, tl_mon, tl_year,
                                           day_atual, mon_atual, year_atual,
                                           peso_dif, peso_pri, peso_te, peso_prazo);

            double delta = novo_custo - custo_atual;
            if (delta < 0 || ((double)rand() / RAND_MAX) < exp(-delta / temperatura)) {
                custo_atual = novo_custo;
            } else {
                swap(&Ordem[a], &Ordem[b]);
            }
        }
        temperatura *= taxa_resfriamento;
    }

    printf("\nMelhor ordem encontrada:\n");
    for (int i = 0; i < N; i++) {
        printf("Tarefa %d - Prazo: %02d/%02d/%04d\n", Ordem[i] + 1, tl_mday[Ordem[i]], tl_mon[Ordem[i]], tl_year[Ordem[i]]);
    }
    //printf("Custo total: %.2f\n", custo_atual);

    free(Ordem);
    free(Dificuldade);
    free(Prioridade);
    free(te_hour);
    free(tl_mday);
    free(tl_mon);
    free(tl_year);

    return 0;
}
