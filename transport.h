//
// Created by wojtek on 05.07.17.
//
#include <list>
#include <string>

using namespace std;

enum MeansOfTransport {PLANE, COACH, TRAIN, SHIP};

namespace mot
{

    extern string MOTToString[4];
}


class Route
{
public:
    Route(int distance, int destination);

    int GetDistance();
    int GetDestination();
private:
    int itsDistance;
    int itsDestination;
};


class FormOfTransport : public Route
{
public:
    FormOfTransport(int distance, int destination, MeansOfTransport mean, int cost);

    void ShowRoute(string start);
    int GetCost();
    MeansOfTransport GetMeansOfTransport();

private:
    MeansOfTransport itsMOT;
    int itsCost;

};



class City
{
public:
    City(string name);
    virtual ~City();

    void AddRoute(int distance, int cost, MeansOfTransport meanOfTransport, int where);

    string GetName();
    int GetNumber();
    void Show();

    static int GetNumberOfCities();

private:
    int itsNumber;
    string itsName;
    list<FormOfTransport> itsMOT[4];
    static int numberOfCities;
};

class TravelAgency
{
public:
    TravelAgency();
    ~TravelAgency();

    void AddCity(string city);
    void AddRoute(string start, string destination, int distance, int cost, MeansOfTransport meanOfTransport);
    void Show();

    City* FindByNum(int number);
    City* FindByName(string name);
    static TravelAgency* GetSingleton();
private:
    static TravelAgency* instance;
    list<City*> itsCities;
};

#ifndef TRANSPORT_TRANSPORT_H
#define TRANSPORT_TRANSPORT_H

#endif //TRANSPORT_TRANSPORT_H
