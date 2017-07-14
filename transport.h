//
// Created by wojtek on 05.07.17.
//
#include <list>
#include <string>
#include <iostream>
#include <queue>

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
    list<FormOfTransport>* GetOneOfMeansOfTransport(MeansOfTransport i);
    void ShowByFormOfTransport(MeansOfTransport mot);
    void Show();
    //void FindTheCheapestPath(int destination);

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
    void FindTheCheapestPath(string start, string destination);

    City* FindByNum(int number);
    City* FindByName(string name);
    static TravelAgency* GetSingleton();
private:
    static TravelAgency* instance;
    list<City*> itsCities;
};

class Dijkstra
{
public:
    //Dijkstra(int number, int distance);
    int itsNumber;
    int itsDistance;


};

class Paths
{
public:
    int getItsDistance() const;
    void setItsDistance(int itsDistance);
    bool isItsVisited() const;
    void setItsVisited(bool itsVisited);
    int getItsPrevious() const;
    void setItsPrevious(int itsPrevious);

private:
    int itsDistance;
    bool itsVisited;
    int itsPrevious;
};

class Compare
{
public:
    bool operator() (Dijkstra *lhs, Dijkstra *rhs) const
    {
        return lhs -> itsDistance > rhs -> itsDistance;
    }
};

#ifndef TRANSPORT_TRANSPORT_H
#define TRANSPORT_TRANSPORT_H

#endif //TRANSPORT_TRANSPORT_H
