//============================================================================
// Name        : TP4_C++.cpp
// Author      : lolo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <fstream>
using namespace std;
//commit test

#include "Forme.h"
#include "PolygoneConvexe.h"
#include "Segment.h"
#include"Point.h"
#include "Rectangle.h"
#include "Union.h"
#include "Intersection.h"
#include "Ardoise.h"


int Split(vector<string>& vecteur, string chaine, char separateur);
int stringToInt(string s);
bool estConvexe(const vector<Point> &lesPoints);
bool nbPairCoordonnees(int debut,int fin);
void ajouterForme(string commande,Ardoise& uneArdoise,bool Load);

int main()
{

	Ardoise lArdoise;
	string commande;
	vector<string> result;

	vector<string> undoLoad;

	do
	{
		cout<<"rentrez la commande"<<endl;
		getline( cin, commande );
		Split(result, commande,' ');

		ajouterForme(commande,lArdoise,false);


		if(result[0].compare("HIT")==0)
		{
			if (result.size() != 4)
			{
				cout << "ERR" << endl;
			}
			else
			{
				Forme* f = lArdoise.rechercheParNom(result[1]);

				if (f == NULL)
				{
					cout << "ERR" << endl;
				}
				else
				{
					if (f->contient(stringToInt(result[2]), stringToInt(result[3])))
					{
						cout << "YES" << endl;
					}
					else
					{
						cout << "NO" << endl;
					}
				}
			}
		}

		else if(result[0].compare("DELETE")==0)
			{

				//----------------------On verifie qu'il y ait suffisamment d'arguments-------------------------------------------------
				if(result.size()<2)
				{
					cout<<"ERR"<<endl;
				}
				else
				{
						//----------------On regarde si tous les noms existent------------------------------------------------------------------
						bool nomsExistent=true;
						for(int i=1;i<result.size();i++)
						{
							if(lArdoise.rechercheParNom(result[i])==NULL)
							{
								nomsExistent=false;
							}
						}
						//---------------------------------------------------------------------------------
						//----------------------------Supprimer les formes---------------------------------
						Forme* f;
						if(nomsExistent)
						{
							string com = "DELETE";

							for(int i=1;i<result.size();i++)
							{
								f=lArdoise.rechercheParNom(result[i]);
								lArdoise.supprimer(f);

								com += " " + result[i];
							}
							cout<<"OK"<<endl;

							lArdoise.ajouterCommande(com);
						}
						//----------------------------------------------------------------------------------
						else
						{
							cout<<"ERR"<<endl;
						}
				}


			}
//-----------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------Deplacer les formes-------------------------------------------------
		else if(result[0].compare("MOVE")==0)
		{
			//----------------------On verifie qu'il y ait suffisamment d'arguments-------------------------------------------------
			if(result.size()!=4)
			{
				cout<<"ERR"<<endl;
			}
			else
			{

				Forme* f=lArdoise.rechercheParNom(result[1]);
				//---------------------------------------verifier que le nom existe------------
				if(f==NULL)
				{
					cout<<"ERR"<<endl;
				}
				else
				{
					f->deplacer(stringToInt(result[2]),stringToInt(result[3]));
					lArdoise.ajouterCommande("MOVE " + result[1] + " " + result[2] + " " + result[3]);
					lArdoise.afficher();//---------------------------------------------------*****
				}
			}

		}

		else if(result[0].compare("LIST")==0)
		{
			lArdoise.afficher();
		}

		else if(result[0].compare("UNDO")==0)
		{
			string com = lArdoise.undo();

			if (com.compare("") == 0)
			{
				cout << "ERR" << endl;
			}
			else
			{
				vector<string> args;
				Split(args, com, ' ');

				if (args[0].compare("S") == 0 || args[0].compare("R") == 0 || args[0].compare("PC") == 0 || args[0].compare("OR") == 0 || args[0].compare("OI") == 0)
				{
					Forme* f = lArdoise.rechercheParNom(args[1]);

					if (f != NULL)
					{
						lArdoise.supprimer(f);

					}
					else
					{
						cout << "ERR" << endl;
					}
				}
				else if (args[0].compare("DELETE") == 0)
				{
					string com2 = "";

					for (int i = 1; i < args.size(); i++)
					{
						Forme* f = lArdoise.rechercheParNom(args[i]);

						if (f == NULL)
						{
							for (int j = 1; j <= 2; j++)
							{
								args.erase(args.begin());
							}

							for (vector<string>::iterator it = args.begin(); it != args.end(); it++)
							{
								if (it < args.end() - 1)
								{
									com2 += *it + " ";
								}
								else
								{
									com2 += *it;
								}
							}
						}

						else
						{
							cout << "ERR" << endl;
							cout << "# La forme " + f->getNom() + " existe d�j�." << endl;
						}

						ajouterForme(com2, lArdoise, false);
					}

				}
				else if (args[0].compare("MOVE") == 0)
				{
					Forme* f=lArdoise.rechercheParNom(args[1]);

					//--------------------------------verifier que le nom existe------------
					if(f==NULL)
					{
						cout<<"ERR"<<endl;
					}
					else
					{
						f->afficher();
						f->deplacer(-(stringToInt(result[2])), -(stringToInt(result[3])));
					}
				}
				else if (args[0].compare("LOAD") == 0)
				{
					for (vector<string>::iterator it = undoLoad.begin(); it != undoLoad.end(); it++)
					{
						vector<string> args;

						Split(args, *it, ' ');

						Forme* f = lArdoise.rechercheParNom(args[1]);

						if (f != NULL)
						{
							lArdoise.supprimer(f);
						}
					}
				}
			}
		}

		else if(result[0].compare("REDO")==0)
		{
			string com = lArdoise.redo();

			if (com.compare("") == 0)
			{
				cout << "ERR" << endl;
			}
			else
			{
				vector<string> args;
				Split(args, com, ' ');

				if (args[0].compare("S") == 0 || args[0].compare("R") == 0 || args[0].compare("PC") == 0 || args[0].compare("OR") == 0 || args[0].compare("OI") == 0)
				{
					Forme* f = lArdoise.rechercheParNom(args[1]);

					if (f == NULL)
					{
						ajouterForme(com, lArdoise, false);
					}
					else
					{
						cout << "ERR" << endl;
						cout << "# La forme " + f->getNom() + " existe d�j�." << endl;
					}
				}
				else if (args[0].compare("DELETE") == 0)
				{
					Forme* f = lArdoise.rechercheParNom(args[1]);

					if (f != NULL)
					{
						lArdoise.supprimer(f);
					}
					else
					{
						cout << "ERR" << endl;
					}
				}

				else if (args[0].compare("MOVE") == 0)
				{
					Forme* f=lArdoise.rechercheParNom(args[1]);
					//---------------------------------------verifier que le nom existe------------
					if(f==NULL)
					{
						cout<<"ERR"<<endl;
					}
					else
					{
						f->deplacer(stringToInt(args[2]),stringToInt(args[3]));
						cout << "OK" << endl;
					}
				}
			}
		}

		else if (result[0].compare("LISTUNDO") == 0)
		{
			lArdoise.afficherCommandes();
		}

		else if(result[0].compare("LOAD")==0)
		{
			string com;
			Ardoise uneArdoise;
			string a=result[1];
			bool b =true;
			ifstream fichier(a.c_str(), ios::in);  // on ouvre le fichier en lecture

						if(fichier)  // si l'ouverture a r�ussi
					    {
							vector<Forme*> formeTemp;
							while(getline(fichier, com))  // tant que l'on peut mettre la ligne dans "contenu"
							{
								ajouterForme(com,uneArdoise,true);
							}
							for(int i=0;i<undoLoad.size();i++)
							{
								if(lArdoise.rechercheParNom(uneArdoise.getFormes()[i]->getNom())!=NULL)
								{
									b=false;
									break;
								}
							}
							if(!b)
							{
								cout<<"ERR"<<endl;
							}
							else
							{
								for(int i=0;i<uneArdoise.getFormes().size();i++)
								{
									undoLoad.push_back(uneArdoise.getFormes()[i]->getNom());
									lArdoise.ajouter(uneArdoise.getFormes()[i],true);
								}

							}
						            // instructions
						     fichier.close();  // on ferme le fichier

					    }
						else
						{
							cout << "ERR" << endl;
						}
		}

		else if(result[0].compare("SAVE")==0)
		{
			if(result.size()!=2)
			{
				cout<<"ERR"<<endl;
			}

			else
			{
				lArdoise.sauvegarder(result[1]);
				cout<<"OK"<<endl;
			}
		}

		else if(result[0].compare("CLEAR")==0)
		{
			lArdoise.vider();
		}

		else
		{
			if((commande.compare("EXIT")!=0)&&(result[0].compare("S"))&&(result[0].compare("R"))&&(result[0].compare("DELETE"))&&(result[0].compare("PC"))&&(result[0].compare("OR"))&&(result[0].compare("OI")))
			{
				cout<<"ERR"<<endl;
			}
		}



	}while((commande.compare("EXIT"))!=0);

	//lArdoise.vider();
	return 0;

}

bool estConvexe(const vector<Point> &lesPoints)
{
	bool cvexe=true;
	bool positif;
	bool changerSigne = true;

	for (int i = 0; i < lesPoints.size() - 2; i++)
	{
		int dx1 = lesPoints[i+1].getX() - lesPoints[i].getX();
		int dy1 = lesPoints[i+1].getY() - lesPoints[i].getY();
		int dx2 = lesPoints[i+2].getX() - lesPoints[i+1].getX();
		int dy2 = lesPoints[i+2].getY() - lesPoints[i+1].getY();

		int zcrossproduct = dx1*dy2 - dy1*dx2;

		if (changerSigne)
		{
			if (zcrossproduct > 0)
			{
				positif = true;
				changerSigne = false;
			}
			else if (zcrossproduct < 0)
			{
				positif = false;
				changerSigne = false;
			}
		}

		if ((zcrossproduct < 0 && positif) || (zcrossproduct > 0 && !positif))
		{
			cvexe = false;
			break;
		}
	}

	return cvexe;
}

bool nbPairCoordonnees(int debut,int fin)
{
	if((fin-debut)%2!=0)//--------------si le nombre de coordonn�es de ppints n'est pas pair
	{
		return false;
	}
	else
	{
		return true;
	}
}

int stringToInt(string s)
{
	const char* c;
	c=s.c_str();
    return atoi(c);
}

int Split(vector<string>& vecteur, string chaine, char separateur)
{
	vecteur.clear();

	string::size_type stTemp = chaine.find(separateur);

	while(stTemp != string::npos)
	{
		vecteur.push_back(chaine.substr(0, stTemp));
		chaine = chaine.substr(stTemp + 1);
		stTemp = chaine.find(separateur);
	}

	vecteur.push_back(chaine);

	return vecteur.size();
}

//---------------------Ajouter forme--------------------------------------------------------------------------------------------

void ajouterForme(string commande,Ardoise& uneArdoise,bool Load)
{
	vector<string> resultLoad;
	Split(resultLoad, commande,' ');
	if(resultLoad[0].compare("S")==0)
	{

		if(uneArdoise.rechercheParNom(resultLoad[1])==NULL)//--------Si le nom n'existe pas d�j�
		{
			if(resultLoad.size()!=6)//--------------- Si il n'y a pas au moins 6 arguments dans la commande
			{
				cout<<"ERR"<<endl;
			}

			else
			{
			Point a(stringToInt(resultLoad[2]),stringToInt(resultLoad[3]));
			Point b(stringToInt(resultLoad[4]),stringToInt(resultLoad[5]));
			uneArdoise.ajouter(new Segment(resultLoad[1],commande,segm,a,b), false);
			cout<<"OK"<<endl;

			}
		}

		else
		{
			cout<<"ERR"<<endl;
		}

	}
	else if(resultLoad[0].compare("R")==0)
			{

				if(uneArdoise.rechercheParNom(resultLoad[1])==NULL)//--------Si le nom n'existe pas d�j�
				{


					//-------------Si il n'y a pas assez d'arguments--------------------
						if(resultLoad.size()!=6)
						{
							cout<<"ERR"<<endl;
						}
	                //--------------------------------------------------------------------
						else
						{
							//------------Si le premier point n'est pas celui en haut � gauche -------------------------------
							if((stringToInt(resultLoad[5])>=stringToInt(resultLoad[3]))|| (stringToInt(resultLoad[2])>=stringToInt(resultLoad[4])))

							{
								cout<<"ERR"<<endl;
							}
							//-----------------------------------------------------------------------------------------------------
							else
							{
							Point c(stringToInt(resultLoad[2]),stringToInt(resultLoad[3]));
							Point d(stringToInt(resultLoad[4]),stringToInt(resultLoad[5]));
							uneArdoise.ajouter(new Rectangle(resultLoad[1],commande,rect,c,d), false);
							cout<<"OK"<<endl;
							}
						}
				}
				else
				{
					cout<<"ERR"<<endl;
				}
			}

	else if(resultLoad[0].compare("PC")==0)
			{

				if(uneArdoise.rechercheParNom(resultLoad[1])==NULL)//--------Si le nom n'existe pas d�j�
				{
					if(resultLoad.size()<8)//---------si il n'y a pas assez d'arguments--------------
					{
						cout<<"ERR"<<endl;
					}
					else if(!nbPairCoordonnees(2,resultLoad.size()))//--------------si le nombre de coordonn�es de ppints n'est pas pair
					{
						cout<<"ERR"<<endl;

					}
					else
					{
						//--------------------ajout des points dans un vecteur de points----------------
						vector<Point> lesPoints;
						for(int i=2;i<resultLoad.size();i+=2)
						{

							lesPoints.push_back(Point(  stringToInt(resultLoad[i])   ,stringToInt(resultLoad[i+1])  ));
						}

						//-----------------------si le polygone est convexe-----------------------

						if(estConvexe(lesPoints))
						{
							uneArdoise.ajouter(new PolygoneConvexe(resultLoad[1],commande,poly,lesPoints), false);
							cout<<"OK"<<endl;

						}
						//------------------------------------------------------------------------------
						else
						{
							cout<<"message d'erreur"<<endl;
						}
					}
				}
				else
				{
					cout<<"ERR"<<endl;
				}

			}

	else if(resultLoad[0].compare("OR")==0)
	{

		if(uneArdoise.rechercheParNom(resultLoad[1])==NULL)//--------Si le nom n'existe pas d�j�
		{
			if(resultLoad.size()<4)//--------------------si il n'y a pas assez d'arguments--------------
			{
				cout<<"ERR"<<endl;
			}

			//-----------------Si les noms n'existent pas-----------------------------
			else
			{

				bool nomsExistent=true;
				for(int i=2;i<resultLoad.size();i++)
				{
					if(uneArdoise.rechercheParNom(resultLoad[i])==NULL)
					{
						nomsExistent=false;
					}
				}

				if(nomsExistent)
				{
					vector<Forme*> z;
					for(int i=2;i<resultLoad.size();i++)
					{

						z.push_back(uneArdoise.rechercheParNom(resultLoad[i]));

					}
					//-----------changement de nom-------------------



					uneArdoise.ajouter(new Union(resultLoad[1],commande,uni,z), false);

					if(!Load)
					{
						uneArdoise.getFormes().back()->changerNom();
					}
					cout<<"OK"<<endl;
					uneArdoise.afficher();
				}

				//-------------------------------------------------------------------------
				else
				{
					cout<<"ERR"<<endl;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------Intersection----------------------------------------
			else if(resultLoad[0].compare("OI")==0)
			{

				if(uneArdoise.rechercheParNom(resultLoad[1])==NULL)//--------Si le nom n'existe pas d�j�
				{
						if(resultLoad.size()<4)//---------------si il n'y a pas assez d'arguments--------------
						{
							cout<<"ERR"<<endl;
						}
						//-----------------------si les noms n'existent pas dans la liste------------------
						else
						{
							bool nomsExistent=true;
							for(int i=2;i<resultLoad.size();i++)
							{
								if(uneArdoise.rechercheParNom(resultLoad[i])==NULL)
								{
									nomsExistent=false;
								}
							}



							if(nomsExistent)
							{
								vector<Forme*> z;
								for(int i=2;i<resultLoad.size();i++)
								{

									z.push_back(uneArdoise.rechercheParNom(resultLoad[i]));

								}

								uneArdoise.ajouter(new Intersection(resultLoad[1],commande,inter,z), false);
								if(!Load)
								{
									uneArdoise.getFormes().back()->changerNom();
								}
								cout<<"OK"<<endl;
								uneArdoise.afficher();
							}
							//-------------------------------------------------------------------------------------------
							else
							{
								cout<<"ERR"<<endl;
							}
						}
				}
				else
				{
					cout<<"ERR"<<endl;
				}


			}

}//---------------------------------------------------Fin ajouter forme--------------------------------------------------------------

