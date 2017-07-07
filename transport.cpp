#include "transport.h"
#include <iostream>


string mot::MOTToString[4] = {"Samolot", "Autobus", "Pociag", "Lodz"};

// Route

Route::Route(int distance, int destination) : itsDistance(distance), itsDestination(destination) {}

int Route::GetDistance() { return itsDistance; }

int Route::GetDestination() { return itsDestination; }
// FormOfTransport

FormOfTransport::FormOfTransport(int distance, int destination, MeansOfTransport mean, int cost) :
        itsMOT(mean), itsCost(cost),Route(distance, destination)
{

}

void FormOfTransport::ShowRoute(string start)
{
    cout << "Mozesz jechac " << GetDistance() << " km z " <<
         start << " do " << TravelAgency::GetSingleton()->FindByNum(GetDestination())->GetName() <<
         " uzywajac " << mot::MOTToString[itsMOT] << " placac " << itsCost << " zloty." << endl;
}

int FormOfTransport::GetCost() { return itsCost; }

MeansOfTransport FormOfTransport::GetMeansOfTransport() { return itsMOT; }
// City

int City::numberOfCities = 0;

int City::GetNumberOfCities() { return numberOfCities; }

City::City(string name) : itsName(name), itsNumber(City::GetNumberOfCities())
{
    numberOfCities++;
}

City::~City()
{
    cout << "destruktor" << endl;
    itsMOT[0].clear();
    itsMOT[1].clear();
    itsMOT[2].clear();
    itsMOT[3].clear();
}

void City::AddRoute(int distance, int cost, MeansOfTransport meanOfTransport, int destination)
{
    FormOfTransport* fot = new FormOfTransport(distance, destination, meanOfTransport, cost);
    itsMOT[fot->GetMeansOfTransport()].push_back(*fot);

}
string City::GetName() { return itsName; }

int City::GetNumber() {return itsNumber; }

void City::Show()
{
    for (int i = 0; i < 4; ++i)
    {
        for (list<FormOfTransport>::iterator it = itsMOT[i].begin(); it != itsMOT[i].end(); ++it)
        {
            it -> ShowRoute(itsName);
        }
    }
}

// TravelAgency

TravelAgency* TravelAgency::instance = NULL;

TravelAgency::TravelAgency()
{

}

TravelAgency::~TravelAgency()
{
    for (list<City*>::iterator it = itsCities.begin(); it != itsCities.end(); ++it)
    {
        delete *it;
    }
    itsCities.clear();
}

void TravelAgency::AddCity(string city)
{
    City* tmp = new City(city);
    itsCities.push_back(tmp);
}

void TravelAgency::AddRoute(string start, string destination, int distance, int cost, MeansOfTransport meanOfTransport)
{
    City* from = FindByName(start);
    int dest = FindByName(destination)->GetNumber();
    from -> AddRoute(distance, cost, meanOfTransport, dest);
}

void TravelAgency::Show()
{
    for (list<City*>::iterator it = itsCities.begin(); it != itsCities.end(); ++it)
    {
        (*it)->Show();
    }
}

City* TravelAgency::FindByNum(int number)
{
    for (list<City*>::iterator it = itsCities.begin(); it != itsCities.end(); ++it)
    {
        if ((*it)->GetNumber() == number) return *it;
    }
    return NULL;
}

City* TravelAgency::FindByName(string name)
{
    for (list<City*>::iterator it = itsCities.begin(); it != itsCities.end(); ++it)
    {
        if ((*it)->GetName() == name) return *it;
    }
    return NULL;
}

TravelAgency* TravelAgency::GetSingleton()
{
    if (instance == NULL)
    {
        instance = new TravelAgency;
    }
    return instance;
}


int main() {
    TravelAgency* instance = TravelAgency::GetSingleton();
    instance ->AddCity("Krakow");
    instance ->AddCity("Warszawa");
    instance ->AddCity("Poznan");
    instance -> AddRoute("Krakow", "Warszawa", 500, 40, TRAIN);
    instance -> AddRoute("Warszawa", "Poznan", 400, 50, PLANE);
    instance -> Show();
    delete instance;

    return 0;
}