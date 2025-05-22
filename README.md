# 🎮 Walpurgis-Night

Unreal Engine 5 기반의 액션 RPG 프로젝트입니다.  
플레이어 캐릭터의 **전투 시스템**, **장비/인벤토리**, **스킬**, **스탯** 등을 C++로 구현하였습니다.

---

## 🛠 사용 기술

- Unreal Engine 5 (C++)
- OOP 기반 전투 컴포넌트 설계
- Object Pooling, Animation Notifies
- UMG UI / 위젯 연동
- Interfaces, Component 기반 구조화

---

## 🧩 주요 시스템 구성

### 📦 캐릭터 관련

| 클래스 | 설명 |
|--------|------|
| `PixelCodeCharacter` | 플레이어 캐릭터의 메인 클래스 |
| `CharacterAnimInstance` | 애님 블루프린트 연동 클래스 |
| `StateComponent` | 캐릭터 상태(Idle, Combat 등) 관리 |
| `CombatComponent` | 공격, 타격, 콤보 등을 제어하는 핵심 컴포넌트 |
| `BaseWeapon` | 무기 장착/공격 애니메이션 연동 |

---

### ⚔️ 전투 시스템

- `CombatComponent`를 통해 공격 로직 수행
- `AnimNotify` (`ResetAttack_AN`, `ToggleCombat_AN` 등)를 통해 애니메이션 타이밍에 맞춘 로직 동기화
- `CombatInterface`, `Equipmentable` 등 인터페이스 기반으로 설계 유연성 확보

---

### 🧠 스탯 & 상태 관리

- `PlayerOrganism`, `pixelPlayerState`, `StateComponent`를 통해 체력/스태미너/경험치 등 관리
- `PlayerStatWidget`로 실시간 상태 UI 구현

---

### 🧰 장비 및 인벤토리 시스템

- `inventory/ParentItem`, `ExpendableItem` 등 아이템 클래스 구현
- `Equipmentable` 인터페이스 기반으로 장비 슬롯 연동
- `Widget/InventoryUI`에서 드래그앤드롭 구현 가능성 내포

---

### 🧨 스킬 시스템

- `SpawnSkillActor` 폴더에서 스킬 이펙트, 유도 미사일, 설치형 스킬 등 지원
- `CountinueAttack_AN`, `CollisionTrace_ANs` 등을 통해 연타 및 적중 판정 처리

---

## 🌀 오브젝트 풀링

- `Pooling`을 통해 리스폰 및 스킬 소환 시 오브젝트 재사용 처리
- 성능 최적화를 위한 메모리 관리 구조 적용

---

## 🖥️ 게임모드 및 매니저

- `PCodeGameInstance`, `pixelGameModeBase`: 전체 흐름 및 전역 상태 관리
- `PCodePlayerController`: UI 및 입력 처리

---

## ▶️ 시연 영상

[![시연 영상](https://img.shields.io/badge/-시연%20영상-red?style=flat-square&logo=youtube&logoColor=white)](https://youtu.be/여기에_링크)

---

## 🧪 개발자 노트

- Animation Notify를 활용한 정밀한 타이밍 제어 경험
- AActor 기반 풀링 시스템을 직접 설계하여 리스폰 최적화
- 스탯 계산 및 실시간 UI 반영 과정에서 구조적 설계 중요성 학습
