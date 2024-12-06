from gtts import gTTS
import playsound
import os


def tts_speech(text):
    tts = gTTS(text=text, lang='ko', slow=False)

    output = 'output.mp3'
    tts.save(output)

    playsound.playsound(output)
    # os.remove(output)


if __name__ == '__main__':
    text = '안녕하세요.'
    tts_speech(text)
