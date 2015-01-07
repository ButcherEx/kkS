#ifndef LPROTOCOL_H
#define LPROTOCOL_H

#include <PreCompier.h>
#include <TcpConnection.h>
#include <TcpClient.h>
#include <TcpServer.h>

CMD_START(tagHello)
	char msg[128];
CMD_END

#define MAX_MSG_SEND (100*10000)

#define CONTINUE_SEC (1*60*1000)

#endif