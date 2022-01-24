#pragma once

#include <iostream>
#include <memory>
#include <fstream>

#include "mqtt/async_client.h"

const std::string DFLT_SERVER_ADDRESS { "tcp://localhost:1883" };

const int QOS__ = 1;

//very simple mqtt server sending messages to choosen topics.
class MqttServer
{
public :

MqttServer(const std::string adress = DFLT_SERVER_ADDRESS, const int & qos = QOS__, const std::string topic = "maze");

bool Publish(const std::string & adress, const std::string msg);
bool Publish(const std::string msg);

private : 
int qos_ = 1;
mqtt::async_client cli_{DFLT_SERVER_ADDRESS, ""};
std::string topic_;

};