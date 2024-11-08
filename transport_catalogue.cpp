/* transport_catalogue.h, transport_catalogue.cpp — класс транспортного справочника; */
#include <iostream> 
#include <istream> 
#include <sstream> 
#include <cassert> 
#include <set> 
#include <algorithm> 
#include <string_view> 
#include <unordered_set> 
#include <utility> 
#include "transport_catalogue.h"
#include "input_reader.h"
#include "geo.h"

using namespace std; 

namespace TransportGuide
{
TransportCatalogue::TransportCatalogue (std::istream& input)
{
    data_catalog = Detail::GetDataTransport (input); // получение данныx

    for (auto data_user: Detail::GetUserAll (input))
    {
        string index = data_user.substr(0, data_user.find(' '));
        string name  = data_user.substr(data_user.find(' ')+1, data_user.size());
        data_user_all.push_back({ index,name});
    }

    // наполнение классами Bus и Stop
    for (string line : data_catalog)
    {
        string index = line.substr(0, line.find(' '));
        if (index == "Stop"){ stops.push_back(Detail::ParseStop (line));
                              stops_map [stops.back().name_stop] = & stops.back();
                            }
        if (index == "Bus"){ buses.push_back(Detail::ParseBus (line));  
                             bus_map[buses.back().name_bus] = &buses.back();
                           }
    }

    // создать ссылки на deque<Stop> т.е. bus будет содержать линки на все stop
    for (auto& bus : buses)
    {
        for (const string &stop_bus: bus.stops)// остановки где автобус останавливается
        {
           auto it = find_if (stops.begin(), stops.end(),[stop_bus](const Detail::Stop &stop)
                   { return stop.name_stop == stop_bus;}); 
           if (it != stops.end())
           {
                bus.stops_link [stop_bus] = &(* it);
                AddPairStop (it-> name_stop);
           }
        }
    }

    // какие автобусы есть на остановке
    for (const auto& bus_pair : bus_map){
        for (auto stopname: bus_pair.second-> stops_link)
        {
            buses_on_stop[stopname.first].insert(bus_pair.first);
        }
    }

}// TransportCatalogue (input)


double TransportCatalogue::LengthBusStops (const string& bus)
{
    double result = 0.0;
    // vector stops т.к. остановки могут повторяться
    for (size_t i = 1 ; i<bus_map.at(bus)-> stops.size(); i++)
    {
        string stop_name1 = bus_map.at(bus)-> stops.at(i-1);
        string stop_name2 = bus_map.at(bus)-> stops.at(i);

result += ComputeDistance (stops_map.at(stop_name1)-> coordinates
                          , stops_map.at(stop_name2)-> coordinates);
    }
    return result;
}


bool TransportCatalogue::HasBus (const Detail::Bus& bus)
{
    return find_if (data_user_all.begin(), data_user_all.end(),
           [bus](pair<string,string> name)
           {
                return (name.first == "Bus" && name.second == bus.name_bus);
               
           })!= data_user_all.end();
}


bool TransportCatalogue::HasStop (const Detail::Stop& stop)
{
    return find_if (data_user_all.begin(), data_user_all.end(),
           [stop](pair<string,string> name)
           {
                return (name.first == "Bus" && name.second == stop.name_stop);
               
           })!= data_user_all.end();
}


Detail::Stop TransportCatalogue::GetStop (const std::string& stopname) const
{
   if (stops_map.find(stopname)!= stops_map.end())
   {
       return * stops_map.at(stopname);
   } 
   return {};
}


Detail::Bus TransportCatalogue::GetBus (const std::string& busname) const 
{
    try
    {
        return * bus_map.at(busname); 
    }
    catch (...)
    {
       // автобуса нет в списке данных 
    }
    return {};
}


std::unordered_set<std::string_view> TransportCatalogue::BusesOnStop (const std::string_view &name)
{

    if (buses_on_stop.find(name)!= buses_on_stop.end())
    {
        return buses_on_stop.at(name);
    }
    return {};
}


void TransportCatalogue::AddPairStop (const std::string_view name_stop)
{
    for (auto i: GetStopLink (name_stop)-> distance_stops)
    {
        std::pair<Detail::Stop *, Detail::Stop *> tmp { GetStopLink (name_stop),
            GetStopLink (i.first)};
        distance_[tmp]= i.second;
    }
}


size_t TransportCatalogue::RouteBusDistance (const Detail::Bus& bus)
{
    size_t totalDistance = 0;
    if (bus.stops.size()<=0)
    {
        return 0;
    }

    for (size_t i = 0; i<bus.stops.size()- 1; i++)
    {
        Detail::Stop * stop1 = GetStopLink (bus.stops[i]);
        Detail::Stop * stop2 = GetStopLink (bus.stops[i+1]);

        std::pair<Detail::Stop *, Detail::Stop *> stopsPair = std::make_pair (stop1, stop2);
        if (distance_.find (stopsPair) != distance_.end())
        {
            totalDistance += distance_.at(stopsPair);
        }
        else
        {
            stopsPair = std::make_pair (stop2, stop1);
            totalDistance += distance_.at(stopsPair);
        }
    }
    return totalDistance;
}
        

}// namespace TransportGuide

