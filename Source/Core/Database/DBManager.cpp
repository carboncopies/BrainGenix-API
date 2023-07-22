#include <Database/DBManager.h>


namespace BG {
namespace API {
namespace DB {

Manager::Manager()
    : driver_(nullptr), connection_(nullptr), statement_(nullptr), result_(nullptr) {
    try {
        driver_ = get_driver_instance();
        connection_ = driver_->connect("tcp://your_mysql_host:your_mysql_port", "your_mysql_username", "your_mysql_password");
        connection_->setSchema("your_database_name");
        statement_ = connection_->createStatement();
    } catch (sql::SQLException& e) {
        // Handle the connection error if needed.
    }
}

Manager::~Manager() {
    // Make sure to release resources and close the connection.
    if (result_) {
        delete result_;
        result_ = nullptr;
    }

    if (statement_) {
        delete statement_;
        statement_ = nullptr;
    }

    if (connection_) {
        connection_->close();
        delete connection_;
        connection_ = nullptr;
    }
}


}; // Close Namespace DB
}; // Close Namespace API
}; // Close Namespace BG