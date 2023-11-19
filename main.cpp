#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>

class InventoryService {
  public: InventoryService(sqlite3 * db): db_(db) {}

  void addProduct() {
      std::string productName;
      float price;
      int supplierId;

      std::cout << "Enter product name: ";
      std::getline(std::cin >> std::ws, productName);
      std::cout << "Enter price: ";
      std::cin >> price;
      std::cout << "Enter supplier id: ";
      std::cin >> supplierId;

      // Implement logic to add a product to the database
      executeQuery("INSERT INTO Products (name, price, supplier_id) VALUES (?, ?, ?);", productName, price, supplierId);

      std::cout << "Product added successfully.\n";
  }

  void removeProduct() {
      int productId;

      std::cout << "Enter product id to remove: ";
      std::cin >> productId;

      // Implement logic to remove a product from the database
      executeQuery("DELETE FROM Products WHERE id = ?;", productId);

      std::cout << "Product removed successfully.\n";
  }

  // TODO: Fix list products
  void listProducts() {
    std::cout << "Product List:\n";
    std::cout << "ID - Name - Price\n";

    executeQuery("SELECT id, name, price FROM Products;", [](void* data, int argc, char** argv, char** /*azColName*/) -> int {
        for (int i = 0; i < argc; ++i) {
            std::cout << argv[i];
            if (i < argc - 1) {
                std::cout << " - ";
            }
        }
        std::cout << "\n";
        return 0;
    });
  }

  void addSupplier() {
      std::string supplierName;

      std::cout << "Enter supplier name: ";
      std::getline(std::cin >> std::ws, supplierName);

      // Implement logic to add a supplier to the database
      executeQuery("INSERT INTO Suppliers (name) VALUES (?);", supplierName);

      std::cout << "Supplier added successfully.\n";
  }

  void removeSupplier() {
      int supplierId;

      std::cout << "Enter supplier id to remove: ";
      std::cin >> supplierId;

      // Implement logic to remove a supplier from the database
      executeQuery("DELETE FROM Suppliers WHERE id = ?;", supplierId);

      std::cout << "Supplier removed successfully.\n";
  }

  private: sqlite3 * db_;
  // Utility function to execute SQL queries
  template <typename... Args>
  void executeQuery(const std::string& query, const Args&... args) const {
      char* errorMessage = nullptr;
      std::string formattedQuery = formatQuery(query, args...);
      int rc = sqlite3_exec(db_, formattedQuery.c_str(), nullptr, nullptr, &errorMessage);
      if (rc != SQLITE_OK) {
          std::cerr << "SQLite error: " << errorMessage << std::endl;
          sqlite3_free(errorMessage);
      }
  }

  // Utility function to format SQL queries with parameters
  template <typename... Args>
  std::string formatQuery(const std::string& query, const Args&... args) const {
      std::stringstream formattedQuery;
      int argIndex = 0;
      for (char ch : query) {
          if (ch == '?') {
              formattedQuery << argsToSql(args...)[argIndex++];
          } else {
              formattedQuery << ch;
          }
      }
      return formattedQuery.str();
  }

  // Utility function to convert arguments to SQL string representations
  template <typename... Args>
  std::vector<std::string> argsToSql(const Args&... args) const {
      return { toString(args)... };
  }

  // Utility function to convert a value to a string
  template <typename T>
  std::string toString(const T& value) const {
      std::stringstream ss;
      if constexpr (std::is_arithmetic<T>::value) {
          ss << value;
      } else {
          ss << "'" << value << "'";
      }
      return ss.str();
  }
};

class OrderService {
  public: OrderService(sqlite3 * db): db_(db) {}

  void createOrder() {
      int productId;
      int customerId;
      float totalPrice;

      std::cout << "Enter product id: ";
      std::cin >> productId;
      std::cout << "Enter customer id: ";
      std::cin >> customerId;
      std::cout << "Enter total price: ";
      std::cin >> totalPrice;

      // Implement logic to add a product to the database
      executeQuery("INSERT INTO Orders (product_id, customer_id, total_price) VALUES (?, ?, ?);", productId, customerId, totalPrice);

      std::cout << "Created order successfully.\n";
  }

  void removeOrder() {
    int orderId;

    std::cout << "Enter product id to remove: ";
    std::cin >> orderId;

    // Implement logic to remove a product from the database
    executeQuery("DELETE FROM Orders WHERE id = ?;", orderId);

    std::cout << "Order removed successfully.\n";
  }

  void listOrders() {
    std::cout << "List all orders:\n";
    executeQuery("SELECT * FROM Orders;", [](void* data, int argc, char** argv, char** /*azColName*/) -> int {
        for (int i = 0; i < argc; ++i) {
            std::cout << argv[i] << ": " << argv[i + 1] << "\n";
            ++i; // Skip the value
        }
        return 0;
    });
  }

  private: sqlite3 * db_;
  // Utility function to execute SQL queries
  template <typename... Args>
  void executeQuery(const std::string& query, const Args&... args) const {
      char* errorMessage = nullptr;
      std::string formattedQuery = formatQuery(query, args...);
      int rc = sqlite3_exec(db_, formattedQuery.c_str(), nullptr, nullptr, &errorMessage);
      if (rc != SQLITE_OK) {
          std::cerr << "SQLite error: " << errorMessage << std::endl;
          sqlite3_free(errorMessage);
      }
  }

  // Utility function to format SQL queries with parameters
  template <typename... Args>
  std::string formatQuery(const std::string& query, const Args&... args) const {
      std::stringstream formattedQuery;
      int argIndex = 0;
      for (char ch : query) {
          if (ch == '?') {
              formattedQuery << argsToSql(args...)[argIndex++];
          } else {
              formattedQuery << ch;
          }
      }
      return formattedQuery.str();
  }

  // Utility function to convert arguments to SQL string representations
  template <typename... Args>
  std::vector<std::string> argsToSql(const Args&... args) const {
      return { toString(args)... };
  }

  // Utility function to convert a value to a string
  template <typename T>
  std::string toString(const T& value) const {
      std::stringstream ss;
      if constexpr (std::is_arithmetic<T>::value) {
          ss << value;
      } else {
          ss << "'" << value << "'";
      }
      return ss.str();
  }
};

class AnalyticsService {
  public: AnalyticsService(sqlite3 * db): db_(db) {}

  void getTotalEarned() {
      // Implement logic to get total earned from orders
      float totalEarned = fetchTotalEarned();
      std::cout << "Total Earned: $" << totalEarned << std::endl;
  }

  void getTotalOrderCount() {
      // Implement logic to get total count of orders
      int totalOrders = fetchTotalOrderCount();
      std::cout << "Total Orders: " << totalOrders << std::endl;
  }

  private: sqlite3 * db_;
  float fetchTotalEarned() {
      float totalEarned = 0.0;
      executeQuery("SELECT SUM(total_price) FROM Orders;", [&totalEarned](void* data, int argc, char** argv, char** /*azColName*/) -> int {
          if (argc > 0 && argv[0] != nullptr) {
              totalEarned = std::stof(argv[0]);
          }
          return 0;
      });

      return totalEarned;
  }

  int fetchTotalOrderCount() {
      int totalOrders = 0;
      executeQuery("SELECT COUNT(*) FROM Orders;", [&totalOrders](void* data, int argc, char** argv, char** /*azColName*/) -> int {
          if (argc > 0 && argv[0] != nullptr) {
              totalOrders = std::stoi(argv[0]);
          }
          return 0;
      });

      return totalOrders;
  }
  // Utility function to execute SQL queries
  template <typename... Args>
  void executeQuery(const std::string& query, const Args&... args) const {
      char* errorMessage = nullptr;
      int rc = sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &errorMessage);
      if (rc != SQLITE_OK) {
          std::cerr << "SQLite error: " << errorMessage << std::endl;
          sqlite3_free(errorMessage);
      }
  }
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
      std::cout << "1. Create new order\n";
      std::cout << "2. Remove an order by id\n";
      std::cout << "3. List Orders\n";
      std::cout << "0. Back to Main Menu\n";
      std::cout << "Enter your choice: ";
      std::cin >> orderChoice;

      switch (orderChoice) {
      case 1:
        orderService_ -> createOrder();
        break;
      case 2:
        orderService_ -> removeOrder();
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