### What is the webcc20200813?
+ the lightweight http server solution, i use it as connector, you can use it connect your module with the other programs run in the variey of computer systems.
+ Because is originated from the webcc open source project at Aug 13 2020, so The solution is called webcc20200813.

### Feature
[1]Support http x-www-form-urlencoded post method.  
[2]Support file upload.  

### How to use
+ Using html5+javascript to demo how to interactive with the http server.
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
