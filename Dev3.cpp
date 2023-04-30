// Dev3.cpp : 
//

#include <iostream>
#include<time.h>
using namespace std;

template <typename unType>
struct unNoeud     //create a node holding a value and a node poiting the the next node 
{
    unType       valeur;
    unNoeud<unType>* suivant;
};

struct unClient  // create a client who comes at a certain time for a specific transaction time
{
    long tempsArrivee;
    long dureeTransaction;
};
template <class unType>
class uneFile
{
private:
    unNoeud<unType>* Tete;
    unNoeud<unType>* Queue;

public:

    uneFile()   // that's an empty queue
    {

        Tete = NULL;
        Queue = NULL;
    }


    ~uneFile()
    {
        unType nombre;
        while (!estVide())
            enlever(nombre);
    }

    bool estVide() //check if the queue is empty
    {
        return NULL == Tete;
    }


    void inserer(unType nombre) // insertion at the tail of the queue
    {
        unNoeud<unType>* temp = new unNoeud<unType>;
        temp->valeur = nombre;
        if (estVide()) {
            Tete = temp;
        }
        else {
            Queue->suivant = temp;
        }
        Queue = temp;
        temp->suivant = NULL;
    }


    void enlever(unType& nombre) // removal at the head of the queue
    {
        unNoeud<unType>* temp = Tete;
        nombre = temp->valeur;
        Tete = Tete->suivant;
        if (estVide())
            Queue = NULL;
        delete temp;
    }

};


bool avonsNousUnClient(double probabilite) {// generate the probability of having a customer entering the bank 
    double alea = rand() / double(32767); //alea is a decimal between 0 and 1
    return alea <= probabilite;
}

long genererTempsTransaction(long dureeMinimum, long dureeMaximum) {// generate a transaction time for each customer
    dureeMaximum++;
    dureeMaximum -= dureeMinimum;
    long duree = dureeMinimum + (rand() % dureeMaximum);
    return  duree;
}

bool guichetVide(unClient guichet[], long nombreGuichets) {
    for (long k = 0; k < nombreGuichets; k++) {
        if (guichet[k].dureeTransaction != 0)
            return false;
    }
    return true;
}



int main()
{
    srand(time(0));

    uneFile<unClient>  file; // Au démarrage, la file est vide.  || empty queue

    unClient guichet[10] = { 0 }; // maximum number of office  ticket in the bank

    unClient customer;

    long nombreGuichets; 
    cout << "Entrez le nombre de guichets (Enter the number of office, iwth a max of 10) :  " << endl; // enter the number of tickets office
    cin >> nombreGuichets;

    long horloge = 0;        // the clock
    double clientServis = 0;               //number of customer served
    long tempsAttente, tempsTotal = 0;      //waiting time , total time 

    double tempsFin;
    cout << "Entrez le temps de fin en minutes (Enter the closing time in minutes) : " << endl;    // set the closing hour of the bank
    cin >> tempsFin;

    long dureeMinimum, dureeMaximum;
    cout << "Entrer la dureeMin et la dureeMax de la transaction (Enter the min and max duration i minutes) : ";   // set the min and max time allow for each transaction per client
    cin >> dureeMinimum >> dureeMaximum;

    long frequenceArrivee;
    cout << "Entrer la frequence d'arrivee des clients (Enter the customer's arrival frequency, between 0 and 60)" << endl;    // set how often are customers coming in the bank. Should be between 0 and 60
    cin >> frequenceArrivee;

    double probabilite = frequenceArrivee / 60.0;       //generate the the probability for  customer to arrive base on the frequency
    //cout << probabilite << endl;

    while (horloge <= tempsFin || !file.estVide() || !guichetVide(guichet, nombreGuichets)) // execute the loop whenever the bank is stil open and there is customer either at an office or in the queue
    {

        if (avonsNousUnClient(probabilite) && horloge <= tempsFin) {
            // write down the arrival time, generate a transaction duration  and put a new client in the queue if he shows up before the closing hour 
            customer.tempsArrivee = horloge;            
            customer.dureeTransaction = genererTempsTransaction(dureeMinimum, dureeMaximum);
            //cout << "dureeTransaction" << customer.dureeTransaction << endl;   //help visualising the duration

            file.inserer(customer);
        }
        for (long k = 0; k < nombreGuichets; k++) {
            if (guichet[k].dureeTransaction == 0 && file.estVide()) {   // nothing happen when there is no one to serve
                continue;
            }
            else if (guichet[k].dureeTransaction == 0 && !file.estVide()) {    //if a customer just end his transaction and there is at least on person in the queue, people at the front of the queue will be redireted to the office available
                file.enlever(customer);
                guichet[k] = customer;
                guichet[k].dureeTransaction--;
                tempsAttente = horloge - customer.tempsArrivee;
                tempsTotal += tempsAttente;
                clientServis++;
            }
            else {
                guichet[k].dureeTransaction--;
            }
        }
        horloge++;
        //cout << "horloge" << horloge << endl;//me permet de visualiser l'horloge
    }

    cout << "\nLe nombre de clients servi est (The number of client served is) " << clientServis << " clients." << endl << endl;

    double tempsMoyen = double(tempsTotal) / clientServis;                       // the average time per client is given by dividing  the total time by the number of customer who passed by the bank
    cout << "Le temps moyen d'attente est de (the average wainting time is) " << tempsMoyen << " minutes." << endl << endl;
    cout << "Le temps de fermeture de la banque est " << horloge << endl << endl;

    double frequenceEffective = clientServis / tempsFin;                 // the efficient frequency is given by the number of customer served divided by the real closing time
    cout << "La frequence effective d'arrivee des clients est (the effective client's arrival frequency is) " << frequenceEffective << " clients/min ou " << clientServis / (tempsFin / 60) << " clients/ heure" << endl;

    return 0;
}





