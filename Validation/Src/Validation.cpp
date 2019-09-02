#include "Validation.h"
using namespace std;
using namespace filtrage;

void Validation::getDate(int* date) const
{

    time_t secondes;
    struct tm instant;
    int day;
    int mon;
    int year;
    time(&secondes);
    instant=*localtime(&secondes);

    day = instant.tm_mday;
    mon = instant.tm_mon+1;
    year = instant.tm_year +1900;

    if(day > 1)
    {
        date[0] = day-1;
        date[1] = mon;
        date[2] = year;
    }
    else
    {
        if(mon == 1)
        {
            date[0] = 31;
            date[1] = 12;
            date[2] = year-1;
        }
        else if(mon == 2 || mon == 4 || mon == 6 || mon == 9 || mon == 8 || mon == 11)
        {
            date[0] = 31;
            date[1] = mon -1;
            date[2] = year;
        }
        else if(mon == 5 || mon == 7 || mon == 10 || mon == 12)
        {
            date[0] = 30;
            date[1] = mon -1;
            date[2] = year;
        }
        else if(mon == 3)
        {
            if(year%4==0) date[0] = 29;
            else date[0] = 28;
            date[1] = mon -1;
            date[2] = year;
        }
    }
}

Validation::Validation()
{
    m_nbStation = 0;
}
/*
void Validation::RemplireDataRef(const std::string& baseName, const std::string& tableRefName, const std::string& tableParametresId, int* date)
{
    m_dataRef.clear();
    string sql;
    try
    {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::ResultSet *keys;

        // Create a connection
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("tcp://127.0.0.1", "abdelali", "abdelali");

        // Connect to the MySQL test database
        con->setSchema(baseName);
        stmt = con->createStatement();
        sql = "USE "+baseName;
        stmt->execute(sql);

        sql = "SELECT column_name FROM information_schema.columns WHERE table_name = '"+tableRefName+"'";
        keys = stmt->executeQuery(sql);
        double  valeur;
        string key;
        //rempilr m_dataRef
        while (keys->next())
        {
            key = keys->getString(1);
            sql = "SELECT * FROM "+ tableRefName+" WHERE mois="+to_string( date[1]);
            res = stmt->executeQuery(sql);
            while (res->next())
            {
                valeur = res->getDouble(key);
            }
            m_dataRef[key]=valeur;
         }

        sql = "SELECT * FROM "+ tableParametresId;
        res = stmt->executeQuery(sql);
        int valeur1;
        string key1;
        while (res->next())
        {
            key1 = res->getString("parametre");
            valeur1 = res->getInt("id");
            m_parametresId[key1]=valeur1;
            //cout << key1 << "   :    " << valeur1 << endl;
        }
        delete res;
        delete keys;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}
*/

Validation::Validation(const std::string& baseName, const std::string& tableRefName, const std::string& tableParametresId,const std::string& tableStationsMeteo,
                       const std::string& chem, int* date)
{
    m_nbStation = 0;
    m_chemin = chem;
    string sql;
    if(date==NULL)
    {
        date = new int[3];
        this->getDate(date);
        this->m_date=date;
    }
    else
    {
        this->m_date=date;
    }

    try
    {
        // Create a connection
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("tcp://127.0.0.1", "abdelali", "abdelali");

        // Connect to the MySQL test database
        con->setSchema(baseName);
        stmt = con->createStatement();
        sql = "USE "+baseName;
        stmt->execute(sql);
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    remplireDataRef(tableRefName, m_date);
    remplireParametresId(tableParametresId);
    remplireFileToValidate(tableStationsMeteo, m_date);

    //this->RemplireDataRef(baseName, tableRefName, tableParametresId, date);
}

void Validation::remplireDataRef(const std::string& tableRefName, int* date)
{
    m_dataRef.clear();
    string sql;

    sql = "SELECT column_name FROM information_schema.columns WHERE table_name = '"+tableRefName+"'";
    keys = stmt->executeQuery(sql);

    double  valeur;
    string key;
    //rempilr m_dataRef
    while (keys->next())
    {
        key = keys->getString(1);
        sql = "SELECT * FROM "+ tableRefName+" WHERE mois="+to_string( date[1]);
        res = stmt->executeQuery(sql);
        while (res->next())
        {
            valeur = res->getDouble(key);
        }
        m_dataRef[key]=valeur;
     }
}

void Validation::remplireParametresId(const std::string& tableParametresId)
{
    m_parametresId.clear();
    string sql;

    sql = "SELECT * FROM "+ tableParametresId;
    res = stmt->executeQuery(sql);
    int valeur;
    string key;
    while (res->next())
    {
        key = res->getString("parametre");
        valeur = res->getInt("id");
        m_parametresId[key]=valeur;
    }
}

void Validation::remplireFileToValidate(const std::string& tableStationsMeteo, int* date)
{
    m_fileToValidate.clear();
    m_fileNoRec.clear();
    string sql;

    sql = "SELECT * FROM "+ tableStationsMeteo;
    res = stmt->executeQuery(sql);
    string id_station;

    while (res->next())
    {
        id_station = res->getString("id_station");
        string mois = to_string(date[1]);
        string jour = to_string(date[0]);
        if(mois.length() == 1 ) mois = "0"+mois;
        if(jour.length() == 1 ) jour = "0"+jour;
        m_fileToValidate.push_back(id_station+to_string(date[2])+mois+jour+".csv");
        m_nbStation ++;
    }

    for(int i=0; i<m_nbStation; i++)
    {
        ifstream ifs(m_chemin+m_fileToValidate[i],ifstream::in | ifstream::binary);
        if(!ifs) m_fileNoRec.insert(m_fileToValidate[i]);
    }
}

Validation::~Validation()
{
    delete[] m_date;
    delete res;
    delete keys;
    delete stmt;
    delete con;
}

bool Validation::verifVariance(const vector<double>& vec) const
{
    auto MIN = min_element(std::begin(vec), std::end(vec));
    auto MAX = max_element(std::begin(vec), std::end(vec));

    double vecMin = *MIN, vecMax = *MAX;

    if(vecMax == vecMin)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Validation::instertToDataBase(const string& nameFile, const string& nameTable, const string *headLst, int ncols)
{
    string sql;
    sql = "LOAD DATA LOCAL INFILE '"+ nameFile + "' " + "INTO TABLE " + nameTable + " FIELDS TERMINATED BY ';' LINES TERMINATED BY '\\n' IGNORE 2 LINES " + "(";
    for(int i = 0; i < ncols; i++ )
    {
        sql = sql + headLst[i];
        if(i < ncols - 1)
        {
        sql = sql + ",";
        }
    }
    stmt->execute(sql);
    sql = sql +")";
    cout << sql<<endl;
}

void Validation::validateFileRec(const string& filename, const int seuilAccept, const int seuilRejet, bool graphe)
{
    string date_enreg = to_string(m_date[2])+"-"+to_string(m_date[1])+"-"+to_string(m_date[0]);
    string id_station;
    string file_name;

    for(int i=0; i < m_nbStation; i++)
    {
        file_name = m_fileToValidate[i];
        id_station = m_fileToValidate[i].substr(0,m_fileToValidate[i].length() - 11);
        string queryDeBase = "INSERT INTO ResValidation (date_enreg, id_station, file_name, code_erreur, pourc_val_abr, pourc_val_mqt) VALUES ('" +
                            date_enreg +"', '"+ id_station +"', '"+ file_name +"', ";

        string sql;
        sql = queryDeBase;
        const bool is_in = m_fileNoRec.find(m_fileToValidate[i]) != m_fileNoRec.end();

        if(is_in)
        {
            sql = sql + to_string(100)+ ", " + to_string(100) + ", " + to_string(100) + ");";
            stmt->execute(sql);
        }
        else
        {
            DTable data; ///< va contenir les données à valider
            DTable masque; ///< contient les résultats de validation
                           ///< (i,j) = TRUE si la valeur existe est valide
                           ///<       = FALSE si la valeur existe est invalide
                           ///<       = NA si la valeur est manquante
            char TRUE = 'T';
            char FALSE = 'F';
            char NA = 'N';
            int nr = 0;
            int nc = 0;
            float pourc_val_abr;
            float pourc_val_mqt;
            string *headLst;
            char *typeLst;
            map<string, float > paramWithHightValAbr; ///< cle : nom du parametre
                                                    ///< valeur : pourcentage de valeurs aberrantes

            try
            {
                data.read(m_chemin+m_fileToValidate[i],';');

                //construction de DTable masque
                nr = data.getNRows();
                nc = data.getNCols();
                headLst = data.getHeaders();
                typeLst = new char[nc];
                for(int i=0; i< nc; i++)
                {
                    typeLst[i]='c';
                }
                masque = DTable(nr,nc,typeLst,headLst);
            }
            catch (exception& e)
            {
                sql = sql + to_string(101)+ ", " + to_string(100) + ", " + to_string(100) + ");";
                stmt->execute(sql);
                continue;
            }

            real type = 0;
            char cval = ' ';
            for(int m=3; m< data.getNCols(); m++)
            {
                vector<double> dataForCalcVariance;
                string nameCol = data.getHeaders()[m];
                int nbValeurAbsant = 0;
                int nbValeurAberrant = 0;

                if(nameCol != "Sensit_nb_particules_Tot" && nameCol != "Sensit_energiekinetique_Tot")
                {
                    for(int n=0; n< data.getNRows(); n++)
                    {
                       if(data.existNA(m,n))
                        {
                            nbValeurAbsant += 1;
                            masque(m,n, &NA);
                        }
                        else if(data(m,n, type) < m_dataRef[nameCol+"_MIN"] || data(m,n,type) > m_dataRef[nameCol+"_MAX"])
                        {
                            dataForCalcVariance.push_back(data(m,n,type));
                            nbValeurAberrant += 1;
                            masque(m,n, &FALSE);
                        }
                        else
                        {
                            dataForCalcVariance.push_back(data(m,n,type));
                            masque(m,n, &TRUE);
                        }
                    }
                    int code;
                    pourc_val_abr =  (100 * nbValeurAberrant)/nr;
                    pourc_val_mqt = (100 * nbValeurAbsant)/nr;
                    cout<< nr << "    :   "<< nbValeurAberrant<< "    :   "<<nbValeurAbsant<<endl;
                    sql = queryDeBase;

                    if(pourc_val_mqt > seuilAccept)
                    {
                        code = 200+m_parametresId[nameCol];
                        sql = sql + to_string(code)+ ", " + to_string(pourc_val_abr) + ", " + to_string(pourc_val_mqt) + ");";
                        stmt->execute(sql);
                    }

                    sql = queryDeBase;

                    if(pourc_val_abr > seuilAccept)
                    {
                        paramWithHightValAbr[nameCol] = pourc_val_abr;
                        code = 300+m_parametresId[nameCol];
                        sql = sql + to_string(code)+ ", " + to_string(pourc_val_abr) + ", " + to_string(pourc_val_mqt) + ");";
                        stmt->execute(sql);
                    }

                    sql = queryDeBase;

                    if(verifVariance(dataForCalcVariance))
                    {
                        code = 400+m_parametresId[nameCol];
                        sql = sql + to_string(code)+ ", " + to_string(pourc_val_abr) + ", " + to_string(pourc_val_mqt) + ");";
                        stmt->execute(sql);

                        for(int n=0; n< data.getNRows(); n++)
                        {
                            if(masque(m,n, cval) != 'N') masque(m,n, &FALSE);
                        }
                    }
                }
            }//fin for m

            if(paramWithHightValAbr.empty())
            {
                instertToDataBase(m_chemin+m_fileToValidate[i], id_station, headLst, nc);
            }

            //if(graphe == true)
           // {
            //    Plot plt;
           //     plt.plotCurve(data, masque, m_fileToValidate[i] );
           // }
            //masque.view();
        }//fin else
    } //fin for i

}

int Validation::getNbstation() const
{
    return m_nbStation;
}

const set<string>&  Validation::getfileNoRec() const
{
    return m_fileNoRec;
}

const set<string>& Validation::getfileNoData() const
{
    return m_fileNoData;
}





