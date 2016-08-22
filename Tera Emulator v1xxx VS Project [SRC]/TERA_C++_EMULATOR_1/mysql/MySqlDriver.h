#ifndef MYSQL_DRIVER_H
#define MYSQL_DRIVER_H

#include <vector>
#include <mutex>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <mysql_error.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>
#include <cppconn\datatype.h>

#define MAX_CLIENTS_PER_CONNECTION 50

class MySqlDriver
{
public:
	MySqlDriver();
	~MySqlDriver();

public:
	sql::Connection * GetConnection();

	const bool initDriver(const char* hostAddress, const char* hostUsername, const char * hostPassword, const char* databaseName);

	sql::ResultSet* ExecuteQuery(const char* query);
	int ExecuteNonQuery(const char* querry);
	void InsertBlob(std::istream * blob);
	const bool Lock();
	const bool Unlock();

private:
	void ReleaseStatement();


	sql::mysql::MySQL_Driver * _driver; 
	sql::Connection * _connection; 
	sql::Statement *_statement; 

	bool _isValid = false;

	const char * _hostAddress;
	const char * _hostUsername;
	const char * _hostPassword;
	std::mutex _locked;
};
#endif
