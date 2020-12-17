# 概要
デバイスドライバによって7セグメントLEDを8の字に3回光らせるプログラムです

## 用意するもの
- ラズパイ（4か3）×1
- 下の画像のようなカソード・コモンの7セグメントLED（カソード・コモン=カソードが共通のもの）×1
  - テストには「KW1-521C2B」を使用
- ブレッドボード ×1
- 抵抗（220Ω）×7
  - 7セグメントLEDの許容電流とGPIOの電圧3.3Vを考慮して選択
- ジャンパ線 ×複数

<img src="https://user-images.githubusercontent.com/55973880/102452592-f165c380-407d-11eb-8da7-7c8f98b57194.jpg" width="320px">

## プログラムのインストール
ラズパイにログインし下記コマンドを実行
 ```bash
  git clone https://github.com/habu94/robosys.git
 ```
 ディレクトリに入り
  ```bash
  cd robosys
  ```
 ラズパイ3の場合はmyled.c内を117行目の記述通りに変更  
 下記コマンドでカーネルモジュールが作成されます（警告は無視）
  ```bash
  make
  ```
## 回路構築
図で見ないとわかりにくいので、「7セグメントLED カソードコモン」などと検索し回路図を参照してください。  
上のLEDから時計回りにa～gまで振られており、それぞれのピンに対応しています。  
カソード・コモンなので、3, 8ピンがカソードになります。そのため、
|  ラズパイGPIOピン |  7セグLEDピン  |
| :----: | :----: |
|  17  |  7(a)  |
|  22  |  6(b)  |
|  23  |  4(c)  |
|  24  |  2(d)  |
|  25  |  1(e)  |
|  26  |  9(f)  |
|  27  |  10(g)  |

と、間に抵抗を介してつなげます。3, 8ピンはブレッドボード側で1つのGNDにまとめ、ラズパイのGNDとつなげます。

## プログラムの実行
下記コマンドを順に実行します。
まずカーネルモジュールをロード
```bash
  sudo insmod myled.ko
```
ノードを作成
```bash
  sudo mknod /dev/myled0 c 507 0
```
許可を変更
```bash
  sudo chmod 666 /dev/myled0
```
以下のコマンドを打ち回路が正しければLEDが点きます
```bash
  echo 1 > /dev/myled0
```
削除
```bash
  sudo rm /dev/myled0
```
```bash
  sudo rmmod myled
```
