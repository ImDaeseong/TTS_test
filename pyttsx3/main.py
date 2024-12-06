import pyttsx3


def tts_speech(text):

    # pyttsx3 엔진 초기화
    engine = pyttsx3.init()

    # 한국어 지원 가능한 음성 확인 (TTS 엔진 설정)
    voices = engine.getProperty('voices')
    for voice in voices:
        if 'korean' in voice.name.lower():  # 한국어 음성 선택
            engine.setProperty('voice', voice.id)
            break

    # 텍스트를 음성으로 변환하여 실행
    engine.say(text)
    engine.runAndWait()


if __name__ == '__main__':
    text = '안녕하세요.'
    tts_speech(text)
