if not exist "pb" md "pb"
protoc.exe -Iproto -Icommon --cpp_out=pb proto\*.proto
pause


  