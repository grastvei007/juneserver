#ifndef TAGAPI_H
#define TAGAPI_H

class QHttpServer;
class QHttpServerResponse;
class QHttpServerRequest;
class TagList;

class TagApi
{
public:
    TagApi(QHttpServer &httpServer, TagList &tagList);

private:
    QHttpServerResponse createTag(const QHttpServerRequest &request);
    QHttpServerResponse update(const QHttpServerRequest &request);
    QHttpServerResponse get(const QHttpServerRequest &request);

    QHttpServer& httpServer_;
    TagList& tagList_;
};

#endif // TAGAPI_H
