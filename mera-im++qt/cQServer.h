#include <QtNetwork>
#include <QWidget>
#include <qtextedit.h>
#include "cMessageManager.h"

class cQServer : public QWidget
{
	Q_OBJECT

public:
	cQServer(void);
	~cQServer(void);
	QTcpServer* m_pTcpServer;

	void StartServer (int Port);

	void OnClientConnected (QTcpServer* Server);

public slots:
	void OnDataFromClient();
};