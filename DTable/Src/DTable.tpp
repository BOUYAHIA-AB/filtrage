#include <iostream>
template<typename U> U DTable::operator()(int i,int j,U refVal) const
{
	if(i<0 || i>=ncols || j<0 || j>=nrows) throw invalid_argument("Index invalide pour DTable::operator() const");

	char typeRef = '\0';
	if(typeid(U)==typeid(char)) typeRef = 'c';
	if(typeid(U)==typeid(bool)) typeRef = 'b';
	if(typeid(U)==typeid(int)) typeRef = 'i';
	if(typeid(U)==typeid(real)) typeRef = 'd';
	if(typeid(U)==typeid(string)) typeRef = 's';

	if(colsType[i]!=typeRef) throw invalid_argument("Type de donnée invalide pour DTable::operator() const");

	void *p = nullptr;
	map<string,pair<char,int> > tmp = index;
	int pos = tmp[headers[i]].second;

	switch(typeRef)
	{
		case 'c':
 			if(!maskChar[pos][j])
				p = &refVal;
			else
				p = &dataChar[pos][j];
			break;
		case 'b':
			if(!maskBool[pos][j])
				p = &refVal;
			else
				p = &dataBool[pos][j];
			break;
		case 'i':
			if(!maskInt[pos][j])
				p = &refVal;
			else
				p = &dataInt[pos][j];
			break;
		case 'd':
			if(!maskDouble[pos][j])
				p = &refVal;
			else
				p = &dataDouble[pos][j];
			break;
		case 's':
			if(!maskString[pos][j])
				p = &refVal;
			else
				p = &dataString[pos][j];
			break;
	}
	return *(U *)p;
}

template<typename U> void DTable::operator()(int i,int j,U *refVal)
{
	if(i<0 || i>=ncols || j<0 || j>=nrows) throw invalid_argument("Index invalide pour DTable::operator()");

	char typeRef = '\0';
	if(typeid(U)==typeid(char)) typeRef = 'c';
	if(typeid(U)==typeid(bool)) typeRef = 'b';
	if(typeid(U)==typeid(int)) typeRef = 'i';
	if(typeid(U)==typeid(real)) typeRef = 'd';
	if(typeid(U)==typeid(string)) typeRef = 's';

	if(colsType[i]!=typeRef) throw invalid_argument("Type de donnée invalide pour DTable::operator()");

	map<string,pair<char,int> > tmp = index;
	int pos = tmp[headers[i]].second;

	switch(typeRef)
	{
		case 'c':
 			maskChar[pos][j] = true;
			dataChar[pos][j] = *refVal;
			break;
		case 'b':
			maskBool[pos][j] = true;
			dataBool[pos][j] = *refVal;
			break;
		case 'i':
			maskInt[pos][j] = true;
			dataInt[pos][j] = *refVal;
			break;
		case 'd':
			maskDouble[pos][j] = true;
			dataDouble[pos][j] = *refVal;
			break;
		case 's':
			maskString[pos][j] = true;
			dataString[pos][j] = *refVal;
			break;
	}
}
