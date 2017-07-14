#include <stack>
#include "transport.h"



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

list<FormOfTransport>* City::GetOneOfMeansOfTransport(MeansOfTransport i)
{
    return &itsMOT[i];
}



void City::ShowByFormOfTransport(MeansOfTransport mot)
{
    for (list<FormOfTransport>::iterator it = itsMOT[mot].begin(); it != itsMOT[mot].end(); ++it)
    {
        it -> ShowRoute(itsName);
    }
}

void City::Show()
{
    for (int i = 0; i < 4; ++i)
    {
        ShowByFormOfTransport((MeansOfTransport)i);
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

void TravelAgency::FindTheCheapestPath(string startS, string destinationS)
{
    City* toStart = FindByName(startS);
    City* toFinish = FindByName(destinationS);

    // Dijkstra
    int N = City::GetNumberOfCities() ;
    int MAX = 1000000;

    Dijkstra** dijkstra = new Dijkstra*[N];
    Paths** paths = new Paths*[N];

    for (int i = 0; i < N; ++i)
    {
        dijkstra[i] = new Dijkstra;
        dijkstra[i] -> itsNumber = i;
        dijkstra[i] -> itsDistance = MAX;
        paths[i] = new Paths;
        paths[i] -> setItsDistance(MAX);
        paths[i] -> setItsPrevious(-1);
        paths[i] -> setItsVisited(false);
    }

    int start = toStart -> GetNumber();
    dijkstra[start] -> itsDistance = 0;
    paths[start] -> setItsDistance(0);

    priority_queue<int, vector<Dijkstra*>, Compare> minPiorityQueue;

    for (int j = 0; j < N; ++j)
    {
        minPiorityQueue.push(dijkstra[j]);
    }

    delete[] dijkstra;


    while(minPiorityQueue.empty() == false)
    {

        Dijkstra* tmp = minPiorityQueue.top();
        minPiorityQueue.pop();
        int v = tmp -> itsNumber;
        delete tmp;
        if (!paths[v] -> isItsVisited())
        {
            paths[v] -> setItsVisited(true);

            City* vCity = FindByNum(v);

            for (int i = 0; i < 4; ++i)
            {
                list<FormOfTransport>* list1 = vCity ->GetOneOfMeansOfTransport((MeansOfTransport)i);

                for (list<FormOfTransport>::iterator it = list1 -> begin(); it != list1 -> end(); ++it)
                {
                    int u = it ->GetDestination();
                    if (paths[u] -> getItsDistance() > paths[v] ->getItsDistance() + it ->GetDistance())
                    {
                        paths[u] ->setItsDistance(paths[v] ->getItsDistance() + it ->GetDistance());
                        paths[u] ->setItsPrevious(v);
                    }
                }
            }
        }

    }

    cout << "Odleglosc wynosi " <<
         paths[toFinish ->GetNumber()] ->getItsDistance() << " km i wiedzie przez " << endl;

    stack<int> sPrevious;
    int k = toFinish ->GetNumber();
    while(k != -1)
    {
        sPrevious.push(k);
        k = paths[k] -> getItsPrevious();
    }

    while (!sPrevious.empty())
    {
        k = sPrevious.top();
        sPrevious.pop();
        cout << FindByNum(k) -> GetName() << endl;
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

// Dijkstra

int Paths::getItsDistance() const
{
    return itsDistance;
}

void Paths::setItsDistance(int itsDistance)
{
    Paths::itsDistance = itsDistance;
}

bool Paths::isItsVisited() const
{
    return itsVisited;
}

void Paths::setItsVisited(bool itsVisited)
{
    Paths::itsVisited = itsVisited;
}

int Paths::getItsPrevious() const
{
    return itsPrevious;
}

void Paths::setItsPrevious(int itsPrevious)
{
    Paths::itsPrevious = itsPrevious;
}


int main() {
    TravelAgency* instance = TravelAgency::GetSingleton();
    instance ->AddCity("Krakow");
    instance ->AddCity("Warszawa");
    instance ->AddCity("Poznan");
    instance ->AddCity("Wroclaw");
    instance -> AddRoute("Krakow", "Warszawa", 500, 40, TRAIN);
    instance -> AddRoute("Warszawa", "Poznan", 400, 50, PLANE);
    instance -> AddRoute("Krakow", "Warszawa", 500, 40, SHIP);
    instance -> AddRoute("Warszawa", "Wroclaw", 450, 35, COACH);
    instance -> AddRoute("Wroclaw", "Krakow" , 450, 35, COACH);
    instance -> AddRoute("Warszawa", "Krakow" , 800, 35, COACH);
    instance -> Show();
    instance -> FindTheCheapestPath("Krakow", "Poznan");
    instance -> FindTheCheapestPath("Warszawa", "Krakow");
    delete instance;

    return 0;
}


