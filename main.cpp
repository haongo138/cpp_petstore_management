#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <memory>

class InventoryService {
  public: InventoryService(sqlite3 * db): db_(db) {}

  void addProduct() {
    std::cout << "Add a product\n";
  }

  void removeProduct() {
    std::cout << "Remove a product\n";
  }

  void listProducts() {
    std::cout << "List all products\n";
  }

  void addSupplier() {
    std::cout << "Add a supplier\n";
  }

  void removeSupplier() {
    std::cout << "Remove a supplier\n";
  }

  private: sqlite3 * db_;
};

class OrderService {
  public: OrderService(sqlite3 * db): db_(db) {}
  void createOrder() {
    std::cout << "Create an order\n";
  }

  void updateStatus() {
    std::cout << "Update order status\n";
  }

  void listOrders() {
    std::cout << "List all orders\n";
  }

  private: sqlite3 * db_;
};

class AnalyticsService {
  public: AnalyticsService(sqlite3 * db): db_(db) {}

  void getTotalEarned() {
    std::cout << "Get total earned from orders\n";
  }

  void getTotalOrderCount() {
    std::cout << "Get total count of orders\n";
  }

  private: sqlite3 * db_;
};

class StoreGateway {
  public: StoreGateway() {
      if (sqlite3_open("petstore.db", & db_) != SQLITE_OK) {
        std::cerr << "Failed to open the database." << std::endl;
        exit(EXIT_FAILURE);
      }

      // Instantiate InventoryService after the database is opened
      inventoryService_ = std::make_unique < InventoryService > (db_);
      orderService_ = std::make_unique < OrderService > (db_);
      analyticsService_ = std::make_unique < AnalyticsService > (db_);
    }

    ~StoreGateway() {
      sqlite3_close(db_);
    }

  void run() {
    int choice;
    do {
      std::cout << "\nPet Store Management System\n";
      std::cout << "1. Inventory Management\n";
      std::cout << "2. Order Management\n";
      std::cout << "3. Analytics\n";
      std::cout << "0. Exit\n";
      std::cout << "Enter your choice: ";
      std::cin >> choice;
      switch (choice) {
      case 1:
        showInventorySubMenu();
        break;
      case 2:
        showOrderSubMenu();
        break;
      case 3:
        showAnalyticsSubMenu();
        break;
      case 0:
        std::cout << "Exiting program.\n";
        break;
      default:
        std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 0);
  }

  private: sqlite3 * db_;
  std::unique_ptr < InventoryService > inventoryService_;
  std::unique_ptr < OrderService > orderService_;
  std::unique_ptr < AnalyticsService > analyticsService_;

  void showInventorySubMenu() {
    int inventoryChoice;
    do {
      std::cout << "\nInventory Management\n";
      std::cout << "1. Add Product\n";
      std::cout << "2. Remove Product\n";
      std::cout << "3. List Products\n";
      std::cout << "4. Add Supplier\n";
      std::cout << "5. Remove Supplier\n";
      std::cout << "0. Back to Main Menu\n";
      std::cout << "Enter your choice: ";
      std::cin >> inventoryChoice;

      switch (inventoryChoice) {
      case 1:
        inventoryService_ -> addProduct();
        break;
      case 2:
        inventoryService_ -> removeProduct();
        break;
      case 3:
        inventoryService_ -> listProducts();
        break;
      case 4:
        inventoryService_ -> addSupplier();
        break;
      case 5:
        inventoryService_ -> removeSupplier();
        break;
      case 0:
        std::cout << "Returning to Main Menu.\n";
        break;
      default:
        std::cout << "Invalid choice. Please try again.\n";
      }
    } while (inventoryChoice != 0);
  }

  void showOrderSubMenu() {
    int orderChoice;
    do {
      std::cout << "\nOrder Management\n";
      std::cout << "1. Create Order\n";
      std::cout << "2. Update Order Status\n";
      std::cout << "3. List Orders\n";
      std::cout << "0. Back to Main Menu\n";
      std::cout << "Enter your choice: ";
      std::cin >> orderChoice;

      switch (orderChoice) {
      case 1:
        orderService_ -> createOrder();
        break;
      case 2:
        orderService_ -> updateStatus();
        break;
      case 3:
        orderService_ -> listOrders();
        break;
      case 0:
        std::cout << "Returning to Main Menu.\n";
        break;
      default:
        std::cout << "Invalid choice. Please try again.\n";
      }
    } while (orderChoice != 0);
  }

  void showAnalyticsSubMenu() {
    int analyticsChoice;
    do {
      std::cout << "\nAnalytics\n";
      std::cout << "1. Get Total Earned\n";
      std::cout << "2. Get Total Order Count\n";
      std::cout << "0. Back to Main Menu\n";
      std::cout << "Enter your choice: ";
      std::cin >> analyticsChoice;

      switch (analyticsChoice) {
      case 1:
        analyticsService_ -> getTotalEarned();
        break;
      case 2:
        analyticsService_ -> getTotalOrderCount();
        break;
      case 0:
        std::cout << "Returning to Main Menu.\n";
        break;
      default:
        std::cout << "Invalid choice. Please try again.\n";
      }
    } while (analyticsChoice != 0);
  }
};

int main() {
  StoreGateway store;
  store.run();
  return 0;
}