protoc.exe -I=./ --cpp_out=./ ./LPDefine.proto
protoc.exe -I=./ --cpp_out=./ ./LPMsg.proto 

protogen -i:LPDefine.proto -o:LPDefine.cs
protogen -i:LPMsg.proto -o:LPMsg.cs

echo "finished" 

pause