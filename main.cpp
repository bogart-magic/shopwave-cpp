#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// Product Structure
struct Product {
    int id;
    string name;
    string category;
    double price;
    int sold;
    int dateAdded; // Lower number = newer
};

// Cart Item Structure
struct CartItem {
    Product product;
    int quantity;
};

class ShopWaveSystem {
private:
    vector<Product> products;
    vector<CartItem> cart;

    // Convert string to lowercase for case-insensitive search
    string toLower(string str) const {
        string lowerStr = str;
        transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

public:
    ShopWaveSystem() {
        // Initialize sample product database
        products = {
            {1, "Wireless Noise-Canceling Headphones", "Electronics", 2499.00, 1250, 1},
            {2, "Smart Fitness Watch Tracker", "Electronics", 1299.00, 3400, 4},
            {3, "Bluetooth Waterproof Portable Speaker", "Electronics", 999.00, 2300, 9},
            {4, "Mechanical Gaming Keyboard RGB", "Electronics", 1850.00, 1540, 10},
            {5, "Minimalist Casual Canvas Backpack", "Fashion", 799.00, 890, 5},
            {6, "Classic Oversized Cotton Hoodie", "Fashion", 899.00, 2100, 3},
            {7, "Vintage Wash Denim Jacket", "Fashion", 1299.00, 740, 15},
            {8, "Breathable Running Sneakers", "Fashion", 1499.00, 2890, 19},
            {9, "Ergonomic Desk LED Lamp with USB", "Home", 550.00, 540, 2},
            {10, "Stainless Steel Insulated Tumbler 32oz", "Home", 450.00, 4120, 6},
            {11, "Aromatherapy Essential Oil Diffuser", "Home", 699.00, 1450, 21},
            {12, "Hydrating Facial Skincare Serum", "Beauty", 399.00, 1890, 7},
            {13, "Professional Hair Styling Diffuser", "Beauty", 1150.00, 310, 8},
            {14, "Matte Liquid Lipstick Makeup Kit", "Beauty", 299.00, 4890, 27},
            {15, "Non-Slip Yoga Exercise Mat 10mm", "Sports", 650.00, 2150, 33},
            {16, "Resistance Workout Bands Set 5pcs", "Sports", 399.00, 4500, 35}
        };
    }

    // Display product list table
    void displayProducts(const vector<Product>& list) const {
        cout << "\n=================================================================\n";
        cout << left << setw(5) << "ID" 
             << setw(35) << "Product Name" 
             << setw(12) << "Category" 
             << setw(10) << "Price" 
             << setw(10) << "Sold" << endl;
        cout << "=================================================================\n";
        
        if (list.empty()) {
            cout << "No products found matching your criteria.\n";
            return;
        }

        for (const auto& p : list) {
            cout << left << setw(5) << p.id 
                 << setw(35) << (p.name.length() > 33 ? p.name.substr(0, 30) + "..." : p.name) 
                 << setw(12) << p.category 
                 << "P" << setw(9) << fixed << setprecision(2) << p.price 
                 << setw(10) << p.sold << endl;
        }
        cout << "=================================================================\n";
    }

    // Search Engine Feature
    void searchEngine() {
        cin.ignore();
        string query;
        cout << "\nEnter keyword to search (name or category): ";
        getline(cin, query);
        string lowerQuery = toLower(query);

        vector<Product> results;
        for (const auto& p : products) {
            if (toLower(p.name).find(lowerQuery) != string::npos || 
                toLower(p.category).find(lowerQuery) != string::npos) {
                results.push_back(p);
            }
        }

        cout << "\n--- Search Results for \"" << query << "\" ---\n";
        displayProducts(results);
    }

    // Sorting Feature
    void sortProducts() {
        vector<Product> sortedList = products;
        int choice;
        cout << "\n--- Sort Options ---\n";
        cout << "1. Price: Low to High\n";
        cout << "2. Price: High to Low\n";
        cout << "3. Popularity (Most Sold)\n";
        cout << "4. Newest Arrivals\n";
        cout << "Select sorting choice (1-4): ";
        cin >> choice;

        if (choice == 1) {
            sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) {
                return a.price < b.price;
            });
        } else if (choice == 2) {
            sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) {
                return a.price > b.price;
            });
        } else if (choice == 3) {
            sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) {
                return a.sold > b.sold;
            });
        } else if (choice == 4) {
            sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) {
                return a.dateAdded < b.dateAdded;
            });
        } else {
            cout << "Invalid choice. Showing default order.\n";
        }

        displayProducts(sortedList);
    }

    // Shopping Cart Add Feature
    void addToCart() {
        int id, qty;
        displayProducts(products);
        cout << "\nEnter Product ID to add to cart: ";
        cin >> id;

        auto it = find_if(products.begin(), products.end(), [id](const Product& p) {
            return p.id == id;
        });

        if (it == products.end()) {
            cout << "Invalid Product ID!\n";
            return;
        }

        cout << "Enter quantity: ";
        cin >> qty;
        if (qty <= 0) {
            cout << "Quantity must be at least 1.\n";
            return;
        }

        bool found = false;
        for (auto& item : cart) {
            if (item.product.id == id) {
                item.quantity += qty;
                found = true;
                break;
            }
        }

        if (!found) {
            cart.push_back({*it, qty});
        }

        cout << "Successfully added " << qty << "x \"" << it->name << "\" to your cart!\n";
    }

    // View Shopping Cart & Checkout
    void viewCart() {
        if (cart.empty()) {
            cout << "\nYour shopping cart is empty.\n";
            return;
        }

        cout << "\n=================================================================\n";
        cout << "                        YOUR SHOPPING CART                       \n";
        cout << "=================================================================\n";
        cout << left << setw(35) << "Product Name" 
             << setw(10) << "Price" 
             << setw(10) << "Qty" 
             << setw(12) << "Subtotal" << endl;
        cout << "-----------------------------------------------------------------\n";

        double total = 0;
        for (const auto& item : cart) {
            double subtotal = item.product.price * item.quantity;
            total += subtotal;
            cout << left << setw(35) << (item.product.name.length() > 33 ? item.product.name.substr(0, 30) + "..." : item.product.name) 
                 << "P" << setw(9) << fixed << setprecision(2) << item.product.price 
                 << setw(10) << item.quantity 
                 << "P" << setw(11) << subtotal << endl;
        }
        cout << "=================================================================\n";
        cout << "TOTAL AMOUNT: P" << fixed << setprecision(2) << total << "\n";
        cout << "=================================================================\n";

        char choice;
        cout << "Would you like to checkout? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "\n🎉 Order placed successfully! Thank you for shopping with ShopWave.\n";
            cart.clear();
        }
    }

    // Main Interactive Menu Loop
    void run() {
        int choice;
        do {
            cout << "\n=== ShopWave C++ Marketplace ===" << endl;
            cout << "1. View All Products" << endl;
            cout << "2. Search Products (Search Engine)" << endl;
            cout << "3. Sort Products (Price / Popularity)" << endl;
            cout << "4. Add Product to Cart" << endl;
            cout << "5. View Cart & Checkout" << endl;
            cout << "6. Exit" << endl;
            cout << "Enter your choice (1-6): ";
            cin >> choice;

            switch (choice) {
                case 1:
                    displayProducts(products);
                    break;
                case 2:
                    searchEngine();
                    break;
                case 3:
                    sortProducts();
                    break;
                case 4:
                    addToCart();
                    break;
                case 5:
                    viewCart();
                    break;
                case 6:
                    cout << "Thank you for using ShopWave! Goodbye.\n";
                    break;
                default:
                    cout << "Invalid choice! Please enter a number between 1 and 6.\n";
            }
        } while (choice != 6);
    }
};

int main() {
    ShopWaveSystem app;
    app.run();
    return 0;
}
