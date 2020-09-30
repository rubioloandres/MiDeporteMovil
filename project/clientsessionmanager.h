#ifndef CLIENTSESSIONMANAGER_H
#define CLIENTSESSIONMANAGER_H

#include <QString>


class ClientSessionManager
{
public:
    int client_id;
    QString role;

    QString email;
    QString first_name;
    QString last_name;
    QString phone;


    ClientSessionManager() = default;
    ClientSessionManager(const ClientSessionManager&) = delete;
    ClientSessionManager(ClientSessionManager&&) = delete;

    static ClientSessionManager& Instance(){
        static ClientSessionManager session;
        return session;
    }

};


#endif // CLIENTSESSIONMANAGER_H
