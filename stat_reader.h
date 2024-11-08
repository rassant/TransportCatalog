#pragma once
#include <string_view> 
#include "transport_catalogue.h"


namespace Detail
{
/* Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length */
/* Bus 751: not found */
void PrintInfoBus(TransportGuide::TransportCatalogue &TC, const std::string &, std::ostream &outputStream);
/* void PrintInfoBus(TransportCatalogue &TC); */

/* Stop Samara: not found */
/* Stop Prazhskaya: no buses */
/* Stop Biryulyovo Zapadnoye: buses 256 828 */
void PrintInfoStop(TransportGuide::TransportCatalogue &TC, const std::string &, std::ostream &outputStream);

        
// выдача информации в очередности ввода запроса пользователя.
// остановки и автобуса
// запросы могут чередоваться
void PrintInfo (TransportGuide::TransportCatalogue &TC, std::ostream &outputStream);

void InfoBusOutput  ( const Detail::Bus &bus, std::ostream &outputStream);
void InfoDistanceStop (TransportGuide::TransportCatalogue &TC, const Bus&, std::ostream &outputStream);
void InfoBusOnStopOutput (TransportGuide::TransportCatalogue &TC, const std::string&, std::ostream &outputStream);
}
