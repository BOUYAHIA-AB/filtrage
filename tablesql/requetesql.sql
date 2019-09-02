DROP TABLE IF EXISTS DataReference;
DROP TABLE IF EXISTS CodeErreur;
DROP TABLE IF EXISTS Parametres;
DROP TABLE IF EXISTS StationsMeteo;
DROP TABLE IF EXISTS ResValidation;
CREATE TABLE IF NOT EXISTS DataReference(  
    	mois SMALLINT UNSIGNED NOT NULL,
	BattV_MIN float NOT NULL,
	BattV_MAX float NOT NULL,
	PTemp_C_MIN float NOT NULL,
	PTemp_C_MAX float NOT NULL,
	Temp_1000cm_MIN float NOT NULL,
	Temp_1000cm_MAX float NOT NULL,
	Temp_464cm_MIN float NOT NULL,
	Temp_464cm_MAX float NOT NULL, 
	Temp_215_cm_MIN float NOT NULL,
	Temp_215_cm_MAX float NOT NULL,
	Direction_1000cm_MIN float NOT NULL,
	Direction_1000cm_MAX float NOT NULL,
	Vent_1000cm_MIN float NOT NULL,
	Vent_1000cm_MAX float NOT NULL,
	Vent_464cm_X19230_MIN float NOT NULL,
	Vent_464cm_X19230_MAX float NOT NULL,
	Vent_215cm_X19229_MIN float NOT NULL, 
	Vent_215cm_X19229_MAX float NOT NULL,
	Vent_100cm_X19228_MIN float NOT NULL,
	Vent_100cm_X19228_MAX float NOT NULL,
	HumRel_464cm_MIN float NOT NULL,
	HumRel_464cm_MAX float NOT NULL,
	Pluie_Tot_MIN float NOT NULL,
	Pluie_Tot_MAX float NOT NULL,
	Sensit_nb_particules_Tot_MIN float NOT NULL,
	Sensit_nb_particules_Tot_MAX float NOT NULL,
	Sensit_energiekinetique_Tot_MIN float NOT NULL,
	Sensit_energiekinetique_Tot_MAX float NOT NULL,
	PRIMARY KEY (mois)
) ENGINE=INNODB;

CREATE TABLE IF NOT EXISTS CodeErreur(
    id SMALLINT UNSIGNED NOT NULL PRIMARY KEY,
    description_erreur TEXT NOT NULL,
    capteur VARCHAR(40) NOT NULL,
    parametre VARCHAR(40)NOT NULL
)ENGINE=INNODB;

CREATE TABLE IF NOT EXISTS Parametres(
    id SMALLINT UNSIGNED NOT NULL,
    parametre VARCHAR(40) NOT NULL,
    PRIMARY KEY (id)
)ENGINE=INNODB;

CREATE TABLE IF NOT EXISTS StationsMeteo(
    id_station VARCHAR(40) NOT NULL,
    longitude float,
    latitude float,
    is_work TINYINT(1),
    PRIMARY KEY (id_station)
)ENGINE=INNODB;

CREATE TABLE IF NOT EXISTS ResValidation(
    id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
    date_enreg DATE NOT NULL,
    id_station VARCHAR(40) NOT NULL,
    file_name VARCHAR(40) NOT NULL,
    code_erreur SMALLINT UNSIGNED NOT NULL,
    pourc_val_abr FLOAT,
    pourc_val_mqt FLOAT,
    CONSTRAINT fk_client_numero          -- On donne un nom à notre clé
        FOREIGN KEY (code_erreur)             -- Colonne sur laquelle on crée la clé
        REFERENCES CodeErreur(id)        -- Colonne de référence
)ENGINE=INNODB;

LOAD DATA LOCAL INFILE '/home/bouyahia/filtrage25072019sansbase/filtrage/tablesql/DataReference.csv'
INTO TABLE DataReference
FIELDS TERMINATED BY ';'
LINES TERMINATED BY '\n' 
IGNORE 1 LINES
(mois,BattV_MIN, BattV_MAX, PTemp_C_MIN, PTemp_C_MAX, Temp_1000cm_MIN, Temp_1000cm_MAX, Temp_464cm_MIN, Temp_464cm_MAX, Temp_215_cm_MIN, Temp_215_cm_MAX, 
Direction_1000cm_MIN, Direction_1000cm_MAX, Vent_1000cm_MIN, Vent_1000cm_MAX, Vent_464cm_X19230_MIN, Vent_464cm_X19230_MAX, Vent_215cm_X19229_MIN, Vent_215cm_X19229_MAX, 
Vent_100cm_X19228_MIN, Vent_100cm_X19228_MAX,HumRel_464cm_MIN, HumRel_464cm_MAX, Pluie_Tot_MIN, Pluie_Tot_MAX, Sensit_nb_particules_Tot_MIN, Sensit_nb_particules_Tot_MAX,
Sensit_energiekinetique_Tot_MIN, Sensit_energiekinetique_Tot_MAX);

LOAD DATA LOCAL INFILE '/home/bouyahia/filtrage25072019sansbase/filtrage/tablesql/CodeErreur.csv'
INTO TABLE CodeErreur
FIELDS TERMINATED BY ';'
LINES TERMINATED BY '\n' 
IGNORE 1 LINES
(id,description_erreur,capteur,parametre);

LOAD DATA LOCAL INFILE '/home/bouyahia/filtrage25072019sansbase/filtrage/tablesql/Parametres.csv'
INTO TABLE Parametres
FIELDS TERMINATED BY ';'
LINES TERMINATED BY '\n' 
IGNORE 1 LINES
(id,parametre);

LOAD DATA LOCAL INFILE '/home/bouyahia/filtrage25072019sansbase/filtrage/tablesql/StationsMeteo.csv'
INTO TABLE StationsMeteo
FIELDS TERMINATED BY ';'
LINES TERMINATED BY '\n' 
IGNORE 1 LINES
(id_station,longitude,latitude,is_work);

DELETE FROM CodeErreur WHERE id=0;
DELETE FROM StationsMeteo WHERE id_station='';

