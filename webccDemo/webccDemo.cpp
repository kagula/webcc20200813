#include <iostream>

#include "../webcc20200813/webcc/logger.h"
#include "../webcc20200813/webcc/response_builder.h"
#include "../webcc20200813/webcc/server.h"



class HelloView : public webcc::View {
public:
    webcc::ResponsePtr Handle(webcc::RequestPtr request) override {
        if (request->method() == "GET") {
            return webcc::ResponseBuilder{}.OK().Body("Hello, World!   My name is WebCC!")();
        }

        return {};
    }
};

class GetCurrentUser : public webcc::View {
public:
    webcc::ResponsePtr Handle(webcc::RequestPtr request) override {
        if (request->method() == "POST") {
            return Post(request);
        }

        return {};
    }

private:
    webcc::ResponsePtr Post(webcc::RequestPtr request) {
        webcc::ResponsePtr rp;

       /*
       * ���ﲻ��ʹ��webcc::ResponseBuilder{}.Json()��������ʽ���ؽ��
       * ����JavaScript��$.Post��ʽcallback function���ᱻ�ص�
       */
       rp = webcc::ResponseBuilder{}.OK().Body("{\"code\":0, \"loginName\":\"abc\", \"authorities\":\"admin\"}")();


        webcc::Header header("Access-Control-Allow-Origin", "*");//  ���������ȷ��д�����������;
        rp->SetHeader(std::move(header));
        return rp;
    }
};

class LoginView : public webcc::View {
public:
    webcc::ResponsePtr Handle(webcc::RequestPtr request) override {
        if (request->method() == "POST") {
            return Post(request);
        }

        return {};
    }

private:
    webcc::ResponsePtr Post(webcc::RequestPtr request) {
        webcc::ResponsePtr rp;

        if ((request->kagulaArgs_.find("username") != request->kagulaArgs_.end()) &&
            (request->kagulaArgs_.find("password") != request->kagulaArgs_.end()))
        {
            //Ҫ���ϵ�console�o�P�], ��tһ�Ж|��ݔ����std::cout�Q�͕̾�����!
            std::cout << "username:" << request->kagulaArgs_["username"].c_str() << std::endl;
            std::cout << "password:" << request->kagulaArgs_["password"].c_str() << std::endl;
            rp = webcc::ResponseBuilder{}.Json().Body("{\"ok\":1, \"message\":\"success\"}")();
        }
        else
        {
            rp = webcc::ResponseBuilder{}.Json().Body("{\"ok\":0, \"message\":\"none exist username or password!\"}")();
        }


        webcc::Header header("Access-Control-Allow-Origin", "*");//  ���������ȷ��д�����������;
        rp->SetHeader(std::move(header));
        return rp;
    }
};

int main() {
    try {
        webcc::Server server(8080,"C:\\Users\\chenlu-li\\source\\repos\\webcc20200813\\htmlRoot");

        //server.Route("/", std::make_shared<HelloView>());
        server.Route("/webccDemo/login.do", std::make_shared<LoginView>(), { "POST", "GET" });
        server.Route("/webccDemo/user/getCurrentUser.do", std::make_shared<GetCurrentUser>(), { "POST", "GET" });
        server.Run();

    }
    catch (const std::exception&) {
        return 1;
    }

    return 0;
}