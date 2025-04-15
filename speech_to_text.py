import sys
import speech_recognition as sr

if len(sys.argv) < 2:
    print("")
    sys.exit()

try:
    r = sr.Recognizer()
    with sr.AudioFile(sys.argv[1]) as source:
        audio = r.record(source)
    print(r.recognize_google(audio, language="fr-FR"))
except:
    print("")
