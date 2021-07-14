# GzzNet
<hr/>

GzzNet 라이브러리와 샘플 코드가 있습니다.
GzzNet는 정적 라이브러리 입니다. GzzNet을 먼저 빌드하시고, SimpleClient, SimpleServer을 빌드하시면 됩니다.

## How to Build
### requires
>Visual Studio 2019  

#### 1. clone this repository onto your local drive
#### 4. Build SimpleClient and SimpleServer

# Explanation

>Simple Server를 먼저 실행합니다. 
>그 다음 Simple Client를 실행합니다. 

>ini 파일의 구성은 다음과 같습니다.

>IP 원격지의 IP 입니다.
>PORT 원격지의 Port 입니다.
>NETWORK_THREAD_COUNT 실행할 IOCP 스레드 갯수 입니다.

>MAX_SESSION 최대 세션 갯수입니다.
>EXTRA_SESSION 확장 세션 갯수입니다.

>CONNECT_SESSION_COUNT 연결할 세션 갯수입니다.

>ECHO_COUNT 한번 에코 패킷 Send 할 때 반복 횟수 입니다.
>ECHO_MIN_SIZE 최소 에코 스트링의 사이즈 입니다.
>ECHO_MAX_SIZE 최대 에코 스트링의 사이즈 입니다.
>ECHO_TERM 에코 패킷을 던질 때의 간격 입니다. ms 단위입니다.
>브로드 캐스팅은 구현 중에 있습니다.

