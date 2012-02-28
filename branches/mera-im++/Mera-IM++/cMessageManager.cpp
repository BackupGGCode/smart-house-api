#include "cMessageManager.h"
cMessageManager* cMessageManager::self = 0;

cMessageManager& cMessageManager::Instance()
{
	if(!self)
	{
		self = new cMessageManager;
	}
	return *self;
}

void cMessageManager::Initialize()
{
	delete self;
	self = new cMessageManager;
}


cMessageManager::cMessageManager ()
{
	StartServer();
}

cMessageManager::~cMessageManager()
{
	delete self;
}

int cMessageManager::StartServer()
{
	SocketVersion = MAKEWORD (2,2);
	result = WSAStartup (SocketVersion,(WSADATA *) &buf[0]);
	Server = socket (AF_INET, SOCK_STREAM, 0);

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(1234);
	ServerAddr.sin_addr.s_addr = 0;

	if ((BindResult = bind(Server,(sockaddr *) &ServerAddr, sizeof(ServerAddr))) < 0)
	{
		cout << "Socket Bind Error" << endl;
		return -1;
	}
	if ((ListenResult = listen(Server, 100)) < 0)
	{
		cout << "Socket Listen Error" << endl;
		return -1;
	}

	ClientSocketAddrSize = sizeof(ClientAddr);

	return 1;
}

SOCKET cMessageManager::GetClient()
{
	return accept(Server, (sockaddr *) &ClientAddr, &ClientSocketAddrSize);
}

void cMessageManager::ProcessDialog(SOCKET Client)
{
	cClient User;
	
	char DialogLength[4];
	int Bytesrecv;
	if ((Bytesrecv = recv(Client,&DialogLength[0],sizeof(DialogLength),0)) > 3)
	{
		return;
	}

	int n = atoi(DialogLength);
	char* buffer = new char [n+1];
	
	recv(Client,&buffer[0],n,0);
	buffer[n] = 0;
	
	eMessageType Type;
	Type = ProcessMessageType(buffer[0]);

	switch (Type)
	{
		case RegisterRequest: {ProcessRegisterRequest(Client,buffer);break;}
		case LoginRequest: { send(Client,"Login Request\n",strlen("Login Request\n"),0);break;}
		case LogoutRequest: {send(Client,"Logout Request\n",strlen("Logout Request\n"),0);break;}
		case IM: {send(Client,"IM\n",strlen("IM\n"),0);break;}
		case StatusChanged: {send(Client,"Status Changed\n",strlen("Status Changed\n"),0);break;}
	}
}

eMessageType cMessageManager::ProcessMessageType(char x)
{
	return (eMessageType)(x-'0');
}

bool cMessageManager::ProcessRegisterRequest(SOCKET Client, char* buffer)
{
	//cClient User;
	std::string Username,Password;
	//As we know, that message format Register Request is "0,username,password",
	//so set i to 2 as we are aware, that username starts with the third element, because two first once are "0" and ","
	
	size_t i = 2;
	while (buffer[i] != ',')
	{
		Username.push_back(buffer[i]);
		i++;
	}
	i++;
	while (buffer[i] != 10) // Replace with 0 after client application is done.
	{
		Password.push_back(buffer[i]);
		i++;
	}


	if (!IsUserRegistered(Username))
	{
		//User.SetSocketID(Client);
		//User.SetUserName(Username);
		//User.SetUserPassword(Password);
		//ClientsList.Insert(&User);
		return true;
	}

	else
	{
		return false;
	}
}

bool cMessageManager::IsUserRegistered(std::string Username)
{
	cClient* Client = ClientsList.Begin();
	while(Client->Next)
	{
		//if (Client->GetUserName() == Username)
		{
			return true;
		}
	}
	return false;
}