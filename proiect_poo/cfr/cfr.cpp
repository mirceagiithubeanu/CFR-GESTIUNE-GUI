#include "cfr.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>

//exceptiile mele
//DE ADAUGAT: exceptie pentru tipuri gresite de date

class OptionError : public std::runtime_error {
    int option;
    
    public:
        OptionError(int opt) : std::runtime_error("Optiune invalida: " + std::to_string(opt)) {}
};

class ObjectError : public std::runtime_error {
    std::string message;

    public:
        ObjectError(std::string object) : std::runtime_error(object + " nu exista!") {}
};

class ValueError : public std::runtime_error {
    std::string message;

    public:
        ValueError(std::string value) : std::runtime_error(value + " nu este valid/a in acest context.") {}
        ValueError(int value) : std::runtime_error(std::to_string(value) + " nu este valid/a in acest context.") {}
};

class MiscError : public std::runtime_error {
    std::string message;
    public:
        MiscError(std::string value) : std::runtime_error(value) {}
};

TicketFactory* getFactory(int type) {
    switch(type) {
        case 0:
            return new FullPriceFactory();
        case 1:
            return new FullPriceFactory();
        case 2:
            return new FullPriceFactory();
        case 3:
            return new FullPriceFactory();
    }
    return nullptr;
}

std::string clockConvert(int time) {
    std::string output = "";
    int hours = (time / 60) % 24;
    int minutes = time % 60;

    std::string h = std::to_string(hours), m = std::to_string(minutes);
    if(hours < 10)
        h = "0" + h;
    if(minutes < 10)
        m = "0" + m;
    
    return h + ":" + m;
} // REVEZI: std::format

std::string toLower(std::string orig) {
    for (char &c : orig) 
        c = std::tolower((unsigned char)c);
    
    return orig;
}

// CLASA DE BAZA CAR
Car::Car(int _capacity, double _price) : 
    capacity(_capacity), 
    seat_price(_price) {
        passengers.resize(_capacity, -1);
}

void Car::print(std::ostream& out) const {
    out << "Vagon(locuri: " << capacity << ", pret: " << seat_price << ")\n";
}

std::ostream& operator<<(std::ostream& out, const Car& c) {
    c.print(out);
    return out;
}

// UNIVERSALE
void Car::printAllPassengers() {
    std::cout << "\n(USA)\n";
    int i = 0;
    for(const int& elem : passengers)
        std::cout << i++ + 1 << ": " << elem << "\n";

    std::cout << "[USA]\n";
}

int Car::occupyEmptySeat(int id) {
    for(int seat : passengers) {
        if(seat == -1) {
            seat = id;
            return seat;
        }
    }
    
    return -1; //nu exista loc gol     
}

//CLASA CUSETA COUCHETCAR---------------------------------------------------------------------------------

CouchetCar::CouchetCar(int _capacity, double _price, int _bed, double _bedsheet) : 
            Car(_capacity, _price),
            bed_nr(_bed),
            bedsheet_price(_bedsheet) {}

CouchetCar::CouchetCar(const CouchetCar& other) : 
            Car(other), 
            bed_nr(other.bed_nr),
            bedsheet_price(other.bedsheet_price) {}

CouchetCar& CouchetCar::operator=(const CouchetCar& other) {
    if (this == &other)
        return *this;

    capacity = other.capacity;
    seat_price = other.seat_price;

    //Car::operator=(other);
    bed_nr = other.bed_nr;
    bedsheet_price = other.bedsheet_price;

    return *this;
}
void CouchetCar::print(std::ostream& out) const {
    Car::print(out);
    out << "Cuseta(numar paturi: " << bed_nr << ", pret lenjerie: " << bedsheet_price << ")\n";
}

double CouchetCar::getPrice() const {
    return seat_price + bedsheet_price;
}

std::string CouchetCar::type() {
    return "Vagon Cuseta";
}


//------------------------------------------------STANDARD CAR---------------------------------------------
StandardCar::StandardCar(int _capacity, double _price, int _class, int _bike) :
        Car(_capacity, _price),
        car_class(_class),
        bike_seats(_bike) {}

StandardCar::StandardCar(const StandardCar& other) :
        Car(other),
        car_class(other.car_class),
        bike_seats(other.bike_seats) {}

StandardCar& StandardCar::operator=(const StandardCar& other) {
    if(&other == this) 
        return *this;
    
    Car::operator=(other);
    car_class = other.car_class;
    bike_seats = other.bike_seats;

    return *this;
}

double StandardCar::getPrice() const {
    return seat_price + (2 - car_class) * 50 / 100 * seat_price;
}

std::string StandardCar::type() {
    if(car_class == 1)
        return "Vagon Clasa Intai";
    else return "Vagon Clasa a Doua";
}

//-----------------------------------------------------------------------------------------------------
//--------------------------------------------------TREN----------------------------------------------
//---------------------------------------------------------------------------------------------------

Train::Train(int _dep, int _time) : departure_time(_dep), time_to_station(_time) {
    cars_nr = 0;
    //std::cout << "train initialized\n";
}

Train::~Train() {
    for(Car* traincar : traincars)
        delete traincar;

    std::cout << "train emptied and erased\n";
}

Train& Train::operator=(const Train& other) {
    if(&other == this)
        return *this;
    
    departure_time = other.departure_time;
    time_to_station = other.time_to_station;
    cars_nr = other.cars_nr;
    traincars = other.traincars;

    return *this;
}

bool Train::operator<(const Train& other) {
    return departure_time < other.departure_time;
}

void Train::pushCar(Car *cart) {
    traincars.push_back(cart);
    cars_nr++;
}

void Train::removeCars() {
    for(Car* traincar : traincars)
        delete traincar;
    traincars.clear();
    cars_nr = 0;
}

void Train::printTraincars() {
    int i = 0;
    for(Car* car : traincars)
        std::cout << ++i << ": " << car->type() << "\n";
}

void Train::mapCars() {
    for(auto it : traincars) {
        if(std::count(carOptions.begin(), carOptions.end(), it->type()) == 0)
            carOptions.push_back(it->type());
    }
}
void Train::printOptions() {
    int i = 0;
    for(auto it : carOptions) {
        std::cout << ++i << ": " << it << "\n";
    }
}

std::string Train::getCarOption(int option) {
    if(option < 0 || option >= carOptions.size())
        return "0";
    
    return carOptions[option];
}
std::vector<std::string> Train::getOptions() {
    std::vector<std::string> opt{};
    for(auto& it : carOptions) {
        opt.push_back(it);
    }

    return opt;
}

carInfo Train::occupyEmptySeat(std::string type, int id) {
    int i = 0;
    int seat = 0;
    
    carInfo info;

    for(auto& car : traincars) {
        if(car->type() != type)
            continue;

        seat = car->occupyEmptySeat(id);
        if(seat != -1) {
            info.seat_number = i + 1;
            info.car_number = seat;
        }

        i++;
    }

    return info;
}

bool Train::isFull() {
    int seat = 0;
    for(auto car : traincars) {
        seat = car->occupyEmptySeat(0); //REVEZI: SA NU EXISTE ID 0
        if(seat != -1)
            return false;
    }

    return true;
}

double Train::getPrice(std::string type) const {
    for(auto car : traincars) {
        if(car->type() == type)
            return car->getPrice();
    }

    return 999999999; //nu se va intampla vreodata
}



//--------------------------------------------------------------------------------------------------------
//-------------------------------------------------RUTA--------------------------------------------------
//------------------------------------------------------------------------------------------------------

Route::Route(const Route& other) {
    stations = other.stations;
    trains = other.trains;
}

Route::~Route() {
    for(auto train : trains)
        delete train;
}

void Route::pushStation(std::string station) {
    stations.push_back(station);
}

void Route::pushTrain(Train* train) {
    trains.push_back(train);   
}
void Route::deleteTrainPtr(int trainIndex) {
    delete trains[trainIndex];
}
void Route::removeTrain(int trainIndex) {
    deleteTrainPtr(trainIndex);
    trains.erase(trains.begin() + trainIndex);
}

std::string Route::getRouteName() const {
    return stations.front() + " - " + stations.back() + " via: " + stations[(stations.size() - 1) / 2];
}

std::string Route::getRouteNameReversed() const {
    return stations.back()  + " - " + stations.front() + " via: " + stations[stations.size() / 2];
}

//  REVEZI: renumeste la findStation sau cv de genul, ca e mai intuitiv
int Route::hasStation(const std::string& station) const {
    int index = 0;

    for(auto& it : stations) {
        if(it == station) return index;
        index++;
    }
    return -1;
}

// aceasta functie a unei rute primeste un timp de plecare tstart, si returneaza indexul primului tren care
// pleaca dupa tstart, folosind cautare binara
int Route::getCompatibleDepTime(int tstart, int index, int sense) {
    if(trains.empty())
        return -1;
    
    if(!isSorted)
        sortTrains();

    int dep = trains[0]->getDeparture();

    // CAUTARE BINARA PE TRENURI
    int start, end, mid;
    start = 0; end = trains.size();

    // INDEXUL ESTE PENTRU STATIA UNDE VREI SA AJUNGI
    // DAR TU VREI INDEXUL PENTRU STATIA DE UNDE PLECI
    // FOLOSESTI SENSE CA SA VEZI DACA TE DUCI IN SUS SAU IN JOS
    // daca sensul e -1(invers), inversezi indexul
    int id = index - sense;
    if(sense == -1)
        id = stations.size() - 1 - id;

    if(tstart > trains[end - 1]->getDeparture() + trains[end - 1]->getSpeed() * id)
        return -1;

    while(start < end) {
        mid = start + (end - start) / 2;

        dep = trains[mid]->getDeparture() + trains[mid]->getSpeed() * id;
        if(dep == tstart)
            return mid;

        if(dep < tstart)
            start = mid + 1;
        else
            end = mid; // pentru ca mid ar putea sa fie upper bound; altfel doar ar trece peste el
    }
    
    return start;
}

void Route::sortTrains() {
    std::sort(trains.begin(), trains.end(), [](Train* &t1, Train* &t2) {
        return *t1 < *t2;
    });
    isSorted = true;
}


//------------------------------------------------------------------------------------------------------
//-------------------------------------------BILET-----------------------------------------------------
//----------------------------------------------------------------------------------------------------
int Trip::trip_counter = 0;

Trip::Trip(std::string dep, std::string arr, std::string routeName, std::string start, std::string end, int seat, int car, std::string type, double _price) :
    departure_time(dep), arrival_time(arr), routeName(routeName), start(start), end(end), seat_nr(seat), car_nr(car),
    car_type(type), price(_price)  {
        trip_id = ++trip_counter;
    }

void Trip::printInfo(std::ostream& os) const {
    os << departure_time << "-" << arrival_time <<" " << start << "-" << end << " (" << routeName << ")\n";
    os << "\t" << car_type << " / Numar vagon: " << car_nr << " / Loc: " << seat_nr << "\n"; 
}

int Ticket::ticket_counter = 0;
int Ticket::ticketCount() {
    return ticket_counter;
}

Ticket::~Ticket() {
    for(auto trip : trips)
        delete trip;
}

Ticket::Ticket() {
    ticket_counter++;
    ticket_id = ticket_counter;
}

Ticket::Ticket(std::string _name) : 
        name(_name) {
            ticket_counter++;
            ticket_id = ticket_counter;
        }

void Ticket::setTravel() {
    if(trips.size() == 0)
        return;

    start = trips.at(0)->getDep();
    destination = trips.at(trips.size() - 1)->getArr();
}
void Ticket::appendTrip(Trip* trip) {
    trips.push_back(trip);
}
void Ticket::appendTrip(std::string dep, std::string arr, std::string routeName, std::string start, std::string end, int seat, int car, std::string type, double _price) {
    Trip* trip = new Trip(dep, arr, routeName, start, end, seat, car, type, _price);
    trips.push_back(trip);
}

void Ticket::printTrips(std::ostream& os) const {
    for(auto trip : trips) {
        trip->printInfo(os);
    }
    os << "\n";
}

void Ticket::setPrice() {
    double price = 0;
    for(auto it : trips)
        price+=it->getPrice();

    this->price = getFinalPrice(price);
}

FullPriceTicket::FullPriceTicket(std::string _name, std::string _id) :
        Ticket(_name), id_card(_id) {}
double FullPriceTicket::getFinalPrice(double initial) {
    return initial;
}
void FullPriceTicket::printInfo(std::ostream& os) const{
    os << "Nume: "<< name << "\nID: " << id_card << "\nPret: " << price << " lei\n\n";
    printTrips();
}

YouthTicket::YouthTicket(std::string _name, std::string _id) :
        Ticket(_name), student_id(_id) {
            this->discount = 90;
        }
double YouthTicket::getFinalPrice(double initial) {
    return (100 - discount) * initial / 100;
}
void YouthTicket::printInfo(std::ostream& os) const{
    os << "Nume: "<< name << "\nID student/elev: " << student_id << "\nPret: " << price <<  " lei\n\n";
    printTrips();
}

ElderlyTicket::ElderlyTicket(std::string _name, std::string _id) :
        Ticket(_name), health_card(_id) {
            discount = 70;
        }
double ElderlyTicket::getFinalPrice(double initial) {
    return (100 - discount) * initial / 100;
}
void ElderlyTicket::printInfo(std::ostream& os) const {
    os << "Nume: "<< name << "\nID: " << health_card << "\nPret: " << price << " lei\n\n";
    printTrips();
}

MembershipTicket::MembershipTicket(std::string _name, std::string _id, std::string _account) :
        Ticket(_name), 
        FullPriceTicket(_name, _id),
        account_name(_account) {
            this->discount = 30;
        }
double MembershipTicket::getFinalPrice(double initial) {
    return (100 - discount) * initial / 100;
}
void MembershipTicket::printInfo(std::ostream& os) const{
    os << "\nNume: "<< name << "\nID: " << id_card << "\nNUME UTILIZATOR: " << account_name << "\nPret: " << price << " lei\n\n";
    printTrips();
}

//-------------------------------------------------------------------------------------------------------
//------------------------------------ROUTE TABLE--------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
RouteTable::~RouteTable() {
    for(auto it : routes)
        delete it;
}

void RouteTable::appendRoute(Route* route) {
    routes.push_back(route);
}

void RouteTable::printRoutes() {
    int i = 0;
    for(auto& route : routes)
        std::cout << ++i << ": " << route->getRouteName() << "\n";
}

Route* RouteTable::findRoute(std::string name) {
    for(auto& route : routes)
        if(route->getRouteName() == name || route->getRouteNameReversed() == name)
            return route;
    
    return nullptr;
}

void RouteTable::deleteRoute(Route* ptr) {
    auto pos = std::find(routes.begin(), routes.end(), ptr);
    if(pos == routes.end())
        return;
    
    routes.erase(pos);
    delete ptr;
}

//-----------------------------------------------------------------------------------------------
//--------------------------------------------MENIUL-----------------------------------------------
//--------------------------------------------------------------------------------------------------

TicketManager* TicketManager::instance = nullptr;

TicketManager::TicketManager() {
    graph = new RouteGraph();
    table = new RouteTable();
}

Train* TicketManager::highSpeedTrain() {
    int dep_time;                                        
    std::cout << "Introduceti ora plecarii: ";
    std::cin >> dep_time;
    if(dep_time < 0 || dep_time >= 1440)
        throw ValueError(std::to_string(dep_time));
    
    std::cout << "\n";

    Train* hst = new Train(dep_time, 20);

    Car* auxcar;
    for(int i = 0; i < 6; i++) {
        auxcar = new StandardCar(50, 30, 2, 6);
        hst->pushCar(auxcar);
    }

    for(int i = 0; i < 6; i++) {
        auxcar = new StandardCar(50, 50, 1, 10);
        hst->pushCar(auxcar);
    }

    if(dep_time > 1230) {
        for(int i = 0; i < 4; i++) {
            auxcar = new CouchetCar(30, 40, 6, 15);
            hst->pushCar(auxcar);
        }
    }

    hst->mapCars();
    return hst;
}
Train* TicketManager::newHST(int dep_time) {
    // nu o sa se intample pentru ca primeste valoare din TIMEEDIT
    if(dep_time < 0 || dep_time >= 1440)
        throw ValueError(std::to_string(dep_time));
    
    //std::cout << "\n";

    Train* hst = new Train(dep_time, 20);

    Car* auxcar;
    for(int i = 0; i < 6; i++) {
        auxcar = new StandardCar(50, 30, 2, 6);
        hst->pushCar(auxcar);
    }

    for(int i = 0; i < 6; i++) {
        auxcar = new StandardCar(50, 50, 1, 10);
        hst->pushCar(auxcar);
    }

    if(dep_time > 1230) {
        for(int i = 0; i < 4; i++) {
            auxcar = new CouchetCar(30, 40, 6, 15);
            hst->pushCar(auxcar);
        }
    }

    hst->mapCars();
    return hst;
}


Train* TicketManager::regionalTrain() {
    int dep_time;                                  
    std::cout << "Introduceti ora plecarii: ";
    std::cin >> dep_time;
    if(dep_time < 0 || dep_time >= 1440)
        throw ValueError(std::to_string(dep_time));
    std::cout << "\n";

    Train* rt = new Train(dep_time, 30);

    Car* auxcar;
    for(int i = 0; i < 4; i++) {
        auxcar = new StandardCar(40, 25, 2, 4);
        rt->pushCar(auxcar);
    }

    for(int i = 0; i < 4; i++) {
        auxcar = new StandardCar(40, 45, 1, 8);
        rt->pushCar(auxcar);
    }

    if(dep_time > 1230) {
        for(int i = 0; i < 3; i++) {
            auxcar = new CouchetCar(20, 40, 4, 10);
            rt->pushCar(auxcar);
        }
    }

    rt->mapCars();
    return rt;
}
Train* TicketManager::newRT(int dep_time) {
    // nici asta nu se va intampla
    if(dep_time < 0 || dep_time >= 1440)
        throw ValueError(std::to_string(dep_time));
    std::cout << "\n";

    Train* rt = new Train(dep_time, 30);

    Car* auxcar;
    for(int i = 0; i < 4; i++) {
        auxcar = new StandardCar(40, 25, 2, 4);
        rt->pushCar(auxcar);
    }

    for(int i = 0; i < 4; i++) {
        auxcar = new StandardCar(40, 45, 1, 8);
        rt->pushCar(auxcar);
    }

    if(dep_time > 1230) {
        for(int i = 0; i < 3; i++) {
            auxcar = new CouchetCar(20, 40, 4, 10);
            rt->pushCar(auxcar);
        }
    }

    rt->mapCars();
    return rt;
}

Car* TicketManager::manualAddCar() {
    int capacity;
    double price;
    std::cout << "Introduceti capacitatea vagonului: ";
    std::cin >> capacity;
    if(capacity <= 0 || capacity > 200)
        throw ValueError(std::to_string(capacity));

    std::cout << "Introduceti pretul per loc: ";
    std::cin >> price;
    if(price <= 0)
        throw ValueError(std::to_string(price));
        
    // 1 - CouchetCar, 2 - StandardCar
    std::cout << "Ce fel de vagon doriti?\n1 = Vagon cuseta\n2 = Vagon Standard\n>";
    int type;
    std::cin >> type;

    Car* car;

    switch(type) 
    {
        case 2:
            int car_class, bike_seats;
            std::cout << "Introduceti clasa vagonului: ";
            std::cin >> car_class;
            if(car_class > 2 || car_class < 1)
                throw ValueError(std::to_string(car_class));

            std::cout << "Introduceti numarul de locuri pentru biciclete: ";
            std::cin >> bike_seats;
            if(bike_seats <= 0)
                throw ValueError(std::to_string(bike_seats));

            car = new StandardCar(capacity, price, car_class, bike_seats);
            break;
        case 1:
            int bed_count;
            std::cout << "Introduceti numarul de paturi per compartiment: ";
            std::cin >> bed_count;
            if(bed_count <= 0)
                throw ValueError(std::to_string(bed_count));

            double sheet_price;
            std::cout << "Introduceti pretul pentru lenjerie: ";
            std::cin >> sheet_price;
            if(sheet_price <= 0)
                throw ValueError(std::to_string(sheet_price));

            car = new CouchetCar(capacity, price, bed_count, sheet_price);
            break;
        default:
            throw OptionError(type);
    }
    return car;
}

Train* TicketManager::manualAddTrain() {
    std::cout << "---- INTRODUCEREA MANUALA A UNUI TREN IN BAZA DE DATE ----\n";
    
    std::cout << "Introduceti timpul de plecare: ";
    int departure_time;
    std::cin >> departure_time;
    if(departure_time < 0 || departure_time >= 1440)
        throw ValueError(std::to_string(departure_time));

    std::cout << "\nIntroduceti timpul mediu petrecut intre statii: ";
    int avg_time;
    std::cin >> avg_time;
    if(avg_time < 5 || avg_time >= 1440)
        throw ValueError(std::to_string(avg_time));

    Train* tren = new Train(departure_time, avg_time);

    //vagoanele
    std::cout << "\nIntroduceti numarul de vagoane: ";
    int car_count;
    std::cin >> car_count;
    if(car_count < 1 || car_count >= 200)
        throw ValueError(std::to_string(car_count));

    bool tryAgain = true;
    Car* aux;
    for(int i = 0; i < car_count; i++) {

        while(tryAgain) {
            try {
                aux = manualAddCar();
                tryAgain = false;   
            }
            catch(const OptionError &e){
                std::cout << "Exceptie: " << e.what() << "\n";
                tryAgain = true;
            }
            catch(const ValueError &e1) {
                std::cout << "Exceptie: " << e1.what() << "\n";
                tryAgain = true;
            }
        }

        tren->pushCar(aux);
    }

    tren->mapCars();
    return tren;
}

void TicketManager::populateTable() {
    initialized = true;

    Route* routepoint = nullptr;
    
    int num = 0;
    std::string statie;
    std::cout << "Introduceti numarul de rute de adaugat: ";
    std::cin >> num;
    if(num < 1) 
            throw ValueError(num);

    std::cout << "Introduceti statiile. \nDupa ultima statie din ruta, introduceti '/'.\n";
    std::cout << "Exemplu: Bucuresti Titu Targoviste /";

    for(int i = 0; i < num; i++) {
        std::cout << "\nruta " + std::to_string(i + 1) + ":\n";
        routepoint = new Route();

        //1.
        //se dau statiile

        std::string s, token;
        //std::cin >> s;
        std::cin.get();
        std::getline(std::cin, s);
        s+=' ';

        while(s.find(" ") != std::string::npos) {
            int pos = s.find(" ");

            token = s.substr(0, pos); // statia
            
            if(token != "")
                routepoint->pushStation(token);

            s.erase(0, s.find(" ") + 1);
        }
        
        if(table->findRoute(routepoint->getRouteName())) {
            std::cout << "EROARE: RUTA EXISTA DEJA";
            return; 
        }
        //1.5
        //se adauga ruta in graf
        graph->addRoute(*routepoint);

        //2.
        //se dau trenurile -> manual sau cu presetari
        int train_count;
        std::cout << "Introduceti numarul de trenuri dorite: ";
        std::cin >> train_count;

        if(train_count < 1) 
            throw ValueError(train_count);

        std::cout << "Ce tip de tren doriti sa creati?\n";

        int ttype = 0;
        for(int i = 0; i < train_count; i++) {
            bool tryAgain = true;

            while(tryAgain) //AICI SUNT PRINSE TOATE ERORILE DE LA CREAREA TRENULUI
            try {
                    std::cout << "Trenul " << std::to_string(i + 1) << ":\n";
                    std::cout << "\n1 - Tren de Inalta Viteza \t(Presetare - Va fi creat automat)\n"\
                    << "2 - Tren Regional \t\t(Presetare - Va fi creat automat)\n"\
                    << "3 - Introducere Manuala\n>";
                    std::cin >> ttype;

                    switch (ttype) {
                    case 1:
                        //tren preset
                        routepoint->pushTrain(highSpeedTrain());
                        tryAgain = false;
                        break;
                    case 2:
                        routepoint->pushTrain(regionalTrain());
                        tryAgain = false;
                        break;
                    case 3:
                        routepoint->pushTrain(manualAddTrain());
                        tryAgain = false;
                        break;
                    default:
                        throw OptionError(ttype); 
                }
            }
            catch(const OptionError &e) {
                std::cout << "Exceptie: " << e.what() << "\n";
                tryAgain = true;
            }
            catch(const ValueError &e1) {
                std::cout << "Exceptie: " << e1.what() << "\n";
                tryAgain = true;
            }
            



        }
        routepoint->sortTrains();
        //3.
        //a terminat de adaugat trenuri; adaugi ruta in tabel
        table->appendRoute(routepoint);
        b_hasTrains = true;
    }
}

TicketManager::~TicketManager() {
    delete graph;
    delete table;
}

void TicketManager::printRouteGraph() {
    graph->printGraph();
}

bool TicketManager::checkForNode(std::string node) {
    return graph->checkForNode(node);
}

/*
1 - Tren de Inalta Viteza   (Presetare - Va fi creat automat)
2 - Tren Regional           (Presetare - Va fi creat automat)
*/

/*
-----------------------------------------------------------------------------------------------------
------------------------------------------ALGORITMUL DE CAUTARE--------------------------------------
-----------------------------------------------------------------------------------------------------
*/

bool RouteGraph::checkForNode(std::string node) {
    return adjList.find(node) != adjList.end();
}

// routeIndex = pozitia statiei in ruta
void RouteGraph::addEdge(std::string st1, std::string st2, Route& route, int routeIndex) {
    // verifici daca muchia exista deja in graf
    int index = RouteGraph::keyHasEdge(st1, st2);
    if(index != -1) {
        bool found = false;
        for(auto& it : adjList[st1][index].routes)
            if(it.route == &route)
                found = true; //daca exista si ruta (pentru a nu adauga de mai multe ori)

        // daca exista muchia de la st1 la st2, dar nu si pe ruta route, atunci adaugi doar ruta
        if(!found)
        {
            RouteIndex ri = {&route, routeIndex};
            adjList[st1][index].routes.push_back(ri);
        }
    }

    else { // daca nu este gasita
        RouteEdge edge;
        RouteIndex ri = {&route, routeIndex};
        edge.toStation = st2;
        edge.routes = {ri};
        adjList[st1].push_back(edge);
    }
}

void RouteGraph::addRoute(Route& route) {
    const std::vector<std::string>& routep = route.getStations();
    int n = routep.size();
    for(int i = 0; i < n - 1; i++) {
        //adaugi ambele legaturi pentru ca este graf neorientat!
        addEdge((routep)[i], (routep)[i + 1], route, i + 1);

        addEdge((routep)[i + 1], (routep)[i], route, i);
    }
}

void RouteGraph::removeEdge(std::string st1, std::string st2, Route* route) {
    int nodeIndex = keyHasEdge(st1, st2);
    
    if(nodeIndex == -1)
        return;

    // cauti indexul muchiei de pe ruta ta
    int i = -1, found = -1;
    for(auto& it : adjList[st1][nodeIndex].routes) {
        i++;
        if(it.route == route){
            found = i;
            break;
        }      
    }

    if(found == -1)
        return;

    adjList[st1][nodeIndex].routes.erase(adjList[st1][nodeIndex].routes.begin() + found);
}

void RouteGraph::removeRoute(Route* route) {
    std::vector<std::string> routep = route->getStations();
    int n = routep.size();
    for(int i = 0; i < n - 1; i++) {
        //adaugi ambele legaturi pentru ca este graf neorientat!
        removeEdge(routep[i], routep[i + 1], route);

        removeEdge(routep[i + 1], routep[i], route);
    }
}

void RouteGraph::printGraph() {
    int n = 0;
    for(auto& it : adjList) {
        std::cout << it.first << ": ";

        n = it.second.size();
        for(int i = 0; i < n; i++) {
            //std::cout << it.second[i].toStation << " (" << it.second[i].routeOrigin->getRouteName() << ")";
            std::cout << it.second[i].toStation << " ";
            for(auto& ri : it.second[i].routes)
                std::cout << "(" << ri.route->getRouteName() << ")";
            if(i < n - 1) std::cout << ", "; 
        }
        std::cout << "\n";
    }
}

//functie ca sa cauti in lista de adiacenta dupa numele statiei, nu dupa RouteEdge
//returneaza indexul muchiei in vector daca este gasita
int RouteGraph::keyHasEdge(std::string key, std::string station) {
    int pos = 0;
    for(auto& it : adjList[key]){
        if(it.toStation == station)
            return pos;
        
        pos++;
    }

    return -1;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


RouteEdge::RouteEdge(std::string _toStation) : toStation(_toStation) {}

void RouteEdge::appendRoute(Route* r, int id) {
    routes.push_back({r, id});
}

//[][][][][][][][][][][][][][][][][][][][][[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
//[][][][][][][][][][][][][][][][][][][][ ALGORITM BAZAT PE DJIKSTRA'S ][][][][][][][][][][][][][][][][][]]
//[][][][][][][][][][][][][][][][][][][][][[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

// fac un min heap de elemente Obj{cost, nod}
struct Obj {
    int cost;
    std::string node;

    Obj() = default;
    Obj(int cost, std::string node) : cost(cost), node(node) {}

    bool operator<(const Obj& other) {
        return this->cost < other.cost;
    }
    bool operator>(const Obj& other) {
        return this->cost > other.cost;
    }
};

template <class T>
class MyMinHeap {
    std::vector<T> heap;
    
    public:
        void insert(T value) {
            // pui elementul la final in heap
            heap.push_back(value);
            // ii memorezi indexul
            int index = heap.size() - 1;
            
            // parcurgi heapul in sus si stabilesti unde pui valoarea noua
            while(index > 0 && heap[index] < heap[(index - 1) / 2]) { // daca e mai mic decat parintele
                std::swap(heap[index], heap[(index - 1) / 2]); // faci swap si apoi urci cu index
                index = (index - 1) / 2;
            }
        }

        T peek() {
            if(!heap.empty())
                return heap[0];
            return T();
        }

        void pop() {
            //sterg ultimul element: il inlocuiesc cu primul
            heap[0] = heap[heap.size() - 1];
            heap.pop_back();

            //HEAPIFY incepand cu root
            int index = 0;
            while(true) {
                int left = 2 * index + 1;
                int right = 2 * index + 2;
                int smallest = index;

                if(left < heap.size() && heap[left] < heap[smallest])
                    smallest = left;
                if(right < heap.size() && heap[right] < heap[smallest])
                    smallest = right;
                if(smallest != index) {
                    std::swap(heap[smallest], heap[index]);
                    index = smallest;
                }
                else
                    break;
            }
        }

        bool empty() {
            return heap.empty();
        }
};

std::vector<SolEdge> RouteGraph::DjikstraSearch (std::string start, std::string end, int time) {   
    // initializezi structurile folosite
    MyMinHeap<Obj> heap;
    std::unordered_map<std::string, SolEdge> visited;
    std::unordered_map<std::string, int> dist;
    for(auto& it : adjList){
        dist[it.first] = __INT_MAX__;
        visited[it.first] = SolEdge();
    }    
    
    //int transit = 7; // TIMPUL ALOCAT PENTRU SCHIMBUL DE RUTE

    // valori auxiliare

    dist[start] = time;
    heap.insert(Obj(time, start));

    visited[start] = SolEdge(); // pentru ca nu a venit de nicaieri in start

    while(!heap.empty()) {
        Obj top = heap.peek();
        heap.pop();

        std::string current = top.node;
        int tstart = top.cost;

        if(tstart > dist[current])
            continue;

        std::cout << "sunt in nodul " << current << " la ora " << tstart << "\n";

        for(auto& edge : adjList[current]) {
            std::string destination = edge.toStation;
            int minCost = __INT_MAX__;
            SolEdge se; // folosesti ca sa salvezi detaliile muchiei(rutei) cele mai bune

            std::cout << destination << "\n";

            // VOI ALEGE MUCHIA CARE AJUNGE CEL MAI RAPID IN ACEA STATIE
            for(auto& ri : edge.routes) {

                int cost = 0;
                bool changedRoute = false;
                Train* ptr;

                // SENSUL DE MERS
                // compari indexul statiei de unde pleci(current) 
                // si indexul statiei unde ajungi(destination, dar il iei din ri.index)
                int sense = 1;
                if(ri.route->hasStation(current) - ri.index >= 0)
                    sense = -1;

                // VERIFICI DACA RUTA CURENTA ESTE DIFERITA DE RUTA CU CARE AI AJUNS IN NODUL CURENT
                // (DACA SCHIMBI TRENUL)
                // SAU DACA ESTI IN NODUL DE START
                // REVEZI: introducerea rutelor automat cu JSON

                if(visited[current].routeName != ri.route->getRouteName()) {
                    changedRoute = true;
                    
                    int id = ri.route->getCompatibleDepTime(tstart, ri.index, sense); // gasesti primul tren
                    std::cout << id << "\n";
                    if(id >= 0) {
                        ptr = ri.route->getTrains()[id]; // pointer auxiliar care memoreaza trenul gasit
                    }
                    else ptr = nullptr;
                    
                }
                // DACA NU S A SCHIMBAT RUTA (TRENUL)
                else {
                    ptr = visited[current].train;
                }

                if(!ptr) {
                    std::cout << "nu exista tren compatibil pe ruta asta\n";
                    continue;
                }
                    
                
                // costul (timpul de sosire) este aflat folosind indexul statiei in ruta, IN FUNCTIE DE SENS
                cost = ptr->getDeparture() + ptr->getSpeed() * ri.index;
                if(sense == -1)
                    cost = ptr->getDeparture() + ptr->getSpeed() * (ri.route->getStations().size() - 1 - ri.index);

                if(cost < tstart) {
                    std::cout << "nu s a gasit tren compatibil(cand nu s a schimbat ruta)\n";
                    continue;
                }

                // DACA AI GASIT O VARIANTA MAI BUNA O MEMOREZI PT MAI TARZIU
                if(cost < minCost) {
                    // pt ca vrei sa construiesti muchia invers(pentru reconstructia solutiei)
                    se.toStation = current; 
                    se.train = ptr;
                    se.arrTime = cost;
                    se.depTime = cost - ptr->getSpeed(); // ca vrei cand a plecat trenul nu cand a ajuns celalalt
                    se.routeName = ri.route->getRouteName();

                    minCost = cost;
                }

            }

            // verifici daca nu cumva nu exista niciun tren care sa plece din nodul ala altundeva (improbabil)
            if(minCost == __INT_MAX__) 
                continue;
            
            //std:: cout << "plec la " << tstart << " si ajung la " << minCost << "\n";
            if(minCost < dist[destination]) {
                visited[destination] = se;
                dist[destination] = minCost;
                heap.insert(Obj(minCost, destination));  

                std::cout << "cea mai buna: " << se.routeName << " ora: " << minCost << "\n";
            }            
        }        
    }

    if(dist[end] == __INT_MAX__)
        return std::vector<SolEdge>();

    // recrearea solutiei
    // in visited am muchiile de unde am ajuns in fiecare nod, doar ca sunt intoarse
    // construiesc solutia pe care o intorc, dar va trebui sa inversez la loc muchiile
    std::vector<SolEdge> solution;

    // parcurg manual vectorul visited cu un pointer
    SolEdge *auxpointer = &visited[end];

    // muchia care o sa fie adaugata; ii trebuie capatul corect
    SolEdge auxedge;
    auxedge.toStation = end;

    while(auxpointer->toStation != "") {
        // copiez toate valorile in afara de toStation, pt ca trebuie inversat
        auxedge.routeName = auxpointer->routeName;
        auxedge.arrTime = auxpointer->arrTime;
        auxedge.depTime = auxpointer->depTime;
        auxedge.train = auxpointer->train;
        solution.push_back(auxedge);

        auxedge.toStation = auxpointer->toStation; // pun ruta de unde vin, ca sa o folosesc iar
        auxpointer = &visited[auxpointer->toStation];
    }
    std::reverse(solution.begin(), solution.end()); //  este mai rapid sa dai reverse la final!
    return solution;  
}

// o functie care primeste ruta de pornire si de final, si apeleaza DjikstraSearch pentru fiecare optiune
void TicketManager::ticketOptions() {
    std::string start, end;
    std::cout << "Statia de plecare: ";
    std::cin >> start;
    std::cout << "Statia de sosire: ";
    std::cin >> end;

    start = toLower(start);
    end = toLower(end);

    // voi pune toate orele de plecare de pe toate rutele din start intr un vector
    // am nevoie de interclasare pe nr variabil de vectori, vectorii fiind rutele
    
    std::vector<int> vec = graph->getMergedTimes(start);

    std::vector<std::vector<SolEdge>> options;

    // daca inversezi vectorul vec, va ajuta sa filtrezi optiunile
    // daca sunt doua optiuni care ajung la aceeasi ora, pastrezi pe cea mai scurta(mai de la capat)
    std::reverse(vec.begin(), vec.end());

    for(auto& it : vec) {
        std::cout << "merge\n";
        std::vector<SolEdge> aux = graph->DjikstraSearch(start, end, it);
        

        if(aux.empty())
            continue;
        
        if(!options.empty())
            if(options.back().back().arrTime == aux.back().arrTime)
                continue;

        options.push_back(aux);
    }

    if(options.empty()) {
        std::cout << "Din pacate, momentan nu exista nicio ruta pentru destinatia introdusa.\n";
        return;
    }

    std::reverse(options.begin(), options.end());

    std::vector<std::vector<SolEdge>> tickets;

    // printez optiunile de rute pentru client
    int count = 1;
    for(auto& opt : options) {
        std::string startStation, endStation, routeName;
        int dep, arr;
        double price = 0;
        Train* ptr = nullptr;

        startStation = start;
        routeName = opt[0].routeName;
        dep = opt[0].depTime;

        // parcurgi statiile din solutie
        // le grupezi in functie de rute si le afisezi odata ce gasesti o ruta noua
        std::cout << "Optiunea " << count++ << ":\n";
        
        std::vector<SolEdge> ticket;

        for(auto& it : opt) { 
            price += it.train->getPrice("Vagon Clasa a Doua"); // pretul total al biletului

            if(it.routeName == routeName) {
                endStation = it.toStation;
                arr = it.arrTime;
                ptr = it.train;
            }
            else {
                // acum afisezi ce ai memorat si treci la urm ruta
                std::cout << "\t" << clockConvert(dep) << " - " << clockConvert(arr) << " # " << startStation << " - " << endStation << " # " << "(" << routeName << ")\n";
                
                SolEdge se;
                se.depTime = dep;
                se.arrTime = arr;
                se.routeName = routeName;
                se.toStation = endStation;
                se.train = ptr;
                ticket.push_back(se);

                startStation = endStation;
                routeName = it.routeName;
                dep = it.depTime;
                
                endStation= it.toStation;
                arr = it.arrTime;
                ptr = it.train;
            }
            
        }
        // mai ai pe ultima de afisat
        std::cout << "\t" << clockConvert(dep) << " - " << clockConvert(arr) << " # " << startStation << " - " << endStation << " # " << "Ruta: " << routeName << "\n";
        std:: cout << "\n\tPretul total(Bilet integ, clasa a 2-a): " << price << " lei\n";

        SolEdge se;
        se.depTime = dep;
        se.arrTime = arr;
        se.routeName = routeName;
        se.toStation = endStation;
        se.train = ptr;
        ticket.push_back(se);

        tickets.push_back(ticket);
    }

    int choice;
    std::cout << "\nCe optiune alegeti?\n>";
    std::cin >> choice;

    buy(start, tickets[choice - 1]);
}

void TicketManager::buy(std::string start, std::vector<SolEdge> trips) {
    std::cout << "Ce tip de bilet doriti sa achizitionati?\n";

    bool tryAgain = true;
    int choice;
    Ticket* ticket;

    while(tryAgain) {
        try {
            std::cout << "1 - Bilet Intreg\n";
            std::cout << "2 - Bilet Tineret (Elev/Student)\n";
            std::cout << "3 - Bilet Pensionar\n";
            std::cout << "4 - Bilet CFR+\n";
            std::cout << ">";
            std::cin >> choice;

            switch(choice) {
                case 1:
                    ticket = buyFullPrice();
                    tryAgain = false;
                    break;
                case 2:
                    ticket = buyYouth();
                    tryAgain = false;
                    break;
                case 3:
                    ticket = buyElderly();
                    tryAgain = false;
                    break;
                case 4:
                    ticket = buyMembership();
                    tryAgain = false;
                    break;
                default:
                    std::cout << "Varianta invalida. Incercati din nou.\n";
            }
        }
        catch(const ValueError &e) {
                std::cout << "Exceptie: " << e.what() << "\n";
        }
    }

    // fac mai intai pentru primul trip
    std::string startStation = start;
    std::cout << "Pentru trenul " << startStation << " - " << trips[0].toStation << ":\n\n";
    
    // TIPUL VAGONULUI
    tryAgain = true;
    std::string car_type;

    while(tryAgain) {
        trips[0].train->printOptions();
        std::cout << "Alegeti tipul de vagon:\n";
        int choice;
        std::cin >> choice;
        car_type = trips[0].train->getCarOption(choice - 1);

        if(car_type != "0")
            tryAgain = false;
    }

    // NUMARUL VAGONULUI SI AL LOCULUI
    int seat, car;
    carInfo info = trips[0].train->occupyEmptySeat(car_type, ticket->getId());
    seat = info.seat_number; 
    car = info.car_number;

    // departure time si arrival time si routeName le am deja din SolEdge

    // PRETUL CALATORIEI
    double tripPrice = trips[0].train->getPrice(car_type) * (trips[0].arrTime - trips[0].depTime) / trips[0].train->getSpeed();
    
    Trip* _trip = new Trip(clockConvert(trips[0].depTime), clockConvert(trips[0].arrTime), trips[0].routeName, startStation, trips[0].toStation, seat, car, car_type, tripPrice);
    ticket->appendTrip(_trip);

    startStation = trips[0].toStation;

    for(int i = 1; i < trips.size(); i++) {
        auto& it = trips[i];

        std::cout << "Pentru trenul " << startStation << " - " << it.toStation << ":\n\n";
        
        // TIPUL VAGONULUI
        tryAgain = true;
        std::string car_type;

        while(tryAgain) {
            it.train->printOptions();
            std::cout << "Alegeti tipul de vagon:\n";
            int choice;
            std::cin >> choice;
            car_type = it.train->getCarOption(choice - 1);

            if(car_type != "0")
                tryAgain = false;
        }

        // NUMARUL VAGONULUI SI AL LOCULUI
        int seat, car;
        carInfo info = it.train->occupyEmptySeat(car_type, ticket->getId());
        seat = info.seat_number; 
        car = info.car_number;

        // departure time si arrival time si routeName le am deja din SolEdge

        // PRETUL CALATORIEI
        double tripPrice = it.train->getPrice(car_type) * (it.arrTime - it.depTime) / it.train->getSpeed();
        
        Trip* _trip = new Trip(clockConvert(it.depTime), clockConvert(it.arrTime), it.routeName, startStation, it.toStation, seat, car, car_type, tripPrice);
        ticket->appendTrip(_trip);

        startStation = it.toStation; // o memorez pt urmatorul trip
    }

    ticket->setTravel();
    ticket->setPrice();
    tickets[ticket->getId()] = ticket;
    b_hasTickets = true;
    std::cout << "Biletul dvs, cu codul " << ticket->getId() << ", a fost cumparat si inregistrat cu succes!\n";
}

struct Time {
        int value, edge, route, index;

        bool operator<(const Time& other) {
            return value < other.value;
        } 
        bool operator>(const Time& other) {
            return value > other.value;
        }
};

// functie care face interclasare pentru toti timpii de plecare din toate rutele care trec prin station 
// si o returneaza ca vector
std::vector<int> RouteGraph::getMergedTimes(std::string station) {
    std::vector<int> vec;

    MyMinHeap<Time> heap;

    // mai intai bagi cate un prim element din fiecare vector
    int edge = 0, route = 0;
    for(auto& re : adjList[station]) { //muchii catre alte noduri
        route = 0;
        for(auto& rte : re.routes) //rute de pe fiecare muchie
            heap.insert({rte.route->getTrains()[0]->getDeparture(), edge, route++, 0});

        edge++;
    }

    while(!heap.empty()) {
        Time el = heap.peek();
        heap.pop();

        vec.push_back(el.value);

        //iau vectorul de unde provine trenul(timpul) pe care tocmai l-am scos din heap
        const std::vector<Train*>& vect = adjList[station][el.edge].routes[el.route].route->getTrains();

        // daca elementul nu este ultimul
        if(el.index + 1 < vect.size()) {
            heap.insert({vect[el.index + 1]->getDeparture(), el.edge, el.route, el.index + 1});
        }
    }

    return vec;
} 
//---------------------------------------------------------------------------------------------------------
//------------------------------------------------MENIUL/INTERFATA---------------------------------------
//-----------------------------------------------------------------------------------------------------

Ticket* TicketManager::buyFullPrice() {
    std::string name;
    std::cout << "Introduceti-va numele complet:\n";
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    std::string id;
    std::cout << "Introduceti CNP-ul: ";
    std::cin >> id;
    //if(id < 1) throw ValueError(id);
    
    Ticket* ticket = new FullPriceTicket(name, id);
    return ticket;
}
Ticket* TicketManager::buyYouth() {
    std::string name;
    std::cout << "Introduceti-va numele complet:\n";
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    std::string id;
    std::cout << "Introduceti numarul matricol (student)/CNP-ul (elev): ";
    std::cin >> id;
    //if(id < 1) throw ValueError(id);
    
    Ticket* ticket = new YouthTicket(name, id);
    return ticket;
}
Ticket* TicketManager::buyElderly() {
    std::string name;
    std::cout << "Introduceti-va numele complet:\n";
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    std::string id;
    std::cout << "Introduceti codul cardului de sanatate: ";
    std::cin >> id;
    //if(id < 1) throw ValueError(id);
    
    Ticket* ticket = new ElderlyTicket(name, id);
    return ticket;
}
Ticket* TicketManager::buyMembership() {
    std::string name, username;
    std::cout << "Introduceti-va numele complet:\n";
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    std::string id;
    std::cout << "Introduceti CNP-ul: ";
    std::cin >> id;
    //if(id < 1) throw ValueError(id);

    std::cout <<"Introduceti numele dvs. de utilizator (FARA SPATII): ";
    std::cin >> username;
    
    Ticket* ticket = new MembershipTicket(name, id, username);
    return ticket;
}

void TicketManager::StartMenu() {
    bool tryAgain = true;
    int option;
    bool menuRunning = true;

    while(menuRunning) {
        tryAgain = true;
        while(tryAgain) {
            try {
                std::cout << "CFR - SISTEM DE CUMPARARE BILETE\n\n";

                std::cout<<"1 - Reteaua de Rute\n2 - Cumparare Bilet\n3 - Informatii Bilete\n";
                std::cout << "0 - Inchide\n";
                std::cout << ">";

                std::cin >> option;

                switch(option) {
                    case 0:
                        std::cout << "Se inchide...\n\n";
                        tryAgain = false;
                        menuRunning = false;
                        break;
                    case 1:
                        RoutesMenu();
                        tryAgain = false;
                        break;
                    case 2:
                        if(initialized)
                            ticketOptions();
                        else
                            std::cout << "Nu exista rute in sistem.\n";
                        tryAgain = false;
                        break;
                    case 3:
                        TicketsInfo();
                        tryAgain = false;
                        break;
                    default:
                        throw OptionError(option);
                }
            }
            catch(const OptionError &e) {
                std::cout << "Exceptie: " << e.what() << "\n";
            }
        }
    }
}

void TicketManager::RoutesMenu() {
    bool running = true;
    while(running) {
        std::cout<<"1 - Reteaua de Rute\n";
        std::cout << "\t1 - Initializare Automata\n\t2 - Stergere Totala\n\t3 - Adaugare Manuala\n";
        std::cout << "\t4 - Afisare Graf Rute(lista de adiacenta)\n\t0 - Inapoi\n";

        int option;
        std::cout << ">";
        std::cin >> option;
        switch(option) {
            case 0:
                running = false;
                break;
            case 1:
                //FUNCTIE DE INITIALIZARE
                std::cout << "Momentan nu exista aceasta optiune!\n";
                //o sa o fac pana luni
                break;
            case 2:
                deleteGraph();
                break;
            case 3:
                populateTable();
                break;
            case 4:
                printGraph();
                std::cout << "\n";
                break;
            default:
                throw OptionError(option);
                running = false;
        }
    }
}

void TicketManager::deleteGraph() {
    if(initialized) {
        delete table;
        delete graph;

        table = new RouteTable();
        graph = new RouteGraph();
    }
    else
        std::cout << "Nu exista nimic de sters!\n";

}

void TicketManager::printGraph() {
    if(initialized) {
        graph->printGraph();
    }
    else
        std::cout << "Nu exista nimic de afisat!\n";
}

void TicketManager::TicketsInfo() {
    if(!initialized) {
        std::cout << "Nu exista rute in sistem.\n";
        return;
    }

    bool running = true;

    std::cout << "\n" << Ticket::ticketCount() << " bilete inregistrate in sistem.\n";
    for(auto it : tickets) {
        std::cout << it.first << "\t" << it.second->getName() << "\n";
    }
    std::cout << "\n";

    while(running) {
        int id;
        std::cout << "Selectati un ID pentru a vedea informatii despre bilet.\n0 - Inapoi la meniul principal\n>";
        std::cin >> id;

        if(tickets.find(id) == tickets.end()) {
            throw OptionError(id);
            running = false;
        }
            
        if(id == 0)
            running = false;
        
        tickets[id]->printInfo();
    }
}

// FUNCTII PENTRU QT GUI

bool TicketManager::addRoute(std::string routes) {
    initialized = true;

    Route *routepoint = new Route();

    std::string s, token;

    s = routes;
    s+=' ';

    while(s.find(" ") != std::string::npos) {
        int pos = s.find(" ");

        token = s.substr(0, pos); // statia
        
        if(token != "")
            routepoint->pushStation(token);

        s.erase(0, s.find(" ") + 1);
    }

    if(table->findRoute(routepoint->getRouteName())) {
            std::cout << "EROARE: RUTA EXISTA DEJA"; // revezi: caz exceptional in QT
            return false; 
    }

    //se adauga ruta in graf SI IN TABELA
    table->appendRoute(routepoint);
    graph->addRoute(*routepoint);
    
    std::cout << "a mers!\n";

    return true;
}

std::vector<std::string> TicketManager::getRoutes() const{
    std::vector<std::string> v{};
    
    for(auto& it : table->getRoutes())
        v.push_back(it->getRouteName());

    return v;
}

std::vector<Route*> TicketManager::getRoutePtrs() const {
    return table->getRoutes();
}

bool TicketManager::addTrain(std::string route, int train, int depTime) {
    Route* routePointer = table->findRoute(route);
    if(!routePointer)
        return false;
    
    Train* trainPointer;
    
    if(train == 0)
        trainPointer = newHST(depTime);
    else
        trainPointer = newRT(depTime);
    
    // verifici daca exista deja un tren care pleaca la ora aceea

    int index = routePointer->getCompatibleDepTime(depTime, 1, 1);
    if(index > -1)
        if(routePointer->getTrains()[index]->getDeparture() == depTime)
            return false;
    
    routePointer->pushTrain(trainPointer);

    std::cout << "Tren introdus cu succes!\n";
    b_hasTrains = true;
    return true;
}
std::vector<std::string> TicketManager::getTrains(std::string route) {
    std::vector<std::string> trains;
    Route* routePtr = table->findRoute(route);
    routePtr->sortTrains();

    for(auto& train : routePtr->getTrains()) {
        trains.push_back(clockConvert(train->getDeparture()));
    }

    return trains;
}

std::string TicketManager::formatTime(int time) {
    std::string output = "";
    int hours = (time / 60) % 24;
    int minutes = time % 60;

    std::string h = std::to_string(hours), m = std::to_string(minutes);
    if(hours < 10)
        h = "0" + h;
    if(minutes < 10)
        m = "0" + m;
    
    return h + ":" + m;
}

bool TicketManager::removeRoute(std::string routeName) {
    Route* routeptr = table->findRoute(routeName);

    //return true;
    graph->removeRoute(routeptr);
    table->deleteRoute(routeptr);
    return true;
}

bool TicketManager::removeTrain(std::string routeName, int depTime) {    
    Route* routeptr = table->findRoute(routeName);
    if(!routeptr)
        return false;

    for(auto& it : routeptr->getTrains()) {
        std::cout << it->getDeparture() << "\n";
    }
    std::cout << "\n";
    
    // indexul trenului pe care vrei sa il stergi
    int index = routeptr->getCompatibleDepTime(depTime, 1, 1);

    routeptr->removeTrain(index);
    
    for(auto& it : routeptr->getTrains()) {
        std::cout << it->getDeparture() << "\n";
    }
    std::cout << "\n";

    b_hasTrains = tableHasTrains(); // dupa ce ai sters exista o sansa sa nu mai existe niciun tren!
    
    return true;
}

std::vector<std::vector<SolEdge>> TicketManager::getTicketOptionsGUI(std::string start, std::string end) {
    start = toLower(start);
    end = toLower(end);

    if(!graph->checkForNode(start) || !graph->checkForNode(end))
        return {{SolEdge()}}; // daca nu s-au gasit in graf se returneaza doar asta ca sa stiu
    
    // VEZI TIcketOptions::ticketOptions
    
    std::vector<int> vec = graph->getMergedTimes(start);

    std::vector<std::vector<SolEdge>> options; //CONSTRUIESC SI RETURNEZ

    // daca inversezi vectorul vec, va ajuta sa filtrezi optiunile
    // daca sunt doua optiuni care ajung la aceeasi ora, pastrezi pe cea mai scurta(mai de la capat)
    std::reverse(vec.begin(), vec.end());

    for(auto& it : vec) {
        std::vector<SolEdge> aux = graph->DjikstraSearch(start, end, it);

        if(aux.empty())
            continue;
        
        if(!options.empty())
            if(options.back().back().arrTime == aux.back().arrTime)
                continue;

        options.push_back(aux);
    }

    if(options.empty()) {
        //std::cout << "Din pacate, momentan nu exista nicio ruta pentru destinatia introdusa.\n";
        return options;
    }

    std::reverse(options.begin(), options.end());

    //return options;
    std::vector<std::vector<SolEdge>> tickets;
    
    // printez optiunile de rute pentru client
    int count = 1;
    for(auto& opt : options) {
        std::string startStation, endStation, routeName;
        int dep, arr;
        double price = 0;
        Train* ptr = nullptr;

        startStation = start;
        routeName = opt[0].routeName;
        dep = opt[0].depTime;

        // parcurgi statiile din solutie
        // le grupezi in functie de rute si le afisezi odata ce gasesti o ruta noua
        std::cout << "Optiunea " << count++ << ":\n";
        
        std::vector<SolEdge> ticket;

        for(auto& it : opt) { 
            price += it.train->getPrice("Vagon Clasa a Doua"); // pretul total al biletului

            if(it.routeName == routeName) {
                endStation = it.toStation;
                arr = it.arrTime;
                ptr = it.train;
            }
            else {
                // acum afisezi ce ai memorat si treci la urm ruta
                std::cout << "\t" << clockConvert(dep) << " - " << clockConvert(arr) << " # " << startStation << " - " << endStation << " # " << "(" << routeName << ")\n";
                
                SolEdge se;
                se.depTime = dep;
                se.arrTime = arr;
                se.routeName = routeName;
                se.toStation = endStation;
                se.train = ptr;
                ticket.push_back(se);

                startStation = endStation;
                routeName = it.routeName;
                dep = it.depTime;
                
                endStation= it.toStation;
                arr = it.arrTime;
                ptr = it.train;
            }
            
        }
        // mai ai pe ultima de afisat
        std::cout << "\t" << clockConvert(dep) << " - " << clockConvert(arr) << " # " << startStation << " - " << endStation << " # " << "Ruta: " << routeName << "\n";
        std:: cout << "\n\tPretul total(Bilet integ, clasa a 2-a): " << price << " lei\n";

        SolEdge se;
        se.depTime = dep;
        se.arrTime = arr;
        se.routeName = routeName;
        se.toStation = endStation;
        se.train = ptr;
        ticket.push_back(se);

        tickets.push_back(ticket);
    }

    return tickets;
}

Ticket* TicketManager::buyTicketGUI(std::string name, std::string id, std::string user, int type) {
    Ticket* tptr = nullptr;
    TicketFactory* factory = getFactory(type);
    tptr = factory->createTicket(name, id, user);

    b_hasTickets = true;
    return tptr;
}

bool TicketManager::createTrip(Ticket& ticketptr, std::string depTime, std::string arrTime, std::string routeName, std::string startSt, std::string endSt, int seat, int car, std::string carType, double price) {
    ticketptr.appendTrip(depTime, arrTime, routeName, startSt, endSt, seat, car, carType, price);
    return true;
}

std::vector<int> TicketManager::getTickets() const {
    std::vector<int> vec{};

    for(auto& it : tickets) {
        vec.push_back(it.first);
    }

    return vec;
}

std::string TicketManager::getTicketInfo(int id) const {
    std::stringstream s;
    tickets.at(id)->printInfo(s);
    tickets.at(id)->printTrips(s);
    return s.str();
}

bool TicketManager::hasTickets() {
    return b_hasTickets;
}
bool TicketManager::hasTrains() {
    return b_hasTrains;
}

bool TicketManager::tableHasTrains() {
    bool hasTrains = false;
    for(auto& route : table->getRoutes())
        if(!route->getTrains().empty())
            hasTrains = true;
    
    return hasTrains;
}