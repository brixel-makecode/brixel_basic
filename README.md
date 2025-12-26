# OLED(SSD1306) 한글 디스플레이 확장

MakeCode용 한글 지원 OLED(SSD1306) 디스플레이 확장 프로그램입니다.

## 주요 기능

- **한글 출력**: 완성형 한글 2,350자 지원 (가-힣)
- **영문 출력**: ASCII 문자 지원
- **큰 숫자 표시**: 7-세그먼트 스타일의 큰 숫자 (0-9)
- **막대 그래프**: 값을 시각적으로 표시하는 막대 그래프
- **반전 모드**: 텍스트 및 숫자 반전 표시 지원

## 지원 디스플레이

- **모델**: SSD1306 OLED 디스플레이
- **해상도**: 128x64 픽셀
- **인터페이스**: I2C (주소: 0x3C)
- **크기**: 0.96인치 (일반적)

## 사용 방법

### 1. 디스플레이 초기화

```blocks
Han_OLED_SSD1306.init()
```

프로그램 시작 시 OLED 디스플레이를 초기화합니다.

### 2. 화면 지우기

```blocks
Han_OLED_SSD1306.clearDisplay()
```

OLED 화면의 모든 내용을 지웁니다.

### 3. 텍스트 출력

```blocks
Han_OLED_SSD1306.printString("안녕하세요", 0, 0, 0)
```

- **매개변수**:
  - `str`: 출력할 텍스트 (한글/영문)
  - `x`: X 좌표 (0-15, 문자 단위)
  - `y`: Y 좌표 (0-7, 라인 단위)
  - `inverse`: 반전 모드 (0=정상, 1=반전)

### 4. 큰 숫자 표시

```blocks
Han_OLED_SSD1306.showBigNumber(5, 0, 0, 0)
```

- **매개변수**:
  - `num`: 표시할 숫자 (0-9)
  - `x`: X 좌표
  - `y`: Y 좌표
  - `inverse`: 반전 모드 (0=정상, 1=반전)

### 5. 막대 그래프 그리기

```blocks
Han_OLED_SSD1306.drawBarGraph(0, 0, 100)
```

- **매개변수**:
  - `x`: X 좌표
  - `y`: Y 좌표
  - `value`: 막대 길이 (0-127)

## 예제 코드

### 기본 사용 예제

```typescript
// 디스플레이 초기화
Han_OLED_SSD1306.init()

// 화면 지우기
Han_OLED_SSD1306.clearDisplay()

// 한글 텍스트 출력
Han_OLED_SSD1306.printString("안녕하세요", 0, 0, 0)

// 영문 텍스트 출력
Han_OLED_SSD1306.printString("Hello", 0, 2, 0)

// 큰 숫자 표시
Han_OLED_SSD1306.showBigNumber(7, 0, 4, 0)
```

### 센서 값 표시 예제

```typescript
basic.forever(function () {
    // 화면 지우기
    Han_OLED_SSD1306.clearDisplay()

    // 온도 표시
    let temp = input.temperature()
    Han_OLED_SSD1306.printString("온도:", 0, 0, 0)
    Han_OLED_SSD1306.showBigNumber(temp, 0, 2, 0)

    // 막대 그래프로 표시
    Han_OLED_SSD1306.drawBarGraph(0, 6, temp * 2)

    basic.pause(1000)
})
```

## 블록 색상 설정

이 확장의 블록 색상은 **주황색(#FFA500)**으로 설정되어 있습니다.

색상을 변경하려면 `main.ts` 파일의 첫 번째 줄을 수정하세요:

```typescript
//% color="#FFA500" icon="\uf108" block="OLED(SSD1306) 한글"
```

**추천 색상**:
- 빨강: `#E74C3C`
- 주황: `#FFA500`
- 노랑: `#F1C40F`
- 초록: `#2ECC71`
- 파랑: `#3498DB`
- 보라: `#9B59B6`

## 다국어 지원

이 확장은 한국어와 영어를 지원합니다:
- MakeCode 언어 설정을 변경하면 블록 텍스트가 자동으로 변경됩니다
- 번역 파일: `locales/ko/`, `locales/en/`

## 기술 사양

### 한글 폰트
- 16x16 픽셀 비트맵 폰트
- 초성, 중성, 종성 조합 방식
- 완성형 한글 2,350자 지원

### 영문 폰트
- 8x16 픽셀 비트맵 폰트
- ASCII 문자 지원

### 큰 숫자 폰트
- 27x32 픽셀 7-세그먼트 스타일
- 숫자 0-9 지원

## 하드웨어 연결

### Micro:bit 연결 (I2C)
- **SDA**: Pin 20
- **SCL**: Pin 19
- **VCC**: 3.3V
- **GND**: GND

### I2C 주소
- 기본 주소: `0x3C`

## 라이선스

이 프로젝트는 오픈소스입니다.

## 기여

버그 리포트나 개선 제안은 alphaco@naver.com를 통해 제출해 주세요.

## 참고

- MakeCode: https://makecode.microbit.org
- SSD1306 데이터시트: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

---

**제작**: MakeCode Extension for Korean OLED Display
**버전**: 1.0.0
**개발자**: 김석전 