#ifndef LOGVALUEAPI_H
#define LOGVALUEAPI_H

class QHttpServer;
class LogValueData;
class QHttpServerResponse;
class QHttpServerRequest;

class LogValueApi
{
public:
    LogValueApi(QHttpServer& httpServer, LogValueData& logValueData);

private:
    QHttpServerResponse get();
    QHttpServerResponse deleteValue(const QHttpServerRequest &request);
    QHttpServerResponse addValue(const QHttpServerRequest &request);

    QHttpServer& httpServer_;
    LogValueData& logValueData_;
};

#endif // LOGVALUEAPI_H
