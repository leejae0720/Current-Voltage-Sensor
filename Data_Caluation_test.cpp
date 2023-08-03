#include <iostream>
#include <cmath>
using namespace std;

double data_set[500];
double data_sum = 0;
double Odc = 0;
double A1 = 0, A2 = 0, X = 0, theta = 0;
double sampling_time[500], angle[500];
double Irms = 0, Vrms = 0;

int main(){
    for(int i = 0; i < 500; i++){
        sampling_time[i] = 0.001*(i+1);
        angle[i] = 2*3.14*60*sampling_time[i];
        data_set[i] = 509;
        data_sum += data_set[i];
    }
    Odc = data_sum / 500;

    for(int i = 0; i < 500; i++){
        A1 = (2/500)*Odc*sin(2*3.14*60*sampling_time[i]);
        A2 = (2/500)*Odc*cos(2*3.14*60*sampling_time[i]);
    }
   
    X = sqrt(A1*A1+A2*A2);
    theta = atan2(A2, A1);

    Irms = X / sqrt(2);

    cout << X << endl;
    cout << theta << endl;
    cout << Irms << endl;
    cout << A1 << endl;
    cout << A2 << endl;
    cout << angle[132] << endl;
    cout << "hello" << endl;
  
    return 0;
}