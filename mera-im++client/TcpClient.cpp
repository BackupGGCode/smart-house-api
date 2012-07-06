#pragma once
#include "TcpClient.h"

QDataStream& operator<<(QDataStream& out, cRegisterRequest& Request) 
{
	out << Request.m_nID 
		<< Request.m_sUsername
		<< Request.m_sPassword;
	return out;
}

cTcpClient* cTcpClient::Instance()
{
	static cTcpClient pSelf;
	return &pSelf;
}

//////////////////////////////////////////////////////////////////////////////////////////
cTcpClient::cTcpClient() : m_nNextBlockSize(0)
{
	m_pTcpSocket = new QTcpSocket(this);
}

//////////////////////////////////////////////////////////////////////////////////////////
cTcpClient::~cTcpClient(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
void cTcpClient::OnDataFromServer()
{
	QDataStream in(m_pTcpSocket);
	//QString debug = m_pTcpSocket->readAll();
	in.setVersion(QDataStream::Qt_4_2);
	for (;;) 
	{
		if (!m_nNextBlockSize) 
		{
			if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) 
			{
				break;
			}
			in >> m_nNextBlockSize;
		}

		if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) 
		{
			break;
		}

		QString str;
		in >> str;

		m_nNextBlockSize = 0;
		emit ClientResponce(str);
	}	
}

void cTcpClient::ConnectClicked (QString sServer, int nPort)
{
	m_pTcpSocket->connectToHost(sServer, nPort);
	emit ClientResponce("Connected to host " + sServer + " on "+ QString("%1").arg(nPort) + " port");
	connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(OnDataFromServer()));
	connect(m_pTcpSocket, SIGNAL(connected()), SLOT(OnConnected()));
}

/*void cTcpClient::OnSendToServer(QString message, int Type)
{
	QByteArray  arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_2);
	out << quint16(0) << Type << message;

	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));

	m_pTcpSocket->write(arrBlock);
}*/



void cTcpClient::OnSendToServer(QString message, int Type)
{
	cRegisterRequest Message;
	Message.m_nID = RegisterRequest;
	Message.m_sUsername = "UserName";
	Message.m_sPassword = "Password";

	QByteArray  arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_2);
	out << quint16(0) << Message.m_nID << Message;

	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));

	m_pTcpSocket->write(arrBlock);
}



void cTcpClient::OnConnected()
{

}