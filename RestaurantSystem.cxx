#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// کلاس خدمه
class Staff {
public:
    string name;
    string phone;
    double salary;
    string role;

    Staff(string n, string p, double s, string r) : name(n), phone(p), salary(s), role(r) {}
};

class StaffManagement {
private:
    vector<Staff> staffList;

public:
    void addStaff(string name, string phone, double salary, string role) {
        staffList.push_back(Staff(name, phone, salary, role));
    }

    void removeStaff(string phone) {
        for (auto it = staffList.begin(); it != staffList.end(); ++it) {
            if (it->phone == phone) {
                staffList.erase(it);
                break;
            }
        }
    }

    void updateStaff(string phone, string name, double salary, string role) {
        for (auto &staff : staffList) {
            if (staff.phone == phone) {
                staff.name = name;
                staff.salary = salary;
                staff.role = role;
                break;
            }
        }
    }

    Staff* searchStaffByName(string name) {
        for (auto &staff : staffList) {
            if (staff.name == name) {
                return &staff;
            }
        }
        return nullptr;
    }

    Staff* searchStaffByPhone(string phone) {
        for (auto &staff : staffList) {
            if (staff.phone == phone) {
                return &staff;
            }
        }
        return nullptr;
    }

    vector<Staff*> searchStaffByRole(string role) {
        vector<Staff*> result;
        for (auto &staff : staffList) {
            if (staff.role == role) {
                result.push_back(&staff);
            }
        }
        return result;
    }

    vector<Staff*> searchStaffBySalary(double salary) {
        vector<Staff*> result;
        for (auto &staff : staffList) {
            if (staff.salary == salary) {
                result.push_back(&staff);
            }
        }
        return result;
    }

    vector<Staff>& getStaffList() {
        return staffList;
    }
};

// کلاس غذا
class Food {
public:
    string name;
    double price;
    string chef;
    vector<int> ratings;

    Food(string n, double p, string c) : name(n), price(p), chef(c) {}

    double getAverageRating() {
        if (ratings.empty()) return 0;
        int sum = 0;
        for (int rating : ratings) {
            sum += rating;
        }
        return (double)sum / ratings.size();
    }
};

class FoodManagement {
private:
    vector<Food> foodList;

public:
    void addFood(string name, double price, string chef) {
        foodList.push_back(Food(name, price, chef));
    }

    void removeFood(string name) {
        for (auto it = foodList.begin(); it != foodList.end(); ++it) {
            if (it->name == name) {
                foodList.erase(it);
                break;
            }
        }
    }

    void updateFood(string name, double price, string chef) {
        for (auto &food : foodList) {
            if (food.name == name) {
                food.price = price;
                food.chef = chef;
                break;
            }
        }
    }

    Food* searchFoodByName(string name) {
        for (auto &food : foodList) {
            if (food.name == name) {
                return &food;
            }
        }
        return nullptr;
    }

    vector<Food*> filterFoodByPrice(double minPrice, double maxPrice) {
        vector<Food*> result;
        for (auto &food : foodList) {
            if (food.price >= minPrice && food.price <= maxPrice) {
                result.push_back(&food);
            }
        }
        return result;
    }

    void rateFood(string name, int rating) {
        Food* food = searchFoodByName(name);
        if (food) {
            food->ratings.push_back(rating);
        }
    }

    vector<Food*> getTopRatedFoods() {
        vector<Food*> result;
        for (auto &food : foodList) {
            if (food.getAverageRating() >= 4) { // Assuming 4 is the threshold for top-rated
                result.push_back(&food);
            }
        }
        return result;
    }

    vector<Food>& getFoodList() {
        return foodList;
    }
};

// کلاس مشتری
class Customer {
public:
    string name;
    string phone;
    int id;

    Customer(string n, string p, int i) : name(n), phone(p), id(i) {}
};

class CustomerManagement {
private:
    vector<Customer> customerList;

public:
    void addCustomer(string name, string phone, int id) {
        customerList.push_back(Customer(name, phone, id));
    }

    Customer* searchCustomerByName(string name) {
        for (auto &customer : customerList) {
            if (customer.name == name) {
                return &customer;
            }
        }
        return nullptr;
    }

    Customer* searchCustomerByID(int id) {
        for (auto &customer : customerList) {
            if (customer.id == id) {
                return &customer;
            }
        }
        return nullptr;
    }

    void updateCustomer(string name, string phone, int id) {
        for (auto &customer : customerList) {
            if (customer.id == id) {
                customer.name = name;
                customer.phone = phone;
                break;
            }
        }
    }

    bool authenticateCustomer(string name, string phone) {
        Customer* cust = searchCustomerByName(name);
        return cust && cust->phone == phone;
    }

    vector<Customer>& getCustomerList() {
        return customerList;
    }
};

// کلاس سفارش
class Order {
public:
    int orderID;
    vector<string> foodNames;
    vector<int> foodCounts;
    int customerID;
    double finalPrice;

    Order(int id, vector<string> fn, vector<int> fc, int cid, double fp) : orderID(id), foodNames(fn), foodCounts(fc), customerID(cid), finalPrice(fp) {}
};

class OrderManagement {
private:
    vector<Order> orderList;

public:
    void addOrder(int orderID, vector<string> foodNames, vector<int> foodCounts, int customerID, FoodManagement& foodMgr) {
        double totalPrice = 0;
        for (size_t i = 0; i < foodNames.size(); ++i) {
            Food* food = foodMgr.searchFoodByName(foodNames[i]);
            if (food) {
                totalPrice += food->price * foodCounts[i];
            }
        }
        orderList.push_back(Order(orderID, foodNames, foodCounts, customerID, totalPrice));
    }

    void displayOrders() {
        for (const auto& order : orderList) {
            cout << "Order ID: " << order.orderID << ", Customer ID: " << order.customerID << ", Total Price: " << order.finalPrice << "\n";
        }
    }

    vector<Order>& getOrderList() {
        return orderList;
    }
};

// کلاس مدیریت فایل‌های CSV
class CSVHandler {
public:
    static void writeStaffData(const vector<Staff>& staff, const string& filename) {
        ofstream file(filename);
        for (const auto& s : staff) {
            file << s.name << "," << s.phone << "," << s.salary << "," << s.role << "\n";
        }
        file.close();
    }

    static void readStaffData(vector<Staff>& staff, const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, phone, salary, role;
            getline(ss, name, ',');
            getline(ss, phone, ',');
            getline(ss, salary, ',');
            getline(ss, role, ',');
            staff.push_back(Staff(name, phone, stod(salary), role));
        }
        file.close();
    }

    static void writeFoodData(const vector<Food>& foods, const string& filename) {
        ofstream file(filename);
        for (const auto& f : foods) {
            file << f.name << "," << f.price << "," << f.chef << "\n";
        }
        file.close();
    }

    static void readFoodData(vector<Food>& foods, const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, price, chef;
            getline(ss, name, ',');
            getline(ss, price, ',');
            getline(ss, chef, ',');
            foods.push_back(Food(name, stod(price), chef));
        }
        file.close();
    }

    static void writeCustomerData(const vector<Customer>& customers, const string& filename) {
        ofstream file(filename);
        for (const auto& c : customers) {
            file << c.name << "," << c.phone << "," << c.id << "\n";
        }
        file.close();
    }

    static void readCustomerData(vector<Customer>& customers, const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, phone, id;
            getline(ss, name, ',');
            getline(ss, phone, ',');
            getline(ss, id, ',');
            customers.push_back(Customer(name, phone, stoi(id)));
        }
        file.close();
    }

    static void writeOrderData(const vector<Order>& orders, const string& filename) {
        ofstream file(filename);
        for (const auto& o : orders) {
            file << o.orderID << ",";
            for (size_t i = 0; i < o.foodNames.size(); ++i) {
                file << o.foodNames[i] << ":" << o.foodCounts[i];
                if (i < o.foodNames.size() - 1) file << "|";
            }
            file << "," << o.customerID << "," << o.finalPrice << "\n";
        }
        file.close();
    }

    static void readOrderData(vector<Order>& orders, const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string orderID, foodData, customerID, finalPrice;
            getline(ss, orderID, ',');
            getline(ss, foodData, ',');
            getline(ss, customerID, ',');
            getline(ss, finalPrice, ',');

            vector<string> foodNames;
            vector<int> foodCounts;
            stringstream ssFood(foodData);
            string foodItem;
            while (getline(ssFood, foodItem, '|')) {
                stringstream ssItem(foodItem);
                string foodName, foodCount;
                getline(ssItem, foodName, ':');
                getline(ssItem, foodCount, ':');
                foodNames.push_back(foodName);
                foodCounts.push_back(stoi(foodCount));
            }
            orders.push_back(Order(stoi(orderID), foodNames, foodCounts, stoi(customerID), stod(finalPrice)));
        }
        file.close();
    }
};

int main() {
    StaffManagement staffMgr;
    FoodManagement foodMgr;
    CustomerManagement custMgr;
    OrderManagement orderMgr;

    // خواندن داده‌های اولیه از فایل‌ها
    CSVHandler::readStaffData(staffMgr.getStaffList(), "staff.csv");
    CSVHandler::readFoodData(foodMgr.getFoodList(), "food.csv");
    CSVHandler::readCustomerData(custMgr.getCustomerList(), "customers.csv");
    CSVHandler::readOrderData(orderMgr.getOrderList(), "orders.csv");

    // افزودن داده‌های نمونه
    staffMgr.addStaff("Ali", "09123456789", 3000, "Chef");
    foodMgr.addFood("Pizza", 100, "Ali");
    custMgr.addCustomer("John Doe", "09987654321", 1);

    // احراز هویت مشتری
    if (custMgr.authenticateCustomer("John Doe", "09987654321")) {
        cout << "Authentication successful.\n";
    } else {
        cout << "Authentication failed.\n";
    }

    // افزودن سفارش
    vector<string> foodNames = {"Pizza"};
    vector<int> foodCounts = {2};
    orderMgr.addOrder(1, foodNames, foodCounts, 1, foodMgr);

    // نمایش سفارشات
    orderMgr.displayOrders();

    // امتیازدهی به غذا
    foodMgr.rateFood("Pizza", 5);

    // نمایش غذاهای محبوب
    vector<Food*> topFoods = foodMgr.getTopRatedFoods();
    for (const auto& food : topFoods) {
        cout << "Top-rated food: " << food->name << " with average rating: " << food->getAverageRating() << "\n";
    }

    // ذخیره داده‌ها در فایل‌ها
    CSVHandler::writeStaffData(staffMgr.getStaffList(), "staff.csv");
    CSVHandler::writeFoodData(foodMgr.getFoodList(), "food.csv");
    CSVHandler::writeCustomerData(custMgr.getCustomerList(), "customers.csv");
    CSVHandler::writeOrderData(orderMgr.getOrderList(), "orders.csv");

    return 0;
}
