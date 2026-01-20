#include <bits/stdc++.h>
using namespace std;

enum class VehicleType { BIKE, CAR };
enum class VehicleStatus { AVAILABLE, RESERVERD, RENTED, MAINTAINENCE };
enum class ReservationStatus { SCHEDULED, COMPLETED, IN_PROGRESS, CANCELLED };
enum class PaymentMethod { CASH, CARD, UPI };
enum class PaymentStatus { UNPAID, PAID, FAILED };

static long long nextid() {
    static long long id = 1000;
    return id++;
}
static bool overlaps(time_t aFrom, time_t aTo, time_t bFrom, time_t bTo) {
    return max(aFrom, bFrom) < min(aTo, bTo);
}
static time_t makeDate(int y, int m, int d, int hh = 0, int mm = 0, int ss = 0) {
    tm t{};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = hh;
    t.tm_min = mm;
    t.tm_sec = ss;
    t.tm_isdst = -1;
    return mktime(&t);
}

class Location {
    string address, city, state, zip;
public:
    Location() {}
    Location(string addr, string c, string s, string z)
        : address(std::move(addr)), city(std::move(c)), state(std::move(s)), zip(std::move(z)) {}

    string getAddress() const { return address; }
    string getCity() const { return city; }
    string getState() const { return state; }
    string getZip() const { return zip; }
};

class User {
    string userId;
    string driverLicence;
public:
    User() {}
    User(string userid, string driverlicence)
        : userId(std::move(userid)), driverLicence(std::move(driverlicence)) {}

    string getUserId() const { return userId; }
    string getDriverLicence() const { return driverLicence; }
};

class Vechicle {
    string vehicleId;
    VehicleStatus vehStatus;
    VehicleType vehType;
public:
    Vechicle(string id, VehicleType t) : vehicleId(std::move(id)), vehStatus(VehicleStatus::AVAILABLE), vehType(t) {}

    string getVehilceId() const { return vehicleId; }
    VehicleStatus getVehicleStatus() const { return vehStatus; }
    VehicleType getVehicleType() const { return vehType; }

    void setStatus(VehicleStatus s) { vehStatus = s; }
};

class Reservation {
    string reservationId;
    string userId;
    string vehilceId;
    int storeId;

    time_t createdAt;
    time_t bookingFrom;
    time_t bookingTo;

    ReservationStatus status;

public:
    Reservation(string uid, string vid, int sid, time_t from, time_t to)
        : reservationId("R" + to_string(nextid())),
          userId(std::move(uid)),
          vehilceId(std::move(vid)),
          storeId(sid),
          createdAt(time(nullptr)),
          bookingFrom(from),
          bookingTo(to),
          status(ReservationStatus::SCHEDULED) {}

    string getReservationId() const { return reservationId; }
    string getUserId() const { return userId; }
    string getVehicleId() const { return vehilceId; }
    int getStoreId() const { return storeId; }

    time_t getBookingFrom() const { return bookingFrom; }
    time_t getBookingTo() const { return bookingTo; }

    bool blocksAvailability() const {
        return status == ReservationStatus::SCHEDULED || status == ReservationStatus::IN_PROGRESS;
    }

    void start() {
        if (status != ReservationStatus::SCHEDULED)
            throw runtime_error("Reservation can start only from SCHEDULED");
        status = ReservationStatus::IN_PROGRESS;
    }

    void complete() {
        if (status != ReservationStatus::IN_PROGRESS)
            throw runtime_error("Reservation can complete only from IN_PROGRESS");
        status = ReservationStatus::COMPLETED;
    }

    void cancel() {
        if (status == ReservationStatus::IN_PROGRESS || status == ReservationStatus::COMPLETED)
            throw runtime_error("Cannot cancel IN_PROGRESS/COMPLETED reservation");
        status = ReservationStatus::CANCELLED;
    }
};

class Bill {
    string billId;
    string reservationId;
    double amount;
    PaymentStatus paystatus;
public:
    Bill(string resId, double amt)
        : billId("B" + to_string(nextid())),
          reservationId(std::move(resId)),
          amount(amt),
          paystatus(PaymentStatus::UNPAID) {}

    string getBillId() const { return billId; }
    string getReservationId() const { return reservationId; }
    double getAmount() const { return amount; }
    PaymentStatus getpaymentStatus() const { return paystatus; }

    void pay(PaymentMethod) { paystatus = PaymentStatus::PAID; }
};

class VehilceInventory {
    unordered_map<string, shared_ptr<Vechicle>> byId;
public:
    void addVechicle(const shared_ptr<Vechicle>& v) {
        byId[v->getVehilceId()] = v;
    }

    shared_ptr<Vechicle> getVechicleById(const string& id) {
        auto it = byId.find(id);
        if (it == byId.end()) return nullptr;
        return it->second;
    }

    vector<shared_ptr<Vechicle>> getAllVehilces() {
        vector<shared_ptr<Vechicle>> res;
        res.reserve(byId.size());
        for (auto &kv : byId) res.push_back(kv.second);
        return res;
    }
};

class Store {
    int storeId;
    Location location;
    VehilceInventory inventory;

    unordered_map<string, shared_ptr<Reservation>> reservations;
    unordered_map<string, shared_ptr<Bill>> bills;

public:
    Store(int id, Location loc) : storeId(id), location(std::move(loc)) {}

    int getStore() const { return storeId; }
    VehilceInventory& getInventory() { return inventory; }

    bool isVehicleAvailable(const string& vehicleId, time_t from, time_t to) {
        for (auto &kv : reservations) {
            auto &r = kv.second;
            if (r->getVehicleId() != vehicleId) continue;
            if (!r->blocksAvailability()) continue;
            if (overlaps(r->getBookingFrom(), r->getBookingTo(), from, to)) return false;
        }
        return true;
    }

    vector<shared_ptr<Vechicle>> SearchAvailableVechicles(VehicleType type, time_t from, time_t to) {
        vector<shared_ptr<Vechicle>> res;
        for (auto &v : inventory.getAllVehilces()) {
            if (v->getVehicleType() != type) continue;
            if (v->getVehicleStatus() == VehicleStatus::MAINTAINENCE) continue;

            if (isVehicleAvailable(v->getVehilceId(), from, to)) {
                res.push_back(v);
            }
        }
        return res;
    }

    
    shared_ptr<Reservation> getReservation(const string& reservationId) {
        auto it = reservations.find(reservationId);
        if (it == reservations.end()) return nullptr;
        return it->second;
    }

    void startReservation(const string& reservationId) {
        auto r = getReservation(reservationId);
        if (!r) throw runtime_error("Reservation not Found");

        auto v = inventory.getVechicleById(r->getVehicleId());
        if (!v) throw runtime_error("Vehicle not Found");

        r->start();
        v->setStatus(VehicleStatus::RENTED);
    }

    shared_ptr<Reservation> createReservation(const string& userId, const string& vehicleId, time_t from, time_t to) {
        auto v = inventory.getVechicleById(vehicleId);
        if (!v) throw runtime_error("Vehicle Not Found");

        if (from >= to) throw runtime_error("Invalid Booking Window");
        if (v->getVehicleStatus() == VehicleStatus::MAINTAINENCE) throw runtime_error("Vehicle is in Maintainence");
        if (!isVehicleAvailable(vehicleId, from, to)) throw runtime_error("Vehicle is not available for these dates");

        auto r = make_shared<Reservation>(userId, vehicleId, storeId, from, to);
        reservations[r->getReservationId()] = r;

        v->setStatus(VehicleStatus::RESERVERD);
        return r;
    }

    void cancelReservation(const string& reservationId) {
        auto r = getReservation(reservationId);
        if (!r) throw runtime_error("Reservation not Found");

        auto v = inventory.getVechicleById(r->getVehicleId());
        if (!v) throw runtime_error("Vehicle not Found");

        r->cancel();
        v->setStatus(VehicleStatus::AVAILABLE);
    }

    shared_ptr<Bill> completeReservation(const string& reservationId) {
        auto r = getReservation(reservationId);
        if (!r) throw runtime_error("Reservation not Found");

        auto v = inventory.getVechicleById(r->getVehicleId());
        if (!v) throw runtime_error("Vehicle not Found");

        r->complete();
        v->setStatus(VehicleStatus::AVAILABLE);

        double seconds = difftime(r->getBookingTo(), r->getBookingFrom());
        double days = max(1.0, ceil(seconds / (24.0 * 3600.0)));
        double amount = days * 500.0;

        auto bill = make_shared<Bill>(r->getReservationId(), amount);
        bills[bill->getBillId()] = bill;
        return bill;
    }

    shared_ptr<Bill> getBill(const string& billId) {
        auto it = bills.find(billId);
        if (it == bills.end()) return nullptr;
        return it->second;
    }
};

class RentalSystem {
    unordered_map<string, shared_ptr<User>> users;
    unordered_map<int, shared_ptr<Store>> stores;

public:
    void addUser(shared_ptr<User> user) {
        users[user->getUserId()] = std::move(user);
    }

    void addStore(shared_ptr<Store> s) {
        stores[s->getStore()] = std::move(s);
    }

    shared_ptr<Store> getStore(int storeId) {
        auto it = stores.find(storeId);
        if (it == stores.end()) return nullptr;
        return it->second;
    }

    shared_ptr<User> getUser(const string& userId) {
        auto it = users.find(userId);
        if (it == users.end()) return nullptr;
        return it->second;
    }

    vector<shared_ptr<Vechicle>> searchVechilceInStore(int storeId, VehicleType type, time_t from, time_t to) {
        auto store = getStore(storeId);
        if (!store) throw runtime_error("Store not Present");
        return store->SearchAvailableVechicles(type, from, to);
    }

    shared_ptr<Reservation> createReservation(int storeId, const string& userId, const string& vehicleId, time_t from, time_t to) {
        auto u = getUser(userId);
        if (!u) throw runtime_error("User Not found");

        auto store = getStore(storeId);
        if (!store) throw runtime_error("Store Not found");

        return store->createReservation(userId, vehicleId, from, to);
    }

    void startReservation(int storeId, const string& reservationId) {
        auto store = getStore(storeId);
        if (!store) throw runtime_error("Store not found");
        store->startReservation(reservationId);
    }

    shared_ptr<Bill> completeReservation(int storeId, const string& reservationId) {
        auto store = getStore(storeId);
        if (!store) throw runtime_error("Store not found");
        return store->completeReservation(reservationId);
    }

    void cancelReservation(int storeId, const string& reservationId) {
        auto store = getStore(storeId);
        if (!store) throw runtime_error("Store not found");
        store->cancelReservation(reservationId);
    }
};

int main() {
    try {
        RentalSystem system;
        system.addUser(make_shared<User>("ID1", "DL-1234"));

        auto store = make_shared<Store>(1, Location("12 MG Road", "Bangalore", "KA", "25155"));
        system.addStore(store);

        store->getInventory().addVechicle(make_shared<Vechicle>("V1", VehicleType::CAR));
        store->getInventory().addVechicle(make_shared<Vechicle>("V2", VehicleType::CAR));
        store->getInventory().addVechicle(make_shared<Vechicle>("V3", VehicleType::BIKE));

        time_t from = makeDate(2026, 1, 21);
        time_t to   = makeDate(2026, 1, 23);

        cout << "Searching Available Cars\n";
        auto cars = system.searchVechilceInStore(1, VehicleType::CAR, from, to);

        for (auto &v : cars) {
            cout << "  VehicleId=" << v->getVehilceId() << "\n";
        }

        if (cars.empty()) {
            cout << "NO Car Available\n";
            return 0;
        }

        string chosenVehicleId = cars[0]->getVehilceId();
        auto res = system.createReservation(1, "ID1", chosenVehicleId, from, to);

        cout << "\nReservation Created " << res->getReservationId()
             << " for vehicle " << res->getVehicleId() << "\n";

        system.startReservation(1, res->getReservationId());
        cout << "Reservation started\n";

        auto bill = system.completeReservation(1, res->getReservationId());
        cout << "Reservation Completed. BillId=" << bill->getBillId()
             << " Amount=" << bill->getAmount() << "\n";

        bill->pay(PaymentMethod::UPI);
        cout << "Bill Paid\n";

    } catch (const exception &e) {
        cout << "ERROR: " << e.what() << "\n";
    }
    return 0;
}
