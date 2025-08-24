# Counter Drug Project 소개
UE5로 제작하는 1인개발 프로젝트입니다.

당신은 마약 카르텔에 잠입하여 각종 임무를 수행하는 로봇입니다.
그들의 작업을 방해하고, 단서를 수집하며, 경비견으로부터 도망치세요.


# 챕터 소개

## 챕터 1 : 잠입 그리고 단서 수집
당신이 조종할 로봇은 RED_Robot001 입니다. 

임무는 마약 창고로 의심받는 장소에 잠입, 단서를 수집하며 그들의 작업을 방해하는 것입니다.
쉽지는 않을 겁니다. 마약 창고의 시스템은 생각보다 견고하거든요!
<img width="1982" height="1128" alt="image" src="https://github.com/user-attachments/assets/b82cc260-fb4e-4ebb-beb6-87cdec737018" />
<img width="1769" height="1007" alt="image" src="https://github.com/user-attachments/assets/1c2d5e83-3eef-4464-89ac-84eb3c8fe551" />



## 챕터 2 : 충분한 단서, 그리고 철수
RED_Robot001 을 조종하여 충분한 단서를 수집하는데 성공했습니다. 하지만 너무 오래 체류한 탓에 흔적을 많이 남겼습니다.
곧 그들이 쫓아올겁니다. 유감스럽게도, RED는 빠른 철수에 적합한 로봇이 아닙니다. BLUE_Sprint001 에게 단서를 넘기세요.
그리고 바톤을 이어받은 BLUE를 조종하여 신속하게 마약 카르텔의 영역에서 벗어나세요!


## 챕터 3 : 추격 그리고 체포
이제 그들을 체포할 근거를 마련했습니다. 하지만 카르텔은 이를 눈치채고 도망갈 준비를 하고 있습니다.
공항에서 그들을 추격하세요. 그리고 체포하세요!


# 챕터 1 : 등장 몬스터 및 시스템 소개
게임모드는 BP_ColorAttackGameMode(cpp 기반)를 사용합니다.

RED의 임무는 단서를 수집 , 그리고 드럼통을 수집기(Furnace)에 넣어서 카르텔에 혼란을 주는 것입니다.
곳곳에 배치된 CCTV를 조심하세요. 발각되면 당신의 위치가 경비견에게 공유됩니다.

## RED_Robot001
WASD로 이동 , E키로 단서 수집 , 마우스 좌클릭으로 물건 잡기/놓기 , 좌클릭 상태에서 Q키를 누르면 펀치가 가능합니다.


## 등장 몬스터

### 몬스터1 : 단순 물류용 로봇
기본적으로 창고에서는 요청받은 마약을 수집기(Furnace)에 넣는 물류 시스템이 있습니다. 몬스터1은 이 작업을 수행합니다.

BP_Monster1(cpp 기반)로 디테일 수정이 가능합니다.

Monster1.cpp : 콜리전 박스가 있어서, Color.Red 태그를 가진 드럼통과 오버랩 시, 3초간 스턴이 걸립니다.

MonsterAIController.cpp : 전용 BT와 연결하기 위한 용도입니다. BT에서는 몬스터1이 마약 큐브를 잡고 있냐 없냐에 따라 로직이 나뉩니다.
각각의 경우에 대해 기제작된 EQ(가장 가까운 cube, furnace를 찾는 용도)에 따라 임무를 수행합니다. 임무 수행은 BTTask_GrabObject , BTTask_DropObject 로 수행합니다.
이 2개 파일은 Monster1.cpp에서 정의된 Grab/Drop 관련 함수를 호출하여 실행하며, Monster1.cpp에서는 Grab/Drop 기능을 Grabber.cpp로부터 구현합니다.

### 몬스터2 : 경비견
평소에는 주변을 배회하다가 플레이어가 CCTV에 발각되거나, 시야에 플레이어가 들어오면 플레이어를 추격하여 파이어볼을 날립니다.

BP_Monster2(cpp 기반)로 디테일 수정이 가능합니다 : 달리는 속도, 스턴 존, 애니메이션 등등

Monster2.cpp : Projectile class 선언 , 스턴 관련된 함수들이 있습니다.

Monster2AIController.cpp : 전용 BT와 연결하기 위한 용도입니다. 몬스터2(경비견)의 BT 로직은 다소 복잡합니다. 
<img width="1639" height="866" alt="image" src="https://github.com/user-attachments/assets/9c4fa928-4d77-4b01-9d74-e8cb858c7920" />
플레이어 위치가 보이는지로 먼저 판단합니다 -> 보이면 추적을 시작합니다. (보라색 노드는 왼쪽부터 BGM 처리 / 추적 / 발사 공격 / 대기) -> 만약 플레이어에게 스턴이 걸리면 4초 대기.

플레이어가 보이지 않으면 주변을 배회합니다. Monster2ChaseEnd노드는 추적 전용 BGM에서 일반 BGM으로 변경하는 작업을 수행합니다. 

## 시스템
CCTV / 단서 수집 기능 / 수집기(Furnace) / BGM 에 대한 설명입니다.

### CCTV 시스템
액터 기반의 BP_CCTV로 작동합니다. Monster2 BB와 WBP_Timer와 연계됩니다.

콜리전 박스 오버랩 이벤트 시에 로직을 진행합니다 : 오버랩 이후 6초동안 타이머로 시간을 잽니다. Monster2 BB에 발각된 위치(벡터 타입)를 줍니다. 그리고 WBP_Timer기반 함수를 호출합니다.
그러면 화면에는 3초동안 특정 메시지(발각되었다는 내용)가 노출됩니다.

### 수집 기능

BP_RedCharacter에 향상된 입력 시스템을 사용했습니다. Customizable Interaction Plugin을 이식해 그대로 사용합니다. E키를 눌러 수집이 가능하며, F키를 꾹 눌러 다른 상호작용 처리 로직도 추가할 예정입니다.
콘텐츠 > Props > Clue 에서 확인 가능합니다.

### 수집기(Furnace)
챕터 1에서 등장하는, 카르텔의 물류 시스템의 핵심입니다. 오버랩 이벤트로 상호작용합니다. 태그(Color.Gray / Color.Red)를 구분하며, 4점을 달성하는 쪽 색깔로 바뀝니다.(카르텔-회색 / 플레이어-빨간색) 점수 상한은 Furnace.cpp에서 수정가능합니다.

WBP_FurnaceUI에서 현재 점수 상태를 수집기 위에 표시하도록 돕습니다. 수집기 BP의 박스 콜리전(Visibility Box) 안에 플레이어가 오버랩되면 UI가 활성화되어 보입니다.( 이 기능은 Furnace.cpp에 구현되어 있습니다.)

### BGM
BP_ColorAttackGameMode 에 로직이 구현되어 있습니다. 
OnChaseStart , OnChaseEnd 로 커스텀 함수를 구현했으며, 이 함수들은 몬스터2 BT의 BTTask BP와 연계됩니다. bIsChasing으로 플레이어가 쫓기는지 여부를 확인합니다. 그리고 상황에 맞는 BGM을 틀어줍니다. 
게임 시작 시에 2개의 BGM을 스폰하고, 추적 BGM은 Pause시켜서 동시 재생을 막았습니다.











