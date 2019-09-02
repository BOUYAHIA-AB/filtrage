#include "DTable.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <locale>
#include <limits>

//real strptime(string date,const char *fmt)
//{
//	real nsec = 0.;			// Nombre de secondes écoulées (valeur retournée)
//								// Le 2001-01-01 00:00 correspond à 0s
//	int co = 0;					// Compteur pour parcourir les chaînes de caractères
//	int bis = 0;				// Variable pour déterminer si l'année est bisextile
//	istringstream istr(date);	// Flux d'entrée
//
//	while(true)	// Poursuivre tant que la chaîne de format n'est pas entièrement parcourue
//	{
//		while(fmt[co]!='%' && fmt[co]!='\0') co++;	// Recherche le descripteur suivant ou la fin de chaîne
//		if(fmt[co]=='\0') break;					// Quitte la boucle si la fin de chaîne atteinte
//		co++;										// Position du caractère décrivant la composante de la date
//		char type = fmt[co];
//		co++;										// Position du caractère servant de séparateur
//		char sep = fmt[co];
//
//		string item;								// Lecture de la composante de date et conversion en un entier
//		getline(istr,item,sep);
//		int value = stoi(item);
//		switch(type)
//		{
//			case 'y':											// Année avec deux chiffres
//				nsec += ((int)((value-1)*365.25))*24*3600;
//				if(value%4==0) bis++;							// Teste si l'année est bisextile
//				break;
//			case 'Y':											// Année avec quatre chiffres
//				nsec += ((int)((value-2001)*365.25))*24*3600;
//				if(value%4==0) bis++;							// Teste si l'année est bisextile
//				break;
//			case 'm':											// Mois
//				switch(value)									// On ajoute le nombre de jours des mois précédents
//				{
//					case 12:
//					nsec += 30*24*3600;
//					case 11:
//					nsec += 31*24*3600;
//					case 10:
//					nsec += 30*24*3600;
//					case 9:
//					nsec += 31*24*3600;
//					case 8:
//					nsec += 31*24*3600;
//					case 7:
//					nsec += 30*24*3600;
//					case 6:
//					nsec += 31*24*3600;
//					case 5:
//					nsec += 30*24*3600;
//					case 4:
//					nsec += 31*24*3600;
//					case 3:
//					nsec += 28*24*3600;
//					case 2:
//					nsec += 31*24*3600;
//					case 1:
//					nsec += 0.;
//				}
//				if(value>2) bis++;								// Teste si on est après le mois de février
//				break;
//			case 'd':											// Nombre de jours complets depuis le début du mois
//				nsec += (value-1)*24*3600;
//				break;
//			case 'H':											// Nombre d'heures
//				nsec += value*3600;
//				break;
//			case 'M':											// Nombre de minutes
//				nsec += value*60;
//				break;
//			case 'S':											// Nombre de secondes
//				nsec += value;
//				break;
//		}
//		if(bis==2) nsec += 24*3600;	// Ajout d'un jour si année bisextile et date postérieure à février
//	}
//return nsec;
//}
//
//real DateToNumber(string *timePar,string date,char unite)
//{
//    // Analyse des instructions pour la colonne temps
//	int timeCol = -1;
//	int timeType = 0;
//	const char *timeFmt = nullptr;
//	real timeFct = 1.;
//	if(timePar!=nullptr)
//	{
//		timeCol = stoi(timePar[0]);
//		timeType = stoi(timePar[1]);
//		timeFmt = timePar[2].data();
//		timeFct = stod(timePar[3]);
//	}
//	real timeRef = -1.;
//
//    real nsec = 0.;
//    switch(timeType)
//    {
//        case 1:     // Temps sous forme d'une chaîne de caractères formatée
//            nsec = strptime(date,timeFmt);
//            nsec *= timeFct;
//            if(timeRef<0) timeRef = nsec;
//            nsec -= timeRef;
//            break;
//        case 2:     // Temps sous forme numérique
//            nsec = stod(date);
//            nsec *= timeFct;
//            if(timeRef<0) timeRef = nsec;
//            nsec -= timeRef;
//            break;
//        //case 0:
//            //default:
//            //nsec = j*timeFct;
//    }
//    //data[i][j] = nsec;
//
//    if(unite == 'm') return nsec/60;
//    else if(unite == 'h') return nsec/(60*60);
//    else if(unite == 'j') return nsec/(60*60*24);
//    else return nsec;
//
//}

// Constructeurs
// -------------

DTable::DTable()
{
	init();
}

DTable::DTable(int nr,int nc,char *typeLst,string *headLst)
{
	init();
	// Construction du tableau
	build(nr,nc,typeLst,headLst);
}

DTable::DTable(string name,char sep)
{
	init();
	// Lecture du tableau à partir d'un fichier
	read(name,sep);
}

DTable::DTable(const DTable &cpy)
{
	init();
	// Copie des éléments
	copy(cpy);
}

DTable::~DTable()
{
	reset();
}

// Méthodes privées
// ----------------

void DTable::init()
{
	nrows = ncols = 0;
	headers = nullptr;
	colsType = nullptr;

	dataDouble = nullptr;
	dataInt = nullptr;
	dataBool = nullptr;
	dataChar = nullptr;
	dataString = nullptr;

	maskDouble = nullptr;
	maskInt = nullptr;
	maskBool = nullptr;
	maskChar = nullptr;
	maskString = nullptr;

	separator = ';';
	filename = "";
}

void DTable::reset()
{
	if(dataDouble!=nullptr)
	{
		for(int i=0;i<occurrences['d'];i++)
		{
			delete[] maskDouble[i];
			delete[] dataDouble[i];
		}
		delete[] maskDouble;
		delete[] dataDouble;
	}
	if(dataInt!=nullptr)
	{
		for(int i=0;i<occurrences['i'];i++)
		{
			delete[] maskInt[i];
			delete[] dataInt[i];
		}
		delete[] maskInt;
		delete[] dataInt;
	}
	if(dataBool!=nullptr)
	{
		for(int i=0;i<occurrences['b'];i++)
		{
			delete[] maskBool[i];
			delete[] dataBool[i];
		}
		delete[] maskBool;
		delete[] dataBool;
	}
	if(dataChar!=nullptr)
	{
		for(int i=0;i<occurrences['c'];i++)
		{
			delete[] maskChar[i];
			delete[] dataChar[i];
		}
		delete[] maskChar;
		delete[] dataChar;
	}
	if(dataString!=nullptr)
	{
		for(int i=0;i<occurrences['s'];i++)
		{
			delete maskString[i];
			delete[] dataString[i];
		}
		delete[] maskString;
		delete[] dataString;
	}
	if(headers!=nullptr) delete[] headers;
	if(colsType!=nullptr) delete[] colsType;
	index.clear();
	occurrences.clear();
}

void DTable::build(int nr,int nc,char *typeLst,string *headLst)
{
	// Taille du tableau de données
	nrows = nr;
	ncols = nc;
    if(nrows<=0 || ncols<=0) throw invalid_argument("Dimension(s) incorrecte(s) pour DTable::build()");

	// En-têtes et type des colonnes
	colsType = new char[nc];
	headers = new string[nc];
	for(int i=0;i<ncols;i++)
	{
		if(typeLst[i]!='c' && typeLst[i]!='b' && typeLst[i]!='i' && typeLst[i]!='d' && typeLst[i]!='s')
				throw invalid_argument("Type de donnée inconnu dans DTable::build()");
		colsType[i] = typeLst[i];
		if(headLst!=nullptr)
			headers[i] = headLst[i];
		else
		{
			ostringstream out;
			out << 'V' << (i+1);
			headers[i] = out.str();
		}

		index[headers[i]].first = colsType[i];
		index[headers[i]].second = occurrences[colsType[i]];
		occurrences[colsType[i]]++;
	}

    // Déclaration des tableaux
    for(map<char,int>::iterator it=occurrences.begin();it!=occurrences.end();it++)
    {
        if(it->first=='b' && it->second>0)
        {
            dataBool = new bool*[it->second];
            maskBool = new bool*[it->second];
            for(int i=0;i<it->second;i++)
            {
				dataBool[i] = new bool[nrows];
				maskBool[i] = new bool[nrows];
				for(int j=0;j<nrows;j++)
				{
					dataBool[i][j] = false;
					maskBool[i][j] = false;
				}
			}
        }
        if(it->first=='i' && it->second>0)
        {
            dataInt = new int*[it->second];
            maskInt = new bool*[it->second];
            for(int i=0;i<it->second;i++)
            {
				dataInt[i] = new int[nrows];
				maskInt[i] = new bool[nrows];
				for(int j=0;j<nrows;j++)
				{
					dataInt[i][j] = 0;
					maskInt[i][j] = false;
				}
			}
        }
        if(it->first=='d' && it->second>0)
        {
            dataDouble = new real*[it->second];
            maskDouble = new bool*[it->second];
            for(int i=0;i<it->second;i++)
            {
				dataDouble[i] = new real[nrows];
				maskDouble[i] = new bool[nrows];
				for(int j=0;j<nrows;j++)
				{
					dataDouble[i][j] = 0.;
					maskDouble[i][j] = false;
				}
			}
        }
        if(it->first=='c' && it->second>0)
        {
            dataChar = new char*[it->second];
            maskChar = new bool*[it->second];
            for(int i=0;i<it->second;i++)
            {
				dataChar[i] = new char[nrows];
				maskChar[i] = new bool[nrows];
				for(int j=0;j<nrows;j++)
				{
					dataChar[i][j] = '\0';
					maskChar[i][j] = false;
				}
			}
        }
        if(it->first=='s' && it->second>0)
        {
            dataString = new string*[it->second];
            maskString = new bool*[it->second];
            for(int i=0;i<it->second;i++)
            {
				dataString[i] = new string[nrows];
				maskString[i] = new bool[nrows];
				for(int j=0;j<nrows;j++)
				{
					dataString[i][j] = "";
					maskString[i][j] = false;
				}
			}
        }
    }
}

void DTable::copy(const DTable &cpy)
{
	// Construction du tableau
	build(cpy.nrows,cpy.ncols,cpy.colsType,cpy.headers);
	// Copie des éléments
	if(dataString!=nullptr)
	{
		for(int i=0;i<occurrences['s'];i++)
			for(int j=0;j<nrows;j++)
			{
				dataString[i][j] = cpy.dataString[i][j];
				maskString[i][j] = cpy.maskString[i][j];
			}
	}
	if(dataChar!=nullptr)
	{
		for(int i=0;i<occurrences['c'];i++)
			for(int j=0;j<nrows;j++)
			{
				dataChar[i][j] = cpy.dataChar[i][j];
				maskChar[i][j] = cpy.maskChar[i][j];
			}
	}
	if(dataInt!=nullptr)
	{
		for(int i=0;i<occurrences['i'];i++)
			for(int j=0;j<nrows;j++)
			{
				dataInt[i][j] = cpy.dataInt[i][j];
				maskInt[i][j] = cpy.maskInt[i][j];
			}
	}
	if(dataDouble!=nullptr)
	{
		for(int i=0;i<occurrences['d'];i++)
			for(int j=0;j<nrows;j++)
			{
				dataDouble[i][j] = cpy.dataDouble[i][j];
				maskDouble[i][j] = cpy.maskDouble[i][j];
			}
	}
	if(dataBool!=nullptr)
	{
		for(int i=0;i<occurrences['b'];i++)
			for(int j=0;j<nrows;j++)
			{
				dataBool[i][j] = cpy.dataBool[i][j];
				maskBool[i][j] = cpy.maskBool[i][j];
			}
	}
}

// Méthodes d'accès
// ---------------

bool DTable::read(string name,char sep)
{
	reset();
	filename = name;
	int skip = 0; 		// Nombre de ligne de commentaire + 2 lignes : headers et colsType
	ncols = 0;
	nrows = 0;
	separator = sep;
	// Ouverture du fichier
	ifstream ifs(filename,ifstream::in | ifstream::binary);
	if(ifs)
    {
        // Détermination du nombre de lignes commentées
        string fline;
        int pos;
        while(!ifs.eof()) 	// Se positionne au début de la ligne qui contient les headers en fin de boucle
        {					// ou au début de la première ligne de données si aucune en-tête.
            pos = ifs.tellg();
            getline(ifs,fline);
            if(fline[0]!='#') break;
            nrows++;
            skip++;
        }
        if(ifs.eof()) return false;
        // Nombre de colonnes
        istringstream istr(fline);
        string item;
        while(getline(istr,item,separator)) ncols++;
        // Complétion de la lecture du fichier pour compter les autres ligne de commentaires
        while(!ifs.eof())
        {
            getline(ifs,fline);
            nrows++;
            if(fline[0]=='#') skip++;
        }
        ifs.clear();
        ifs.seekg(pos,ifs.beg);
        // Nombre de rangées
        nrows = nrows-skip-2; // Soustraction du nombre de commentaires (skip), d'une ligne d'en-têtes et d'une de types.
        // Lecture des en-têtes
        string *headLst = nullptr;
		getline(ifs,fline);
		istringstream istrHeaders(fline);
		headLst = new string[ncols];
		int i = 0;
		while(getline(istrHeaders,item,separator))
		{
			if(i==ncols-1) item = item.substr(0,item.length()); //Élimination du dernier caractère de retour chariot.
			if(i<ncols) headLst[i] = item;
			i++;
		}
		if(i!=ncols) throw invalid_argument("Nombre d'en-têtes invalide dans DTable::read()");
		// Lecture des types de colonne
		char *typeLst = nullptr;
		getline(ifs,fline);
		istringstream istrTypeCol(fline);
		typeLst = new char[ncols];
		i = 0;
		while(getline(istrTypeCol,item,separator))
		{
			if(i<ncols) typeLst[i] = item.at(0);
			i++;
		}
		if(i!=ncols) throw invalid_argument("Nombre de types de donnée invalide dans DTable::read()");
        // Initialisation des tableaux

        build(nrows,ncols,typeLst,headLst);
        delete [] typeLst;
        delete [] headLst;
        // Lecture des données
        int j = 0; // Compteur de lignes lues
        while(getline(ifs,fline))
        {
            if(fline[0]!='#')
            {
                istr.str(fline);
                istr.clear();
                istr.seekg(0,istr.beg);
                string item;
                int i = 0;	// Compteur global de colonnes lues
                map<char,int> compteur; // Compteur par type de données
                while(getline(istr,item,';'))
                {
                    if(i==ncols-1) item = item.substr(0,item.length()); // Suppression du retour chariot
                    if(colsType[i]=='s')
                    {
                        if(item!="NA")
                        {
							dataString[compteur['s']][j] = item;
							maskString[compteur['s']][j] = true;
						}
                        else
                        {
							dataString[compteur['s']][j] = "NA";
							maskString[compteur['s']][j] = false;
						}
                        compteur['s']++;
                    }
                    if(colsType[i]=='c')
                    {
                        if(item!="NA")
                        {
							dataChar[compteur['c']][j] = item[0];
							maskChar[compteur['c']][j] = true;
						}
                        else
                        {
							dataChar[compteur['c']][j] = '\0';
							maskChar[compteur['c']][j] = false;
						}
                        compteur['c']++;
                    }
                    if(colsType[i]=='b')
                    {
                        // Conversion en majuscule pour éviter plusieurs tests
                        locale loc;
                        for(string::size_type i=0;i<item.length();i++) item[i]=toupper(item[i],loc);

                        if(item!="NA")
                        {
							if(item=="0" || item=="F"|| item=="FALSE")
								dataBool[compteur['b']][j] = false;
							else
								dataBool[compteur['b']][j] = true;
							maskBool[compteur['b']][j] = true;
						}
						else
						{
							dataBool[compteur['b']][j] = false;
							maskBool[compteur['b']][j] = false;
						}
                        compteur['b']++;
                    }
                    if(colsType[i]=='i')
                    {
                        if(item!="NA")
                        {
							dataInt[compteur['i']][j] = stoi(item);
							maskInt[compteur['i']][j] = true;
                        }
                        else
                        {
							dataInt[compteur['i']][j] = 0;
							maskInt[compteur['i']][j] = false;
                        }
                        compteur['i']++;
                    }
                    if(colsType[i]=='d')
                    {
                        //size_t found = item.find(',');               // Recherche la position du séparateur décimal
                        //if(found!=string::npos) item[found] = '.';   // Impose le point comme séparateur décimal
                        if(item!="NA")
                        {
							dataDouble[compteur['d']][j] = stod(item);
							maskDouble[compteur['d']][j] = true;
                        }
                        else
                        {
							dataDouble[compteur['d']][j] = 0.;
							maskDouble[compteur['d']][j] = false;
                        }


//                        ///< pour la colonne des dates, l'utilisateur doit indiquer float commme type du colonne et pas string
//                        //si le nom de la colonne est date alors on fait la conversion à l'aide de la fonction DateToNomber
//                        //int DateToNumber(string *timePar, string date, char unite)
//                        if(headers[i]=='date')
//                        {
//                            dataDouble[compteur['i']][j] = DateToNumber(timePar,item,'s');
//                        }
//                        else
//                        {
//							float StringToFloat;
//							std::stringstream stream(item);
//							if(stream >> StringToFloat)
//								dataDouble[compteur['d']][j] = StringToFloat;
//							else
//								dataDouble[compteur['d']][j] = std::numeric_limits<float>::infinity();
//                        }
                        compteur['d']++;
                    }
                    i++;
                }
                j++;
            }
        }
        // Fermeture du fichier
        ifs.close();
        return true;
    }

	// Échec de lecture du fichier
	return false;
}

void DTable::write(string filename,char sep)
{
	ofstream ofs(filename,ofstream::out);
	for(int j=0;j<ncols-1;j++) ofs << headers[j] << sep;
	ofs << headers[ncols-1] << endl;

	for(int j=0;j<ncols-1;j++) ofs << colsType[j] << sep;
	ofs << colsType[ncols-1] << endl;

	char type;
	int pos;
	for(int i=0;i<nrows;i++)
	{
		for(int j=0;j<ncols;j++)
        {
            type = index[headers[j]].first;
			pos = index[headers[j]].second;
			switch(type)
			{
				case 's':
					if(maskString[pos][i])
						ofs << dataString[pos][i];
					else
						ofs << "NA";
					break;
				case 'c':
					if(maskChar[pos][i])
						ofs << dataChar[pos][i];
					else
						ofs << "NA";
					break;
				case 'b':
					if(maskBool[pos][i])
						ofs << dataBool[pos][i];
					else
						ofs << "NA";
					break;
				case 'i':
					if(maskInt[pos][i])
						ofs << dataInt[pos][i];
					else
						ofs << "NA";
					break;
				case 'd':
					if(maskDouble[pos][i])
						ofs << dataDouble[pos][i];
					else
						ofs << "NA";
					break;
			}
			if(j<ncols-1) ofs << sep;
        }
        ofs << endl;
	}
	ofs.close();
}

// Méthodes pour les propriétés
// ----------------------------

int DTable::getNRows() const
{
	return nrows;
}

int DTable::getNCols() const
{
	return ncols;
}

void DTable::setHeaders(string *hdrs)
{
	if(headers!=nullptr) delete[] headers;
	headers = new string[ncols];
	for(int i=0;i<ncols;i++) headers[i] = hdrs[i];
}

//void DTable::setColsType(string *typeLst)
//{
//	if(colsType!=nullptr) delete[] colsType;
//	colsType = new string[ncols];
//	for(int i=0;i<ncols;i++) colsType[i] = typeLst[i];
//}

string* DTable::getHeaders() const
{
	if(headers==nullptr) return nullptr;
	string *tmp = new string[ncols];
	for(int i=0;i<ncols;i++) tmp[i] = headers[i];
	return tmp;
}

char* DTable::getColsType() const
{
	if(colsType==nullptr) return nullptr;
	char *tmp = new char[ncols];
	for(int i=0;i<ncols;i++) tmp[i] = colsType[i];
	return tmp;
}

// Visualisation
// -------------

void DTable::view(int limit)
{
    // Affichage des en-têtes
	cout << '\t';
	for(int i=0;i<ncols;i++) cout << headers[i] << '\t';
	cout << endl;
	// Affichage des types de colonnes
	//if(colsType!=nullptr)
	cout << '\t';
	for(int i=0;i<ncols;i++) cout << colsType[i] << '\t';
	cout << endl;
	// Affichage des données en début de tableau
	int nlines = limit;
	if(nrows<limit) nlines = nrows; // Vérifie le nombre de lignes vraiment disponibles
	for(int j=0;j<nlines;j++)
	{
		cout << j+1 << '\t';
		for(int i=0;i<ncols;i++)
		{
		    char type = index[headers[i]].first;
			int pos = index[headers[i]].second;
			switch(type)
			{
				case 's':
					if(maskString[pos][j])
						cout << dataString[pos][j];
					else
						cout << "NA";
					break;
				case 'c':
					if(maskChar[pos][j])
						cout << dataChar[pos][j];
					else
						cout << "NA";
					break;
				case 'b':
					if(maskBool[pos][j])
						cout << boolalpha << dataBool[pos][j];
					else
						cout << "NA";
					break;
				case 'i':
					if(maskInt[pos][j])
						cout << dataInt[pos][j];
					else
						cout << "NA";
					break;
				case 'd':
					if(maskDouble[pos][j])
						cout << dataDouble[pos][j];
					else
						cout << "NA";
					break;
			}
			cout << '\t';
		}
		cout << endl;
	}
	// Affichage des données en fin de tableau
	if(limit>=nrows) return;
	if(nrows>2*limit) cout << "--- SKIPPING ---" << endl;
	int start = nrows-limit;
	if(start<limit) start = limit;  // Evite de réafficher les lignes du début
	for(int j=start;j<nrows;j++)
	{
		cout << j+1 << '\t';
		for(int i=0;i<ncols;i++)
		{
		    char type = index[headers[i]].first;
			int pos = index[headers[i]].second;
			switch(type)
			{
				case 's':
					if(maskString[pos][j])
						cout << dataString[pos][j];
					else
						cout << "NA";
					break;
				case 'c':
					if(maskChar[pos][j])
						cout << dataChar[pos][j];
					else
						cout << "NA";
					break;
				case 'b':
					if(maskBool[pos][j])
						cout << boolalpha << dataBool[pos][j];
					else
						cout << "NA";
					break;
				case 'i':
					if(maskInt[pos][j])
						cout << dataInt[pos][j];
					else
						cout << "NA";
					break;
				case 'd':
					if(maskDouble[pos][j])
						cout << dataDouble[pos][j];
					else
						cout << "NA";
					break;
			}
			cout << '\t';
		}
		cout << endl;
	}
}

// Opérateurs
// ----------

DTable& DTable::operator=(const DTable &cpy)
{
	if(this!=&cpy)
	{
		// Remise à zéro
		reset();
		// Copie des éléments
		copy(cpy);
	}
	return *this;
}

// Accés aux données
// -----------------

void DTable::toNA(int i,int j)
{
	if(i<0 || i>=ncols || j<0 || j>=nrows) throw invalid_argument("Index invalide pour DTable::toNA()");

	char refType = index[headers[i]].first;
	int pos = index[headers[i]].second;

	switch(refType)
	{
		case 'c':
			maskChar[pos][j] = false;
			break;
		case 'b':
			maskBool[pos][j] = false;
			break;
		case 'i':
			maskInt[pos][j] = false;
			break;
		case 'd':
			maskDouble[pos][j] = false;
			break;
		case 's':
			maskString[pos][j] = false;;
			break;
	}
}

DTable DTable::mergeTable(DTable &other,int *cols1,int *cols2)
{
	// Nombre de colonnes
	int ncls = 0;
	(cols1!=nullptr) ? ncls += cols1[0] : ncls += ncols;
	(cols2!=nullptr) ? ncls += cols2[0] : ncls += other.ncols;

	// En-têtes
	string *newHdrNames = new string[ncls];
	int co = 0;
	if(cols1==nullptr)
		for(int i=0;i<ncols;i++) newHdrNames[co++] = headers[i];
	else
		for(int i=0;i<cols1[0];i++) newHdrNames[co++] = headers[cols1[i+1]];
	if(cols2==nullptr)
		for(int i=0;i<other.ncols;i++) newHdrNames[co++] = other.headers[i];
	else
		for(int i=0;i<cols2[0];i++) newHdrNames[co++] = other.headers[cols2[i+1]];
	// Renommage des doublons
	string *oldHdrNames = new string[ncls];
	for(int i=0;i<ncls;i++) oldHdrNames[i] = newHdrNames[i];
	for(int i=0;i<ncls-1;i++)
	{
		int rep = 1;
		for(int j=i+1;j<ncls;j++)
			if(newHdrNames[j]==newHdrNames[i])
			{
				rep++;
				ostringstream hdr;
				hdr << newHdrNames[i] << '-' << rep;
				newHdrNames[j] = hdr.str();
			}
		if(rep>1)
		{
			ostringstream hdr;
			hdr << newHdrNames[i] << '-' << 1;
			newHdrNames[i] = hdr.str();
		}
	}
    // Type de colonnes
    char *tpCol = new char[ncls];
	co = 0;
	if(cols1==nullptr)
		for(int i=0;i<ncols;i++) tpCol[co++] = colsType[i];
	else
		for(int i=0;i<cols1[0];i++) tpCol[co++] = colsType[cols1[i+1]];
	if(cols2==nullptr)
		for(int i=0;i<other.ncols;i++) tpCol[co++] = other.colsType[i];
	else
		for(int i=0;i<cols2[0];i++) tpCol[co++] = other.colsType[cols2[i+1]];
	// Nombre de lignes (imposé par le tableau le plus petit)
	int nrws = 0;
	(nrows>other.nrows) ? nrws = nrows : nrws = other.nrows;
	// Initialisation du tableau final
	DTable output(nrws,ncls,tpCol,newHdrNames);
	// Chargement des données
	int ncols1 = 0;
	(cols1==nullptr) ? ncols1 = ncols : ncols1 = cols1[0];
	DTable *srcTbl = nullptr;
	/*char typSrc = '\0';
	int posSrc = -1;
	int posDest = -1;*/
	for(int i=0;i<ncls;i++)
	{
		(i<ncols1) ? srcTbl = this : srcTbl = &other;
		char srcType = srcTbl->index[oldHdrNames[i]].first;
		int srcPos = srcTbl->index[oldHdrNames[i]].second;
		int dstPos = output.index[newHdrNames[i]].second;
		for(int j=0;j<srcTbl->nrows;j++)
		{
			if(srcType=='c')
			{
				output.maskChar[dstPos][j] = srcTbl->maskChar[srcPos][j];
				output.dataChar[dstPos][j] = srcTbl->dataChar[srcPos][j];
			}
			if(srcType=='b')
			{
				output.maskBool[dstPos][j] = srcTbl->maskBool[srcPos][j];
				output.dataBool[dstPos][j] = srcTbl->dataBool[srcPos][j];
			}
			if(srcType=='i')
			{
				output.maskInt[dstPos][j] = srcTbl->maskInt[srcPos][j];
				output.dataInt[dstPos][j] = srcTbl->dataInt[srcPos][j];
			}
			if(srcType=='d')
			{
				output.maskDouble[dstPos][j] = srcTbl->maskDouble[srcPos][j];
				output.dataDouble[dstPos][j] = srcTbl->dataDouble[srcPos][j];
			}
			if(srcType=='s')
			{
				output.maskString[dstPos][j] = srcTbl->maskString[srcPos][j];
				output.dataString[dstPos][j] = srcTbl->dataString[srcPos][j];
			}
		}
	}
	// Nettoyage
	delete [] newHdrNames;
	delete [] oldHdrNames;
	delete [] tpCol;

	return output;
}

bool DTable::existNA(int i, int j)
{
    if(i<0 || i>=ncols || j<0 || j>=nrows) throw invalid_argument("Index invalide pour DTable::existNA()");

    char type = index[headers[i]].first;
	int pos = index[headers[i]].second;
    switch(type)
	{
		case 'c':
			return !maskChar[pos][j];
			break;
		case 'b':
			return !maskBool[pos][j];
			break;
		case 'i':
			return !maskInt[pos][j];
			break;
		case 'd':
			return !maskDouble[pos][j];
			break;
		case 's':
			return !maskString[pos][j];
			break;
        default:
            return true;
            break;
	}

}

//float** DTable::pointerFloat()
//{
//	return dataDouble;
//}
//
//int** DTable::pointerInt()
//{
//	return dataInt;
//}
//
//
//string** DTable::pointerStrnig()
//{
//	return dataString;
//}
//
//char** DTable::pointerChar()
//{
//	return dataChar;
//}
//
//bool** DTable::pointerBool()
//{
//	return dataBool;
//}


//#if defined(__CUDA_ARCH__)
//void DTable::toCUDATable(real *&cudaTbl,int *cols)
//{
//	// Taille du tableau final
//	int nRows = getNRows();
//	int nCols = getNCols();
//	int nCells = nRows;
//	if(cols==nullptr)
//		nCells *= nCols;
//	else
//	{
//		nCells *= cols[0];
//		nCols = cols[0];
//	}
//	// Initialisation du tableau
//	if(cudaTbl!=nullptr) cudaFree(cudaTbl);
//	cudaMallocManaged(&cudaTbl,nCells*sizeof(real));
//	// Copie des éléments
//	for(int i=0;i<nCols;i++)
//		for(int j=0;j<nRows;j++)
//			if(cols==nullptr)
//				cudaTbl[i*nRows+j] = data[i][j];
//			else
//				cudaTbl[i*nRows+j] = data[cols[i+1]][j];
//}
//
//DTable DTable::fromCUDATable(real *cudaTbl,int nr,int nc,string *hdrs)
//{
//	DTable tbl(nr,nc,hdrs);
//	// Copie des éléments
//	for(int i=0;i<nc;i++)
//		for(int j=0;j<nr;j++)
//			tbl(i,j) = cudaTbl[j+i*nr];
//	return tbl;
//}
//#endif
//
//
