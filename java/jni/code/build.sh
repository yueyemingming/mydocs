 g++ Hello.cpp -fPIC -shared -o libhello.so -I /opt/jdk1.6.0_45/include -I /opt/jdk1.6.0_45/include/linux
 export LD_LIBRARY_PATH=/myspace/jni
 javac ToSay.java
 java ToSay
