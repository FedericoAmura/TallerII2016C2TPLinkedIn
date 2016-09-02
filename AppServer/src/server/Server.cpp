/*
 * Server.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#include "Server.h"
#include <pthread.h>
#include <iostream>

Server::Server() {
	mg_mgr_init(&mgr_client, NULL);
	mg_mgr_init(&mgr_server, NULL);

	start_connections();
}

bool Server::settting_ok(){
	return (client_mode_running && server_mode_running);
}

Server::~Server() {
	mg_mgr_free(&mgr_client);
	mg_mgr_free(&mgr_server);
}

static void* run_client_mode(void* arg){
	mg_mgr *mgr = (mg_mgr*) arg;

	while (client_mode_running){
		mg_mgr_poll(mgr, 1000);
	}

	return NULL;
}

static void* run_server_mode(void* arg){
	mg_mgr *mgr = (mg_mgr*) arg;

	while (server_mode_running){
		mg_mgr_poll(mgr, 1000);
	}

	return NULL;
}

void Server::run(){
	pthread_t t_server;
	pthread_t t_client;

	pthread_create(&t_client, NULL, run_client_mode, &mgr_client);
	pthread_create(&t_server, NULL, run_server_mode, &mgr_server);

	pthread_join(t_server, NULL);
	pthread_join(t_client, NULL);

	pthread_exit(NULL);
}

/* communication between appclient and appserver*/
static void server_event_handler(struct mg_connection* c, int event, void* p){
	  if (event == MG_EV_HTTP_REQUEST) {
	    struct http_message *hm = (struct http_message *) p;

	    std::cout << "Android device connected. " <<std::endl;

	    mg_send_head(c, 200, hm->message.len, "Content-Type: text/plain");
	    mg_printf(c, "%.*s", hm->message.len, hm->message.p);
	  }

	  if (!client_mode_running)
	    server_mode_running = false;

}

/* communication between appserver and sharedserver*/
static void client_event_handler(struct mg_connection* c, int event, void* p){
	  if (event == MG_EV_CONNECT) {
	    std::cout << "Server connected. " <<std::endl;
	  }

	  if (event == MG_EV_HTTP_REPLY){
		struct http_message *hm = (struct http_message *) p;
		std::cout << "Got reply: " << hm->body.p <<std::endl;
		client_mode_running = false;
	  }
}

void Server::start_connections(){
	/* Connecting to Shared Server */
	struct mg_connection* c = mg_connect_http(&mgr_client, client_event_handler, ip_sharedServer, NULL, NULL);
	if (!c)
		return;
	client_mode_running = true;


	/* Creating a listening connection */
	connection_server = mg_bind(&mgr_server, server_http_port, server_event_handler);
	if (!connection_server)
		return;
	server_mode_running = true;
	mg_set_protocol_http_websocket(connection_server);
}

