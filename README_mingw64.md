**IN SUMMARY**

Cross-compiling from Ubuntu 18.04 to Windows using MinGW64 is possible without modifying the Wt sources using the current Conan package versions and MinGW64 7.3.0 if you do the following:

```
sudo ln -s /usr/x86_64-w64-mingw32/include/windows.h /usr/x86_64-w64-mingw32/include/Windows.h
sudo ln -s /usr/x86_64-w64-mingw32/include/shlobj.h /usr/x86_64-w64-mingw32/include/ShlObj.h
```

Then reference the relevant files from https://github.com/allquixotic/conanpatcher-server --

 - `cmc`: script for the conan package dependencies for MinGW64 (others with similar name: c for Linux, mc for MacOS)
 - `cmz`: script for building the code I wrote for MinGW64 (others with similar name: z for Linux, mz for MacOS)
 - `conanfile.txt`: I disabled wttest and wtisapi modules with `options`.
 - `l2w.profile`: A cross-compile profile provided by, I think it was memshared or you, in another issue.
 - `CMakeLists.txt`: Had to add `-lwsock32` to link libraries by setting `target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} wsock32)` (oh, shoot, I just realized this won't work for non-Windows platforms :D)

To successfully run the executable on Windows, you must copy files from your MinGW installation to your Windows box:

 - /usr/lib/gcc/x86_64-w64-mingw32/7.3-posix/*.dll
 - /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll

And then:

```
PS C:\Users\Sean\Documents\conanpatcher-server-test> .\conanpatcher-server.exe --docroot docroot --http-listen 127.0.0.1
:8085 --accesslog log.txt --config wt_config.xml --pid-file pidfile.pid
Option no-compression is implied because wthttp was built without zlib support.
[2018-Aug-05 12:19:23.612] 10296 - [info] "config: reading Wt config file: wt_config.xml (location = 'C:\Users\Sean\Documents\conanpatcher-server-test\conanpatcher-server.exe')"
Option no-compression is implied because wthttp was built without zlib support.
[2018-Aug-05 12:19:23.615] 10296 - [info] "Auth.GoogleService: not configured: OAuth: no 'google-oauth2-redirect-endpoint' property configured"
[2018-Aug-05 12:19:23.615] 10296 - [info] "Auth.FacebookService: not configured: OAuth: no 'facebook-oauth2-redirect-endpoint' property configured"
[2018-Aug-05 12:19:23.615] 10296 - [info] "WServer/wthttp: initializing built-in wthttpd"
INFO: Opened log file (log.txt).
[2018-Aug-05 12:19:23.616] 10296 - [info] "wthttp: started server: http://127.0.0.1:8085"
```

And when accessing the app via browser, **IT WORKS!**


@SSE4 Thank you so much for all your help with this, I really, really appreciate it. The bincrafters community is fantastic!
