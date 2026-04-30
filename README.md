![JoyKey](https://github.com/yagennankoz/JoyKey/blob/main/image/JoyKey.jpg "完成写真")
# JoyKey
keyboard like gamepad<br>
<br>
古のPCゲームをテンキー感覚で操作することを目的としたゲームパッドです。<br>
Windows等でも使用できますが、X68000 Zをターゲットとしています。<br>
<br>
### インストール方法
- RaspberryPi picoのBOOTSELボタンを押しながらPCとUSB接続してください。<br>
- Winの場合、エクスプローラで立ち上がってきますので、firmwareフォルダにあるfirmware.uf2をコピーしてください。<br>
- ファイルのコピーが完了するとpicoが再起動します。<br>

### 操作方法
- MODEボタンでキー配置のプリセットが選択できます。<br>
プリセット変更/追加はkeyAssign.hppを変更し、再ビルドしてください。<br>
- RAPIDボタンを押しながら任意のキーを押すと連射のON/OFFを切り替えることができます。<br>
- SPEEDボタンで連射速度を変更することができます。<br>
連射速度は4/8/16/20連射としていますが、define.hppのinterval定義を変更、再ビルドで変更、追加が行えます。<br>
- キー表示「*」はグラナダのブラスター発射キーとなっています。<br>

### 現在実装されているモード
- MODE 1<br>
FIGHTING STICK V3モード<br>
FGC2と組み合わせてPS5で使用できました<br>
- MODE 2<br>
X68000 Z ゲームパッドモード<br>
- MODE 3<br>
X68000 Z グラディウスモード<br>
ショットとミサイルの撃ち分けができます<br>
- MODE 4<br>
ラグーンモード<br>
キーボードにアサインされているインベントリ等を呼び出すことができます<br>
- MODE 5<br>
ネメシス'90改モード<br>
ショットボタンを複数設定しています<br>
連射と溜め撃ちを分けて使用できます<br>
- MODE 6<br>
Ys Iモード<br>
速度変更等の特殊なキーはキー数が足りなくて入れられませんでしたが、通常プレイに使用するキーは網羅しました<br>
- MODE 7<br>
Ys IIモード<br>
Ys Iモードと同様<br>
- MODE 8<br>
Ys IIIモード<br>
Ys Iモードと同様<br>


### 開発環境
- PlatformIO

### おまけ機能
- ターミナルブロックと3.5mmTRSを付けてUARTストレートケーブルで接続するとMIDI I/Fにもなります。<br>
結線ミス等でX68000 Zが壊れるかもしれないので、使用するかどうかは慎重に判断してください。<br>

