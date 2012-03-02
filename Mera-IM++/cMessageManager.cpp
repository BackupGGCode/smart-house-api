#include "cMessageManager.h"

using namespace std;
DWORD WINAPI Run (LPVOID CL);
cMessageManager* cMessageManager::m_pSelf = 0;

////////////////////////////////////////////////////////////////////////////////

cMessageManager* cMessageManager::Instance()
{
	if (!m_pSelf)
	{
		m_pSelf = new cMessageManager;
	}
	return m_pSelf;
}

////////////////////////////////////////////////////////////////////////////////

cMessageManager::cMessageManager ()
{
	ClientsList = new cClientsList;
}

////////////////////////////////////////////////////////////////////////////////

cMessageManager::~cMessageManager()
{
	delete m_pSelf;
	delete ClientsList;
}

////////////////////////////////////////////////////////////////////////////////

int cMessageManager::StartServer()
{
	SOCKET Server;
	int nResult, nBindResult, nListenResult;
	char buf[512];
	sockaddr_in ServerAddr;
	WORD SocketVersion;

	SocketVersion = MAKEWORD (2,2);
	nResult = WSAStartup (SocketVersion,(WSADATA *) &buf[0]);
	Server = socket (AF_INET, SOCK_STREAM, 0);

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(1234);
	ServerAddr.sin_addr.s_addr = 0;

	if ((nBindResult = bind(Server,(sockaddr *) &ServerAddr, sizeof(ServerAddr))) < 0)
	{
		cout << "Socket Bind Error" << endl;
		return -1;
	}
	if ((nListenResult = listen(Server, 100)) < 0)
	{
		cout << "Socket Listen Error" << endl;
		return -1;
	}

	SOCKET* ClientSocket = new SOCKET;
	DWORD thID;

	while((*ClientSocket = GetClient(Server)) != -1)
	{		
		CreateThread(NULL,NULL,Run,ClientSocket,NULL,&thID);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

SOCKET cMessageManager::GetClient(SOCKET Server)
{
	sockaddr_in m_ClientAddr;
	int m_nClientSocketAddrSize = sizeof(m_ClientAddr);
	return accept(Server, (sockaddr *) &m_ClientAddr, &m_nClientSocketAddrSize);
}

////////////////////////////////////////////////////////////////////////////////

bool cMessageManager::ProcessDialog(SOCKET ClientSocket)
{
	cClient User;
	bool rc = false;
	
	char MessageLength[4];
	int Bytesrecv;
	if ((Bytesrecv = recv(ClientSocket,&MessageLength[0],sizeof(MessageLength),0)) > 3)
	{
		return rc;
	}

	int nMessageLength = atoi(MessageLength);
	char* sMessage = new char [nMessageLength+1];
	
	recv(ClientSocket,&sMessage[0],nMessageLength,0);
	sMessage[nMessageLength+1] = '\0';


	// Putting '\0' where ever we encounter '\n'. This is for development testing using netcat because netcat puts \n in the end of
	// message when pressing ENTER but we're parsing \0's 
	for (size_t i = 0; i <= nMessageLength; i++) {sMessage[i] = (sMessage[i] == '\n' ? '\0':sMessage[i]);}
	
	eMessageType eType;
	eType = ProcessMessageType(sMessage[0]);

	switch (eType)
	{
		case eMESSAGE_RegisterRequest:
			{
				rc = ProcessRegisterRequest(ClientSocket,sMessage);
				break;
			}

		case eMESSAGE_LoginRequest:
			{
				//send(ClientSocket,"Login Request\n",strlen("Login Request\n"),0);
				rc = ProcessLoginRequest(ClientSocket,sMessage);
				break;
			}
		case eMESSAGE_LogoutRequest:
			{
				//send(ClientSocket,"Logout Request\n",strlen("Logout Request\n"),0);
				rc = ProcessLogoutRequest(ClientSocket,sMessage);
				break;
			}
		case eMESSAGE_IM:
			{
				rc = ProcessIMRequest(ClientSocket,sMessage);
				break;
			}
		case eMESSAGE_StatusChanged:
			{
				send(ClientSocket,"Status Changed\n",strlen("Status Changed\n"),0);
				rc = true;
				break;
			}
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////

eMessageType cMessageManager::ProcessMessageType(char x)
{
	return (eMessageType)(x-'0');
}

////////////////////////////////////////////////////////////////////////////////

bool cMessageManager::ProcessRegisterRequest(SOCKET ClientSocket, char* sMessage)
{
	cClient* pUser = new cClient;
	string sUsername,sPassword;
	
	//As we know, that message format Register Request is "0,username,password",
	//so set i to 2 as we are aware, that username starts with the third element, because two first once are "0" and ","
	
	size_t i = 2;
	while (sMessage[i] != ',')
	{
		sUsername.push_back(sMessage[i]);
		i++;
	}
	i++;
	while (sMessage[i] != '\0')
	{
		sPassword.push_back(sMessage[i]);
		i++;
	}


	if (!IsUserRegistered(sUsername))
	{
		pUser->SetSocketID(ClientSocket);
		pUser->SetUserName(sUsername);
		pUser->SetUserPassword(sPassword);
		ClientsList->Insert(pUser);
		return true;
	}

	else
	{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////

bool cMessageManager::ProcessLoginRequest(SOCKET ClientSocket, char* sMessage)
{
	string sUsername,sPassword;
	
	size_t i = 2;
	while (sMessage[i] != ',')
	{
		sUsername.push_back(sMessage[i]);
		i++;
	}
	i++;
	while (sMessage[i] != '\0')
	{
		sPassword.push_back(sMessage[i]);
		i++;
	}

	cClient* pClient = ClientsList->FindByUserName(sUsername);
	if (!pClient)
	{
		closesocket(ClientSocket);
		return false;
	}
	else
	{
		if (pClient->GetUserPassword() == sPassword)
		{
			pClient->SetConnected(true);
			return true;
		}
		else
		{
			closesocket(ClientSocket);
			return false;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

bool cMessageManager::ProcessLogoutRequest(SOCKET ClientSocket, char* sMessage)
{
	string sUsername;
	
	size_t i = 2;
	while (sMessage[i] != '\0')
	{
		sUsername.push_back(sMessage[i]);
		i++;
	}

	cClient* pClient = ClientsList->FindByUserName(sUsername);
	if (!pClient)
	{
		return false;
	}
	else
	{
		pClient->SetConnected(false);
		closesocket(ClientSocket);
		return true;
	}
}
////////////////////////////////////////////////////////////////////////////////

bool cMessageManager::IsUserRegistered(string sUserName)
{
	return (ClientsList->FindByUserName(sUserName) != NULL) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////

bool cMessageManager::ProcessIMRequest(SOCKET ClientSocket,char* sMessage)
{
	string sDestination, sIM;

	size_t i = 2;
	while (sMessage[i] != ',')
	{
		sDestination.push_back(sMessage[i]);
		i++;
	}

	i++;
	while (sMessage[i] != '\0')
	{
		sIM.push_back(sMessage[i]);
		i++;
	}

	if (FindSocketByUsername(sDestination) > 0)
	{
		send(FindSocketByUsername(sDestination),&sIM[0],sIM.length(),0);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////

SOCKET cMessageManager::FindSocketByUsername(string sUsername)
{
	cClient* pClient = ClientsList->FindByUserName(sUsername);
	if (pClient)
	{
		return pClient->GetSocketID();
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI Run(LPVOID CL)
{
	SOCKET ClientSocket;
	ClientSocket = *(SOCKET *)CL;


	char * a = "Hello, Client! your ID is ";
	char * b = new char[sizeof(ClientSocket)+1];
	_itoa(ClientSocket,b,10);
	send (ClientSocket,a, strlen(a),0);
	send (ClientSocket,b, sizeof(b),0);
	send (ClientSocket,"\n",2,0);

	unsigned int MCount = 0;

	while (cMessageManager::Instance()->ProcessDialog(ClientSocket))
	{
		MCount++;
	}
	closesocket(ClientSocket);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////