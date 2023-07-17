# GAME-Styx
[DirectX 3D - Individual Project] 스틱스

동영상(Client)    : https://www.youtube.com/watch?v=gf9KTKMfrRw
<br/>동영상(Tool)      : https://www.youtube.com/watch?v=Jzo6Vq71sAQ
<br/>장르              : 잠입액션
<br/>날짜              : 2019.11.01 ~ 2019.12.01
<br/>인원              : 1명
<br/>개발 환경         : Visual Studio 2015 (x64)
<br/>개발 언어 및 도구  : C++, MFC, DirectX9, HLSL
<br/>Blog : https://song-ift.tistory.com/46

<hr size="5">

<b>* 메인 프레임워크 (DLL로 분리해 엔진과 클라이언트를 세분화)</b>
  - Component, Prototype Pattern을 사용해 독립적, 효율적 구조 형성
  - Reference Count로 객체의 생명주기 관리

* 더 많은 MFC Tool 제작
  - 오브젝트 툴, 네비메쉬 툴, 카메라 툴, 애니메이션 툴 제작

* 광원의 수에 오브젝트의 Draw Call을 줄이기 위해 Deferred Rendering 적용
  - Diffuse, Normal, Specular, Depth 등 다중 렌더링 타겟을 이용해 각 정보  를 각각 다른 렌더링 타겟에 기록 (HLSL 사용)

* 네비게이션 메쉬를 통한 객체의 이동구역 설정
  - 슬라이딩 벡터를 통해 네비게이션 메쉬 모서리에서 자연스러운 이동 구현
  - 메쉬의 다양한 옵션 부여(낭떠러지, 벽, 트리거 등)

* Catmull-Rom Spline을 활용한 Event Camera 구현

* 절두체 컬링을 통해 Draw Call을 줄이는 최적화 적용
  - 평면의 내적 값을 비교해 객체들의 위치 판단 후 렌더 제어
