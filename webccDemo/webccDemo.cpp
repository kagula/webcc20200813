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

        //允许跨域访问本接口.  这里最好明确写出允许的域名;
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
            //要把老的console給關閉, 否則一有東西輸出到std::cout綫程就會阻塞!
            std::cout << "username:" << request->kagulaArgs_["username"].c_str() << std::endl;
            std::cout << "password:" << request->kagulaArgs_["password"].c_str() << std::endl;

            //設置session
            boost::shared_ptr<kagula::SessionInfo> pSI = kagula::SessionSupport::Instance().getSessionInfo(request->kagulaArgs_[kagula::session_key]);
            pSI->session["username"] = request->kagulaArgs_["username"];

            //設置返回内容
            rp = webcc::ResponseBuilder{}.Json().Body("{\"ok\":1, \"message\":\"success\"}")();//XX.Json().XX支持javascript $.ajax语法, 但是不支持javascript $.post语法!
        }
        else
        {
            rp = webcc::ResponseBuilder{}.Json().Body("{\"ok\":0, \"message\":\"none exist username or password!\"}")();//XX.Json().XX支持javascript $.ajax语法, 但是不支持javascript $.post语法!
        }

        return rp;
    };

    webcc::ResponsePtr onLogout(webcc::RequestPtr request, boost::shared_ptr<kagula::SessionInfo> pSI)
    {
        if (pSI != nullptr)
        {
            //清除用户信息
            pSI->session["username"] = "";
        }

        webcc::ResponsePtr rp;

        //返回字符串, 在chrome客户端中得调用字符串转json对象函数.
        rp = webcc::ResponseBuilder{}.OK().Body("{\"code\":0}")();//XX.Json().XX支持javascript $.post调用.

        return rp;
    };

    webcc::ResponsePtr onGetInfo(webcc::RequestPtr request, boost::shared_ptr<kagula::SessionInfo> pSI)
    {
        webcc::ResponsePtr rp;

        //設置session
        if (pSI->session.find("username") != pSI->session.end() && pSI->session["username"].empty() == false)
        {
            /*
            * 这里不能使用webcc::ResponseBuilder{}.Json()这样的形式返回结果
            * 否则JavaScript中$.Post方式callback function不会被回调
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