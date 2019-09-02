#ifndef DTABLE_H
#define DTABLE_H

#include "Definitions.h"

#include <string>
#include <map>
#include <assert.h>
#include <typeinfo>
//#include <cuda.h>

#ifdef __CUDACC__
    __host__ __device__
#endif
using namespace std;

/// \defgroup GlbFct Fonctions globales
/// \brief Fonctions globales
/// @{
/// \brief Conversion d'une date en seconde.
/// \param date Chaîne de caractères représentant la date à convertir.
/// \param *fmt Chaîne de caractères représentant le format de la date.
///  - %y : année (2 chiffres)
///  - %Y : année (4 chiffres)
///  - %m : mois (2 chiffres)
///  - %d : jour (2 chiffres)
///  - %H : heure (2 chiffres)
///  - %M : minute (2 chiffres)
///  - %S : seconde (2 chiffres)<br>
/// La fonction suppose l'existence d'un séparateur entre les composantes de la date.
/// \return Nombre de secondes écoulées depuis le 1 janvier 2001 à 0h.
//real strptime(string date,const char *fmt);
/// @}
/// \brief convertir une date sous forme de chaine de caracère un réel.
/// \param timePar Tableau de 4 chaînes de caractères décrivant les instructions pour le temps.
///  - 1er élément : indice de la colonne temps.
///  - 2e élément  : type de format de temps.
///      - 0 : indice de la ligne.
///      - 1 : temps exprimé sous forme d'une chaîne de caractères formatée.
///      - 2 : temps exprimé sous forme numérique.
///  - 3e élément  : format du temps (requis dans le cas du format de type 1).
///  - 4e élément  : facteur de conversion à appliquer (requis par tous les types de format de temps).
/// \param date le date à convertir Boléen pour indiquer la présence d'en-têtes.
/// \param unite unite de sortie : s = second, m=minute, h=heure, j=jour
/// \see DateToNumber(string *timePar, string date, char unite);.
//real DateToNumber(string *timePar, string date, char unite);

/// \class DTable
/// \brief Tableau de données.
class DTable
{
	private:
		int nrows;		///< Nombre de lignes.
		int ncols;		///< Nombre de colonnes.

		real **dataDouble;///< Données (réel).
		int **dataInt; 		///< Données (entier).
		bool **dataBool; 	///< Données (booléen).
		char **dataChar; 	///< Données (caractère).
		string **dataString;///< Données (chaine de caractère).

		bool **maskDouble;	///< Masque (réel).
		bool **maskInt;		///< Masque (entier).
		bool **maskBool;	///< Masque (booléen).
		bool **maskChar;	///< Masque (caractère).
		bool **maskString;	///< Masque (chaine de caractère).

		string *headers;	///< En-tête des colonnes.
		char *colsType;		///< type des colonnes.
		map<string,pair<char,int> > index;	///< Clé : nom de la colonne.
											///< Pair : type de la colonne et son indice dans la table respective.
        map<char,int> occurrences;			///< Nombre de colonne pour chaque type de données s, d, i, b, c.
		char separator;		///< Séparateur de champs.
		string filename;	///< Nom du fichier associé.

		void init();					///< Initialisation des membres de la classe à leur valeur par défaut.
		void reset();					///< Réinitialisation des membres de la classe.
		void build(int nr,int nc,char *typeLst,string *headLst);	///< Construction de la matrice de données.
		void copy(const DTable &cpy);	///< Copie les éléments d'une autre instance.
//		void readHeaders();				///< Lecture des en-têtes.

	public:
		/// \brief Constructeur.<br>
		/// Aucune données par défaut.
		DTable();
		/// \brief Constructeur.<br>
		/// Tableau de données vides.
		/// Le nom des en-têtes est généré automatiquement si aucune liste de noms n'est fournie.
		/// \param nr Nombre de lignes.
		/// \param nc Nombre de colonnes.
		/// \param *typeLst Liste des types de colonnes.
		/// \param *headLst Liste des en-têtes.
		DTable(int nr,int nc,char *typeLst,string *headLst=nullptr);
		/// \brief Constructeur.<br>
		/// Données chargées depuis un fichier.
		/// \param name Nom du fichier.
		/// \param timePar Tableau de 4 chaînes de caractères décrivant les instructions pour le temps.
		///  - 1er élément : indice de la colonne temps.
		///  - 2e élément  : type de format de temps.
		///      - 0 : indice de la ligne.
		///      - 1 : temps exprimé sous forme d'une chaîne de caractères formatée.
		///      - 2 : temps exprimé sous forme numérique.
		///  - 3e élément  : format du temps (requis dans le cas du format de type 1).
		///  - 4e élément  : facteur de conversion à appliquer (requis par tous les types de format de temps).
		/// \param sep Séparateur de champs.
		/// Le premier élément de la colonne temps vaut toujours zéro.<br>
		/// S'il n'y a aucune colonne de temps, laisser l'argument 'timePar' à sa valeur par défaut (nullptr).
		DTable(string name,char sep=';');
		/// \brief Constructeur par copie.
		/// \param &cpy Instance à copier.
		DTable(const DTable &cpy);
		/// \brief Destructeur.
		~DTable();

		/// \brief Lecture des données.
		/// \param name Nom du fichier.
		/// \param timePar Tableau de 4 chaînes de caractères décrivant les instructions pour le temps.
		/// \param skip Nombre de lignes à sauter en début de fichier.
		/// \param sep Séparateur de champs.
		/// \see DTable(string name,bool header=true,string *timePar=nullptr,int skip=0,char sep=';').
		bool read(string name,char sep=';');
		/// \brief Écriture des données.
		/// \param name Nom du fichier.
		/// \param sep Séparateur de champs.
		void write(string name,char sep=';');

		/// \brief Obtention du nombre de lignes.
		/// \return Nombre de lignes.
		int getNRows() const;
		/// \brief Obtention du nombre de colonnes.
		/// \return Nombre de colonnes.
		int getNCols() const;

		/// \brief Initialisation des en-têtes.
		/// \param *hdrs Liste des en-têtes.
		void setHeaders(string *hdrs);
		/// \brief Initialisation des type de colonnes.
		/// \param *tpCol Liste des type de colonnes.
//		void setColsType(char *typeLst);
		/// \brief Obtention des en-têtes.
		/// \return Un pointeur vers la liste des en-têtes ; sinon un pointeur vide.
		string* getHeaders() const;
        /// \brief Obtention des types de colonnes.
		/// \return Un pointeur vers la liste des types ; sinon un pointeur vide.
		char* getColsType() const;

		/// \brief Affichage des données.
		/// \param limit Restreint le nombre de lignes affichées en début et en fin de tableau.
		void view(int limit=4);

		/// \brief Retrait d'une valeur.
		/// \param i Index de la colonne.
		/// \param j Index de la ligne.
		void toNA(int i,int j);
		/// \brief Accès à une donnée.
		/// \param i Index de la colonne.
		/// \param j Index de la ligne.
		/// \return La valeur de l'élément désiré.
		template<typename U> U operator()(int i,int j,U refVal) const;
		/// \brief Accès à une donnée.
		/// \param i Index de la colonne.
		/// \param j Index de la ligne.
		/// \return La valeur de l'élément désiré.
		template<typename U> void operator()(int i,int j,U *refVal);

		/// \brief Opérateur d'assignation.
		/// \param &cpy Objet à copier.
		/// \return Objet avec les valeurs de l'instance copiée.
		DTable& operator=(const DTable &cpy);

		/// \brief Fusion de tableaux.
		/// \param other Tableau à accoler.
		/// \param cols1 Sélection des colonnes du premier tableau.
		/// \param cols2 Sélection des colonnes du second tableau.
		/// \return Le tableau résultant de la fusion.
		DTable mergeTable(DTable &other,int *cols1=nullptr,int *cols2=nullptr);

		/// \brief vérifie si une valeur de la table vaut NA ou non.
		/// \param i Index de la colonne.
		/// \param j Index de la ligne.
		/// \return true si la valeur vaut NA et false sinon.
		bool existNA(int i, int j);

		#if defined(__CUDA_ARCH__)
		/// \brief Conversion en un tableau unidimensionnel pour une routine CUDA.
		/// \param *&cudaTbl Tableau à retourner.
		/// \param cols1 Sélection des colonnes.
//		void toCUDATable(real *&cudaTbl,int *cols=nullptr);
		/// \brief Lecture à partir d'un tableau unidimensionnel créé par une routine CUDA.
		/// \param *cudaTbl Tableau unidimensionnel.
		/// \param nr Nombre de lignes.
		/// \param nc Nombre de colonnes.
		/// \param hdrs Liste des en-têtes.
//		DTable fromCUDATable(real *cudaTbl,int nr,int nc,string *hdrs=nullptr);
        #endif
		/// \brief Accès au tableau de données float.
//		float** pointerFloat();
		/// \brief Accès au tableau de données entier.
//		int** pointerInt();
		/// \brief Accès au tableau de données string.
//		string** pointerStrnig();
		/// \brief Accès au tableau de données char.
//		char** pointerChar();
		/// \brief Accès au tableau de données bool.
//		bool** pointerBool();
};

#include "DTable.tpp"

#endif
