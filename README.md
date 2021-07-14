# GzzNet
<hr/>

GzzNet 라이브러리와 샘플 코드가 있습니다.
GzzNet는 정적 라이브러리 입니다. GzzNet을 먼저 빌드하시고, SimpleClient, SimpleServer을 빌드하시면 됩니다.

## How to Build
### requires
>Visual Studio 2019  

#### 1. clone this repository onto your local drive
#### 2. Build SimpleClient and SimpleServer

## Explanation
### Initialize File  
ini 파일의 구성은 다음과 같습니다.  
  
IP : 원격지의 IP 입니다.  
PORT : 원격지의 Port 입니다.  
NETWORK_THREAD_COUNT : 실행할 IOCP 스레드 갯수 입니다.  
MAX_SESSION : 최대 세션 갯수입니다.  
EXTRA_SESSION : 확장 세션 갯수입니다.  
CONNECT_SESSION_COUNT : 연결할 세션 갯수입니다.  
ECHO_COUNT : 한번 에코 패킷 Send 할 때 반복 횟수 입니다.  
ECHO_MIN_SIZE : 최소 에코 스트링의 사이즈 입니다.  
ECHO_MAX_SIZE : 최대 에코 스트링의 사이즈 입니다.  
ECHO_TERM : 에코 패킷을 던질 때의 간격 입니다. ms 단위입니다.    
브로드 캐스팅은 구현 중에 있습니다.  

### Execution
#### 1. Execution 폴더에 들어가서 ini 파일을 셋팅해줍니다.
> Server : simple_server.ini을 수정해줍니다.   
> Server : ini 파일의 옵션을 전부 수정할 필요 없이 Port, Session의 갯수 정도만 확인해주면 됩니다.  
> Client : simple_client.ini을 수정해줍니다.    

#### 2. Execution 폴더에 들어가서 SimpleServer를 실행합니다.
> A를 입력하면 현재까지 송신실패 횟수 / 송신성공 횟수를 보여줍니다.

#### 3. Execution 폴더에 들어가서 SimpleClient를 실행합니다.
> Client : 실행하고 c 를 입력하면 Connect를 시도합니다.  
> Client : D를 입력하면 Disconnect를 시도합니다.  
> Client : R을 입력하면 Reconnect를 시도합니다. 하지만, TimeWait 문제로 재연결은 실패합니다. 이것은 의도된 것 입니다.  
> Client : Q를 입력하면 서버에 Echo Packet을 송신합니다.  
> Client : Space를 입력하면 송신을 종료합니다.
> Client : Space로 송신 종료모드를 한 후, A를 입력하면 연결된 세션의 통계를 보여줍니다.

#### 4. Login Server DBAgent
>실행은 되나, 따로 Database 셋팅을 해줘야 합니다. 이 부분은 추후에 수정하겠습니다.  

#### 5. 모든 실행파일은 ESC를 누르면 종료됩니다.
