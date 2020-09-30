#ifndef SERVERPARAMS_H
#define SERVERPARAMS_H

#include <QString>

class ServerParams
{
public:
    static QString getBaseUrl(){
        return "http://206.81.1.187/api/";
        //return "http://reservas/api/";
    }
};

#endif // SERVERPARAMS_H
