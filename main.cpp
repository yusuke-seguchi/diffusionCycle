#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#define NDX 100
#define NDY 1

int ndmx = NDX - 1;
int ndmy = NDY - 1;

int step = 0;
int nstep = 20000001;
int pstep = 200000;
int stepSwitch = 3333;

double dx = 1.;
double dtime = 0.1;
double Da = 10.;
double Qa = 1.0e+4;
double rr = 8.14315;

double humidHigh = 100.;
double humidLow = 10.;
double tempHigh = 400.;
double tempLow = 300.;

int environment = 1; // High: odd , Low: even
double temp;
double humid;

int i, j, ip, im, jp, jm;
double a[NDX][NDY], a2[NDX][NDY];
double dadt, dbdt;
double fae, faw, fan, fas;

void init_cond(){
    temp = tempHigh;
    for (i = 0; i <= ndmx; i++){
        for (j = 0; j <= ndmy; j++){
            // if (i <= NDX){
            if (i <= 10){
                a[i][j] = humidHigh;
            }
            else{
                a[i][j] = 10.;
            }
        }
    }
}

void datasave(int step){
    
    FILE *stream; //ストリームのポインタ設定
    char buffer[30];
    sprintf(buffer, "data1/2d%d.csv", step);
    stream = fopen(buffer, "w"); //書き込む先のファイルを追記方式でオープン

    for (int i = 0; i <= ndmx; i++){
        for (j=0; j<=ndmy; j++){
            fprintf(stream, "%e   ", a[i][j]);
            fprintf(stream, "\n");
        }
    }
    fclose(stream); //ファイルをクローズ
}

void set_environment(int environment){
    if (environment % 2 == 0){
        temp = tempLow;
        humid = humidLow;
    }
    else{
        temp = tempHigh;
        humid = humidHigh;
    }
}

int main(void)
{
    init_cond();

start:;
    if ((((int)(step) % pstep) == 0))
    {
        datasave(step);
        cout << step << " steps(" << step * dtime << " seconds) has done!" << endl;
    }

    set_environment(environment);

    for (i=0; i<=ndmx; i++){
        for (j=0; j<=ndmy; j++){
            ip = i + 1;
            im = i - 1;
            jp = j + 1;
            jm = j - 1;
            if (i == ndmx){ip = ndmx;}
            if (i == 0){im = 0;}
            if (j == ndmy){jp = 0;}
            if (j == 0){jm = ndmy;}
            a[0][j] = humid;
            a[1][j] = humid;
            a[2][j] = humid;
            a[3][j] = humid;
            a[4][j] = humid;
            a[5][j] = humid;
            a[6][j] = humid;
            a[7][j] = humid;
            a[8][j] = humid;
            a[9][j] = humid;
            a[10][j] = humid;
            
            fae = Da * exp(-Qa / rr / temp) * (a[ip][j] - a[i][j])/dx;
            faw = Da * exp(-Qa / rr / temp) * (a[i][j] - a[im][j])/dx;
            fan = Da * exp(-Qa / rr / temp) * (a[i][jp] - a[i][j])/dx;
            fas = Da * exp(-Qa / rr / temp) * (a[i][j] - a[i][jm])/dx;
            dadt = (fae - faw + fan - fas) / dx;
    
            a2[i][j] = a[i][j] + dadt * dtime;
        }
    }

    for (i = 0; i <= ndmx; i++){
        for (j = 0; j <= ndmy; j++){
            a[i][j] = a2[i][j];
        }
    }

    step = step + 1;

    // 環境の入れ替え（HighとLow）
    if (step % stepSwitch == 0)
    {
        environment += 1;
    }

    if (step < nstep)
    {
        goto start;
    }

end:;
    return 0;
}