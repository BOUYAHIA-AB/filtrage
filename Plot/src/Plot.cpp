
#include "Plot.h"

Plot::Plot()
{
    //ctor
}


/*
void Plot::plotCurve(const vector<double>& x , const vector<double>& y ,  const vector<double>& x_abr, const vector<double>& y_abr, const vector<double>& x_NA,
                        const vector<double>& y_NA, const char* out, const char* xlab, const char* ylab, const char* title) const
{
    if(x.size() != y.size() ) throw invalid_argument ("x et y ont des dimensions différentes : plotCurve( x , y )");
    if(x_abr.size() != y_abr.size() ) throw invalid_argument ("x_abr et y_abr ont des dimensions différentes : plotCurve( x , y )");
    if(x_NA.size() != y_NA.size() ) throw invalid_argument ("x_NA et y_NA ont des dimensions différentes : plotCurve( x , y )");

    size_t XSIZE = x.size();
    size_t X_ABR_SIZE = x_abr.size();
    size_t X_NA_SIZE = x_NA.size();

    plstream* pls;

    auto MIN = min_element(std::begin(y), std::end(y));
    auto MAX = max_element(std::begin(y), std::end(y));

    PLFLT xx[XSIZE], yy[XSIZE];
    PLFLT xmin = 0 , xmax = 23, ymin = *MIN, ymax = *MAX;

    for (unsigned int i = 0; i < XSIZE ; i++ )
    {
        xx[i] = (PLFLT) x[i];
        yy[i] = (PLFLT) y[i];
    }

    //preparer les données abberantes
    PLFLT xabr[X_ABR_SIZE], yabr[X_ABR_SIZE];
    for (unsigned int i = 0; i < X_ABR_SIZE ; i++ )
    {
        xabr[i] = (PLFLT) x_abr[i];
        yabr[i] = (PLFLT) y_abr[i];
    }

    //preparer les données manquantes
    PLFLT xNA[X_NA_SIZE], yNA[X_NA_SIZE];
    for (unsigned int i = 0; i < X_NA_SIZE ; i++ )
    {
        xNA[i] = (PLFLT) x_NA[i];
        yNA[i] = (PLFLT) y_NA[i];
    }

    pls = new plstream();

    pls->sdev("pngcairo");
    plscolbg(255,255,255);

    pls->sfnam(out);
    // Initialize plplot
    pls->init();
    // Create a labelled box to hold the plot.
    pls->env( xmin-1, xmax+1, ymin-1, ymax+1, 0, 0 );

    pls->lab( xlab, ylab, title );

    // Plot the data that was prepared above.
    pls->line( XSIZE , xx, yy );
    pls->col0( 3 );
    pls->string(XSIZE , xx, yy, "#(728)");

    // tracé des données abberrantes
    pls->col0( 2 );
    pls->string(X_ABR_SIZE , xabr, yabr, "#(728)");

    // tracé des données manquantes
    pls->col0( 1 );
    pls->string(X_NA_SIZE , xNA, yNA, "#(728)");

    delete pls;
}

void Plot::plotCurve(const vector<double>& y, const char* out , const char* xlab, const char* ylab, const char* title) const
{
    size_t NSIZE = y.size();
    plstream* pls;
    auto MIN = min_element(std::begin(y), std::end(y));
    auto MAX = max_element(std::begin(y), std::end(y));

    PLFLT xx[NSIZE], yy[NSIZE];
    PLFLT xmin = 0 , xmax = NSIZE, ymin = *MIN, ymax = *MAX;

    for (unsigned int i = 0; i < NSIZE; i++ )
    {
        xx[i] = (PLFLT) i;
        yy[i] = (PLFLT) y[i];
    }

    pls = new plstream();

    pls->sdev("pngcairo");

    pls->sfnam(out);
    // Initialize plplot
    pls->init();

    // Create a labelled box to hold the plot.
    pls->env( xmin, xmax, ymin-1, ymax+1, 0, 0 );

    pls->lab( xlab, ylab, title );

    // Plot the data that was prepared above.
    pls->line( NSIZE, xx, yy );

    delete pls;
}

void Plot::plotCurve(DTable &table, DTable &masque, string& idDate)
{
    real type = 0.;
    char cval = ' ';
    for(int i = 1; i < table.getNCols(); i++)
    {
        vector<double> y;
        vector<double> x;
        vector<double> x_abr;
        vector<double> x_NA;
        vector<double> y_abr;
        vector<double> y_NA;

        table = inputing(table);
        for(int j = 0; j < table.getNRows(); j++)
        {
            if(masque(i, j, cval) != 'N')
            {
                x.push_back(table(0,j, type));
                y.push_back(table(i,j, type));

                if(masque(i, j, cval) == 'F')
                {
                    x_abr.push_back(table(0,j, type));
                    y_abr.push_back(table(i,j, type));
                }
            }
            else if(masque(i, j, cval) == 'N')
            {
                x.push_back(table(0,j, type));
                y.push_back(table(i,j, type));
                x_NA.push_back(table(0,j, type));
                y_NA.push_back(table(i,j, type));
            }
        }
        string tempo;
        if(idDate.substr(idDate.length()-4,idDate.length())== ".csv") idDate = idDate.substr(0,idDate.length()-4);

        string name = table.getHeaders()[i]+idDate+".png";
        const char* out = name.c_str();
        const char* xlab = "heure d'enregistrement";
        const char* ylab = table.getHeaders()[i].c_str();
        tempo = "Evolution de "+table.getHeaders()[i];
        const char* title = tempo.c_str();

        cout<<out<<endl;

        if(table.getHeaders()[i] != "RR")
        {
            plotCurve(x, y,x_abr, y_abr, x_NA, y_NA ,out, xlab, ylab, title);
        }
        else
        {

        }
    }
}
*/

void Plot::windrose(const std::string& filename, const std::string& title, const std::string& out)
{

   /* setenv("PYTHONPATH",".",1);
    Py_Initialize();

    PyRun_SimpleString( "import sys \n"
                       //"sys.path.insert(0, \"/home/bouyahia/anaconda3/lib/python3.7/site-packages\") \n"
                       "from windrose import WindroseAxes \n"
                        //"from matplotlib import pyplot as plt \n"
                        //"import matplotlib.cm as cm \n"
                      //  /home/bouyahia/anaconda3/lib/python3.7/site-packages (1.16.2)
                    "import numpy as np \n"
                    "import pandas as pd \n"

                    "ws = np.random.random(500) * 6 \n"
                    "wd = np.random.random(500) * 360\n"
                    //"print(wd)\n"
                    "ax = WindroseAxes.from_ax() \n"
                    "ax.bar(wd, ws, normed=True, opening=0.8, edgecolor='white')\n");
    Py_Finalize(); */

    string stemp = "/usr/bin/python3 /home/bouyahia/filtrage/plot.py " + filename + " " + title + " " +out;
    const char* commande = stemp.c_str();

        system(commande);
}

const DTable Plot::inputing(DTable table) const
{
    DTable other(table);
    for(int i = 1; i < other.getNCols(); i++)
    {
        //calculer la valeur moyenne des données valides
        real dval = 0.;
        for(int j = 0; j < table.getNRows(); j++)
        {
            int BACK = j;
            int FORTH = j;
            if(table.existNA(i, j))
            {
                while(BACK >= 0 && FORTH < table.getNRows())
                {
                    if(!table.existNA(i, BACK))
                    {
                        real val = table(i,BACK, dval);
                        other(i,j, &val);
                        break;
                    }
                    if(!table.existNA(i, FORTH))
                    {
                        real val = table(i,FORTH, dval);
                        other(i,j, &val);
                        break;
                    }
                    BACK -= 1;
                    FORTH += 1;

                    if(BACK < 0 ) BACK = 0;
                    else if(FORTH >=  table.getNRows()) FORTH = table.getNRows() - 1;
                }
            }
        }
    }
    return other;
}



Plot::~Plot()
{
    //dtor
}
