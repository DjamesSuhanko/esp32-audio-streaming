# esp32-audio-streaming
Código para streaming de áudio de um ESP32 para um Raspberry.

## Como utilizar
O código foi escrito utilizando Atom com PlatformIO, mas basta copiar o código para a IDE do Arduino.
Coloque o microfone de eletreto (módulo MAX9814) conectado ao 3v3, GND e OUT em SPV. Conecte também o GAIN ao GND.

Antes de subir o sketch no ESP32, execute o programa de recepção escrito em Python ou então digite essa linha de comando no Raspberry (supondo o uso da porta 8888):

nc -l 8888 | aplay -r 8000 -f U8

Depois, suba o sketch no ESP32. Se o som ficar baixo, no console do Raspberry digite:
alsamixer

E aumente tanto quanto desejado.

Se o programa Python reclamar da dependência do pyaudio, instale-o:
sudo apt-get install python-pyaudio

O tutorial relacionado está no link https://www.dobitaobyte.com.br/como-usar-microfone-no-esp32-esp32-voice-streamer/
