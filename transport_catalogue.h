#pragma once  
#include <iostream> 
#include <ostream> 
#include <string_view> 
#include <unordered_map> 
#include <unordered_set> 
#include <vector> 
#include <deque> 
#include "geo.h"
#include "input_reader.h"

namespace TransportGuide
{
class TransportCatalogue
{
    private:
        struct Hasher {
            size_t operator ()(const std::pair<Detail::Stop *, Detail::Stop *> & data) const {
                std::size_t h1 = std::hash<Detail::Stop *> {}(data.first);
                std::size_t h2 = std::hash<Detail::Stop *> {}(data.second);
                return h1 ^ (h2<<1);
            }
        };

        std::pair<Detail::Stop *, Detail::Stop *> distance;
        void AddPairStop (const std::string_view name_stop);

        std::deque<Detail::Bus> buses;
        std::deque<Detail::Stop> stops;
        std::vector<std::string> data_catalog;    // ввод данных

        std::vector<std::pair<std::string, std::string> > data_user_all;       // запрос пользователя

        std::unordered_map<std::string_view, Detail::Bus *> bus_map;    // быстрый доступ к Bus из deque
        std::unordered_map<std::string_view, Detail::Stop *> stops_map;

        std::unordered_map<std::string_view,  std::unordered_set<std:: string_view> > buses_on_stop;

    public:
        std::unordered_map<std::pair<Detail::Stop *, Detail::Stop *> , size_t, Hasher> distance_;

        TransportCatalogue (std::istream &); const std::deque<Detail::Bus> GetBus () const { return buses;}
        Detail::Bus * GetBusLink (const std::string_view busname){ return bus_map.at(busname);}
        Detail::Stop * GetStopLink (const std::string_view stopname){ return stops_map.at(stopname);}
        Detail::Bus GetBus (const std::string& busname) const;
        Detail::Stop GetStop (const std::string& stopname) const;

        double LengthBusStops (const std::string & bus); // остановки где останавливается автобус
        bool HasBus (const Detail::Bus&);
        bool HasStop (const Detail::Stop&);
        const std::vector<std::pair<std::string, std::string> > * GetDataUser () const { return &data_user_all;}
        std::unordered_set<std::string_view> BusesOnStop (const std::string_view &);
        size_t RouteBusDistance (const Detail::Bus& bus);
};

}

