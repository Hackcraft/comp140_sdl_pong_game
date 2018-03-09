#include "stdafx.h"
#include "SerialInterface.h"
#include <vector>
#include <iostream>

using std::cout;
using std::vector;
using std::exception;
using std::stoi;
using std::string;

SerialInterface::SerialInterface()
{
	vector <serial::PortInfo> devicesFound = serial::list_ports();

	vector <serial::PortInfo>::iterator iter = devicesFound.begin();

	while (iter != devicesFound.end())
	{
		serial::PortInfo device = *iter++;
		string port = device.port.c_str();

		try
		{
			mySerial = new serial::Serial(port, 115200, serial::Timeout::simpleTimeout(250));

			if (mySerial->isOpen())
			{
				cout << "Connection success: " << port << std::endl;
				connected = true;

				break;
			}
		}
		catch (exception &e)
		{

		}
	}
}


SerialInterface::~SerialInterface()
{
}

void SerialInterface::send(std::string msg)
{
	if (connected)
	{
		mySerial->write(msg);
	}
}

void SerialInterface::getPositions()
{
	if (connected)
	{
		mySerial->write("P");

		string result = mySerial->readline();

		if (result.length() >= 9) {

			// 0000-0000
			string sub1 = result.substr(0, 4);
			string sub2 = result.substr(5, 9);

			// Convert string to interger
			pot1 = std::stoi(sub1);
			pot2 = std::stoi(sub2);
		}
	}
}

void SerialInterface::close()
{
	mySerial->flush();
	mySerial->close();
}
