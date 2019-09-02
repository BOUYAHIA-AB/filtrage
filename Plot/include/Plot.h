#ifndef PLOT_H
#define PLOT_H
#include <DTable.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <Python.h>
#include "plplot.h"
#include "plstream.h"

class Plot
{
    private:

    public:
    Plot();
    virtual ~Plot();


    /// \brief tracer un graphe
    /// \param x l'axe des abscices
    /// \param y l'axe des ordonnées
    /// \param x_abr les heures qui contiennent des données abberrantes
    /// \param y les heures qui ne contiennent pas des données
    /// \param out nom de l'image de sortie
    /// \param xlab label de l'axe des abscices
    /// \param ylab label de l'axe des ordonnées
    /// \param title titre du graphique
    void plotCurve(const vector<double>& x , const vector<double>& y ,  const vector<double>& x_abr, const vector<double>& y_abr, const vector<double>& x_NA,
                const vector<double>& y_NA, const char* out, const char* xlab, const char* ylab, const char* title) const;

    /// \brief tracer un graphe
    /// \param y vecteur des valeur à tracer
    /// \param out nom de l'image de sortie
    /// \return un graphe sous format png
    void plotCurve(const vector<double>& y, const char* out, const char* xlab, const char* ylab, const char* title) const;

    /// \brief tracer la rose du vent
    /// \param filename le fichier qui contient les donnees
    /// \param title titre a mettre dans le graphique
    /// \param out nom de l image de sortie
    void windrose(const std::string& filename, const std::string& title, const std::string& out = "rose");

    /// \brief tracer un ensemble de graphes
    /// \param table DTable des paramètre à tracer
    /// \param idDate l'id et la date d'enregistrement pour construire les nom des fichiers de sortie
    void plotCurve(DTable &table, DTable &masque, std::string& idDate);

    /// \brief remplace chaque donnée manquante d'un DTable par la valeur la plus proche
    /// \param table : le fichier qui contient des données manquantes
    /// \return un DTable sans données manquantes
    const DTable inputing(DTable table) const;

};

#endif // PLOT_H
