#include <iostream>
#include <math.h>

using namespace std;

double calcSlope(double v)
{
    double slope = 150 - (5 * v) - pow(v,2);
    return slope;
}

/**
Eulers and Heun's methods in C++
*/
int main()
{
    double euler [20];
    double original[20];
    double trapezoidal[20];
    double heun[20];
    
    double vInitial = 0;
    
    cout << "Value of the Euler's Method" << endl;
    cout << "______________________________" << endl;
    for (int i = 0; i <= 20; i++)
    {
        if (i == 0)
        {
              euler[i] = 0;
        }
        else
        {
            euler[i] = euler[i - 1] + .1 * calcSlope(i * .1);
        }
              
        cout << i << ":" << euler[i] << endl;
    }
    cout << "______________________________" << endl;
    cout << endl;
    
    cout << "Value of the Trapezoidal's Method" << endl;
    cout << "______________________________" << endl;
    for (int i = 0; i <= 20; i++)
    {
        if (i == 0)
        {
              trapezoidal[i] = 0;
        }
        else
        {
            trapezoidal[i] = trapezoidal[i - 1] + .05 * (calcSlope(i) + calcSlope(i - 1));
        }

        cout << i << ":" << trapezoidal[i] << endl;
    }
    cout << "______________________________" << endl;
    cout << endl;
    
    cout << "Value of the Heun's Method" << endl;
    cout << "______________________________" << endl;
    for (int i = 0; i <= 20; i++)
    {
        if (i == 0)
        {
              heun[i] = 0;
        }
        else
        {
            heun[i] = heun[i - 1] + .05 * (pow(calcSlope(i), 2) - pow(calcSlope(i + 1), 2));
        }

        cout << i << ":" << heun[i] << endl;
    }
    cout << "______________________________" << endl;
    cout << endl;
    
    
    system("pause");
    system(0);
}
