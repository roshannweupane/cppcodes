#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

// ---------------------- Utility Functions -------------------------
string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[30];
    sprintf(buffer, "%02d-%02d-%04d %02d:%02d:%02d",
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer);
}

// ---------------------- Item Class -------------------------
class Item {
public:
    int id;
    string name;
    double price;
    int quantity;

    Item() {}
    Item(int i, string n, double p, int q) {
        id = i; name = n; price = p; quantity = q;
    }

    void display() {
        cout << left << setw(5) << id
             << setw(20) << name
             << setw(10) << price
             << setw(10) << quantity << "\n";
    }
};

// ---------------------- Inventory Class -------------------------
class Inventory {
private:
    vector<Item> items;

public:
    // Load items from file
    void loadFromFile() {
        ifstream file("inventory.txt");
        if (!file) return;
        items.clear();
        int id, qty;
        string name;
        double price;
        while (file >> id >> name >> price >> qty) {
            items.push_back(Item(id, name, price, qty));
        }
        file.close();
    }

    // Save items to file
    void saveToFile() {
        ofstream file("inventory.txt");
        for (auto &i : items) {
            file << i.id << " " << i.name << " " << i.price << " " << i.quantity << "\n";
        }
        file.close();
    }

    void addItem() {
        int id, qty;
        string name;
        double price;

        cout << "\nEnter ID: ";
        cin >> id;
        cout << "Enter Name: ";
        cin >> name;
        cout << "Enter Price: ";
        cin >> price;
        cout << "Enter Quantity: ";
        cin >> qty;

        items.push_back(Item(id, name, price, qty));
        saveToFile();
        cout << "\nItem added successfully!\n";
    }

    void viewItems() {
        cout << "\n------------------ Inventory Items ------------------\n";
        cout << left << setw(5) << "ID" << setw(20) << "Name"
             << setw(10) << "Price" << setw(10) << "Qty" << "\n";
        cout << "-----------------------------------------------------\n";
        for (auto &i : items) {
            i.display();
        }
    }

    Item* searchItem(int id) {
        for (auto &i : items) {
            if (i.id == id) return &i;
        }
        return nullptr;
    }

    void updateItem() {
        int id;
        cout << "Enter Item ID to update: ";
        cin >> id;

        Item* item = searchItem(id);
        if (item == nullptr) {
            cout << "Item not found!\n";
            return;
        }

        cout << "Enter new name: ";
        cin >> item->name;
        cout << "Enter new price: ";
        cin >> item->price;
        cout << "Enter new quantity: ";
        cin >> item->quantity;

        saveToFile();
        cout << "Item updated successfully!\n";
    }

    void deleteItem() {
        int id;
        cout << "Enter ID to delete: ";
        cin >> id;

        for (int i = 0; i < items.size(); i++) {
            if (items[i].id == id) {
                items.erase(items.begin() + i);
                saveToFile();
                cout << "Item deleted!\n";
                return;
            }
        }
        cout << "Item not found!\n";
    }

    vector<Item>& getItemList() {
        return items;
    }
};

// ---------------------- Billing Class -------------------------
class Billing {
private:
    Inventory &inv;
    struct BillItem {
        string name;
        int qty;
        double price;
        double total;
    };
    vector<BillItem> bill;

public:
    Billing(Inventory &inventory) : inv(inventory) {}

    void generateBill() {
        bill.clear();
        int id, quantity;
        char choice;

        do {
            cout << "\nEnter Item ID: ";
            cin >> id;

            Item* item = inv.searchItem(id);
            if (item == nullptr) {
                cout << "Item not found!\n";
                continue;
            }

            cout << "Enter quantity: ";
            cin >> quantity;

            if (quantity > item->quantity) {
                cout << "Not enough stock!\n";
                continue;
            }

            // Reduce stock
            item->quantity -= quantity;

            bill.push_back({item->name, quantity, item->price,
                            item->price * quantity});

            cout << "Add another item? (y/n): ";
            cin >> choice;

        } while (choice == 'y' || choice == 'Y');

        inv.saveToFile();
        printBill();
    }

    void printBill() {
        double grandTotal = 0;
        cout << "\n=================== BILL ===================\n";
        cout << "Date/Time: " << getCurrentTime() << "\n";
        cout << "--------------------------------------------\n";
        cout << left << setw(20) << "Item"
             << setw(10) << "Qty"
             << setw(10) << "Price"
             << setw(10) << "Total\n";
        cout << "--------------------------------------------\n";

        for (auto &b : bill) {
            cout << left << setw(20) << b.name
                 << setw(10) << b.qty
                 << setw(10) << b.price
                 << setw(10) << b.total << "\n";
            grandTotal += b.total;
        }

        cout << "--------------------------------------------\n";
        cout << "Grand Total: Rs. " << grandTotal << "\n";
        cout << "============================================\n";
    }
};

// ---------------------- Main Menu -------------------------
int main() {
    Inventory inv;
    inv.loadFromFile();
    Billing bill(inv);

    int choice;

    while (true) {
        cout << "\n================= INVENTORY SYSTEM =================\n";
        cout << "1. Add Item\n";
        cout << "2. View Items\n";
        cout << "3. Update Item\n";
        cout << "4. Delete Item\n";
        cout << "5. Search Item\n";
        cout << "6. Generate Bill\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: inv.addItem(); break;
            case 2: inv.viewItems(); break;
            case 3: inv.updateItem(); break;
            case 4: inv.deleteItem(); break;

            case 5: {
                int id;
                cout << "Enter ID to search: ";
                cin >> id;
                Item* item = inv.searchItem(id);
                if (item) {
                    cout << "Item Found:\n";
                    item->display();
                } else cout << "Item not found!\n";
                break;
            }

            case 6: bill.generateBill(); break;

            case 7: cout << "Exiting...\n"; return 0;

            default: cout << "Invalid option!\n";
        }
    }
    return 0;
}
