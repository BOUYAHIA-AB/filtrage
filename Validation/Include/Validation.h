#ifndef VALIDATION_H
#define VALIDATION_H
#include <DTable.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <assert.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "mysql_connection.h"
#include "mysql_driver.h"

#include "Plot.h"

namespace filtrage
{
    /// \class Validation
    /// \brief Classe permattant la validation des donnees en se basant sur des donnees de reference valides.
    class Validation
    {
        private:
            vector<std::string> m_fileToValidate; ///< les fichiers à valide
            std::string m_chemin; ///< chemin vers le repértoire contenant les fichiers
            map<std::string,double> m_dataRef; ///< map qui contient les données de référence
            map<std::string,int> m_parametresId; ///< map les ids des parametres et donc les ids des capteurs
            int* m_date; ///< date du jour des données à valider, celui d'enregistrement. 1er élément : jour. 2e élément  : mois. 3e élément  : année.
            int m_nbStation; ///< nombre de stations
            set<std::string> m_fileNoRec;///< set contenant les noms des fichiers non recus ou qu'on peut pas les lire
            set<std::string> m_fileNoData;///< set contenant les noms des fichiers recus mais qui ne contiennent pas de données


            sql::mysql::MySQL_Driver *driver;
            sql::Connection *con;
            sql::Statement *stmt;
            sql::ResultSet *res;
            sql::ResultSet *keys;

        public:
            /// \brief Constructeur.<br>
            /// Aucune données par défaut.
            Validation();

            /// \brief Constructeur.<br>
            /// charger les données de référence.
            /// récupérer la date de validation.
            /// remplir fileTovalidate.
            /// remplir fileNoRec
            /// \param baseName Nom de la base ou se trouve les tables.
            /// \param tableRefName Nom de la table contenant les données de référence.
            /// \param fileid Nom du fichier contenant les id des stations.
            /// \param chem vers le répértoire contenant les fichiers.
            /// \param date Tableau de 3 entiers.
            ///  - 1er élément : jour.
            ///  - 2e élément  : mois.
            ///  - 3e élément  : année.
            /// si date vaut NULL, alors on récupère la date du jours J.<br>
            Validation(const std::string& baseName, const std::string& tableRefName, const std::string& tableParametresId, const std::string& tableStationsMeteo,
                       const std::string& chem, int* date=NULL );

            /// \brief validation des fichiers. pour chaque station :
            ///     - verfie la reception du fichier
            ///     - verifie si on peut lire le fichier
            ///     - calcule le pourcentage de donnees aberrantes et de donnees manquantes pour chaque parametres
            /// \param filname : fichier de sortie
            /// \param seuilAccept : le pourcentage d'enregistrement manquantes ou aberrants à qu on autorise
            /// \param seuilAccept : le pourcentage d'enregistrement manquantes ou aberrants à qu on n autorise pas
            /// \param graphe : vaut true si on veut tracer des graphiques et vaut false sinon
            void validateFileRec(const std::string& filename, const int seuilAccept, const int seuilRejet,bool graphe = true );

            /// \brief Remplire la map dataRef.
            /// \param tableRefName : nom de la table qui contient les donnees de reference
            /// \param date Tableau de 3 entiers.
            ///  - 1er élément : jour.
            ///  - 2e élément  : mois.
            ///  - 3e élément  : année.
            void remplireDataRef(const std::string& tableRefName, int* date);

            /// \brief Remplire la map parametresId.
            /// \param tableParametresId : nom de la table qui contient les ids des parametres mesures
            void remplireParametresId(const std::string& tableParametresId);

            /// \brief Remplire la map fileToValidate.
            /// \param tableStationsMeteo : nom de la table qui contient les ids des stations
            /// \param date Tableau de 3 entiers.
            ///  - 1er élément : jour.
            ///  - 2e élément  : mois.
            ///  - 3e élément  : année.
            void remplireFileToValidate(const std::string& tableStationsMeteo, int* date);

            /// \brief Inserer les donnees contennues dans une fichier dans un table dela base de donnees.
            /// \param nameFile : fichier qui contient les donnees
            /// \param nameTable : la table dans laquelle on veut inserer les donnees.
            /// \pre nameFile et nameTable doivent contenir le meme nombre et les memes noms de colonnes.
            void instertToDataBase(const std::string& nameFile, const std::string& nameTable, const std::string *headLst, int ncols);

            /// \brief Méthode qui recupère la date du jour
            /// \param date tableau qui va contenir la date. 1er élément : jour. 2e élément  : mois. 3e élément  : année.
            void getDate(int* date) const;

            /// \brief Obtention du nombre de stations à valider.
            /// \return nombre de stations.
            int getNbstation() const;

            /// \brief Obtention des fichiers non recus.
            /// \return set fichiers non recus.
            const set<std::string>& getfileNoRec() const;

            /// \brief Obtention des fichiers recus mais qui n'ont pas des données.
            /// \return set fichiers non recus mais qui n'ont pas des données.
            const set<std::string>& getfileNoData() const;

            /// \brief test si les valeurs du vector passé en paramètre sont toute identique.
            /// \return vrai s'il sont identique et et faut sinon.
            bool verifVariance(const vector<double>& vec) const;

            /// \brief destructeur.<br>.
            virtual ~Validation();
    };
}


#endif // VALIDATION_H
