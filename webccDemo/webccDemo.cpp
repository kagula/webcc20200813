#include <iostream>
#include <regex>

#include "../webcc20200813/webcc/logger.h"
#include "../webcc20200813/webcc/response_builder.h"
#include "../webcc20200813/webcc/server.h"

#include "sessionSupport.h"

class HelloView : public webcc::View {
public:
    webcc::ResponsePtr Handle(webcc::RequestPtr request, boost::shared_ptr<kagula::SessionInfo> pSI) override {
        if (request->method() == "GET") {
            return webcc::ResponseBuilder{}.OK().Body("Hello, World!   My name is WebCC!")();
        }

        return {};
    }
};

class UserView : public webcc::View {
public:
    webcc::ResponsePtr Handle(webcc::RequestPtr request, boost::shared_ptr<kagula::SessionInfo> pSI) override {
        using namespace std;
        const string& path = request->url().path();

        regex rex("/webccDemo/user/(\\w+).do");
        smatch what;

        if (!regex_match(path, what, rex))
        {
            return {};
        }

        webcc::ResponsePtr rp;
        string path2 = what[1];
        if (path2.empty())
        {
            return {};
        }

        if (path2 == "login")
        {
            rp = onLogin(request, pSI);
        }
        else if (path2 == "logout")
        {
            rp = onLogout(request, pSI);
        }
        else if (path2 == "getInfo")
        {
            rp = onGetInfo(request, pSI);
        }
        else
        {
            return {};
        }

        //���������ʱ��ӿ�.  ���������ȷд�����������;
        webcc::Header header("Access-Control-Allow-Origin", "*");
        rp->SetHeader(std::move(header));

        return rp;
    };
private:
    webcc::ResponsePtr onLogin(webcc::RequestPtr request, boost::shared_ptr<kagula::SessionInfo> pSI)
    {
        webcc::ResponsePtr rp;

        if ((request->kagulaArgs_.find("username") != request->kagulaArgs_.end()) &&
            (request->kagulaArgs_.find("password") != request->kagulaArgs_.end()))
        {
            //Ҫ���ϵ�console�o�P�], ��tһ�Ж|��ݔ����std::cout�Q�͕̾�����!
            std::cout << "username:" << request->kagulaArgs_["username"].c_str() << std::endl;
            std::cout << "password:" << request->kagulaArgs_["password"].c_str() << std::endl;

            //�O��session
            boost::shared_ptr<kagula::SessionInfo> pSI = kagula::SessionSupport::Instance().getSessionInfo(request->kagulaArgs_[kagula::session_key]);
            pSI->session["username"] = request->kagulaArgs_["username"];

            //�O�÷�������
            rp = webcc::ResponseBuilder{}.Json().Body("{\"ok\":1, \"message\":\"success\"}")();//XX.Json().XX֧��javascript $.ajax�﷨, ���ǲ�֧��javascript $.post�﷨!
        }
        else
        {
            rp = webcc::ResponseBuilder{}.Json().Body("{\"ok\":0, \"message\":\"none exist username or password!\"}")();//XX.Json().XX֧��javascript $.ajax�﷨, ���ǲ�֧��javascript $.post�﷨!
        }

        return rp;
    };

    webcc::ResponsePtr onLogout(webcc::RequestPtr request, boost::shared_ptr<kagula::SessionInfo> pSI)
    {
        if (pSI != nullptr)
        {
            //����û���Ϣ
            pSI->session["username"] = "";
        }

        webcc::ResponsePtr rp;

        //�����ַ���, ��chrome�ͻ����еõ����ַ���תjson������.
        rp = webcc::ResponseBuilder{}.OK().Body("{\"code\":0}")();//XX.Json().XX֧��javascript $.post����.

        return rp;
    };

    webcc::ResponsePtr onGetInfo(webcc::RequestPtr request, boost::shared_ptr<kagula::SessionInfo> pSI)
    {
        webcc::ResponsePtr rp;

        //�O��session
        if (pSI->session.find("username") != pSI->session.end() && pSI->session["username"].empty() == false)
        {
            /*
            * ���ﲻ��ʹ��webcc::ResponseBuilder{}.Json()��������ʽ���ؽ��
            * ����JavaScript��$.Post��ʽcallback function���ᱻ�ص�
            */
            rp = webcc::ResponseBuilder{}.OK().Body("{\"code\":0, \"loginName\":\"" + pSI->session["username"] + "\", \"authorities\":\"admin\"}")();
        }
        else
        {
            rp = webcc::ResponseBuilder{}.OK().Body("{\"code\":0, \"loginName\":\"Unknown\", \"authorities\":\"admin\"}")();
        }

        return rp;
    };
};

int main() {
    try {
        webcc::Server server(8080,"C:\\Users\\chenlu-li\\source\\repos\\webcc20200813\\htmlRoot");

        //server.Route("/", std::make_shared<HelloView>());

        server.Route(webcc::UrlRegex("/webccDemo/user/(\\w+).do"), std::make_shared<UserView>(), { "POST" });
        server.Run();

    }
    catch (const std::exception&) {
        return 1;
    }

    return 0;
}