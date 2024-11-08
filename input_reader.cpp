#include <cstdlib> // stoi
#include <iomanip> 
#include <algorithm>
#include <iostream> 
#include <algorithm> 
#include <limits>
#include <sstream> 
#include <string> 
#include <cstdlib> 
#include <sstream> 
#include <vector> 
#include "geo.h"
#include "input_reader.h"

using namespace std; 

namespace Detail
{

vector<string> GetDataTransport (std::istream &input)
{
    string line ;
    getline (input,line);
    // данные
    istringstream str (line);
    size_t data_line;
    str >> data_line;

    vector<string> base_data;
    base_data.reserve(data_line);

    while (data_line -- && getline (input, line))
    {
        base_data.push_back(line); 
    } 
    return  base_data;
}


vector<string> GetUserAll (std::istream &input)
{
    // users ввод
    string line;
    getline (input,line);

    istringstream str (line);
    size_t users_line;
    str >> users_line;

    vector<string> users_data;
    users_data.reserve(users_line);

    while (users_line -- && getline (input, line))
    {
        users_data.push_back(line);
    }
    return users_data;
}


Stop ParseStop (const std::string &line)
{
    string index = line.substr(0, line.find(' '));
    if (index != "Stop")
    {
        return{};
    }

    Stop stop;
    stop.name_stop = line.substr(line.find(' ')+1, line.find(':')- line.find(' ')-1);
    stop.coordinates.lat = std::stod (line.substr(line.find(':')+2, line.find_first_of (',')- line.find(':')-2));
    stop.coordinates.lng = std::stod (line.substr(line.find_first_of (',')+ 2, line.size()- line.find_first_of (',')-2));

    // наполнение: Текущая остановка1 pair<остановка2, расстояние до нее> 
    string distance = line.substr(line.find(',')+ 2, line.size());
    if (distance.find(' ')!=string::npos)
    {
        while (distance.size()> 0)
        {
            distance.erase(0, distance.find(',') + 2);
            int distance_to_stop = stoi (distance.substr(0, distance.find('m')));
            distance.erase(0, distance.find("to ") + 3);
            string name = distance.substr(0, distance.find(','));
            distance.erase(0, distance.find(','));

            stop.distance_stops.push_back ({name,distance_to_stop});
        }
    } 
    return stop;
}


Bus ParseBus (const std::string& line)
{
    string index = line.substr(0, line.find(' '));
    if (index != "Bus")
    {
        return{};
    }

    Bus bus;
    bus.name_bus = line.substr(line.find(' ')+1, line.find(':')- line.find(' ')-1);

    int stops_count = static_cast<int>(count (line.begin(), line.end(), '>'));
    if (stops_count> 0)
    {
        string stops = line;
        bus.stops.reserve(static_cast<size_t>(++stops_count));

        stops.erase(0, stops.find(':')+2);

        while (stops_count--)
        {
            bus.stops.push_back(stops.substr(0, stops.find('>')-1));
            stops.erase(0,stops.find('>')+2);
        }
    }

    int stops_count_reverse = static_cast<int>(count (line.begin(), line.end(), '-') )* 2 ;
    if (stops_count_reverse++> 0)
    {
        bus.stops.reserve(static_cast<size_t>(stops_count_reverse));

        string stops_to = line;
        stops_to.erase(0, stops_to.find(':')+2);
        string stops_back = stops_to;

        int stops_count_reverse_back = stops_count_reverse / 2 ;
        stops_count_reverse = stops_count_reverse_back + 1;

        while (stops_count_reverse--)
        {
            bus.stops.push_back(stops_to.substr(stops_to.find_first_not_of (' '), stops_to.find('-')-1));
            stops_to.erase(0,stops_to.find('-')+ 2);
        }


        stops_back.erase(stops_back.find_last_of ('-')-1, stops_back.size());
        while (-- stops_count_reverse_back)
        {
            bus.stops.push_back(stops_back.substr(stops_back.find_last_of ('-')+2, stops_back.size()- stops_back.find_last_of ('-')- 2));
            stops_back.erase(stops_back.find_last_of ('-')-1, stops_back.size()- stops_back.find_last_of ('-')+1);
        }
        bus.stops.push_back(stops_back.substr(stops_back.find_last_of (':')+1, stops_back.size()- stops_back.find_last_of ('-')- 1));
    }
    return bus;    
}


std::ostream &operator<<(std::ostream & out, const Bus& bus)
{
    out<<bus.stops.size()<<":";
    out<<"["<<bus.name_bus<<"] |";

    for (auto i: bus.stops)
    {
        out<<i<<'|' ;
    }
    return out;
}


std::ostream &operator<<(std::ostream & out, const Stop& stop)
{
    out<<"["<<stop.name_stop<<"] ";
    out<<"{"<<fixed<<std::setprecision (6)
            <<stop.coordinates.lat<<", "
            <<stop.coordinates.lng<<"}";
    return out; 
}


// сравнивает только размеры но не содержимое.
bool Bus::operator == (const Bus & bus)
{
    if (bus.name_bus == name_bus
            && stops.size() == bus.stops.size()
            && bus.stops_link.size() == stops_link.size())
    {
        {
            return true;
        }
    }
    return false;        
}


bool Bus::operator != (const Bus &bus)
{
    return !(* this == bus);
}


bool Stop::operator == (const Stop& stop)
{
  const auto epsilon = std::numeric_limits<double>::epsilon();
    if (name_stop == stop.name_stop &&
        fabs (coordinates.lat - stop.coordinates.lat ) < epsilon && 
        fabs (coordinates.lng - stop.coordinates.lng ) < epsilon )

            /*coordinates.lat == stop.coordinates.lat &&*/
            /*coordinates.lng == stop.coordinates.lng)*/
    {
        return true;
    }
    return false;
}
}

