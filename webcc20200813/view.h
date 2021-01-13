#ifndef WEBCC_VIEW_H_
#define WEBCC_VIEW_H_

#include <memory>

#include <boost/shared_ptr.hpp>

#include "request.h"
#include "response.h"
#include "sessionInfo.h"

namespace webcc {

class View {
public:
  virtual ~View() = default;

  virtual ResponsePtr Handle(RequestPtr request, boost::shared_ptr<kagula::SessionInfo> pSI) = 0;

  // Return true if you want the request data of the given method to be streamed
  // to a temp file. Data streaming is useful for receiving large data, e.g.,
  // a JPEG image, posted from the client.
  virtual bool Stream(const std::string& /*method*/) {
    return false;  // No streaming by default
  }
};

using ViewPtr = std::shared_ptr<View>;

}  // namespace webcc

#endif  // WEBCC_VIEW_H_
