#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

class Car { // VEZI CA E CLASA ABSTRACTA ACUM !!
    protected:
        int capacity{0};

        double seat_price{10};

        std::vector<int> passengers;

    public:
        Car() = default;
        Car(int _capacity, double _price);
        //Car(const Car& other);
        
        //destructor virtual
        virtual ~Car() = default;

        // AFISARE
        virtual void print(std::ostream& out) const;

        //asta e friend, deci nu poate fi virtuala si folosita de alte clase pt ca nu apartine clasei!
        friend std::ostream& operator<<(std::ostream& out, const Car& c); 
        
        virtual double getPrice() const = 0; //se schimba la fiecare tip de vagon

        //FUNCTIE VIRTUALA PURA
        virtual std::string type() = 0;

        //FUNCTII UNIVERSALE PASAGERI
        void addPassenger(int _id, int _seat);
        void printAllPassengers();
        int occupyEmptySeat(int id);
};

class CouchetCar : public Car {
    int bed_nr{4};
    double bedsheet_price{15};

    public:
        CouchetCar() = default;
        CouchetCar(int _capacity, double _price, int _bed, double _bedsheet);
        CouchetCar(const CouchetCar& other);

        void print(std::ostream& out) const override;

        CouchetCar& operator=(const CouchetCar& other);
        //noua functie getprice pentru cuseta
        double getPrice() const override;

        std::string type() override;
};

class StandardCar : public Car {
    int car_class{2}, bike_seats{5};

    public:
        StandardCar() = default;
        StandardCar(int _capacity, double _price, int _class, int _bike);
        StandardCar(const StandardCar& other);

        StandardCar& operator=(const StandardCar& other);

        double getPrice() const override;

        std::string type() override;
};

//am nevoie de numarul vagonului si numarul locului
struct carInfo {
    int car_number = 0;
    int seat_number = 0;
};


class Train {
    int departure_time{0};
    int time_to_station{0};
    int cars_nr{0};
    
    std::vector<Car*> traincars{};
    std::vector<std::string> carOptions{};

    public:
        Train() = default;
        Train(int _dep, int _time);
        Train(const Train& other) = default;

        ~Train();

        Train& operator=(const Train& other);

        void pushCar(Car* cart);
        void removeCars();

        void printTraincars();

        bool operator<(const Train& other); //pentru sortare

        int getDeparture() const { return departure_time; }
        int getSpeed() const { return time_to_station; }

        void mapCars(); //pune intr un vector toate tipurile de vagon disponibile
        void printOptions();
        std::vector<std::string> getOptions();
        std::string getCarOption(int option);


        carInfo occupyEmptySeat(std::string type, int id);
        bool isFull();

        double getPrice(std::string type) const;
};

class Route {
    private:
        std::vector<std::string> stations;
        std::vector<Train*> trains;

        bool isSorted = false;
        void deleteTrainPtr(int trainIndex);

    public:
        Route() = default;
        Route(const Route& other);
        ~Route();

        //void textToStations(std::string text);
        void pushStation(std::string string);
        void printStations();

        void pushTrain(Train* train);
        void removeTrain(int trainIndex);
    
        std::string getRouteName() const;
        std::string getRouteNameReversed() const;
        int hasStation(const std::string& station) const;

        const std::vector<std::string>& getStations() const { return stations; } //CONST REFERENCE E MAI EFICIENT!
        const std::vector<Train*>& getTrains() const { return trains; }
        
        int getCompatibleDepTime(int tstart, int index, int sense);
        void sortTrains();
        
        //void printTrains();

};

class Trip {
    static int trip_counter;
    int trip_id;

    std::string departure_time, arrival_time;
    std::string routeName;

    std::string start, end;

    //date de vagon
    int seat_nr;
    int car_nr;
    std::string car_type;

    double price;

    public:
        Trip(std::string dep, std::string arr, std::string routeName, std::string start, std::string end, int seat, int car, std::string type, double _price);
        std::string getDep() const { return departure_time; }
        std::string getArr() const { return arrival_time; }
        double getPrice() const { return price; }

        void printInfo(std::ostream& os = std::cout) const;
};


class Ticket {
    static int ticket_counter;
    protected:
        std::vector<Trip*> trips;

        // DATE FURNIZATE DE CLIENT(TREBUIE CONSTRUITE)
        std::string name;
        std::string start, destination;

        // date create automat
        int ticket_id;
        double price{0};

    public:
        Ticket();
        Ticket(std::string _name);

        virtual ~Ticket();
        //nu am de ce sa fac copy constructor si assignment operator! cred

        virtual double getFinalPrice(double initial) = 0;


        void appendTrip(Trip* trip);
        void appendTrip(std::string dep, std::string arr, std::string routeName, std::string start, std::string end, int seat, int car, std::string type, double _price);
        void printTrips (std::ostream& os = std::cout) const;

        virtual void printInfo (std::ostream& os = std::cout) const = 0;

        int getId() const { return ticket_id; }
        std::string getName() const { return name; }

        void setTravel();
        void setPrice();
        double getPrice() { return price; }

        static int ticketCount();
};

class DiscountTicket : virtual public Ticket {
    protected: double discount{0};
};

class FullPriceTicket : virtual public Ticket {
    protected: std::string id_card;

    public:
        FullPriceTicket() = default;
        FullPriceTicket(std::string _name, std::string _id);
        double getFinalPrice(double initial) override;

        void printInfo(std::ostream& os) const override;
};

class YouthTicket : public DiscountTicket {
    std::string student_id;
    public:
        YouthTicket(std::string _name, std::string _id);
        double getFinalPrice(double initial) override;

        void printInfo(std::ostream& os) const override;
};

class ElderlyTicket : public DiscountTicket {
    std::string health_card;

    public:
        ElderlyTicket(std::string _name, std::string _id);
        double getFinalPrice(double initial) override;

        void printInfo(std::ostream& os) const override;
};

class MembershipTicket : public FullPriceTicket, public DiscountTicket {
    std::string account_name;

    public:
        MembershipTicket(std::string _name, std::string _id, std::string _account);
        double getFinalPrice(double initial) override;

        void printInfo(std::ostream& os) const override;
};

class TicketFactory {
    public:
        // factory method
        virtual Ticket* createTicket(std::string name, std::string id, std::string username = "") = 0;
        virtual ~TicketFactory() {}
};

class FullPriceFactory : public TicketFactory {
    public:
        Ticket* createTicket(std::string name, std::string id, std::string username = "") override {
            return new FullPriceTicket(name, id);
        }
};
class YouthFactory : public TicketFactory {
    public:
        Ticket* createTicket(std::string name, std::string id, std::string username = "") override {
            return new YouthTicket(name, id);
        }
};
class ElderlyFactory : public TicketFactory {
    public:
        Ticket* createTicket(std::string name, std::string id, std::string username = "") override {
            return new ElderlyTicket(name, id);
        }
};
class MembershipFactory : public TicketFactory {
    public:
        Ticket* createTicket(std::string name, std::string id, std::string username = "") override {
            return new MembershipTicket(name, id, username);
        }
};


class RouteTable {
    std::vector<Route*> routes;

    public:
        ~RouteTable();

        void appendRoute(Route* route);
        void printRoutes();

        Route* findRoute(std::string name);
        void deleteRoute(Route* ptr);
        const std::vector<Route*>& getRoutes() const { return routes; }
};


/*
⠀⠀⠀⠀⠘⣿⡇⢀⢉⣙⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⢠⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣛⣭⣭⣷⣶⣶⠌⠀⠀⠀⠀⠀⢀⣤⠎⠀⠀⠀⠀
⣄⣀⣀⣀⣀⣹⡿⠉⠉⠀⢩⣿⣿⡇⠀⠀⡇⠀⠀⠀⢸⡜⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⠀⠀⠀⠀⠀⠀⡽⠁⠀⠀⠀⠀⠀
⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⡀⢸⣿⡇⠀⠀⡇⠀⠀⠀⡞⡇⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⣀⠞⠀⠀⠀⠀⠀⠀⠀
⣿⣿⣿⣿⣿⣿⠀⠀⠀⠙⡦⣸⣭⢤⡉⠑⡧⠀⠀⠀⢧⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⡟⠄⠀⠀⠀⠀⣼⢯⠀⠀⠀⠢⡀⠀⢠⠀
⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠈⢣⣙⠦⣈⠢⡉⠀⠀⢠⢸⠖⢀⡄⣀⡤⠚⠁⣿⣿⣿⣿⣿⣿⣿⣿⡍⠀⠀⠀⢀⣸⠃⠈⠀⠀⠀⠀⠱⠀⢸⣿
⠿⣿⣿⣿⣿⣿⠁⠀⠀⠀⠢⢸⣿⡟⠗⢿⠙⠀⠀⢸⣾⡾⢏⠻⡌⠳⣆⠀⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠢⡇⠀⠀⠀⠀⠀⠀⠀⠀⢸⠉
⠶⢿⣿⣿⣿⣿⠀⠀⠀⠀⠀⢸⣿⡇⠀⠘⡠⠀⢀⡀⣿⠧⠤⠷⠬⠦⣈⣳⣿⣿⣿⣿⣿⣿⡻⠁⠀⠀⢠⡲⠛⠀⠀⠀⠀⠀⠀⢀⡜⠀⣸⣀
⣸⡿⢿⣿⣿⣿⠀⠀⠄⠀⠀⢸⣿⣇⣠⡤⠿⠭⠤⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠿⠏⠀⠀⠀⢠⢳⣋⣉⣉⣉⣉⣉⢉⠩⠍⡉⣉⢉⡛
⣿⣧⣾⣿⣿⣿⣤⣤⣄⣄⣀⣜⡩⠋⠁⠀⠀⠀⠀⠀⠉⠀⠀⠀⢰⠞⠛⠋⠉⠙⠒⠢⢤⡀⠢⣀⢠⢯⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⡇
⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣤⣤⢤⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⡀⠁⠸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⡇
⠈⠉⠉⠈⢉⢻⠛⠛⢛⣻⡏⢸⣿⣿⠀⠀⠀⠀⠉⡽⠁⠀⣶⣶⣦⣤⢄⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠱⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣿⡇
⣀⣀⣐⣐⣒⣸⠂⣤⣤⣩⠣⡸⣿⣿⡄⠀⢀⣠⠞⠀⠀⠀⢻⣿⣿⣿⡇⡈⠠⠭⠽⠙⠐⠒⠲⣤⠄⠀⠀⢡⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇
⠉⠉⠉⠉⠉⢹⣿⣿⣿⢿⡀⠈⠉⠉⠓⠊⠉⠁⠀⠀⠀⠀⠈⠻⣿⣿⣷⠀⠀⠀⠀⠀⠀⢀⡰⠃⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇
⢤⣀⣀⣠⣤⣼⣿⣿⣿⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠧⠤⠤⠤⠤⠖⠋⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇
⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠸⡀⠀⠀⠀⣀⣀⣀⡀⠀⠀⠀⠀⢀⣀⡀⠀⢀⣀⣀⣀⡀⠠⢤⣴⠖⠀⠀⠀⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇
⣿⣿⣿⣿⣿⣿⣿⣟⣿⣶⣄⣱⣄⠀⠀⠙⠲⣄⠈⡏⠉⠉⠉⠁⢸⠈⠉⠉⠀⡘⠀⠀⢀⣼⡏⠀⠀⢀⣼⣁⣀⣀⣀⣀⣀⣀⣀⣀⣠⣼⣿⣷
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠉⠓⠤⢤⣀⣀⣠⣀⣀⣀⣤⣧⠴⠞⠋⠋⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠛⠛⠛⠉⠛⠢⢄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡴⠚⠉⠀⠉⠉⠉⠉⠉⠉⠉⠉⠋⠉⠉⠉⠉⠁
⣩⣽⣿⣿⣿⣿⣿⣶⣶⣶⣶⣶⣶⣶⣶⣶⡶⠶⠿⠿⢿⣖⠠⣤⣄⣤⣤⣤⣤⣤⣴⣚⣋⣥⣤⣤⣤⣤⣤⣄⣀⣀⣀⣀⠤⠄⠠⠤⠤⢤⣤⣄
⠀⠂⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠚⠛⠉⠉⠉⠉⠉⠉⢼⠀⠀⠃⡌⠉⠙⠛⠛⠛⠭⠭⣍⣉⡉⠉⠉⠉⠉⠉⠉⠉⠙⠛⠛⠛⠛⠒⠒⠒⠒
⠓⠂⠤⢄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡏⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁⠒⠒⠀⠤⠤⣤⣀⡤⠤⠤⠤⠤⠤
⠀⠀⠀⠀⠀⠈⠛⠒⠀⠀⠠⢤⣀⠀⠀⠀⠀⠀⠀⠒⡞⠁⠀⠀⠀⡗⠒⠒⠒⠛⠛⠉⠉⠉⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠉⠉⠓⠒⠂⠠⠤
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠒⠢⠤⢀⣀⢇⠀⠀⠀⢸⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⢤⠖⠈⣩⠟⠊⠞⠀⠀⠀⠘⠆⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠖⠁⡰⠁⢀⡾⠁⠀⠀⠀⠀⠀⠀⣠⠖⠁⠀⣩⠥⢦⡤⠤⠤⠤⠤⠐⠒⠒⠒⠒⠒⠒⠒⠀⠀⠀⠀⠉⠉⠉⠉
⠀⠀⠤⠤⠤⠖⠒⠒⢒⡲⠣⠂⡰⠃⢀⡾⠀⠀⠀⠀⠀⠀⠀⠞⠁⠀⢀⠞⠁⠀⠀⠈⠑⢶⣦⣤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/

//clase folosite pentru aplicarea BFS printre rutele date

//queue ul meu
template <class T>
struct Node {
    Node* next = nullptr;
    T value;
};

template<class T>
class Queue {
    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;

    public:
        Queue() = default;
        Queue(const Queue<T>& other);
        ~Queue();
        bool isEmpty();
        void push(T val);
        void pop();
        T front();

        Queue<T>& operator=(const Queue<T>& other);
};

// pentru typename, functiile se definesc in acelasi fisier
template <class T>
bool Queue<T>::isEmpty() {
    return head == nullptr;
}
template <class T>
void Queue<T>::push(T val) {
    Node<T>* p = new Node<T>();
    p->value = val;
    if(isEmpty())
        head = tail = p;
    else {
        tail->next = p;
        tail = p;
    }
}
template <class T>
void Queue<T>::pop() {
    if(isEmpty()) {
        std::cout << "Queue already empty before popping!\n"; //REVEZI EXCEPTIE
        return;
    }
    Node<T>* p = head;
    head = head->next;
    if(head == nullptr)
        tail = nullptr; //in cazul in care coada mai avea doar 1 element

    delete p;
}
template <class T>
T Queue<T>::front() {
    if(!isEmpty()) return head->value;
    return nullptr;
}
template <class T>
Queue<T>::~Queue() {
    Node<T>* p = head, *aux;
    while(p != nullptr) {
        aux = p;
        p = p->next;
        delete aux;
    }
}

//vreau sa le pot egala
template <class T>
Queue<T>& Queue<T>::operator=(const Queue<T>& other) {
    if(this == &other)
        return *this;

    while(!this->isEmpty())
        pop();

    Node<T>* current = other.head;
    while(current != nullptr) {
        this->push(current->value);
        current = current->next;
    }

    return *this;
}
template <typename T>
Queue<T>::Queue(const Queue<T>& other) : head(nullptr), tail(nullptr) {
    Node<T>* curr = other.head;
    while (curr != nullptr) {
        push(curr->value);
        curr = curr->next;
    }
}

// EXPLICATIE:
// memorez un graf RouteGraph printr-o lista de adiacenta cu MUCHII, nu noduri, pentru a sti ce rute trec prin fiecare 
// nod/statie; muchiile, de tipul RouteEdge (as vrea sa il schimb in edge), contin statia la care duc, dar si
// un vector cu elemente de tip RouteIndex, in care memorez un pointer catre ruta, si indexul statiei destinatie
// in cadrul rutei respective(pentru acces mai usor; presupune faptul ca nu se modifica rutele odata ce au fost create)
struct RouteIndex {
    Route* route;
    int index; 
    // indexul nodului spre care arata muchia
    // relativ la ordinea originala in care a fost initializata ruta
};

struct RouteEdge {
    std::vector<RouteIndex> routes{};
    std::string toStation{""};

    RouteEdge() = default;
    RouteEdge(std::string toStation);
    virtual ~RouteEdge() = default;

    virtual void appendRoute(Route* rp, int id);
};

struct SolEdge {
    std::string toStation{""}, routeName{"0"};
    Train* train{nullptr};
    int arrTime{0}, depTime{0};

    bool isEmpty() {return (toStation == "");}
};

class RouteGraph {
    std::unordered_map<std::string, std::vector<RouteEdge>> adjList{};

    void addEdge(std::string st1, std::string st2, Route& route, int routeIndex);
    void removeEdge(std::string st1, std::string st2, Route* route);


    public:
        // te rog... SA REZOLVI problema cu trenurile si orele compatibile.
        // maine: bfs, recreerea solutiei, BILETE...
        // (comentarii vechi, acum merge perfect (cred)
        void addRoute(Route& route);
        void removeRoute(Route *route);
        void printGraph();

        int keyHasEdge(std::string key, std::string station);
        std::vector<RouteEdge> bfs(std::string start, std::string end);

        bool checkForNode(std::string node);

        std::vector<SolEdge> DjikstraSearch(std::string start, std::string end, int time);
        std::vector<int> getMergedTimes(std::string station);

};

class TicketManager {
    static TicketManager* instance;
    TicketManager();

    bool initialized = false;
    bool b_hasTickets = false;
    bool b_hasTrains = false;

    //salvate in memorie
    RouteGraph* graph = nullptr;
    RouteTable* table = nullptr;
    std::unordered_map<int, Ticket*> tickets;

    Train* highSpeedTrain();
    Train* newHST(int depTime); // FUNCTIE APELATA DE GUI

    Train* regionalTrain();
    Train* newRT(int depTime); // FUNCTIE APELATA DE GUI

    Car* manualAddCar();
    Train* manualAddTrain();

    Ticket* buyFullPrice();
    Ticket* buyYouth();
    Ticket* buyElderly();
    Ticket* buyMembership();

    void buy(std::string start, std::vector<SolEdge> trips);

    bool tableHasTrains();
    
    public:
        static TicketManager* getInstance() {
            if(!instance) instance = new TicketManager();
            return instance;
        }
        ~TicketManager();
        
        void populateTable();
        void printRouteGraph();

        bool checkForNode(std::string node);

        void StartMenu();
        void RoutesMenu();
        void deleteGraph();
        void TicketsInfo();
        void printGraph();

        void ticketOptions();

        //functii pentru GUI
        bool addRoute(std::string routes);
        std::vector<std::string> getRoutes() const;
        std::vector<Route*> getRoutePtrs() const;

        bool addTrain(std::string route, int train, int depTime);
        std::vector<std::string> getTrains(std::string route);

        bool removeRoute(std::string route);
        bool removeTrain(std::string route, int depTime);

        std::string formatTime(int time);

        std::vector<std::vector<SolEdge>> getTicketOptionsGUI(std::string start, std::string end);
        Ticket* buyTicketGUI(std::string name, std::string id, std::string user, int type);
        bool createTrip(Ticket& ticketptr, std::string depTime, std::string arrTime, std::string routeName, std::string startSt, std::string endSt, int seat, int car, std::string carType, double price);
        std::vector<int> getTickets() const;
        std::string getTicketInfo(int id) const;
        bool hasTickets();
        bool hasTrains();
};