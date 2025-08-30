#pragma once
#include <memory>
#include <BG/Common/Logger/Logger.h>
#include <Config/Config.h>
#include <Server/Server.h>

void StartClusterRPC(BG::Common::Logger::LoggingSystem*,
                     Config::Config*,
                     Server::Server*);