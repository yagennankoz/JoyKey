![JoyKey](https://github.com/yagennankoz/JoyKey/blob/main/image/JoyKey.jpg "完成写真")
# JoyKey
keyboard like gamepad<br>
<br>
古のPCゲームをテンキー感覚で操作することを目的としたゲームパッドです。<br>
Windows等でも使用できますが、X68000 Zをターゲットとしています。<br>
<br>
### インストール方法
- RaspberryPi picoのBOOTSELボタンを押しながらPCとUSB接続してください。<br>
- Winの場合、エクスプローラで立ち上がってきますので、firmware/picoフォルダにあるfirmware.uf2をコピーしてください。<br>
- ファイルのコピーが完了するとpicoが再起動します。<br>

### 操作方法
- MODEボタンでキー配置のプリセットが選択できます。<br>
プリセット変更/追加はkeyAssign.hppを変更し、再ビルドしてください。<br>
- RAPIDボタンを押しながら任意のキーを押すと連射のON/OFFを切り替えることができます。<br>
- SPEEDボタンで連射速度を変更することができます。<br>
連射速度は4/8/16/20連射としていますが、define.hppのinterval定義を変更、再ビルドで変更、追加が行えます。<br>
- キー表示「*」はグラナダのブラスター発射キーとなっています。<br>

### 開発環境
- PlatformIO

### おまけ機能
- ターミナルブロックと3.5mmTRSを付けてUARTストレートケーブルで接続するとMIDI I/Fにもなります。<br>
結線ミス等でX68000 Zが壊れるかもしれないので、使用するかどうかは慎重に判断してください。<br>

### VID/PID
- 個人使用の範囲かつ自己責任でどうにかしていただくと、L+Rが有効になる気がします。<br>
