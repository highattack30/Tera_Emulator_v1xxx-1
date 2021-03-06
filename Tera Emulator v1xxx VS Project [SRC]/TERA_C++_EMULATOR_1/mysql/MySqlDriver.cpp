#include "MySqlDriver.h"
#include <mysql_error.h>


MySqlDriver::MySqlDriver()
{
	_driver = 0;
	_connection = 0;
	_isValid = false;
	_connection = 0;
	
}

MySqlDriver::~MySqlDriver()
{
	if (_driver)
	{
		delete _driver;
		_driver = 0;

	}

	if (_connection)
	{
		_connection->close();
		delete _connection;
		_connection = 0;
	}
	ReleaseStatement();
}

sql::Connection * MySqlDriver::GetConnection()
{
	return _connection;
}

const bool MySqlDriver::initDriver(const char* hostAddress, const char* hostUsername, const char * hostPassword, const char* databaseName)
{
	_driver = sql::mysql::get_mysql_driver_instance(); // get driver
	if (!_driver)
		return false;

	_driver->threadInit();

	_isValid = true;

	_hostAddress = hostAddress;
	_hostUsername = hostUsername;
	_hostPassword = hostPassword;


	try
	{
		_connection = _driver->connect(hostAddress, hostUsername, hostPassword); //start connection
	}
	catch (sql::SQLException e) {}

	if (!_connection || !_connection->isValid())
		return false;

	_connection->setSchema("tera_q"); //set databse

	_statement = _connection->createStatement(); //create statement to execute mysql query


	return true;
}


void MySqlDriver::ReleaseStatement()
{
	if (_statement)
	{
		delete _statement;
		_statement = 0;
	}
}


sql::ResultSet * MySqlDriver::ExecuteQuery(const char* query)
{

	sql::ResultSet * out = 0;

	try
	{
		out = _statement->executeQuery(query);
	}

	catch (sql::SQLException e)
	{
		std::string error = e.what(); //debug here...
	}

	return out;
}

int MySqlDriver::ExecuteNonQuery(const char * query)
{
	int out = -1;

	try
	{
		out = (int)_statement->execute(query);
	}

	catch (sql::SQLException e)
	{
		std::string error = e.what();
	}

	return  out;
}

void MySqlDriver::InsertBlob(std::istream * blob)
{

}

const bool MySqlDriver::Lock()
{
	return _locked.try_lock(); //its a try to lock...you know
}

const bool MySqlDriver::Unlock()
{
	_locked.unlock();
	return true;
}
