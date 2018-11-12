thrift -r --gen java -out ./ rpc/tutorial.thrift

java -classpath libs/libthrift-1.0.0.jar:libs/log4j-1.2.14.jar:libs/slf4j-api-1.5.8.jar:libs/slf4j-log4j12-1.5.8.jar:libs/servlet-api-2.5.jar:libs/commons-codec-1.6.jar:libs/commons-logging-1.1.1.jar:httpclient-4.2.5.jar:libs/httpcore-4.2.4.jar:libs/junit-4.4.jar server.JavaServer

