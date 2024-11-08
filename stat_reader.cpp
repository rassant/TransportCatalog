#include <algorithm> 
#include <array> 
#include <exception> 
#include <iomanip> 
#include <iostream> 
#include <ostream> 
#include <string_view> 
#include <unordered_map> 
#include <unordered_set> 
#include <vector> 
#include <deque> 
#include "transport_catalogue.h"
#include "stat_reader.h"

using namespace std; 

void Detail::PrintInfo (TransportGuide::TransportCatalogue &TC, std::ostream &outputStream)
{
    for (auto [index, name]: *TC.GetDataUser())
    {
        if (index == "Bus")
        {
            Detail::PrintInfoBus (TC, name, outputStream);
        }
        if (index == "Stop")
        {
            Detail::PrintInfoStop (TC, name, outputStream);
        }
    }
}


void Detail::InfoBusOutput (const Detail::Bus &bus, std::ostream &outputStream)
{
    outputStream<<"Bus "<<bus.name_bus<<": "
        <<bus.stops.size()<<" stops on route, "
        <<bus.stops_link.size()<<" unique stops";
}


void Detail::InfoDistanceStop (TransportGuide::TransportCatalogue &TC, const Bus& bus, std::ostream &outputStream)
{
    size_t distance = TC.RouteBusDistance(bus);

    outputStream<<", "<<distance<<" route length, "
        <<fixed<<std::setprecision (6)<< static_cast<double>(distance )/ static_cast<double>(TC.LengthBusStops(bus.name_bus))
        <<" curvature"
        <<endl;
}


void Detail::PrintInfoBus (TransportGuide::TransportCatalogue &TC, const string& name, std::ostream &outputStream)
{
    Detail::Bus bus = TC.GetBus(name);
    if (TC.HasBus(bus))
    {
        InfoBusOutput (bus, outputStream);
        InfoDistanceStop (TC, bus, outputStream);
    }
    else
    {
        outputStream<<"Bus "<<name<<": not found"<<endl;
    }
}


void Detail::InfoBusOnStopOutput (TransportGuide::TransportCatalogue &TC, const std::string &name, std::ostream &outputStream)
{
    try
    {
        // вывести список автобусов
        std::unordered_set<std::string_view> buse = TC.BusesOnStop(name);
        std::vector<std::string> sorted_buses (buse.begin(), buse.end());
        std::sort (sorted_buses.begin(), sorted_buses.end(), [](const auto& a, const auto& b){
                return a<b;
                });
        if (sorted_buses.size()> 0)
        {
            outputStream<<"buses";
            for (auto bus_name: sorted_buses)
            {
                outputStream<<" "<<bus_name;
            } outputStream<<endl;
        }
        if (TC.GetStop(name).name_stop == name  && sorted_buses.size() == 0)
        { 
            outputStream<<"no buses"<<endl;
        }
    } catch (std::exception & ex)
    {
    }
}


void Detail::PrintInfoStop (TransportGuide::TransportCatalogue &TC, const std::string &name, std::ostream &outputStream)
{
    outputStream<<"Stop "<<name<<": ";

    Detail::Stop stop = TC.GetStop(name);
    if (stop.name_stop != name)
    {
        outputStream<<"not found"<<endl;
    }
    InfoBusOnStopOutput (TC, name, outputStream);
}
