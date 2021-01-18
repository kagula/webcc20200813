### What is the webcc20200813?
+ The lightweight HTTP server solution, I use it as a connector, you can use it connect your module with the other programs run in a variety of computer systems.
+ Because it is originated from the webcc open source project on Aug 13, 2020, so The solution is called webcc20200813.
+ **[Introduction in Chinese.](https://blog.csdn.net/lee353086/article/details/109486317?ops_request_misc=%25257B%252522request%25255Fid%252522%25253A%252522161068094116780277020706%252522%25252C%252522scm%252522%25253A%25252220140713.130102334.pc%25255Fblog.%252522%25257D&request_id=161068094116780277020706&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_v2~rank_v29-1-109486317.pc_v2_rank_blog_default&utm_term=webcc)**
 
### Feature
[1]Support http x-www-form-urlencoded post method.  
[2]Cookies support.
[3]Support file upload.  

### How to use
+ Using html5+javascript to demo how to interact with the HTTP server.
+ Frontend code is in the htmlRoot directory.
+ backend code is in the webccDemo project.  

##### Demo of  http post method
+ After run webccDemo, using Chrome open http:\\127.0.0.1:8080
##### Demo of the file upload
+ After run webcDemo, using chrome open http:\\127.0.0.1:8080\Demo_fileUpload.htm
+ Demo_fileUpload.htm is a standalone html that don't depend on the others file in the htmlRoot directory.

### Dependencies
+ boost 1.75 (minimum is 1.66)
+ zlib 1.2.11 (minimum is 1.2.8)
+ openssl 1.1.1g
+ cJson
  
### Development Environment
+ Microsoft Windows [version 10.0.18363.1256]
+ Visual Studio 2019 Community
+ __c++14__ this is default (minimum __c++11__)
  
### CentOS7 Environment
+ gcc 10.2 (minimum is 5.5)
+ cmake 3.19.2 (minimum is 2.8.0)
