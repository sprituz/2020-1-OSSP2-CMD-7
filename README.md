# 2020-1-OSSP2-CMD-7
박채은, 이다연, 조민지
Open Source Software Project

## 1. 프로젝트 명
FOOD-FIGHTER


## 2. 프로젝트 개요
FaceTracker와 OpenCV를 이용하여 얼굴을 인식하여 음식모형을 먹어서 점수를 쌓고, 랭킹을 확인할 수 있는 게임입니다.


## 3. 프로젝트 목표
기존 프로젝트를 향상시켜 더욱 흥미로운 게임 제작을 목표로 합니다. 

1. 현재는 한 사람만 게임 플레이가 가능합니다. 저희는 두 사람이 동시에 플레이한다면 더욱 게임의 재미를 더할 수 있을 것이라고 생각합니다. 한 사람이 플레이하는 싱글 모드와 두 사람이 함께 플레이하는 멀티 모드 개발을 목표로 합니다.

2. 현재는 화면에 다양한 도형들이 날아다닙니다. 저희는 이것을 음식 모형으로 변경하는 것을 목표로 합니다. 얼굴과 입을 인식하여 먹는 게임이기 때문에 단순한 도형 대신 음식모형을 먹는 형식으로 재미를 더할 계획입니다.

3. 위에서 제시한 음식모형과 관련한 사항입니다. 이에 대해서는 두 가지 방법을 고안하였습니다.

    3-1. 싱글 모드 게임 시작 전, 음식이 랜덤으로(혹은 사용자가 선택) 설정됩니다. 게임을 시작하면 선택된 음식에 필요한 재료들의 리스트가 화면의 상단 혹은 옆쪽에 띄워집니다. 생명 소멸 전까지 재료를 모두 먹으면 성공, 먹지 못하면 실패합니다.

    3-2. 멀티 모드 게임 시작 전, 음식이 랜덤으로(혹은 사용자가 선택) 설정됩니다. 게임을 시작하면 선택된 음식에 필요한 재료들의 리스트가 화면의 상단 혹은 옆쪽에 띄워집니다. 두 명의 플레이어 중 누가 먼저 재료들을 먹어 선택한 음식을 완성하는지 경쟁합니다.


## 4. 개발환경
Develop Tool : C++, OpenCV, OpenGL

Develop Enviornment : Ubuntu Linux Extension Pack


## 5. 사용한 오픈소스
FaceTracker : https://github.com/kylemcdonald/FaceTracker

OpenCV : https://github.com/opencv/opencv

OpenGL : https://www.opengl.org/


## 6. 기존 프로젝트
"YAM-YAM" Game program using Face Tracker : https://github.com/CSID-DGU/2019-1-OSSP2-Openthedoor-7


## 7. 라이센스
MIT License


