//============================================================================
// Name        : AppServer.cpp
// Author      : Taller2
// Version     :
// Copyright   : Do not copy
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "mongoose/mongoose.h"
using namespace std;

bool exit_flag = false;

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data){
	struct http_message *hm = (struct http_message *) ev_data;
	int connect_status;

	switch (ev){
		case MG_EV_CONNECT:
			connect_status = *(int *) ev_data;
			if (connect_status != 0){
				cout <<"ERROR connecting to http://127.0.0.1:3000 "<<strerror(connect_status)<<endl;
				exit_flag = true;
			}
			break;
		case MG_EV_HTTP_REPLY:
			cout <<"Got reply: "<<(int)hm->body.len<<" "<<hm->body.p<<endl;
			nc->flags |= MG_F_SEND_AND_CLOSE;
			exit_flag = true;
			break;
		/*
		case MG_EV_RECV:
			mg_send(nc, io->buf, io->len);
			mbuf_remove(io, io->len);
			break;
		*/
		default:
			break;
	}
}

int main() {
	struct mg_mgr mgr;
	mg_mgr_init(&mgr, NULL);

	struct mg_connection* connection;
	connection = mg_connect_http(&mgr, ev_handler, "http://localhost:3000", NULL, NULL);

	mg_set_protocol_http_websocket(connection);

	cout << "Starting RESTful client against http://127.0.0.1:3000"<<endl;

	while (!exit_flag){
		mg_mgr_poll(&mgr, 1000);
	}

	mg_mgr_free(&mgr);
	cout << "GoodBye!!!" << endl;
	return 0;
}
