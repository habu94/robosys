# 概要
デバイスドライバによって7セグメントLEDを8の字に3回光らせるプログラムです

## 用意するもの
- ラズパイ（4か3）×1
- カソード・コモンの7セグメントLED（カソード・コモン=カソードが共通のもの）×1
- ブレッドボード ×1
- 抵抗（220Ω）×7
- ジャンパ線 ×複数

## プログラムのインストール
ラズパイにログインし下記コマンドを実行
 ```bash
  git clone https://github.com/habu94/robosys.git
 ```
 下記コマンドでカーネルモジュールが作成されます（警告は無視）
  ```bash
  cd myled
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
