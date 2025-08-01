#ifndef TAGSOCKETBINDINGAPI_H
#define TAGSOCKETBINDINGAPI_H

class QHttpServer;
class TagSocketList;
class QHttpServerResponse;
class QHttpServerRequest;

class TagSocketBindingApi
{
public:
    TagSocketBindingApi(QHttpServer &httpServer, TagSocketList& tagSocketList);

private:
    QHttpServerResponse get();
    QHttpServerResponse hookup(const QHttpServerRequest& request);

    QHttpServer& httpServer_;
    TagSocketList& tagSocketList_;
};

#endif // TAGSOCKETBINDINGAPI_H
