#include <iostream>
#include <algorithm>
#include <vector>
#include <Python.h>

#include "DTable.h"
#include "Validation.h"
#include "Plot.h"


using namespace std;
using namespace filtrage;

int main(int argc, char *argv[])
{
    Validation validate("filtrage","DataReference","Parametres", "StationsMeteo","DataFiles/",NULL);
    validate.validateFileRec("out.txt", 0.3, 90, true);

   // Plot plt;

    //plt.windrose("DataFiles/dataWindeRose.csv", "Rose_du_vent", "rose");


    return 0;
}
