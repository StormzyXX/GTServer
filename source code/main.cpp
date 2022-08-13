#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <signal.h>
#include <filesystem>

//static libraries:
#include <enet/include/enet.h>
#include <fmt/include/core.h>

#include <SFML/Graphics.hpp>

//server struct
//players:
#include <NetAvatar/NetAvatar.hpp>

//events:
#include <events/events.hpp>
#include <events/text/text_events.hpp>

//server:
#include <server/server.hpp>
#include <server/pool.hpp>

//database:
#include <database/database.hpp>

//items:
#include <database/itemdb/itemdb.hpp>
//tribute:
#include <database/player_tribute/tribute_data.hpp>

using namespace svr;
pool* g_servers;
events* g_event_manager;
std::vector<std::thread*> g_threads;
database* g_database;

void exit_handler(int a) 
{
    for (auto& pair : g_servers->get_servers())
        g_servers->stop_instance(pair.first);
    exit(EXIT_SUCCESS);
}
bool onetimeinit(){
return enet_initialize() == 0;
}
int main() {
    fmt::print("starting server...\n");
    signal(SIGINT, exit_handler);

    {
       onetimeinit();
        g_servers = new pool();
        fmt::print("server pool started!\n");
    }

    fmt::print("initializing database...\n"); {
        g_database->init();
        fmt::print("server data -> {}.\n", g_database->serialize_server_data(g_servers) ? fmt::format("successed, players: {}", g_servers->get_user_id()) : "failed");
    }

    fmt::print("initializing items.dat...\n"); {
        fmt::print("items.dat serialization -> {}.\n", item_database::instance().init() ? fmt::format("successed, hash: {}", item_database::instance().get_hash()) : "failed");
    }

    fmt::print("initializing player tribute...\n"); {
        player_tribute::init();
    }

    fmt::print("loading events...\n"); 
    {
        g_event_manager = new events();
        g_event_manager->load_events();
        fmt::print("events pushes!\n");
    }

    ENetServer server;
	if (!server.start())
	{
		  fmt::print("Creating ENetHost failed!\n");
		return EXIT_FAILURE;
	}
        server->set_component(g_event_manager, g_database);

        for (const auto& thread : g_threads)
            thread->detach();
        server->start_service();
        fmt::print("server loaded successfuly!\n");
    }
    while (true);
}
