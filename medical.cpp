#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>

using namespace std;

#define HASH_SIZE 100

// Structure to represent an inventory item
class InventoryItem {
public:
    int product_id;
    string name;
    int quantity;
    float price;
    string expiration_date;

    InventoryItem(int id, string n, int q, float p, string exp) {
        product_id = id;
        name = n;
        quantity = q;
        price = p;
        expiration_date = exp;
    }
};

// Structure to represent a customer order
class CustomerOrder {
public:
    string name;
    int product_id;
    int quantity;
    int urgency; // Priority of the order

    CustomerOrder(string n, int id, int q, int u) {
        name = n;
        product_id = id;
        quantity = q;
        urgency = u;
    }

    // Custom comparator for priority queue (higher urgency gets processed first)
    bool operator<(const CustomerOrder &o) const {
        return urgency < o.urgency;
    }
};

// Global variable to generate product IDs
int next_product_id = 1;

// Hash map to store inventory items
unordered_map<int, InventoryItem*> inventory;

// Priority queue for customer orders
priority_queue<CustomerOrder> orderQueue;

// Function prototypes
void addItem();
void displayInventory();
void placeOrder();
void processOrders();

int main() {
    int choice;

    do {
        cout << "\nMedical Store Inventory Management System\n";
        cout << "1. Add Item\n";
        cout << "2. Display Inventory\n";
        cout << "3. Place Order\n";
        cout << "4. Process Orders\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                displayInventory();
                break;
            case 3:
                placeOrder();
                break;
            case 4:
                processOrders();
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    // Free allocated memory
    for (auto &item : inventory) {
        delete item.second;
    }

    return 0;
}

// Function to add an item to the inventory
void addItem() {
    string name, expiration_date;
    int quantity;
    float price;

    cout << "Enter item name: ";
    cin >> name;
    cout << "Enter quantity: ";
    cin >> quantity;
    cout << "Enter price: ";
    cin >> price;
    cout << "Enter expiration date: ";
    cin >> expiration_date;

    int product_id = next_product_id++;
    inventory[product_id] = new InventoryItem(product_id, name, quantity, price, expiration_date);

    cout << "Item added successfully. Product ID: " << product_id << endl;
}

// Function to display the current inventory
void displayInventory() {
    cout << "\nCurrent Inventory:\n";
    cout << "-----------------------------------------------------------\n";
    cout << "ID       Name                 Quantity   Price    Expiration Date\n";
    cout << "-----------------------------------------------------------\n";

    for (const auto &pair : inventory) {
        InventoryItem *item = pair.second;
        cout << item->product_id << "   " << item->name << "      " << item->quantity
             << "        " << item->price << "      " << item->expiration_date << endl;
    }
}

// Function to place an order
void placeOrder() {
    string customerName;
    int productId, quantity, urgency;

    cout << "Enter customer name: ";
    cin >> customerName;
    cout << "Enter product ID: ";
    cin >> productId;
    cout << "Enter quantity: ";
    cin >> quantity;
    cout << "Enter order urgency (1 - Low, 2 - Medium, 3 - High): ";
    cin >> urgency;

    // Check if product exists in inventory
    if (inventory.find(productId) == inventory.end()) {
        cout << "Product with ID " << productId << " not found in inventory. Order cannot be placed.\n";
        return;
    }

    InventoryItem *item = inventory[productId];

    if (quantity > item->quantity) {
        cout << "Requested quantity exceeds available stock. Order cannot be placed.\n";
        return;
    }

    // Place order in priority queue
    orderQueue.push(CustomerOrder(customerName, productId, quantity, urgency));
    cout << "Order placed successfully.\n";
}

// Function to process orders
void processOrders() {
    if (orderQueue.empty()) {
        cout << "No orders to process.\n";
        return;
    }

    CustomerOrder processedOrder = orderQueue.top();
    orderQueue.pop();
    cout << "Processing order for customer: " << processedOrder.name << endl;

    if (inventory.find(processedOrder.product_id) != inventory.end()) {
        InventoryItem *item = inventory[processedOrder.product_id];

        if (item->quantity >= processedOrder.quantity) {
            item->quantity -= processedOrder.quantity;
            cout << "Order processed successfully.\n";
        } else {
            cout << "Insufficient stock. Order cannot be fulfilled.\n";
        }
    } else {
        cout << "Product not found in inventory.\n";
    }
}
